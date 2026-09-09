#ifndef PHYSICS_OBJECT_H
#define PHYSICS_OBJECT_H

#include <memory>
#include <string>
#include "Vector2D.h"
#include "Shape.h"

// PhysicsObject (Abstract Base Class)
// ----------------------------------------------------------------
// Represents the PHYSICAL STATE of a body in the simulation:
// mass, position, velocity, acceleration and accumulated force.
//
// Deliberately does NOT inherit from Shape. Instead it HAS-A Shape
// (composition). This lets geometry and physics evolve independently:
// e.g. two very different physics behaviours (Dynamic vs Static) can
// each be paired with any of the three shape types.
//
// OOP concepts demonstrated:
//   - Encapsulation (private state, public controlled accessors)
//   - Static members (nextId, shared across all instances)
//   - Constructors validating input + throwing custom exceptions
//   - Abstract class / pure virtual function (update())
//   - Inheritance + runtime polymorphism (DynamicObject, StaticObject)
// ----------------------------------------------------------------
class PhysicsObject {
protected:
    static int nextId; // static member: shared ID counter across all objects

    int id;
    double mass;
    Vector2D position;
    Vector2D velocity;
    Vector2D acceleration;
    Vector2D forceAccumulator;
    std::unique_ptr<Shape> shape;

public:
    // mass == 0 is treated as a special "infinite mass / immovable" case,
    // used internally by StaticObject. Negative mass is always invalid.
    PhysicsObject(double mass, const Vector2D& position, std::unique_ptr<Shape> shape);
    virtual ~PhysicsObject() = default;

    // Disable copying: a PhysicsObject uniquely owns its Shape.
    PhysicsObject(const PhysicsObject&) = delete;
    PhysicsObject& operator=(const PhysicsObject&) = delete;

    void applyForce(const Vector2D& force);

    // Pure virtual: DynamicObject integrates motion, StaticObject ignores it.
    virtual void update(double dt) = 0;
    virtual bool isStatic() const = 0;
    virtual std::string getKindName() const = 0;

    int getId() const { return id; }
    double getMass() const { return mass; }
    Vector2D getPosition() const { return position; }
    Vector2D getVelocity() const { return velocity; }
    Vector2D getAcceleration() const { return acceleration; }
    Shape* getShape() const { return shape.get(); }

    void setPosition(const Vector2D& pos) { position = pos; }
    void setVelocity(const Vector2D& vel) { velocity = vel; }

    virtual void printInfo() const;

    static void resetIdCounter() { nextId = 1; } // useful when loading a file
};

class DynamicObject : public PhysicsObject {
public:
    DynamicObject(double mass, const Vector2D& position, std::unique_ptr<Shape> shape);
    void update(double dt) override; // Euler integration
    bool isStatic() const override { return false; }
    std::string getKindName() const override { return "Dynamic"; }
};

class StaticObject : public PhysicsObject {
public:
    // Static objects conceptually have infinite mass; we store mass=0
    // as the sentinel and never let them move.
    StaticObject(const Vector2D& position, std::unique_ptr<Shape> shape);
    void update(double dt) override; // no-op: static bodies never move
    bool isStatic() const override { return true; }
    std::string getKindName() const override { return "Static"; }
};

#endif // PHYSICS_OBJECT_H
