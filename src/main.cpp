// ================================================================
// MINI PHYSICS ENGINE
// An OOP + STL based 2D Physics Simulation in C++
// ================================================================
// This file is intentionally "thin": it only handles console
// input/output and delegates all real work to PhysicsEngine/World.
// This keeps the UI layer separate from simulation logic, so the
// same World/PhysicsEngine classes could later be reused with a
// different UI (e.g. a graphical one) without modification.
// ================================================================

#include <iostream>
#include <iomanip>
#include <limits>
#include <vector>
#include <string>

#include "PhysicsEngine.h"
#include "Exceptions.h"
#include "FileHandler.h"
#include "MathUtils.h"
#include "TraceRecorder.h"

static void printMenu() {
    std::cout << "\n===== MINI PHYSICS ENGINE =====\n";
    std::cout << " 1. Create Object\n";
    std::cout << " 2. Remove Object\n";
    std::cout << " 3. Apply Force (one-time impulse)\n";
    std::cout << " 4. Set Gravity\n";
    std::cout << " 5. Set Friction\n";
    std::cout << " 6. Run Simulation Steps\n";
    std::cout << " 7. Pause / Resume Simulation\n";
    std::cout << " 8. View Objects\n";
    std::cout << " 9. View Recent Collision Events\n";
    std::cout << "10. Save Simulation\n";
    std::cout << "11. Load Simulation\n";
    std::cout << "12. Show Statistics\n";
    std::cout << "13. Undo Last Step\n";
    std::cout << "14. Record Simulation for Web Visualizer\n";
    std::cout << "15. Exit\n";
    std::cout << "Choose an option: ";
}

static int readInt() {
    int val;
    while (!(std::cin >> val)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter a number: ";
    }
    return val;
}

static double readDouble() {
    double val;
    while (!(std::cin >> val)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter a number: ";
    }
    return val;
}

static void createObjectFlow(PhysicsEngine& engine) {
    std::cout << "\n-- Create Object --\n";
    std::cout << "Kind: 1) Dynamic  2) Static : ";
    int kind = readInt();

    std::cout << "Shape: 1) Circle  2) Rectangle  3) Triangle : ";
    int shapeChoice = readInt();

    std::cout << "Position x y: ";
    double px = readDouble(), py = readDouble();

    try {
        std::unique_ptr<Shape> shape;
        if (shapeChoice == 1) {
            std::cout << "Radius: ";
            double r = readDouble();
            shape = std::make_unique<Circle>(r);
        } else if (shapeChoice == 2) {
            std::cout << "Width Height: ";
            double w = readDouble(), h = readDouble();
            shape = std::make_unique<Rectangle>(w, h);
        } else if (shapeChoice == 3) {
            std::cout << "Base Height: ";
            double b = readDouble(), h = readDouble();
            shape = std::make_unique<Triangle>(b, h);
        } else {
            std::cout << "Invalid shape choice.\n";
            return;
        }

        std::unique_ptr<PhysicsObject> obj;
        if (kind == 1) {
            std::cout << "Mass (kg): ";
            double mass = readDouble();
            obj = std::make_unique<DynamicObject>(mass, Vector2D(px, py), std::move(shape));
        } else if (kind == 2) {
            obj = std::make_unique<StaticObject>(Vector2D(px, py), std::move(shape));
        } else {
            std::cout << "Invalid kind choice.\n";
            return;
        }

        int id = engine.getWorld().addObject(std::move(obj));
        std::cout << "Created object with ID " << id << ".\n";

    } catch (const PhysicsException& e) {
        // Demonstrates catching our custom exception hierarchy by base
        // reference - works for InvalidMassException, InvalidShapeException,
        // InvalidObjectException, etc. without needing a catch per type.
        std::cout << "Error creating object: " << e.what() << "\n";
    }
}

