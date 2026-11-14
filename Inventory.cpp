#include "Inventory.h"

namespace ReliefOps {
namespace Inventory {

int ReliefCargo::totalDispatchedTons = 0;

ReliefCargo::ReliefCargo() {
    cargoName = new char[10];
    strcpy(cargoName, "UNKNOWN");
    cargoId = 0;
    weightKg = 0;
    quantity = 0;
    priority = false;
}

ReliefCargo::ReliefCargo(const char* name, int id, double w, int q, bool p) {
    cargoName = new char[strlen(name) + 1];
    strcpy(cargoName, name);
    cargoId = id;
    weightKg = w;
    quantity = q;
    priority = p;
}

ReliefCargo::ReliefCargo(const ReliefCargo& other) {
    cargoName = new char[strlen(other.cargoName) + 1];
    strcpy(cargoName, other.cargoName);
    cargoId = other.cargoId;
    weightKg = other.weightKg;
    quantity = other.quantity;
    priority = other.priority;
}

ReliefCargo& ReliefCargo::operator=(const ReliefCargo& other) {
    if (this == &other) return *this;     // self assign check
    delete[] cargoName;
    cargoName = new char[strlen(other.cargoName) + 1];
    strcpy(cargoName, other.cargoName);
    cargoId = other.cargoId;
    weightKg = other.weightKg;
    quantity = other.quantity;
    priority = other.priority;
    return *this;
}

ReliefCargo::~ReliefCargo() {
    delete[] cargoName;
}

void ReliefCargo::setName(const char* n) {
    delete[] cargoName;
    cargoName = new char[strlen(n) + 1];
    strcpy(cargoName, n);
}

ReliefCargo& ReliefCargo::operator+=(int qty) {
    if (qty > 0) quantity += qty;
    return *this;
}

ReliefCargo& ReliefCargo::operator-=(int qty) {
    if (qty > 0 && qty <= quantity) {
        quantity -= qty;
        totalDispatchedTons += (int)toTons(qty * weightKg);
    }
    return *this;
}

bool ReliefCargo::operator==(const ReliefCargo& other) const {
    return cargoId == other.cargoId && strcmp(cargoName, other.cargoName) == 0;
}

ostream& operator<<(ostream& os, const ReliefCargo& c) {
    os << c.cargoName << " (ID:" << c.cargoId << ", " << c.weightKg << "kg"
       << ", qty:" << c.quantity << ", priority:" << (c.priority ? "yes" : "no") << ")";
    return os;
}

istream& operator>>(istream& is, ReliefCargo& c) {
    char buf[50];
    is >> buf >> c.cargoId >> c.weightKg >> c.quantity;
    int p;
    is >> p;
    c.priority = (p == 1);
    c.setName(buf);
    return is;
}

// volume based - assume 250kg fits in 1 cubic meter
void allocateSupply(ReliefCargo& cargo, double volumeM3) {
    int fit = (int)(volumeM3 * 250.0 / cargo.getWeight());
    if (fit > 0 && fit <= cargo.getQuantity())
        cargo -= fit;
}

// priority based - critical takes everything
void allocateSupply(ReliefCargo& cargo, int triageLevel, bool emergency) {
    if (emergency && triageLevel == 1)
        cargo -= cargo.getQuantity();
    else if (triageLevel == 2)
        cargo -= cargo.getQuantity() / 2;
}

void restockByRef(ReliefCargo& cargo, int units, int& totalStock) {
    cargo += units;
    totalStock += units;    // caller sees this change
}

} // namespace Inventory
} // namespace ReliefOps