# Mini Physics Project

A 2D physics simulation engine developed in **C++ using Object-Oriented Programming**, with an interactive **React + TypeScript web interface** powered by **WebAssembly**.

The project demonstrates how an object-oriented C++ physics engine can be compiled to WebAssembly and integrated with a modern web frontend to create an interactive physics laboratory.

---

## 🚀 Project Overview

This project consists of three major layers:

```text
┌─────────────────────────────────────────────┐
│             React + TypeScript              │
│              Web Interface                  │
│                                             │
│  Controls • Experiments • Visualization    │
└──────────────────────┬──────────────────────┘
                       │
                       │ JavaScript API
                       ▼
┌─────────────────────────────────────────────┐
│                 WebAssembly                 │
│                                             │
│        Compiled C++ Physics Engine          │
└──────────────────────┬──────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────┐
│              C++ Physics Engine             │
│                                             │
│  World • Objects • Forces • Collisions      │
│  Shapes • Simulation • Statistics • Undo    │
└─────────────────────────────────────────────┘

The C++ engine performs the actual physics calculations, while the React frontend provides an interactive interface for controlling and visualizing the simulation.
✨ Key Features
- 2D physics simulation
- Object-oriented C++ architecture
- Dynamic and static physics objects
- Circle, rectangle, and triangle shapes
- Gravity simulation
- Friction simulation
- Collision detection and resolution
- Elastic collision response using restitution
- Boundary collision handling
- Velocity and impulse control
- Pause and resume simulation
- Adjustable simulation timestep
- Object creation and removal
- Undo functionality
- Simulation statistics
- Collision tracking
- JSON-based state transfer
- WebAssembly integration
- Interactive React + TypeScript interface
- Physics experiments for demonstrating physical laws
🏗️ System Architecture
The complete application follows this architecture:
                         USER
                           │
                           ▼
              ┌────────────────────────┐
              │   React + TypeScript   │
              │      Frontend          │
              └────────────┬───────────┘
                           │
                           │ JavaScript API
                           ▼
              ┌────────────────────────┐
              │      WebAssembly       │
              │    Emscripten Layer    │
              └────────────┬───────────┘
                           │
                           ▼
              ┌────────────────────────┐
              │     WebAPI.cpp/.h     │
              │   Browser Facade API   │
              └────────────┬───────────┘
                           │
                           ▼
              ┌────────────────────────┐
              │    PhysicsEngine       │
              └────────────┬───────────┘
                           │
                           ▼
              ┌────────────────────────┐
              │         World          │
              └────────────┬───────────┘
                           │
          ┌────────────────┼────────────────┐
          │                │                │
          ▼                ▼                ▼
   ObjectManager     Force System    CollisionManager
          │                │                │
          ▼                ▼                ▼
   Physics Objects    Gravity/Friction   Collision
   Circle             Forces             Detection
   Rectangle                             Resolution
   Triangle
🔄 Application Data Flow
The application follows a clear data flow between the frontend and physics engine.
User Interaction
       │
       ▼
React UI
       │
       │ createObject()
       │ applyImpulse()
       │ setVelocity()
       │ setPosition()
       ▼
WebAssembly API
       │
       ▼
C++ Physics Engine
       │
       ▼
World
       │
       ├── Physics Objects
       ├── Forces
       ├── Collision Detection
       ├── Collision Resolution
       └── Boundary Handling
       │
       ▼
Simulation State
       │
       │ getStateJSON()
       ▼
WebAssembly
       │
       ▼
React Frontend
       │
       ▼
Canvas Visualization
⚙️ Simulation Loop
The simulation advances in discrete timesteps.
For every simulation step:
1. Apply forces
        ↓
2. Calculate acceleration
        ↓
3. Integrate velocity
        ↓
4. Integrate position
        ↓
5. Detect collisions
        ↓
6. Resolve collisions
        ↓
7. Enforce boundaries
        ↓
8. Update statistics/state
        ↓
9. Return state to frontend
The engine uses Euler integration for dynamic objects.
The basic integration is:
v = v + a × dt

position = position + v × dt
where:
- v = velocity
- a = acceleration
- dt = simulation timestep
📁 Project Structure
MINI-PHYSICS-PROJECT/
│
├── README.md
├── .gitignore
│
├── backend/
│   │
│   ├── src/
│   │   ├── Collision.cpp
│   │   ├── Collision.h
│   │   ├── Exceptions.h
│   │   ├── FileHandler.cpp
│   │   ├── Force.cpp
│   │   ├── Force.h
│   │   ├── MathUtils.h
│   │   ├── ObjectManager.h
│   │   ├── PhysicsEngine.cpp
│   │   ├── PhysicsEngine.h
│   │   ├── PhysicsObject.cpp
│   │   ├── PhysicsObject.h
│   │   ├── Shape.cpp
│   │   ├── Shape.h
│   │   ├── SimulationState.cpp
│   │   ├── SimulationState.h
│   │   ├── Statistics.h
│   │   ├── TraceRecorder.cpp
│   │   ├── TraceRecorder.h
│   │   ├── Vector2D.cpp
│   │   ├── Vector2D.h
│   │   ├── WebAPI.cpp
│   │   ├── WebAPI.h
│   │   ├── World.cpp
│   │   ├── World.h
│   │   └── main.cpp
│   │
│   ├── web/
│   │   ├── index.html
│   │   ├── physics_engine.js
│   │   └── physics_engine.wasm
│   │
│   ├── build_web.sh
│   └── README.md
│
└── frontend/
    │
    ├── public/
    │   ├── physics_engine.js
    │   ├── physics_engine.wasm
    │   └── physics-lab.svg
    │
    ├── src/
    │   ├── components/
    │   ├── lib/
    │   ├── routes/
    │   ├── experiments.ts
    │   ├── format.ts
    │   ├── scenes.ts
    │   └── ...
    │
    ├── package.json
    ├── package-lock.json
    ├── tsconfig.json
    ├── vite.config.ts
    └── ...
🧠 Physics Engine Architecture
The physics engine follows an object-oriented architecture.
Core Classes
Class	Responsibility
PhysicsEngine	Controls the simulation and timestep
World	Owns and manages the simulation world
PhysicsObject	Abstract base class for physics objects
DynamicObject	Represents moving objects
StaticObject	Represents immovable objects
Shape	Abstract shape representation
Circle	Circular shape
Rectangle	Rectangular shape
Triangle	Triangular shape
Force	Abstract force representation
Gravity	Applies gravitational force
Friction	Applies friction
CollisionManager	Detects and resolves collisions
ObjectManager	Manages physics objects
SimulationState	Represents simulation state
Statistics	Stores simulation statistics
TraceRecorder	Records simulation information
WebAPI	Exposes C++ functionality to JavaScript


🧩 Object-Oriented Programming
The project demonstrates several important OOP concepts.
1. Encapsulation
Classes encapsulate their internal state and expose controlled interfaces.
For example:
class PhysicsEngine {
private:
    World world;
    double timeStep;
    bool paused;

public:
    void step();
    void pause();
    void resume();
};
The internal state is protected while public methods provide controlled access.
2. Abstraction
Abstract base classes represent common behavior.
For example:
class PhysicsObject {
public:
    virtual void update(double dt) = 0;
    virtual ~PhysicsObject() = default;
};
The base class defines the common interface while derived classes implement specific behavior.
3. Inheritance
The engine uses inheritance for physics objects and shapes.
PhysicsObject
     │
     ├── DynamicObject
     │
     └── StaticObject
and:
Shape
 │
 ├── Circle
 ├── Rectangle
 └── Triangle
4. Polymorphism
The engine uses virtual functions to allow different objects to behave through common interfaces.
For example, PhysicsObject can represent both dynamic and static objects while allowing each derived class to provide its own implementation.
5. Composition
The system heavily uses composition.
For example:
PhysicsEngine
      │
      └── World
            │
            ├── ObjectManager
            ├── Force objects
            ├── CollisionManager
            ├── Statistics
            └── Simulation State
This allows the engine to be modular and maintainable.
🔷 Physics Objects
The engine supports multiple types of physics objects.
Dynamic Objects
Dynamic objects respond to forces and their positions change during the simulation.
Examples:
- Projectile
- Ball
- Falling object
- Moving body
Static Objects
Static objects do not move during simulation.
Examples:
- Ground
- Walls
- Obstacles
🔵 Shape System
The engine currently supports:
Circle
Defined using:
center + radius
Rectangle
Defined using:
center + width + height
Triangle
Defined using its geometric representation and position.
The shape system is separated from the physics-object system so that physical behavior and geometric representation remain modular.
🌍 World
The World class is responsible for managing the simulation environment.
It contains:
- Physics objects
- Forces
- Collision manager
- Simulation time
- Collision statistics
- Undo information
- Recent collision information
- World boundaries
The world coordinates are used to determine where objects exist and how boundaries are enforced.
💨 Force System
The force system is designed around the Force abstraction.
Gravity
Gravity applies a downward force to dynamic objects.
Conceptually:
F = m × g
where:
- F = gravitational force
- m = mass
- g = gravitational acceleration
Friction
Friction reduces the motion of objects based on the simplified friction model implemented by the engine.
Both gravity and friction can be enabled or disabled through the simulation API.
💥 Collision System
The engine contains a dedicated collision-management system.
The collision process consists of:
Collision Detection
        ↓
Collision Normal
        ↓
Positional Correction
        ↓
Velocity Resolution
The engine uses a coefficient of restitution of approximately:
e = 0.6
for collision response.
Circle–Rectangle Collision
Circle-versus-axis-aligned-rectangle collisions use the closest-point method.
The closest point on the rectangle is calculated relative to the circle center.
The collision is detected when:
distance(circle center, closest point)
    <=
circle radius
This prevents large rectangular objects such as floors from incorrectly behaving like large circles.
For other shape combinations, the engine uses bounding-radius based collision approximation.
🧱 Boundary System
The world enforces boundaries so that objects remain inside the simulation area.
When an object reaches a boundary:
- Its position is corrected.
- Its velocity is reflected.
- Collision response is applied.
This allows the simulation to behave like a bounded physics environment.
💥 Impulse System
The engine supports applying impulses to objects.
An impulse changes an object's velocity.
Conceptually:
J = Δp
where:
- J = impulse
- Δp = change in momentum
This is used by the frontend to create interactive experiments involving sudden changes in velocity.
↩️ Undo System
The world stores previous simulation states so that actions can be reversed.
The undo mechanism allows the user to restore a previous state of the simulation.
This is particularly useful during interactive experimentation.
📊 Simulation Statistics
The engine maintains simulation information such as:
- Simulation time
- Collision count
- Recent collisions
- Object state
The frontend uses this information to display useful simulation data to the user.
🌐 WebAssembly Integration
The C++ physics engine is compiled to WebAssembly using Emscripten.
The browser-facing API is implemented in:
backend/src/WebAPI.cpp
backend/src/WebAPI.h
The API acts as a facade between JavaScript and the C++ engine.
Data Flow
React / TypeScript
        │
        ▼
JavaScript
        │
        ▼
WebAssembly
        │
        ▼
WebAPI
        │
        ▼
PhysicsEngine
        │
        ▼
World
        │
        ▼
Physics Simulation
The engine state is returned to the frontend through JSON.
🔌 Web API
The WebAssembly API exposes operations such as:
reset()
pause()
resume()
isPaused()

setTimeStep()
enableGravity()
disableGravity()
enableFriction()
disableFriction()

createCircle()
createRectangle()
createTriangle()

removeObject()

setObjectPosition()
setObjectVelocity()

applyImpulse()

step()
undo()

getStateJSON()
These functions allow the frontend to control the underlying C++ simulation without directly implementing physics calculations in TypeScript.
💻 Frontend
The frontend is built using:
- React
- TypeScript
- Vite
- TanStack Start
- HTML/CSS
- WebAssembly
Its primary responsibilities are:
1. Provide user controls.
2. Create and modify simulation objects.
3. Start and stop the simulation.
4. Display physics experiments.
5. Visualize the simulation.
6. Read simulation state from WebAssembly.
7. Present statistics and results.
The frontend does not replace the C++ physics engine.
Instead:
Frontend = Presentation + Interaction

Backend = Physics Computation
🧪 Interactive Physics Experiments
The web interface provides experiments designed to demonstrate physics concepts interactively.
Examples include:
- Free Fall
- Projectile Motion
- Collision experiments
- Gravity experiments
- Friction experiments
- Impulse experiments
These experiments allow users to change parameters and observe the resulting motion.
🛠️ Technology Stack
Layer	Technology
Physics Engine	C++
Programming Paradigm	Object-Oriented Programming
Web Compilation	Emscripten
Runtime	WebAssembly
Frontend	React
Frontend Language	TypeScript
Build Tool	Vite
Application Framework	TanStack Start
Version Control	Git
Repository	GitHub


▶️ Running the Project
Prerequisites
Install:
- Git
- Node.js
- npm
- Emscripten SDK
🖥️ Run the Frontend
Navigate to the frontend:
cd frontend
Install dependencies:
npm install
Start the development server:
npm run dev
Open the localhost URL displayed by Vite in your browser.
🔧 Rebuilding WebAssembly
If the C++ physics engine is modified, rebuild the WebAssembly files.
First activate Emscripten:
cd ~/emsdk
source ./emsdk_env.sh
Then navigate to the backend:
cd /path/to/MINI-PHYSICS-PROJECT/backend
Run:
./build_web.sh
The generated WebAssembly files are then copied into:
frontend/public/
The frontend can subsequently use the updated engine.
🧱 Native C++ Build
The C++ engine can also be compiled as a native application.
The native build is useful for testing the physics engine independently from the browser interface.
Generated build artifacts such as:
*.o
backend/bin/
are excluded from Git using .gitignore.
🔬 Design Highlights
Separation of Concerns
The project separates:
Physics
   ↓
WebAssembly API
   ↓
Frontend
This prevents UI code from being tightly coupled to the physics implementation.
Modular Physics Engine
Major systems are separated into independent classes:
Objects
Forces
Shapes
Collisions
World
Simulation
Statistics
Reusable Interfaces
Abstract classes such as:
PhysicsObject
Shape
Force
allow additional implementations to be introduced without redesigning the entire engine.
🚀 Future Scope
Possible future improvements include:
- More accurate polygon collision detection
- Rotational dynamics
- Angular velocity
- Torque
- Advanced rigid-body dynamics
- SAT-based polygon collision detection
- Spatial partitioning for performance
- More physics experiments
- 3D simulation
- Multiplayer simulation
- Cloud deployment
- Persistent experiment configurations
- Improved numerical integration such as RK4
🎓 Academic Objectives
This project demonstrates practical application of:
- Object-Oriented Programming
- Classes and objects
- Encapsulation
- Abstraction
- Inheritance
- Polymorphism
- Composition
- STL containers
- Exception handling
- Modular software design
- Physics simulation
- Numerical integration
- Collision detection
- WebAssembly
- C++ and web integration
📌 Project Summary
The Mini Physics Project combines a modular C++ physics engine with a modern web interface.
The C++ backend performs the physics simulation, WebAssembly makes the engine executable inside a browser, and the React + TypeScript frontend provides an interactive environment for users to visualize and experiment with the simulation.
The resulting architecture demonstrates how object-oriented C++ systems can be integrated with modern web technologies while maintaining a clear separation between computation and presentation.
👨‍💻 Project
Mini Physics Project
Built using:
C++ + OOP + Emscripten + WebAssembly + React + TypeScript