static void removeObjectFlow(PhysicsEngine& engine) {
    std::cout << "Enter object ID to remove: ";
    int id = readInt();
    if (engine.getWorld().removeObject(id)) {
        std::cout << "Object " << id << " removed.\n";
    } else {
        std::cout << "No object with ID " << id << " found.\n";
    }
}

static void applyForceFlow(PhysicsEngine& engine) {
    std::cout << "Enter object ID: ";
    int id = readInt();
    std::cout << "Force x y: ";
    double fx = readDouble(), fy = readDouble();
    try {
        engine.getWorld().applyImpulse(id, Vector2D(fx, fy));
        std::cout << "Force applied to object " << id << ".\n";
    } catch (const PhysicsException& e) {
        std::cout << "Error applying force: " << e.what() << "\n";
    }
}

static void setGravityFlow(PhysicsEngine& engine) {
    std::cout << "Enter gravity acceleration (0 to disable, default 9.8): ";
    double g = readDouble();
    if (g == 0.0) {
        engine.getWorld().disableGravity();
        std::cout << "Gravity disabled.\n";
    } else {
        engine.getWorld().setGravity(g);
        std::cout << "Gravity set to " << g << " m/s^2.\n";
    }
}

static void setFrictionFlow(PhysicsEngine& engine) {
    std::cout << "Enter friction coefficient (0 to disable, typical 0.0-1.0): ";
    double c = readDouble();
    if (c == 0.0) {
        engine.getWorld().disableFriction();
        std::cout << "Friction disabled.\n";
    } else {
        engine.getWorld().setFriction(c);
        std::cout << "Friction coefficient set to " << c << ".\n";
    }
}

static void runSimulationFlow(PhysicsEngine& engine) {
    std::cout << "How many steps to run? ";
    int steps = readInt();
    try {
        int executed = engine.runSteps(steps);
        if (engine.isPaused()) {
            std::cout << "Simulation is paused. No steps executed.\n";
        } else {
            std::cout << "Executed " << executed << " step(s). Simulation time = "
                      << engine.getWorld().getSimulationTime() << " s\n";
        }
    } catch (const PhysicsException& e) {
        std::cout << "Simulation error: " << e.what() << "\n";
    }
}

static void togglePauseFlow(PhysicsEngine& engine) {
    if (engine.isPaused()) {
        engine.resume();
        std::cout << "Simulation resumed.\n";
    } else {
        engine.pause();
        std::cout << "Simulation paused.\n";
    }
}

static void viewObjectsFlow(PhysicsEngine& engine) {
    const auto& all = engine.getWorld().getObjects().getAll();
    if (all.empty()) {
        std::cout << "No objects in the simulation.\n";
        return;
    }

    std::cout << std::fixed << std::setprecision(2);
    std::vector<double> speeds;
    for (const auto& pair : all) {
        pair.second->printInfo();
        speeds.push_back(pair.second->getVelocity().magnitude());
    }
    // Genuine use of the average<T> function template.
    std::cout << "Average speed across all objects: " << average(speeds) << " m/s\n";
}

static void viewCollisionsFlow(PhysicsEngine& engine) {
    auto collisions = engine.getWorld().getRecentCollisionsSnapshot();
    if (collisions.empty()) {
        std::cout << "No collisions in the most recent step.\n";
        return;
    }
    std::cout << "Recent collisions:\n";
    for (const auto& c : collisions) {
        std::cout << "  Object " << c.idA << " <-> Object " << c.idB
                  << " (penetration=" << c.penetration << ")\n";
    }
}

static void saveSimulationFlow(PhysicsEngine& engine) {
    std::cout << "Enter filename to save (e.g. data/save1.txt): ";
    std::string filename;
    std::cin >> filename;
    try {
        SimulationState state = SimulationState::captureFrom(engine.getWorld());
        FileHandler::saveToFile(state, filename);
        std::cout << "Simulation saved to " << filename << "\n";
    } catch (const PhysicsException& e) {
        std::cout << "Error saving simulation: " << e.what() << "\n";
    }
}

