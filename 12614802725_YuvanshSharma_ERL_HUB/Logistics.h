#ifndef LOGISTICS_H
#define LOGISTICS_H

#include "Inventory.h"
#include <iostream>

namespace ReliefOps {
namespace Logistics {

using namespace Inventory;

class TransportVehicle {
private:
    static int operationalVehicles;
    int vehicleId;
    char* driverName;
    double capacityKg;

public:
    TransportVehicle();
    TransportVehicle(int id, const char* driver, double cap);
    TransportVehicle(const TransportVehicle& other);
    ~TransportVehicle();

    int getVehicleId() const { return vehicleId; }
    double getCapacity() const { return capacityKg; }
    const char* getDriver() const { return driverName; }
    static int& getOperationalRef() { return operationalVehicles; }

    friend ostream& operator<<(ostream& os, const TransportVehicle& v);
    friend class IncidentCommander;
};

class DistributionDepot {
private:
    int depotId;
    char* location;
    ReliefCargo* manifests;    // dynamic array
    int manifestCount;
    int maxCapacity;

public:
    DistributionDepot();
    DistributionDepot(int id, const char* loc, int cap);
    DistributionDepot(const DistributionDepot& other);
    ~DistributionDepot();

    void addManifest(const ReliefCargo& c);
    bool removeManifest(int cargoId);
    ReliefCargo* findManifest(int cargoId);
    int getTotalWeightKg() const;

    int getDepotId() const { return depotId; }
    int getManifestCount() const { return manifestCount; }
    const char* getLocation() const { return location; }

    DistributionDepot& operator+=(const ReliefCargo& c);
    DistributionDepot& operator-=(int cargoId);
    bool operator==(const DistributionDepot& other) const;

    friend ostream& operator<<(ostream& os, const DistributionDepot& d);
    friend istream& operator>>(istream& is, DistributionDepot& d);
    friend class IncidentCommander;
};

// check if vehicle can move this cargo
inline bool canCarry(const TransportVehicle& v, const ReliefCargo& c) {
    return v.getCapacity() >= c.getWeight() * c.getQuantity();
}

} // namespace Logistics
} // namespace ReliefOps

#endif