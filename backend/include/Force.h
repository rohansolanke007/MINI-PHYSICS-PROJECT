#ifndef FORCE_H
#define FORCE_H

#include <string>
#include "Vector2D.h"

class PhysicsObject; // forward declaration - Force only needs a reference/pointer

// Force (Abstract Base Class)
// ----------------------------------------------------------------
// Represents anything that can push/pull a PhysicsObject.
// Each concrete Force knows HOW to compute and apply itself to a
// given object; the caller (World) does not need to know the details.
//
// OOP concepts demonstrated:
//   - Abstract class / pure virtual apply()
//   - Runtime polymorphism (World stores Force* / holds them uniformly)
//   - Inheritance (GravityForce, FrictionForce, AppliedForce)
// ----------------------------------------------------------------
class Force {
public:
    virtual ~Force() = default;
    virtual void apply(PhysicsObject& obj) const = 0;
    virtual std::string getName() const = 0;
};

class GravityForce : public Force {
private:
    double g; // gravitational acceleration (m/s^2), e.g. 9.8
public:
    explicit GravityForce(double g = 9.8);
    void apply(PhysicsObject& obj) const override;
    std::string getName() const override { return "Gravity"; }
    double getG() const { return g; }
};

class FrictionForce : public Force {
private:
    double coefficient; // simplified friction coefficient (0..1)
public:
    explicit FrictionForce(double coefficient = 0.1);
    void apply(PhysicsObject& obj) const override;
    std::string getName() const override { return "Friction"; }
    double getCoefficient() const { return coefficient; }
};

class AppliedForce : public Force {
private:
    Vector2D force;
public:
    explicit AppliedForce(const Vector2D& force);
    void apply(PhysicsObject& obj) const override;
    std::string getName() const override { return "AppliedForce"; }
};

#endif // FORCE_H
