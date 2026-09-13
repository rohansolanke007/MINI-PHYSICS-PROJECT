#include "Vector2D.h"

Vector2D::Vector2D(double x, double y) : x(x), y(y) {}

Vector2D Vector2D::operator+(const Vector2D& other) const {
    return Vector2D(x + other.x, y + other.y);
}

Vector2D Vector2D::operator-(const Vector2D& other) const {
    return Vector2D(x - other.x, y - other.y);
}

Vector2D Vector2D::operator*(double scalar) const {
    return Vector2D(x * scalar, y * scalar);
}

Vector2D Vector2D::operator/(double scalar) const {
    // Note: division by zero is not thrown as a custom exception here
    // because it is a math-layer concern, not a simulation-data concern.
    // Callers (PhysicsObject) are responsible for ensuring mass != 0
    // before dividing force by mass.
    return Vector2D(x / scalar, y / scalar);
}

bool Vector2D::operator==(const Vector2D& other) const {
    const double EPS = 1e-9;
    return std::fabs(x - other.x) < EPS && std::fabs(y - other.y) < EPS;
}

bool Vector2D::operator!=(const Vector2D& other) const {
    return !(*this == other);
}

Vector2D& Vector2D::operator+=(const Vector2D& other) {
    x += other.x;
    y += other.y;
    return *this;
}

double Vector2D::magnitudeSquared() const {
    return x * x + y * y;
}

double Vector2D::magnitude() const {
    return std::sqrt(magnitudeSquared());
}

double Vector2D::distanceTo(const Vector2D& other) const {
    return (*this - other).magnitude();
}

Vector2D Vector2D::normalized() const {
    double m = magnitude();
    if (m < 1e-12) return Vector2D(0, 0);
    return Vector2D(x / m, y / m);
}

std::ostream& operator<<(std::ostream& out, const Vector2D& v) {
    out << v.x << " " << v.y;
    return out;
}

std::istream& operator>>(std::istream& in, Vector2D& v) {
    in >> v.x >> v.y;
    return in;
}
