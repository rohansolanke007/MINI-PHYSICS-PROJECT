#include "Shape.h"
#include "Exceptions.h"
#include <iostream>
#include <cmath>

// ----------------------- Circle -----------------------

Circle::Circle(double radius) : radius(radius) {
    if (radius <= 0.0) {
        throw InvalidShapeException("Circle radius must be positive.");
    }
}

double Circle::getArea() const {
    return M_PI * radius * radius;
}

double Circle::getBoundingRadius() const {
    return radius;
}

ShapeType Circle::getType() const {
    return ShapeType::CIRCLE;
}

std::string Circle::getName() const {
    return "Circle";
}

void Circle::draw() const {
    std::cout << "  [Circle] radius=" << radius << "\n";
}

std::unique_ptr<Shape> Circle::clone() const {
    return std::make_unique<Circle>(radius);
}

// ----------------------- Rectangle -----------------------

Rectangle::Rectangle(double width, double height) : width(width), height(height) {
    if (width <= 0.0 || height <= 0.0) {
        throw InvalidShapeException("Rectangle width/height must be positive.");
    }
}

double Rectangle::getArea() const {
    return width * height;
}

double Rectangle::getBoundingRadius() const {
    // Half-diagonal: the smallest circle that fully encloses the rectangle.
    return std::sqrt((width / 2.0) * (width / 2.0) + (height / 2.0) * (height / 2.0));
}

ShapeType Rectangle::getType() const {
    return ShapeType::RECTANGLE;
}

std::string Rectangle::getName() const {
    return "Rectangle";
}

void Rectangle::draw() const {
    std::cout << "  [Rectangle] width=" << width << " height=" << height << "\n";
}

std::unique_ptr<Shape> Rectangle::clone() const {
    return std::make_unique<Rectangle>(width, height);
}

// ----------------------- Triangle -----------------------

Triangle::Triangle(double base, double height) : base(base), height(height) {
    if (base <= 0.0 || height <= 0.0) {
        throw InvalidShapeException("Triangle base/height must be positive.");
    }
}

double Triangle::getArea() const {
    return 0.5 * base * height;
}

double Triangle::getBoundingRadius() const {
    // Approximation: treat the triangle as roughly enclosed by a circle
    // whose radius is derived from half the longer of base/height.
    // This is a deliberate simplification (documented) so that
    // collision detection can use one uniform circle-based broad phase
    // for every shape type, instead of needing exact polygon collision
    // math, which is out of scope for this course project.
    double longerSide = (base > height) ? base : height;
    return longerSide / 2.0;
}

ShapeType Triangle::getType() const {
    return ShapeType::TRIANGLE;
}

std::string Triangle::getName() const {
    return "Triangle";
}

void Triangle::draw() const {
    std::cout << "  [Triangle] base=" << base << " height=" << height << "\n";
}

std::unique_ptr<Shape> Triangle::clone() const {
    return std::make_unique<Triangle>(base, height);
}
