#ifndef BIN_H
#define BIN_H

#include <string>

enum class WasteType {
    PLASTIC,
    FOOD,
    METAL,
    MIXED
};

class Bin {
    int id;
    std::string location;
    WasteType type;
    double fillLevel; // 0–100
    bool needsCollection;
    double recyclableAmount;
    double nonRecyclableAmount;
public:
    Bin(int id, const std::string& loc, WasteType t, double fill);
    int getId() const;
    std::string getLocation() const;
    WasteType getType() const;
    double getFillLevel() const;
    bool getNeedsCollection() const;
    double getRecyclableAmount() const;
    double getNonRecyclableAmount() const;

    void setLocation(const std::string& loc);
    void setType(WasteType t);
    void setFillLevel(double level);
    void updateStatus();
    void increaseFill(double amount);
    void sortWaste();
};

std::string wasteTypeToString(WasteType t);
WasteType wasteTypeFromString(const std::string& s);

#endif
