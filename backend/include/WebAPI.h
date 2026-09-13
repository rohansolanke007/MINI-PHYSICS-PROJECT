#ifndef WEB_API_H
#define WEB_API_H

#include <string>

// Browser-facing facade for the existing C++ physics engine.
// The console application remains unchanged; this API is compiled
// separately to WebAssembly with Emscripten and called by web/app.js.
namespace WebAPI {
    void reset();
    void pause();
    void resume();
    bool isPaused();

    void setTimeStep(double dt);
    double getTimeStep();

    void setGravity(double g);
    void disableGravity();
    void setFriction(double coefficient);
    void disableFriction();

    int createCircle(bool isStatic, double mass, double x, double y,
                     double radius, double vx, double vy);
    int createRectangle(bool isStatic, double mass, double x, double y,
                        double width, double height, double vx, double vy);
    int createTriangle(bool isStatic, double mass, double x, double y,
                       double base, double height, double vx, double vy);

    bool removeObject(int id);
    void setObjectPosition(int id, double x, double y);
    void setObjectVelocity(int id, double vx, double vy);
    void applyImpulse(int id, double fx, double fy);

    void step(int count = 1);
    bool undo();

    // Returns a compact JSON snapshot of the CURRENT C++ World state.
    // JSON is transport only; it is not a recorded trace and the browser
    // never uses it to simulate physics.
    std::string getStateJSON();
}

#endif // WEB_API_H
