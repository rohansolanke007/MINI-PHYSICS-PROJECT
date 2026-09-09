#include "PhysicsObject.h"
#include "Exceptions.h"
#include <iostream>

int PhysicsObject::nextId = 1;

PhysicsObject::PhysicsObject(double mass, const Vector2D& position, std::unique_ptr<Shape> shape)
    : id(nextId++), mass(mass), position(position),
      velocity(0, 0), acceleration(0, 0), forceAccumulator(0, 0),
      shape(std::move(shape))
{
    if (mass < 0.0) {
        throw InvalidMassException("Mass cannot be negative (object id " + std::to_string(id) + ").");
    }
    if (!this->shape) {
        throw InvalidObjectException("PhysicsObject created without a valid Shape.");
    }
}

void PhysicsObject::applyForce(const Vector2D& force) {
    forceAccumulator += force;
}

void PhysicsObject::printInfo() const {
    std::cout << "Object #" << id << " [" << getKindName() << "] "
              << shape->getName()
              << " | mass=" << mass
              << " | pos=(" << position.x << ", " << position.y << ")"
              << " | vel=(" << velocity.x << ", " << velocity.y << ")\n";
}

// ----------------------- DynamicObject -----------------------

DynamicObject::DynamicObject(double mass, const Vector2D& position, std::unique_ptr<Shape> shape)
    : PhysicsObject(mass, position, std::move(shape))
{
    if (mass <= 0.0) {
        throw InvalidMassException("Dynamic objects must have strictly positive mass.");
    }
}

void DynamicObject::update(double dt) {
    // Newton's second law: a = F / m
    acceleration = forceAccumulator / mass;

    // Euler integration:
    //   v = u + a*t
    //   x = x + v*t
    velocity += acceleration * dt;
    position += velocity * dt;

    // Forces are per-frame (recomputed every step by the World), so
    // the accumulator must be cleared after being consumed.
    forceAccumulator = Vector2D(0, 0);
}

// ----------------------- StaticObject -----------------------

StaticObject::StaticObject(const Vector2D& position, std::unique_ptr<Shape> shape)
    : PhysicsObject(0.0, position, std::move(shape))
{
    // mass = 0 is the internal sentinel for "infinite mass / immovable".
}

void StaticObject::update(double /*dt*/) {
    // Static bodies (e.g. floors, walls) never move, regardless of
    // any force applied to them. Any accumulated force is simply discarded.
    forceAccumulator = Vector2D(0, 0);
}
