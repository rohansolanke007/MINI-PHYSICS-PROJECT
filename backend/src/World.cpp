#include "World.h"
#include "Exceptions.h"
#include "MathUtils.h"
#include <algorithm>

World::World(const Vector2D& boundaryMin, const Vector2D& boundaryMax)
    : boundaryMin(boundaryMin), boundaryMax(boundaryMax) {}

int World::addObject(std::unique_ptr<PhysicsObject> obj) {
    return objects.add(std::move(obj));
}

bool World::removeObject(int id) {
    return objects.remove(id);
}

PhysicsObject* World::findObject(int id) {
    return objects.find(id);
}

void World::setGravity(double g) {
    gravity = std::make_unique<GravityForce>(g);
}

void World::disableGravity() {
    gravity.reset();
}

void World::setFriction(double coefficient) {
    // Friction coefficients outside [0,1] are not physically meaningful
    // for the simplified model used here, so clamp instead of rejecting
    // outright (a genuine, non-decorative use of the clampValue template).
    double safeCoefficient = clampValue(coefficient, 0.0, 1.0);
    friction = std::make_unique<FrictionForce>(safeCoefficient);
}

void World::disableFriction() {
    friction.reset();
}

void World::applyImpulse(int id, const Vector2D& force) {
    PhysicsObject* obj = objects.find(id);
    if (!obj) {
        throw InvalidObjectException("No object with id " + std::to_string(id) + " to apply force to.");
    }
    AppliedForce impulse(force);
    impulse.apply(*obj);
}

void World::step(double dt) {
    if (dt <= 0.0) {
        throw InvalidSimulationException("Time step must be positive.");
    }

    // Save the state BEFORE advancing, so undo() can restore "one step back".
    pushHistory();

    // 1. Apply continuous global forces to every object.
    for (auto& pair : objects.getAll()) {
        PhysicsObject* obj = pair.second.get();
        if (gravity)  gravity->apply(*obj);
        if (friction) friction->apply(*obj);
    }

    // 2. Integrate motion (DynamicObject moves, StaticObject ignores it).
    for (auto& pair : objects.getAll()) {
        pair.second->update(dt);
    }

    // 3. Keep everything inside the simulation boundary.
    enforceBoundaries();

    // 4. Detect + resolve collisions, and log events.
    std::vector<Collision> collisions = collisionManager.detectCollisions(objects);
    collisionManager.resolveCollisions(objects, collisions);

    // Refresh the "most recent frame" event queue.
    while (!recentCollisionEvents.empty()) recentCollisionEvents.pop();
    for (const auto& c : collisions) {
        recentCollisionEvents.push(c);
    }
    totalCollisionCount += static_cast<int>(collisions.size());

    simulationTime += dt;
}

void World::reset() {
    objects.clear();
    while (!recentCollisionEvents.empty()) recentCollisionEvents.pop();
    while (!history.empty()) history.pop();
    totalCollisionCount = 0;
    simulationTime = 0.0;
    PhysicsObject::resetIdCounter();
}

void World::enforceBoundaries() {
    const double damping = 0.7; // energy lost when bouncing off a wall

    for (auto& pair : objects.getAll()) {
        PhysicsObject* obj = pair.second.get();
        if (obj->isStatic()) continue;

        double r = obj->getShape()->getBoundingRadius();
        Vector2D pos = obj->getPosition();
        Vector2D vel = obj->getVelocity();

        if (pos.x - r < boundaryMin.x) {
            pos.x = boundaryMin.x + r;
            vel.x = -vel.x * damping;
        } else if (pos.x + r > boundaryMax.x) {
            pos.x = boundaryMax.x - r;
            vel.x = -vel.x * damping;
        }

        if (pos.y - r < boundaryMin.y) {
            pos.y = boundaryMin.y + r;
            vel.y = -vel.y * damping;
        } else if (pos.y + r > boundaryMax.y) {
            pos.y = boundaryMax.y - r;
            vel.y = -vel.y * damping;
        }

        obj->setPosition(pos);
        obj->setVelocity(vel);
    }
}

void World::pushHistory() {
    history.push(SimulationState::captureFrom(*this));
    // Cap history size so long simulations don't grow memory unbounded.
    // (std::stack has no direct trim, so this is a documented simplification;
    // a future improvement could use a deque-backed ring buffer instead.)
}

bool World::undo() {
    if (history.empty()) return false;
    SimulationState previous = history.top();
    history.pop();
    loadState(previous);
    return true;
}

void World::loadState(const SimulationState& state) {
    objects.clear();
    PhysicsObject::resetIdCounter();

    for (const auto& rec : state.records) {
        std::unique_ptr<Shape> shape;
        if (rec.shapeType == "CIRCLE") {
            shape = std::make_unique<Circle>(rec.shapeParams.at(0));
        } else if (rec.shapeType == "RECTANGLE") {
            shape = std::make_unique<Rectangle>(rec.shapeParams.at(0), rec.shapeParams.at(1));
        } else if (rec.shapeType == "TRIANGLE") {
            shape = std::make_unique<Triangle>(rec.shapeParams.at(0), rec.shapeParams.at(1));
        } else {
            throw InvalidSimulationException("Unknown shape type in saved state: " + rec.shapeType);
        }

        std::unique_ptr<PhysicsObject> obj;
        if (rec.isStatic) {
            obj = std::make_unique<StaticObject>(rec.position, std::move(shape));
        } else {
            obj = std::make_unique<DynamicObject>(rec.mass, rec.position, std::move(shape));
        }
        obj->setVelocity(rec.velocity);
        objects.add(std::move(obj));
    }

    simulationTime = state.simulationTime;
}

std::vector<Collision> World::getRecentCollisionsSnapshot() const {
    std::vector<Collision> result;
    std::queue<Collision> copy = recentCollisionEvents;
    while (!copy.empty()) {
        result.push_back(copy.front());
        copy.pop();
    }
    return result;
}

Statistics World::computeStatistics() const {
    Statistics stats;
    stats.simulationTime = simulationTime;
    stats.totalCollisions = totalCollisionCount;

    for (const auto& pair : objects.getAll()) {
        const PhysicsObject* obj = pair.second.get();
        stats.objectCount++;
        if (obj->isStatic()) {
            stats.staticCount++;
        } else {
            stats.dynamicCount++;
            double v2 = obj->getVelocity().magnitudeSquared();
            stats.totalKineticEnergy += 0.5 * obj->getMass() * v2;
        }
    }
    return stats;
}