static void loadSimulationFlow(PhysicsEngine& engine) {
    std::cout << "Enter filename to load (e.g. data/save1.txt): ";
    std::string filename;
    std::cin >> filename;
    try {
        SimulationState state = FileHandler::loadFromFile(filename);
        engine.getWorld().loadState(state);
        std::cout << "Simulation loaded from " << filename << "\n";
    } catch (const PhysicsException& e) {
        // Demonstrates stack unwinding: the exception can be thrown deep
        // inside FileHandler::loadFromFile or World::loadState (e.g. from
        // a Shape constructor while reconstructing an object) and is
        // still caught cleanly here at the UI boundary.
        std::cout << "Error loading simulation: " << e.what() << "\n";
    }
}

static void showStatisticsFlow(PhysicsEngine& engine) {
    Statistics stats = engine.getWorld().computeStatistics();
    stats.print();
}

static void recordTraceFlow(PhysicsEngine& engine) {
    if (engine.getWorld().getObjects().size() == 0) {
        std::cout << "No objects in the simulation - create some objects first.\n";
        return;
    }

    std::cout << "How many steps to record? ";
    int steps = readInt();
    std::cout << "Enter filename to export (e.g. web/trace.json): ";
    std::string filename;
    std::cin >> filename;

    try {
        TraceRecorder recorder(engine.getTimeStep(),
                                engine.getWorld().getBoundaryMin(),
                                engine.getWorld().getBoundaryMax());

        // Record the CURRENT state as frame 0, then advance + record
        // after every subsequent step, so the very first frame the web
        // player shows matches exactly what "View Objects" showed here.
        recorder.recordFrame(engine.getWorld());
        for (int i = 0; i < steps; ++i) {
            engine.getWorld().step(engine.getTimeStep());
            recorder.recordFrame(engine.getWorld());
        }

        recorder.exportToJSON(filename);
        std::cout << "Recorded " << recorder.frameCount() << " frames to " << filename << "\n";
        std::cout << "Open web/index.html in a browser and load this file to play it back.\n";

    } catch (const PhysicsException& e) {
        std::cout << "Error recording trace: " << e.what() << "\n";
    }
}

static void undoFlow(PhysicsEngine& engine) {
    if (engine.getWorld().undo()) {
        std::cout << "Reverted to previous state. Simulation time = "
                  << engine.getWorld().getSimulationTime() << " s\n";
    } else {
        std::cout << "Nothing to undo.\n";
    }
}

int main() {
    std::cout << "Welcome to the Mini Physics Engine (2D OOP simulation)\n";

    // Simulation boundary: a 100 x 60 unit "box" the objects live inside.
    PhysicsEngine engine(Vector2D(0, 0), Vector2D(100, 60), 0.05);

    bool exit = false;
    while (!exit) {
        printMenu();
        int choice = readInt();

        switch (choice) {
            case 1:  createObjectFlow(engine); break;
            case 2:  removeObjectFlow(engine); break;
            case 3:  applyForceFlow(engine); break;
            case 4:  setGravityFlow(engine); break;
            case 5:  setFrictionFlow(engine); break;
            case 6:  runSimulationFlow(engine); break;
            case 7:  togglePauseFlow(engine); break;
            case 8:  viewObjectsFlow(engine); break;
            case 9:  viewCollisionsFlow(engine); break;
            case 10: saveSimulationFlow(engine); break;
            case 11: loadSimulationFlow(engine); break;
            case 12: showStatisticsFlow(engine); break;
            case 13: undoFlow(engine); break;
            case 14: recordTraceFlow(engine); break;
            case 15: exit = true; break;
            default: std::cout << "Invalid option, try again.\n"; break;
        }
    }

    std::cout << "Exiting Mini Physics Engine. Goodbye!\n";
    return 0;
}
