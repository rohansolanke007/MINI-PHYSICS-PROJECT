#include "Collision.h"

std::vector<Collision> CollisionManager::detectCollisions(const ObjectManager<PhysicsObject>& objects) {
    std::vector<Collision> results;

    // std::set is used here to guarantee that each unordered pair of
    // object IDs is only ever tested/reported once per frame, even
    // though our loop structure already avoids duplicates by
    // construction (i < j) - this makes the "no duplicate pair"
    // invariant explicit and easy to defend in viva.
    std::set<std::pair<int, int>> testedPairs;

    const auto& all = objects.getAll();
    for (auto itA = all.begin(); itA != all.end(); ++itA) {
        for (auto itB = std::next(itA); itB != all.end(); ++itB) {
            PhysicsObject* a = itA->second.get();
            PhysicsObject* b = itB->second.get();

            int idA = a->getId();
            int idB = b->getId();
            std::pair<int, int> key = (idA < idB) ? std::make_pair(idA, idB) : std::make_pair(idB, idA);

            if (testedPairs.count(key)) continue;
            testedPairs.insert(key);

            // Two static objects can never meaningfully "collide" for
            // our purposes (neither can move), so skip that case.
            if (a->isStatic() && b->isStatic()) continue;

            double dist = a->getPosition().distanceTo(b->getPosition());
            double combinedRadius = a->getShape()->getBoundingRadius() + b->getShape()->getBoundingRadius();

            if (dist < combinedRadius) {
                Collision c;
                c.idA = idA;
                c.idB = idB;
                Vector2D diff = b->getPosition() - a->getPosition();
                c.normal = (dist > 1e-9) ? diff.normalized() : Vector2D(1, 0);
                c.penetration = combinedRadius - dist;
                results.push_back(c);
            }
        }
    }
    return results;
}

void CollisionManager::resolveCollisions(ObjectManager<PhysicsObject>& objects, const std::vector<Collision>& collisions) {
    for (const auto& c : collisions) {
        PhysicsObject* a = objects.find(c.idA);
        PhysicsObject* b = objects.find(c.idB);
        if (!a || !b) continue;

        // 1. Positional correction: push the two shapes apart along the
        //    collision normal so they no longer overlap. A static object
        //    does not move, so a dynamic object colliding with a static
        //    one is pushed out fully; two dynamic objects share the
        //    correction equally.
        if (!a->isStatic() && !b->isStatic()) {
            Vector2D correction = c.normal * (c.penetration / 2.0);
            a->setPosition(a->getPosition() - correction);
            b->setPosition(b->getPosition() + correction);
        } else if (a->isStatic() && !b->isStatic()) {
            b->setPosition(b->getPosition() + c.normal * c.penetration);
        } else if (!a->isStatic() && b->isStatic()) {
            a->setPosition(a->getPosition() - c.normal * c.penetration);
        }

        // 2. Simple velocity response along the normal (basic elastic-ish
        //    bounce). This is a simplified impulse model - not a full
        //    conservation-of-momentum solver - kept intentionally
        //    understandable for a second-year course project.
        const double restitution = 0.6; // 0 = fully inelastic, 1 = fully elastic

        if (!a->isStatic() && !b->isStatic()) {
            Vector2D relVel = b->getVelocity() - a->getVelocity();
            double velAlongNormal = relVel.x * c.normal.x + relVel.y * c.normal.y;
            if (velAlongNormal > 0) continue; // already separating

            double impulseMag = -(1 + restitution) * velAlongNormal / 2.0;
            Vector2D impulse = c.normal * impulseMag;
            a->setVelocity(a->getVelocity() - impulse);
            b->setVelocity(b->getVelocity() + impulse);
        } else if (a->isStatic() && !b->isStatic()) {
            double velAlongNormal = b->getVelocity().x * c.normal.x + b->getVelocity().y * c.normal.y;
            if (velAlongNormal < 0) {
                Vector2D reflected = b->getVelocity() - c.normal * ((1 + restitution) * velAlongNormal);
                b->setVelocity(reflected);
            }
        } else if (!a->isStatic() && b->isStatic()) {
            double velAlongNormal = a->getVelocity().x * (-c.normal.x) + a->getVelocity().y * (-c.normal.y);
            if (velAlongNormal < 0) {
                Vector2D reflected = a->getVelocity() - (c.normal * -1.0) * ((1 + restitution) * velAlongNormal);
                a->setVelocity(reflected);
            }
        }
    }
}
