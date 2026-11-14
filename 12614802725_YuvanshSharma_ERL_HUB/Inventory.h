#ifndef INVENTORY_H
#define INVENTORY_H

#include <iostream>
#include <cstring>
#include <cmath>
using namespace std;

namespace ReliefOps {
namespace Inventory {

class IncidentCommander;   // friend, for emergency overrides

// quick distance in km (simple flat-earth)
inline double distanceKm(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    return sqrt(dx*dx + dy*dy);
}

// convert kg to tons
inline double toTons(double kg) {
    return kg / 1000.0;
}

class ReliefCargo {
private:
    static int totalDispatchedTons;
    char* cargoName;
    int cargoId;
    double weightKg;
    int quantity;
    bool priority;

public:
    ReliefCargo();
    ReliefCargo(const char* name, int id, double w, int q, bool p);
    ReliefCargo(const ReliefCargo& other);
    ReliefCargo& operator=(const ReliefCargo& other);
    ~ReliefCargo();

    // ref returns so caller can update directly
    int& getQuantityRef() { return quantity; }
    double& getWeightRef() { return weightKg; }

    const char* getName() const { return cargoName; }
    int getId() const { return cargoId; }
    int getQuantity() const { return quantity; }
    double getWeight() const { return weightKg; }
    bool isPriority() const { return priority; }
    void setPriority(bool p) { priority = p; }
    void setName(const char* n);

    ReliefCargo& operator+=(int qty);      // restock
    ReliefCargo& operator-=(int qty);      // dispatch
    bool operator==(const ReliefCargo& other) const;

    friend ostream& operator<<(ostream& os, const ReliefCargo& c);
    friend istream& operator>>(istream& is, ReliefCargo& c);

    static int getTotalDispatched() { return totalDispatchedTons; }

    friend class IncidentCommander;   // emergency access
};

// overloaded allocate - volume based or priority based
void allocateSupply(ReliefCargo& cargo, double volumeM3);
void allocateSupply(ReliefCargo& cargo, int triageLevel, bool emergency);

// call by reference - updates caller's counter too
void restockByRef(ReliefCargo& cargo, int units, int& totalStock);

} // namespace Inventory
} // namespace ReliefOps

#endif