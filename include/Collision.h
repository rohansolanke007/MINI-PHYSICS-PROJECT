#ifndef COLLISION_H
#define COLLISION_H

#include <vector>
#include <set>
#include <utility>
#include "Vector2D.h"
#include "ObjectManager.h"
#include "PhysicsObject.h"

// Collision
// ----------------------------------------------------------------
// A plain data record describing one collision event between two
// objects. Kept deliberately simple (no behaviour) - it is produced
// by CollisionManager and consumed by World/Statistics/UI.
// ----------------------------------------------------------------
struct Collision {
    int idA;
    int idB;
    Vector2D normal;      // direction from A to B, unit length
    double penetration;   // how much the two bounding circles overlap
};

// CollisionManager
// ----------------------------------------------------------------
// Detects and resolves collisions between all active PhysicsObjects.
//
// Design simplification (documented, not accidental): rather than
// implementing exact polygon-vs-polygon collision math for every
// Circle/Rectangle/Triangle combination - which is real geometry
// research, not a second-year OOP exercise - every shape exposes
// getBoundingRadius(), and collisions are detected/resolved using
// circle-circle math against those bounding radii. This keeps the
// physics "mathematically understandable" while still supporting
// any mix of shape types.
//
// OOP concepts demonstrated:
//   - STL usage: std::vector<Collision>, std::set<pair<int,int>>
//   - Works purely through the PhysicsObject public interface
//     (encapsulation - it never touches shape internals directly)
// ----------------------------------------------------------------
class CollisionManager {
public:
    std::vector<Collision> detectCollisions(const ObjectManager<PhysicsObject>& objects);
    void resolveCollisions(ObjectManager<PhysicsObject>& objects, const std::vector<Collision>& collisions);
};

#endif // COLLISION_H
