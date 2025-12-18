#include "Bin.h"

Bin::Bin(int id, const std::string& loc, WasteType t, double fill)
    : id(id), location(loc), type(t), fillLevel(fill),
      needsCollection(false), recyclableAmount(0), nonRecyclableAmount(0) {
    updateStatus();
}

int Bin::getId() const { return id; }
std::string Bin::getLocation() const { return location; }
WasteType Bin::getType() const { return type; }
double Bin::getFillLevel() const { return fillLevel; }
bool Bin::getNeedsCollection() const { return needsCollection; }
double Bin::getRecyclableAmount() const { return recyclableAmount; }
double Bin::getNonRecyclableAmount() const { return nonRecyclableAmount; }

void Bin::setLocation(const std::string& loc) { location = loc; }
void Bin::setType(WasteType t) { type = t; }
void Bin::setFillLevel(double level) {
    if (level < 0) level = 0;
    if (level > 100) level = 100;
    fillLevel = level;
    updateStatus();
}

void Bin::updateStatus() {
    needsCollection = (fillLevel >= 80);
}

void Bin::increaseFill(double amount) {
    setFillLevel(fillLevel + amount);
}

void Bin::sortWaste() {
    if (type == WasteType::PLASTIC || type == WasteType::METAL) {
        recyclableAmount = fillLevel * 0.7;
        nonRecyclableAmount = fillLevel * 0.3;
    } else if (type == WasteType::FOOD) {
        recyclableAmount = fillLevel * 0.4;
        nonRecyclableAmount = fillLevel * 0.6;
    } else {
        recyclableAmount = fillLevel * 0.5;
        nonRecyclableAmount = fillLevel * 0.5;
    }
}

std::string wasteTypeToString(WasteType t) {
    switch(t) {
        case WasteType::PLASTIC: return "plastic";
        case WasteType::FOOD:    return "food";
        case WasteType::METAL:   return "metal";
        case WasteType::MIXED:   return "mixed";
    }
    return "mixed";
}

WasteType wasteTypeFromString(const std::string& s) {
    if (s == "plastic") return WasteType::PLASTIC;
    if (s == "food")    return WasteType::FOOD;
    if (s == "metal")   return WasteType::METAL;
    return WasteType::MIXED;
}
