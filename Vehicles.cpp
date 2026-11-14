#include "Vehicles.h"
#include "Exceptions.h"
#include <fstream>
#include <cstring>
#include <iostream>

namespace ReliefOps {
namespace Vehicles {

int ReliefVehicle::totalVehicles = 0;

// --- GPSBeacon ---
GPSBeacon::GPSBeacon() {
    lat = 0.0;
    lon = 0.0;
    beaconId = "UNSET";
}

GPSBeacon::GPSBeacon(double la, double lo, string id) {
    lat = la;
    lon = lo;
    beaconId = id;
}

void GPSBeacon::ping() const {
    cout << "[BEACON " << beaconId << "] lat=" << lat << " lon=" << lon << "\n";
}

// --- EmergencyLifeSupportUnit ---
EmergencyLifeSupportUnit::EmergencyLifeSupportUnit() {
    oxygenLevel = 100;
    active = false;
}

EmergencyLifeSupportUnit::EmergencyLifeSupportUnit(int o2) {
    oxygenLevel = o2;
    active = true;
}

void EmergencyLifeSupportUnit::activate() { active = true; }
void EmergencyLifeSupportUnit::deactivate() { active = false; }

void EmergencyLifeSupportUnit::consumeOxygen(int percent) {
    oxygenLevel -= percent;
    if (oxygenLevel < 0) oxygenLevel = 0;
}

// --- ReliefVehicle ---
ReliefVehicle::ReliefVehicle() {
    vehicleId = 0;
    callSign = "UNNAMED";
    maxPayloadKg = 0;
    currentLoadKg = 0;
    fuelLevelL = 0;
    fuelCapacityL = 0;
    totalVehicles++;
}

ReliefVehicle::ReliefVehicle(int id, string cs, double maxLoad, double fuelCap) {
    vehicleId = id;
    callSign = cs;
    maxPayloadKg = maxLoad;
    currentLoadKg = 0;
    fuelLevelL = fuelCap;
    fuelCapacityL = fuelCap;
    totalVehicles++;
}

ReliefVehicle::~ReliefVehicle() {
    if (totalVehicles > 0) totalVehicles--;
}

void ReliefVehicle::loadCargo(double kg) {
    if (currentLoadKg + kg > maxPayloadKg)
        throw Exceptions::CargoOverweightException(callSign + ": cargo exceeds max payload!");
    currentLoadKg += kg;
}

void ReliefVehicle::unloadCargo(double kg) {
    currentLoadKg -= kg;
    if (currentLoadKg < 0) currentLoadKg = 0;
}

void ReliefVehicle::refuel(double liters) {
    fuelLevelL += liters;
    if (fuelLevelL > fuelCapacityL) fuelLevelL = fuelCapacityL;
}

void ReliefVehicle::consumeFuel(double liters) {
    if (liters > fuelLevelL)
        throw Exceptions::FuelDepletionException(callSign + ": not enough fuel!");
    fuelLevelL -= liters;
}

void ReliefVehicle::checkLifeSupport() const {
    if (!lifeSupport.isActive())
        cout << callSign << ": life support OFFLINE\n";
    else
        cout << callSign << ": life support OK, O2 = " << lifeSupport.getOxygen() << "%\n";
}

void ReliefVehicle::completeTrip(double km, double fuelUsed) {
    consumeFuel(fuelUsed);      // throws FuelDepletionException if short
    TransitLog log;
    log.vehicleId = vehicleId;
    strncpy(log.callSign, callSign.c_str(), 31);
    log.callSign[31] = '\0';
    log.distanceKm = km;
    log.fuelUsedL = fuelUsed;
    log.completed = 1;
    saveTransitLog(log, "transit_log.dat");
    cout << callSign << " finished " << km << " km trip.\n";
}

// --- AerialTransport ---
AerialTransport::AerialTransport(int id, string cs, double maxLoad, double fuelCap, double ceiling)
    : ReliefVehicle(id, cs, maxLoad, fuelCap) {
    altitudeCeilingM = ceiling;
}

AerialTransport::~AerialTransport() {}

void AerialTransport::checkRoute() const {
    if (currentLoadKg > maxPayloadKg)
        throw Exceptions::CargoOverweightException(callSign + ": aerial overloaded!");
    cout << "Aerial route for " << callSign << " looks clear.\n";
}

double AerialTransport::getSpeedKmh() const { return 250.0; }

// --- GroundConvoy ---
GroundConvoy::GroundConvoy(int id, string cs, double maxLoad, double fuelCap, int wheels)
    : ReliefVehicle(id, cs, maxLoad, fuelCap) {
    numberOfWheels = wheels;
    corridorDamaged = false;
}

GroundConvoy::~GroundConvoy() {}

void GroundConvoy::checkRoute() const {
    if (corridorDamaged)
        throw Exceptions::DamagedCorridorException(callSign + ": road blocked!");
    if (currentLoadKg > maxPayloadKg)
        throw Exceptions::CargoOverweightException(callSign + ": ground overloaded!");
    cout << "Ground route for " << callSign << " is passable.\n";
}

double GroundConvoy::getSpeedKmh() const { return 60.0; }

// --- AmphibiousCraft ---
AmphibiousCraft::AmphibiousCraft(int id, string cs, double maxLoad, double fuelCap, double draft)
    : ReliefVehicle(id, cs, maxLoad, fuelCap) {
    draftMeters = draft;
}

AmphibiousCraft::~AmphibiousCraft() {}

void AmphibiousCraft::checkRoute() const {
    if (currentLoadKg > maxPayloadKg)
        throw Exceptions::CargoOverweightException(callSign + ": craft overloaded!");
    cout << "Water route for " << callSign << " looks passable.\n";
}

double AmphibiousCraft::getSpeedKmh() const { return 40.0; }

// --- AmphibiousMedicalUnit (diamond) ---
// most-derived class must initialise the virtual base ReliefVehicle directly
AmphibiousMedicalUnit::AmphibiousMedicalUnit(int id, string cs, double maxLoad, double fuelCap,
                                             double ceiling, double draft, int beds)
    : ReliefVehicle(id, cs, maxLoad, fuelCap),      // virtual base init (only this counts)
      AerialTransport(id, cs, maxLoad, fuelCap, ceiling),
      AmphibiousCraft(id, cs, maxLoad, fuelCap, draft) {
    bedCount = beds;
    lifeSupport.activate();   // it's a medical unit after all
}

AmphibiousMedicalUnit::~AmphibiousMedicalUnit() {}

void AmphibiousMedicalUnit::checkRoute() const {
    if (currentLoadKg > maxPayloadKg)
        throw Exceptions::CargoOverweightException(callSign + ": medical unit overloaded!");
    cout << "Medical unit " << callSign << " can use air OR water route.\n";
}

double AmphibiousMedicalUnit::getSpeedKmh() const { return 200.0; }

// --- binary transit log ---
void saveTransitLog(const TransitLog& log, const string& filename) {
    ofstream fout(filename, ios::binary | ios::app);
    if (!fout) { cout << "Cannot open " << filename << "\n"; return; }
    fout.write((char*)&log, sizeof(log));
    fout.close();
}

void printAllTransitLogs(const string& filename) {
    ifstream fin(filename, ios::binary);
    if (!fin) { cout << "No transit logs found.\n"; return; }
    TransitLog log;
    while (fin.read((char*)&log, sizeof(log))) {
        cout << "Vehicle " << log.vehicleId << " [" << log.callSign << "] "
             << log.distanceKm << "km, fuel " << log.fuelUsedL << "L, "
             << (log.completed ? "delivered" : "aborted") << "\n";
    }
    fin.close();
}

} // namespace Vehicles
} // namespace ReliefOps