#include "SimulationState.h"
#include "World.h"
#include "Exceptions.h"

SimulationState SimulationState::captureFrom(const World& world) {
    SimulationState state;
    state.simulationTime = world.getSimulationTime();

    for (const auto& pair : world.getObjects().getAll()) {
        const PhysicsObject* obj = pair.second.get();
        ObjectRecord rec;
        rec.id = obj->getId();
        rec.isStatic = obj->isStatic();
        rec.mass = obj->getMass();
        rec.position = obj->getPosition();
        rec.velocity = obj->getVelocity();

        const Shape* shape = obj->getShape();
        switch (shape->getType()) {
            case ShapeType::CIRCLE: {
                // Downcasting: we know (via getType()) that this Shape*
                // is really a Circle*, so a safe static_cast retrieves
                // the shape-specific dimension we need to serialize.
                const Circle* c = static_cast<const Circle*>(shape);
                rec.shapeType = "CIRCLE";
                rec.shapeParams = { c->getRadius() };
                break;
            }
            case ShapeType::RECTANGLE: {
                const Rectangle* r = static_cast<const Rectangle*>(shape);
                rec.shapeType = "RECTANGLE";
                rec.shapeParams = { r->getWidth(), r->getHeight() };
                break;
            }
            case ShapeType::TRIANGLE: {
                const Triangle* t = static_cast<const Triangle*>(shape);
                rec.shapeType = "TRIANGLE";
                rec.shapeParams = { t->getBase(), t->getHeight() };
                break;
            }
            default:
                throw InvalidSimulationException("Unknown shape type encountered while capturing state.");
        }

        state.records.push_back(rec);
    }

    return state;
}
