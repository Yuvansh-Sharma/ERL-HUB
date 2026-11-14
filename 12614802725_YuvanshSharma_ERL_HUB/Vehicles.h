// Vehicles.h
#ifndef VEHICLES_H
#define VEHICLES_H

#include "Inventory.h"
#include "Logistics.h"
#include "Exceptions.h"
#include <string>
using namespace std;

namespace ReliefOps {
namespace Vehicles {

// --- composition helpers ---

class GPSBeacon {
private:
    double lat, lon;
    string beaconId;
public:
    GPSBeacon();
    GPSBeacon(double la, double lo, string id);
    void ping() const;
    double getLat() const { return lat; }
    double getLon() const { return lon; }
    string getId() const { return beaconId; }
};

class EmergencyLifeSupportUnit {
private:
    int oxygenLevel;   // percentage remaining
    bool active;
public:
    EmergencyLifeSupportUnit();
    EmergencyLifeSupportUnit(int o2);
    void activate();
    void deactivate();
    void consumeOxygen(int percent);
    int getOxygen() const { return oxygenLevel; }
    bool isActive() const { return active; }
};

// --- base class ---

class ReliefVehicle {
protected:
    static int totalVehicles;
    int vehicleId;
    string callSign;
    double maxPayloadKg;
    double currentLoadKg;
    double fuelLevelL;
    double fuelCapacityL;
    GPSBeacon beacon;                  // has-a (composition)
    EmergencyLifeSupportUnit lifeSupport;  // has-a

public:
    ReliefVehicle();
    ReliefVehicle(int id, string cs, double maxLoad, double fuelCap);
    virtual ~ReliefVehicle();          // virtual dtor for safe deletion

    virtual void checkRoute() const = 0;
    virtual double getSpeedKmh() const = 0;

    int getId() const { return vehicleId; }
    string getCallSign() const { return callSign; }
    double getMaxPayload() const { return maxPayloadKg; }
    double getCurrentLoad() const { return currentLoadKg; }
    double getFuel() const { return fuelLevelL; }

    void loadCargo(double kg);
    void unloadCargo(double kg);
    void refuel(double liters);
    void consumeFuel(double liters);
    void pingBeacon() const { beacon.ping(); }
    void checkLifeSupport() const;
    void completeTrip(double km, double fuelUsed);

    static int getTotalVehicles() { return totalVehicles; }
};

// --- derived 1 ---
class AerialTransport : virtual public ReliefVehicle {
protected:
    double altitudeCeilingM;
public:
    AerialTransport(int id, string cs, double maxLoad, double fuelCap, double ceiling);
    virtual ~AerialTransport();
    void checkRoute() const override;
    double getSpeedKmh() const override;
};

// --- derived 2 ---
class GroundConvoy : virtual public ReliefVehicle {
protected:
    int numberOfWheels;
    bool corridorDamaged;
public:
    GroundConvoy(int id, string cs, double maxLoad, double fuelCap, int wheels);
    virtual ~GroundConvoy();
    void setCorridorDamaged(bool d) { corridorDamaged = d; }
    void checkRoute() const override;
    double getSpeedKmh() const override;
};

// --- derived 3 ---
class AmphibiousCraft : virtual public ReliefVehicle {
protected:
    double draftMeters;
public:
    AmphibiousCraft(int id, string cs, double maxLoad, double fuelCap, double draft);
    virtual ~AmphibiousCraft();
    void checkRoute() const override;
    double getSpeedKmh() const override;
};

// --- diamond: hybrid inherits from two paths ---
class AmphibiousMedicalUnit : public AerialTransport, public AmphibiousCraft {
private:
    int bedCount;
public:
    AmphibiousMedicalUnit(int id, string cs, double maxLoad, double fuelCap,
                          double ceiling, double draft, int beds);
    virtual ~AmphibiousMedicalUnit();
    void checkRoute() const override;
    double getSpeedKmh() const override;
    int getBeds() const { return bedCount; }
};

// --- binary transit log ---
struct TransitLog {
    int vehicleId;
    char callSign[32];
    double distanceKm;
    double fuelUsedL;
    int completed;   // 0 = aborted, 1 = delivered
};

void saveTransitLog(const TransitLog& log, const string& filename);
void printAllTransitLogs(const string& filename);

} // namespace Vehicles
} // namespace ReliefOps
#endif