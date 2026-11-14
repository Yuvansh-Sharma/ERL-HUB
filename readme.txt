cat << 'EOF' > readme.txt
====================================================================
ERL-HUB: Post-Earthquake Multi-Agency Emergency Relief Logistics Hub
====================================================================
Course: Object-Oriented Programming with C++ (B.Tech CSE, 2nd Year)
Institution: Maharaja Agrasen Institute of Technology (MAIT)

1. PROJECT OVERVIEW
ERL-HUB is a modular C++ console application that simulates disaster
relief inventory management, heterogeneous vehicle routing, custom
exception handling, binary transit logging, and priority medical
dispatch scheduling.

2. SOURCE FILE STRUCTURE
- Exceptions.h       : Custom exception hierarchy (std::exception)
- Inventory.h/.cpp   : ReliefCargo class, deep copy control, inline math
- Logistics.h/.cpp   : DistributionDepot, TransportVehicle, operators
- Vehicles.h/.cpp    : Polymorphic fleet, virtual diamond inheritance,
                       composition, and binary TransitLog I/O
- DispatchSystem.h   : Template PriorityDispatchQueue<T> & manipulators
- main.cpp           : Driver program demonstrating all Units (I-IV)

3. COMPILATION & EXECUTION (g++ / clang++)
Open terminal in the project root folder and run:
  g++ -std=c++11 Inventory.cpp Logistics.cpp Vehicles.cpp main.cpp -o erl_hub
  ./erl_hub

4. GENERATED FILES
- transit_log.dat    : Binary file storing serialized TransitLog records
====================================================================
EOF
zip -r ERL_HUB_Code.zip *.h *.cpp *.dat readme.txt
