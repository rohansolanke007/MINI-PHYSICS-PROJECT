# Mini Physics Engine
### An OOP + STL Based 2D Physics Simulation in C++

A console-based 2D physics simulator built for a second-year OOP course
project. The primary goal of this codebase is to **demonstrate OOP
concepts genuinely and defensibly**, using a physics simulation as the
vehicle — not to be a research-grade physics engine.

---

## 1. How to Build and Run

You need a C++17-capable compiler (g++ 7+ or clang++ 5+).

```bash
# From the project root:
make          # builds bin/physics_engine
make run      # builds (if needed) and runs it
make clean    # removes build artifacts
```

If you don't have `make`, you can compile directly:

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o bin/physics_engine
./bin/physics_engine
```

The program is an interactive console menu — no external libraries or
frameworks are required, only the standard C++ library.

A `data/` folder is provided as a convenient place to save/load
simulation files from the menu (e.g. type `data/save1.txt` when
prompted), but you can save/load using any path.

---

## 2. Project Structure

```
include/            Header files (class declarations)
  Vector2D.h         - 2D vector math + operator overloading
  Exceptions.h        - Custom exception hierarchy
  Shape.h              - Abstract Shape + Circle/Rectangle/Triangle
  PhysicsObject.h      - Abstract PhysicsObject + Dynamic/StaticObject
  Force.h              - Abstract Force + Gravity/Friction/Applied
  ObjectManager.h      - Class template (generic ID-keyed registry)
  MathUtils.h          - Function templates (clampValue, average)
  Collision.h          - Collision struct + CollisionManager
  SimulationState.h    - Serializable snapshot of the World
  FileHandler.h        - Save/load simulation state to text files
  Statistics.h         - Aggregate simulation statistics
  World.h              - Owns objects/forces, drives simulation state
  PhysicsEngine.h      - Top-level controller (timestep, pause/resume)
  TraceRecorder.h      - Records a full run and exports JSON for the web viewer

src/                Implementation (.cpp) files, one per header
  main.cpp             - Console menu / UI layer only

Makefile            Build script
data/                Suggested folder for save files (FileHandler format)

web/                 Browser-based trace viewer (the "frontend")
  index.html           - Self-contained HTML/CSS/JS trace player (no build step, no dependencies)
  examples/            - Two ready-made trace.json files to try immediately:
    gravity_bounce_demo.json  - two circles falling and bouncing off the floor
    collision_demo.json       - two circles colliding head-on (momentum-conserving)
```

---

## 3. Architecture at a Glance

```
PhysicsEngine  (simulation control: timestep, pause/resume)
      |
      v
    World  (physics state + rules)
      |
      +-- ObjectManager<PhysicsObject>  (owns all bodies)
      |         |
      |         +-- DynamicObject  --\
      |         +-- StaticObject   ---> both "have-a" Shape (composition)
      |                                        |
      |                                        +-- Circle
      |                                        +-- Rectangle
      |                                        +-- Triangle
      |
      +-- Force (gravity, friction)
      |         +-- GravityForce
      |         +-- FrictionForce
      |         +-- AppliedForce
      |
      +-- CollisionManager --> produces Collision events
      |
      +-- SimulationState (snapshot) <--> FileHandler (save/load)
      +-- std::stack<SimulationState> (undo history)

