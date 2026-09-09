#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

// PhysicsException hierarchy
// ----------------------------------------------------------------
// OOP concepts demonstrated:
//   - Inheritance (all custom exceptions derive from PhysicsException,
//     which itself derives from std::exception)
//   - Polymorphism (what() is virtual and overridden implicitly by
//     reusing the base implementation; each subclass just sets a
//     different message, and callers can catch by base reference)
//   - Custom exception classes (syllabus requirement)
// ----------------------------------------------------------------
class PhysicsException : public std::exception {
protected:
    std::string message;
public:
    explicit PhysicsException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class InvalidMassException : public PhysicsException {
public:
    explicit InvalidMassException(const std::string& msg)
        : PhysicsException("InvalidMassException: " + msg) {}
};

class InvalidShapeException : public PhysicsException {
public:
    explicit InvalidShapeException(const std::string& msg)
        : PhysicsException("InvalidShapeException: " + msg) {}
};

class InvalidObjectException : public PhysicsException {
public:
    explicit InvalidObjectException(const std::string& msg)
        : PhysicsException("InvalidObjectException: " + msg) {}
};

class FileException : public PhysicsException {
public:
    explicit FileException(const std::string& msg)
        : PhysicsException("FileException: " + msg) {}
};

class InvalidSimulationException : public PhysicsException {
public:
    explicit InvalidSimulationException(const std::string& msg)
        : PhysicsException("InvalidSimulationException: " + msg) {}
};

#endif // EXCEPTIONS_H
