#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include "World.h"

// PhysicsEngine
// ----------------------------------------------------------------
// The top-level controller for the whole simulation. It OWNS a World
// (physics state + rules) and is responsible for simulation-level
// concerns: the fixed timestep, and whether the simulation is
// currently running/paused.
//
// Why this exists separately from World: keeping "control flow"
// (start/pause/resume/reset) out of World prevents World from turning
// into a god-object that both IS the physics and DRIVES the physics.
// PhysicsEngine is intentionally thin - it delegates almost
// everything to World.
// ----------------------------------------------------------------
class PhysicsEngine {
private:
    World world;
    double timeStep;
    bool paused;

public:
    PhysicsEngine(const Vector2D& boundaryMin, const Vector2D& boundaryMax, double timeStep = 0.05);

    World& getWorld() { return world; }
    const World& getWorld() const { return world; }

    double getTimeStep() const { return timeStep; }
    void setTimeStep(double dt);

    bool isPaused() const { return paused; }
    void pause() { paused = true; }
    void resume() { paused = false; }

    // Advances the simulation by 'steps' timesteps, unless paused.
    // Returns the number of steps actually executed.
    int runSteps(int steps);
};

#endif // PHYSICS_ENGINE_H
