#ifndef SHAPE_H
#define SHAPE_H

#include <string>
#include <memory>

// Used so CollisionManager and SimulationState can identify a shape's
// concrete type without repeated dynamic_cast chains everywhere.
enum class ShapeType { CIRCLE, RECTANGLE, TRIANGLE };

// Shape (Abstract Base Class)
// ----------------------------------------------------------------
// Represents pure GEOMETRY only - no mass, velocity or physics state.
// This is deliberately separate from PhysicsObject: a PhysicsObject
// "has-a" Shape (composition), it does not "is-a" Shape.
//
// OOP concepts demonstrated:
//   - Abstract classes / pure virtual functions
//   - Runtime polymorphism (getArea(), draw() dispatched via base ptr)
//   - Inheritance (Circle, Rectangle, Triangle)
// ----------------------------------------------------------------
class Shape {
public:
    virtual ~Shape() = default;

    virtual double getArea() const = 0;
    virtual double getBoundingRadius() const = 0; // used for broad-phase collision
    virtual ShapeType getType() const = 0;
    virtual std::string getName() const = 0;
    virtual void draw() const = 0; // simple textual representation

    // Deep-copy support. Needed because PhysicsObject owns a
    // std::unique_ptr<Shape> and unique_ptr cannot be copied directly;
    // clone() lets us duplicate a PhysicsObject's shape when needed
    // (e.g. when reconstructing objects while loading a saved file).
    virtual std::unique_ptr<Shape> clone() const = 0;
};

class Circle : public Shape {
private:
    double radius;
public:
    explicit Circle(double radius);
    double getArea() const override;
    double getBoundingRadius() const override;
    ShapeType getType() const override;
    std::string getName() const override;
    void draw() const override;
    std::unique_ptr<Shape> clone() const override;

    double getRadius() const { return radius; }
};

class Rectangle : public Shape {
private:
    double width;
    double height;
public:
    Rectangle(double width, double height);
    double getArea() const override;
    double getBoundingRadius() const override;
    ShapeType getType() const override;
    std::string getName() const override;
    void draw() const override;
    std::unique_ptr<Shape> clone() const override;

    double getWidth() const { return width; }
    double getHeight() const { return height; }
};

class Triangle : public Shape {
private:
    double base;
    double height;
public:
    Triangle(double base, double height);
    double getArea() const override;
    double getBoundingRadius() const override;
    ShapeType getType() const override;
    std::string getName() const override;
    void draw() const override;
    std::unique_ptr<Shape> clone() const override;

    double getBase() const { return base; }
    double getHeight() const { return height; }
};

#endif // SHAPE_H
