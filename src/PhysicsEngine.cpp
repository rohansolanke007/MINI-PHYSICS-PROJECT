#include "PhysicsEngine.h"
#include "Exceptions.h"

PhysicsEngine::PhysicsEngine(const Vector2D& boundaryMin, const Vector2D& boundaryMax, double timeStep)
    : world(boundaryMin, boundaryMax), timeStep(timeStep), paused(false)
{
    if (timeStep <= 0.0) {
        throw InvalidSimulationException("Time step must be positive.");
    }
}

void PhysicsEngine::setTimeStep(double dt) {
    if (dt <= 0.0) {
        throw InvalidSimulationException("Time step must be positive.");
    }
    timeStep = dt;
}

int PhysicsEngine::runSteps(int steps) {
    if (paused) return 0;
    int executed = 0;
    for (int i = 0; i < steps; ++i) {
        world.step(timeStep);
        ++executed;
    }
    return executed;
}
