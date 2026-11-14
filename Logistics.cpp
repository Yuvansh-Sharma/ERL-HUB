#include "Logistics.h"

namespace ReliefOps {
namespace Logistics {

int TransportVehicle::operationalVehicles = 0;

TransportVehicle::TransportVehicle() {
    vehicleId = 0;
    driverName = new char[5];
    strcpy(driverName, "NONE");
    capacityKg = 0;
}

TransportVehicle::TransportVehicle(int id, const char* driver, double cap) {
    vehicleId = id;
    driverName = new char[strlen(driver) + 1];
    strcpy(driverName, driver);
    capacityKg = cap;
    operationalVehicles++;
}

TransportVehicle::TransportVehicle(const TransportVehicle& other) {
    vehicleId = other.vehicleId;
    driverName = new char[strlen(other.driverName) + 1];
    strcpy(driverName, other.driverName);
    capacityKg = other.capacityKg;
    operationalVehicles++;
}

TransportVehicle::~TransportVehicle() {
    delete[] driverName;
    if (operationalVehicles > 0) operationalVehicles--;
}

ostream& operator<<(ostream& os, const TransportVehicle& v) {
    os << "Vehicle#" << v.vehicleId << " driver=" << v.driverName
       << " cap=" << v.capacityKg << "kg";
    return os;
}

DistributionDepot::DistributionDepot() {
    depotId = 0;
    location = new char[6];
    strcpy(location, "UNSET");
    manifests = NULL;
    manifestCount = 0;
    maxCapacity = 0;
}

DistributionDepot::DistributionDepot(int id, const char* loc, int cap) {
    depotId = id;
    location = new char[strlen(loc) + 1];
    strcpy(location, loc);
    manifests = new ReliefCargo[cap];
    manifestCount = 0;
    maxCapacity = cap;
}

// deep copy - each manifest copied properly
DistributionDepot::DistributionDepot(const DistributionDepot& other) {
    depotId = other.depotId;
    location = new char[strlen(other.location) + 1];
    strcpy(location, other.location);
    maxCapacity = other.maxCapacity;
    manifestCount = other.manifestCount;
    manifests = new ReliefCargo[maxCapacity];
    for (int i = 0; i < manifestCount; i++)
        manifests[i] = other.manifests[i];
}

DistributionDepot::~DistributionDepot() {
    delete[] location;
    delete[] manifests;
}

void DistributionDepot::addManifest(const ReliefCargo& c) {
    if (manifestCount < maxCapacity)
        manifests[manifestCount++] = c;
}

bool DistributionDepot::removeManifest(int cargoId) {
    for (int i = 0; i < manifestCount; i++) {
        if (manifests[i].getId() == cargoId) {
            for (int j = i; j < manifestCount - 1; j++)
                manifests[j] = manifests[j + 1];
            manifestCount--;
            return true;
        }
    }
    return false;
}

ReliefCargo* DistributionDepot::findManifest(int cargoId) {
    for (int i = 0; i < manifestCount; i++)
        if (manifests[i].getId() == cargoId)
            return &manifests[i];
    return NULL;
}

int DistributionDepot::getTotalWeightKg() const {
    double total = 0;
    for (int i = 0; i < manifestCount; i++)
        total += manifests[i].getWeight() * manifests[i].getQuantity();
    return (int)total;
}

DistributionDepot& DistributionDepot::operator+=(const ReliefCargo& c) {
    addManifest(c);
    return *this;
}

DistributionDepot& DistributionDepot::operator-=(int cargoId) {
    removeManifest(cargoId);
    return *this;
}

bool DistributionDepot::operator==(const DistributionDepot& other) const {
    if (manifestCount != other.manifestCount) return false;
    for (int i = 0; i < manifestCount; i++)
        if (!(manifests[i] == other.manifests[i])) return false;
    return true;
}

ostream& operator<<(ostream& os, const DistributionDepot& d) {
    os << "Depot#" << d.depotId << " @" << d.location
       << " (" << d.manifestCount << " items)\n";
    for (int i = 0; i < d.manifestCount; i++)
        os << "  " << d.manifests[i] << "\n";
    return os;
}

istream& operator>>(istream& is, DistributionDepot& d) {
    char loc[50];
    int count;
    is >> d.depotId >> loc >> count;
    delete[] d.location;
    d.location = new char[strlen(loc) + 1];
    strcpy(d.location, loc);
    if (count > d.maxCapacity) count = d.maxCapacity;
    d.manifestCount = count;
    for (int i = 0; i < count; i++)
        is >> d.manifests[i];
    return is;
}

} // namespace Logistics
} // namespace ReliefOps