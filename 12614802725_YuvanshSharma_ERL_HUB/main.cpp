#include "Vehicles.h"
#include "DispatchSystem.h"
#include "Exceptions.h"
#include <iostream>
using namespace ReliefOps;

int main() {
    // create a mixed fleet as base pointers -> runtime polymorphism
    Vehicles::ReliefVehicle* fleet[3];
    fleet[0] = new Vehicles::AerialTransport(1, "Hawk-1", 5000, 800, 3000);
    fleet[1] = new Vehicles::GroundConvoy(2, "Truck-7", 12000, 400, 8);
    fleet[2] = new Vehicles::AmphibiousMedicalUnit(3, "Medic-Duo", 3000, 500, 2000, 1.5, 12);

    for (int i = 0; i < 3; i++) {
        try {
            fleet[i]->checkRoute();
            cout << "  speed = " << fleet[i]->getSpeedKmh() << " km/h\n";
        } catch (Exceptions::CargoOverweightException& e) {
            cout << "OVERWEIGHT: " << e.what() << "\n";
        } catch (Exceptions::DamagedCorridorException& e) {
            cout << "DAMAGED: " << e.what() << "\n";
        }
    }

    // overload demo -> CargoOverweightException
    try {
        fleet[0]->loadCargo(99999);
    } catch (Exceptions::CargoOverweightException& e) {
        cout << "Caught: " << e.what() << "\n";
    }

    // trip -> binary log written to transit_log.dat
    fleet[1]->completeTrip(45.0, 30.0);
    cout << "\n--- transit logs from file ---\n";
    Vehicles::printAllTransitLogs("transit_log.dat");

    // priority dispatch queue with custom functor
    Dispatch::PriorityDispatchQueue<Dispatch::MedicalNeed> q;
    q.push({101, 2, 8,  "Ward-4"});
    q.push({102, 1, 20, "ICU-East"});
    q.push({103, 3, 3,  "Clinic-North"});
    q.push({104, 1, 5,  "ER-South"});
    cout << "\n--- dispatch queue ---\n";
    q.printTable();
    q.sortByPriority();
    cout << "top urgent: id=" << q.popMostUrgent().id << "\n";

    for (int i = 0; i < 3; i++) delete fleet[i];
    cout << "\nfleet size = " << Vehicles::ReliefVehicle::getTotalVehicles() << "\n";
    return 0;
}