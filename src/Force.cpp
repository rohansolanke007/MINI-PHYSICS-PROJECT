#include "Force.h"
#include "PhysicsObject.h"

// ----------------------- GravityForce -----------------------

GravityForce::GravityForce(double g) : g(g) {}

void GravityForce::apply(PhysicsObject& obj) const {
    if (obj.isStatic()) return; // gravity has no visible effect on immovable bodies
    // Fg = m * g, acting downward (negative Y)
    Vector2D gravityForce(0.0, -obj.getMass() * g);
    obj.applyForce(gravityForce);
}

// ----------------------- FrictionForce -----------------------

FrictionForce::FrictionForce(double coefficient) : coefficient(coefficient) {}

void FrictionForce::apply(PhysicsObject& obj) const {
    if (obj.isStatic()) return;
    Vector2D vel = obj.getVelocity();
    if (vel.magnitude() < 1e-6) return; // nothing to oppose

    // Simplified kinetic friction model: a force opposing the direction
    // of motion, proportional to normal force (approximated as m*g)
    // and the friction coefficient. This is intentionally simplified
    // (no real normal-force/contact computation) to stay within the
    // "mathematically understandable" scope of the project.
    const double gConst = 9.8;
    double frictionMag = coefficient * obj.getMass() * gConst;
    Vector2D frictionForce = vel.normalized() * (-frictionMag);
    obj.applyForce(frictionForce);
}

// ----------------------- AppliedForce -----------------------

AppliedForce::AppliedForce(const Vector2D& force) : force(force) {}

void AppliedForce::apply(PhysicsObject& obj) const {
    if (obj.isStatic()) return;
    obj.applyForce(force);
}