Statistics is computed on-demand from World, not stored redundantly.
```

**The most important design decision**: `PhysicsObject` (mass, position,
velocity — physics state) and `Shape` (area, bounding radius — geometry)
are two *separate* class hierarchies connected by composition
(`PhysicsObject` has a `std::unique_ptr<Shape>`), not by inheritance.
This means physics behaviour (Dynamic vs Static) and geometry (Circle vs
Rectangle vs Triangle) can vary independently.

**Documented simplification**: collision detection/resolution uses each
shape's *bounding radius* (a circle-circle check) rather than exact
polygon-vs-polygon math. This is intentional — exact geometric collision
for arbitrary shape pairs is a real research problem, not a second-year
OOP exercise. It is called out here explicitly so it reads as a designed
trade-off, not a gap.

---

## 4. OOP Concept → Code Location Map (for viva prep)

| Concept | Where |
|---|---|
| Encapsulation | `PhysicsObject` (private state, public accessors) |
| Constructors validating input | `Circle`, `Rectangle`, `Triangle`, `PhysicsObject`, `DynamicObject` constructors |
| Static members | `PhysicsObject::nextId` |
| Abstract classes / pure virtual functions | `Shape`, `PhysicsObject`, `Force` |
| Inheritance | `Shape` → `Circle/Rectangle/Triangle`; `PhysicsObject` → `Dynamic/StaticObject`; `Force` → `Gravity/Friction/Applied`; exception hierarchy |
| Runtime polymorphism / virtual dispatch | `shape->getArea()`, `force->apply(obj)`, `obj->update(dt)` |
| Upcasting | Storing `Circle*`/`Rectangle*`/`Triangle*` as `Shape*` (or `unique_ptr<Shape>`) |
| Downcasting | `SimulationState::captureFrom()` — `static_cast<const Circle*>(shape)` after checking `getType()` |
| Operator overloading | `Vector2D` (`+ - * / == != << >>`) |
| Class templates | `ObjectManager<T>` |
| Function templates | `clampValue<T>()`, `average<T>()` in `MathUtils.h` |
| STL — vector | `std::vector<Collision>` results |
| STL — map | `ObjectManager<T>` internal `std::map<int, unique_ptr<T>>` |
| STL — set | `std::set<pair<int,int>>` for collision pair de-duplication |
| STL — queue | `World`'s `recentCollisionEvents` |
| STL — stack | `World`'s undo `history` |
| Custom exceptions | `Exceptions.h` (`InvalidMassException`, `InvalidShapeException`, `InvalidObjectException`, `FileException`, `InvalidSimulationException`) |
| try/catch/throw + stack unwinding | `main.cpp` flows catch `PhysicsException&` around deep call chains (e.g. `loadSimulationFlow`) |
| File I/O (text files) | `FileHandler::saveToFile` / `loadFromFile` |
| Dynamic memory management | `std::unique_ptr<Shape>`, `std::unique_ptr<PhysicsObject>`, `std::unique_ptr<Force>` throughout (no raw `new`/`delete`) |
| Composition / class reuse | `TraceRecorder` is built entirely on top of `SimulationState` rather than re-deriving object data — a second, JSON-based file format sitting alongside the text-based `FileHandler` format |

---

## 5. Web Frontend (Trace Viewer)

The console app **is** the physics engine — the web page is a pure
**playback/telemetry viewer**. It never re-simulates anything; it only
renders exactly what the C++ program already computed. This is a
deliberate design choice: it means the C++ OOP backend stays the real
deliverable, and the browser cannot silently diverge from it.

**How the two pieces connect:**

1. In the console app, build a scene (create objects, set gravity/friction).
2. Choose menu option **14: Record Simulation for Web Visualizer**.
   This uses the new `TraceRecorder` class, which calls
   `SimulationState::captureFrom(world)` after every single step (reusing
   the exact same snapshot logic that already powers Save/Load), and
   writes the whole sequence of frames to a `.json` file.
3. Open `web/index.html` directly in any browser (double-click it — no
   server or internet connection needed).
4. Click **"Load trace"** and pick the `.json` file you exported.
5. Use the transport controls (play/pause, step, scrub bar, speed) to
   replay the run. The sidebar shows live telemetry (sim time, kinetic
   energy, collision count) and a per-object table, recomputed per frame
   using the same formulas as the `Statistics` class — purely for display.

Two example trace files are included in `web/examples/` so you can see
it working immediately without running the console app first:
- `gravity_bounce_demo.json` — two circles fall under gravity and bounce
  off the simulation floor.
- `collision_demo.json` — two equal-mass circles collide head-on; the
  resulting velocities (2 m/s and 8 m/s from an initial 10 m/s and 0 m/s)
  exactly match the impulse formula in `CollisionManager`, and total
  momentum (20 kg·m/s) is conserved before and after — a good one to
  point at in viva as evidence the collision math is genuinely correct,
  not just visually plausible.

**Why not a "live" web app controlling the engine directly?** That would
require the C++ program to run as a network server (sockets/HTTP), which
is real scope creep outside your OOP syllabus and adds a lot of risk for
a second-year team on a deadline. The trace-replay approach gets you a
genuine, working web frontend without touching that complexity, and
without ever duplicating the physics logic in JavaScript.

---

## 6. Known Limitations / Future Scope (be ready to state these in viva)

- Collision shapes are approximated by bounding circles — no exact
  polygon collision for rotated rectangles/triangles.
- No object rotation/angular velocity — only linear motion.
- Friction is a simplified kinetic-friction model, not a full contact solver.
- Undo history (`std::stack`) grows unbounded during a long session —
  a capped ring buffer would be a natural improvement.
- Only text-based file format is implemented; binary save/load was
  identified as an optional stretch feature but not required for the
  core deliverable.

## Interactive Web Interface

The project now includes an interactive browser frontend backed by the **same C++ physics engine**. The browser does not load a recorded JSON trace. The C++ engine is compiled to WebAssembly with Emscripten and JavaScript calls a small `WebAPI` facade.

### Architecture

```text
Browser UI (HTML/CSS/JS)
        |
        v
WebAPI.cpp (Emscripten bindings)
        |
        v
PhysicsEngine -> World -> PhysicsObject / Shape / Force / Collision
```

The JSON trace recorder remains available for the original record/export feature, but it is no longer part of the interactive web workflow.

### 1. Install Emscripten on macOS

Install and activate the official Emscripten SDK (`emsdk`). Follow the current installation instructions at:

https://emscripten.org/docs/getting_started/downloads.html

After installation, activate the SDK and load its environment in the terminal. A typical setup is:

```bash
cd ~/emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
```

You should then be able to run:

```bash
emcc --version
```

### 2. Build the C++ WebAssembly module

From the project root:

```bash
./build_web.sh
```

or:

```bash
make web
```

This creates:

```text
web/physics_engine.js
web/physics_engine.wasm
```

### 3. Start the web server

Do **not** rely on opening `index.html` with `file://` for the WASM build. Serve the `web` folder over localhost. You can do the build and server startup in one command:

```bash
./start_web.sh
```

Or, manually:

```bash
cd web
python3 -m http.server 8080
```

Then open:

```text
http://localhost:8080
```

### 4. Using the interface

- **Add object:** create Dynamic or Static Circle, Rectangle, or Triangle objects.
- **World settings:** change gravity, friction, and timestep.
- **Run / Pause:** advances the actual C++ simulation continuously.
- **Step:** advances exactly one C++ timestep.
- **Apply impulse:** sends an impulse into the selected C++ PhysicsObject.
- **Drag:** click and drag an object to reposition it; dragging pauses the simulation.
- **Undo:** restores the previous state using the existing C++ history stack.
- **Load demo:** creates a small scene through the WebAssembly API.
- **Reset:** clears the World and resets its simulation state.

### Console version

The original console program is still available:

```bash
make
./bin/physics_engine
```

The console and web interfaces are two different frontends over the same physics classes.
