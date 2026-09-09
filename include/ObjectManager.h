#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include <map>
#include <memory>
#include <utility>

// ObjectManager<T> (Class Template)
// ----------------------------------------------------------------
// A small, generic registry that owns objects of type T by ID.
// T is required to expose: int getId() const;
//
// This is intentionally a REUSABLE utility rather than something
// tied only to PhysicsObject: World uses ObjectManager<PhysicsObject>
// for the active bodies, which is exactly the kind of genuine,
// non-decorative template usage this project aims for.
//
// OOP concepts demonstrated:
//   - Class templates
//   - STL (wraps std::map)
//   - Dynamic memory management via std::unique_ptr
// ----------------------------------------------------------------
template <typename T>
class ObjectManager {
private:
    std::map<int, std::unique_ptr<T>> objects;

public:
    // Takes ownership of obj; returns the id it was stored under.
    int add(std::unique_ptr<T> obj) {
        int id = obj->getId();
        objects[id] = std::move(obj);
        return id;
    }

    bool remove(int id) {
        return objects.erase(id) > 0;
    }

    T* find(int id) {
        auto it = objects.find(id);
        return (it != objects.end()) ? it->second.get() : nullptr;
    }

    const T* find(int id) const {
        auto it = objects.find(id);
        return (it != objects.end()) ? it->second.get() : nullptr;
    }

    const std::map<int, std::unique_ptr<T>>& getAll() const {
        return objects;
    }

    size_t size() const {
        return objects.size();
    }

    void clear() {
        objects.clear();
    }
};

#endif // OBJECT_MANAGER_H
