#ifndef SIMULATION_STATE_H
#define SIMULATION_STATE_H

#include <vector>
#include <string>
#include "Vector2D.h"

class World; // forward declaration only - avoids a circular #include with World.h

// ObjectRecord
// ----------------------------------------------------------------
// A flat, file-friendly description of a single PhysicsObject: enough
// information to fully reconstruct it (kind, shape type + dimensions,
// mass, position, velocity).
// ----------------------------------------------------------------
struct ObjectRecord {
    int id;
    bool isStatic;
    std::string shapeType;        // "CIRCLE" | "RECTANGLE" | "TRIANGLE"
    std::vector<double> shapeParams; // Circle: [radius] | Rectangle: [w,h] | Triangle: [base,height]
    double mass;
    Vector2D position;
    Vector2D velocity;
};

// SimulationState
// ----------------------------------------------------------------
// A snapshot of the entire World at one point in time. Used for two
// distinct purposes that both benefit from the same representation:
//   1) Saving/loading a simulation to/from a file (FileHandler)
//   2) Undo history (World keeps a std::stack<SimulationState>)
//
// OOP concepts demonstrated:
//   - Separation of concerns (data snapshot vs. behaviour classes)
//   - Works together with File I/O (Unit IV) and STL (stack) usage
// ----------------------------------------------------------------
class SimulationState {
public:
    double simulationTime = 0.0;
    std::vector<ObjectRecord> records;

    // Builds a snapshot from the current state of a World.
    static SimulationState captureFrom(const World& world);
};

#endif // SIMULATION_STATE_H
