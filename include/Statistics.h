#ifndef STATISTICS_H
#define STATISTICS_H

#include <iostream>

// Statistics
// ----------------------------------------------------------------
// A small plain data + behaviour class that reports aggregate
// information about the current state of the World. It never stores
// its own copy of simulation state permanently - World computes a
// fresh Statistics snapshot on demand (computeStatistics()), which
// avoids duplicated/stale data.
// ----------------------------------------------------------------
class Statistics {
public:
    int objectCount = 0;
    int dynamicCount = 0;
    int staticCount = 0;
    double totalKineticEnergy = 0.0;
    int totalCollisions = 0;
    double simulationTime = 0.0;

    void print() const {
        std::cout << "----- Simulation Statistics -----\n";
        std::cout << "Simulation Time     : " << simulationTime << " s\n";
        std::cout << "Total Objects       : " << objectCount
                  << " (Dynamic: " << dynamicCount << ", Static: " << staticCount << ")\n";
        std::cout << "Total Kinetic Energy: " << totalKineticEnergy << " J\n";
        std::cout << "Total Collisions    : " << totalCollisions << "\n";
        std::cout << "----------------------------------\n";
    }
};

#endif // STATISTICS_H
