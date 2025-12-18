#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
    typedef int socklen_t;
#else
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <unistd.h>
#endif

#include "Bin.h"

/* ===================== BIN SERVICE ===================== */

class BinService {
    std::vector<Bin> bins;
    int nextId = 1;

public:
    Bin addBin(const std::string& loc, WasteType t, double fill) {
        Bin b(nextId++, loc, t, fill);
        bins.push_back(b);
        return b;
    }

    std::vector<Bin>& getAll() { return bins; }

    Bin* findById(int id) {
        for (auto& b : bins)
            if (b.getId() == id) return &b;
        return nullptr;
    }

    std::string generateRoute() {
        std::vector<int> ids;
        for (auto& b : bins)
            if (b.getNeedsCollection())
                ids.push_back(b.getId());

        std::sort(ids.begin(), ids.end());

        std::ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < ids.size(); ++i) {
            oss << ids[i];
            if (i + 1 < ids.size()) oss << ", ";
        }
        oss << "]";
        return oss.str();
    }
};

/* ===================== JSON HELPERS ===================== */

std::string binToJson(const Bin& b) {
    std::ostringstream oss;
    oss << "{"
        << "\"id\":" << b.getId() << ","
        << "\"location\":\"" << b.getLocation() << "\","
        << "\"type\":\"" << wasteTypeToString(b.getType()) << "\","
        << "\"fillLevel\":" << b.getFillLevel() << ","
        << "\"needsCollection\":" << (b.getNeedsCollection() ? "true" : "false") << ","
        << "\"recyclable\":" << b.getRecyclableAmount() << ","
        << "\"nonRecyclable\":" << b.getNonRecyclableAmount()
        << "}";
    return oss.str();
}

std::string allBinsToJson(std::vector<Bin>& bins) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < bins.size(); ++i) {
        oss << binToJson(bins[i]);
        if (i + 1 < bins.size()) oss << ",";
    }
    oss << "]";
    return oss.str();
}

/* ===================== FORM PARSER ===================== */

void parseFormBody(const std::string& body,
                   std::string& loc,
                   std::string& type,
                   double& fill) {
    loc = "";
    type = "mixed";
    fill = 0;

    std::istringstream iss(body);
    std::string pair;

    while (std::getline(iss, pair, '&')) {
        auto pos = pair.find('=');
        if (pos == std::string::npos) continue;

        std::string key = pair.substr(0, pos);
        std::string val = pair.substr(pos + 1);

        if (key == "location") loc = val;
        else if (key == "type") type = val;
        else if (key == "fill") fill = std::stod(val);
    }
}

/* ===================== REQUEST HANDLER ===================== */

std::string handleRequest(const std::string& req, BinService& service) {
    std::istringstream iss(req);
    std::string method, path, version;
    iss >> method >> path >> version;

    std::string line, body;
    bool blank = false;

    while (std::getline(iss, line)) {
        if (!blank) {
            if (line == "\r" || line.empty())
                blank = true;
        } else {
            body += line;
        }
    }

    std::string responseBody = "{}";
    int status = 200;
    std::string statusText = "OK";

    if (method == "GET" && path == "/bins") {
        responseBody = allBinsToJson(service.getAll());
    }
    else if (method == "POST" && path == "/bins") {
        std::string loc, type;
        double fill;
        parseFormBody(body, loc, type, fill);
        responseBody = binToJson(service.addBin(loc, wasteTypeFromString(type), fill));
    }
    else if (method == "POST" && path.find("/bins/") == 0 && path.find("/fill") != std::string::npos) {
        int id = std::stoi(path.substr(6, path.find("/fill") - 6));
        Bin* b = service.findById(id);
        if (!b) status = 404, statusText = "Not Found";
        else b->increaseFill(10), responseBody = binToJson(*b);
    }
    else if (method == "POST" && path.find("/bins/") == 0 && path.find("/sort") != std::string::npos) {
        int id = std::stoi(path.substr(6, path.find("/sort") - 6));
        Bin* b = service.findById(id);
        if (!b) status = 404, statusText = "Not Found";
        else b->sortWaste(), responseBody = binToJson(*b);
    }
    else if (method == "GET" && path == "/route") {
        responseBody = "{\"route\":" + service.generateRoute() + "}";
    }
    else {
        status = 404;
        statusText = "Not Found";
    }

    std::ostringstream resp;
    resp << "HTTP/1.1 " << status << " " << statusText << "\r\n";
    resp << "Content-Type: application/json\r\n";
    resp << "Access-Control-Allow-Origin: *\r\n";
    resp << "Content-Length: " << responseBody.size() << "\r\n\r\n";
    resp << responseBody;

    return resp.str();
}

/* ===================== MAIN ===================== */

int main() {
    BinService service;
    service.addBin("Campus Gate", WasteType::PLASTIC, 40);

#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in address{};
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    std::cout << "Server running on port 8080\n";

    while (true) {
        SOCKET client = accept(server_fd, (sockaddr*)&address, &addrlen);
        char buffer[4096]{};
        recv(client, buffer, sizeof(buffer), 0);

        std::string response = handleRequest(buffer, service);
        send(client, response.c_str(), response.size(), 0);

#ifdef _WIN32
        closesocket(client);
#else
        close(client);
#endif
    }
}
