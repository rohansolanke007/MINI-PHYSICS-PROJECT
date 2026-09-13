# ⚛️ Mini Physics Engine

A modular 2D physics simulation engine developed in **C++ using Object-Oriented Programming principles**, integrated with an interactive web interface built using **React and TypeScript**.

The C++ physics engine is compiled to **WebAssembly (WASM)** using **Emscripten**, allowing the physics simulation to execute directly inside a web browser.

---

# 📌 Table of Contents

- [Project Overview](#-project-overview)
- [Key Features](#-key-features)
- [System Architecture](#-system-architecture)
- [Application Data Flow](#-application-data-flow)
- [Project Structure](#-project-structure)
- [Physics Engine Architecture](#-physics-engine-architecture)
- [Core Components](#-core-components)
- [Physics Objects](#-physics-objects)
- [Shape System](#-shape-system)
- [Force System](#-force-system)
- [Collision System](#-collision-system)
- [Physics Simulation](#-physics-simulation)
- [Numerical Integration](#-numerical-integration)
- [WebAssembly Integration](#-webassembly-integration)
- [Web API](#-web-api)
- [Frontend Architecture](#-frontend-architecture)
- [Interactive Experiments](#-interactive-experiments)
- [OOP Concepts](#-object-oriented-programming-concepts)
- [STL Usage](#-stl-usage)
- [Exception Handling](#-exception-handling)
- [Simulation State and Undo](#-simulation-state-and-undo)
- [Technology Stack](#-technology-stack)
- [How to Run](#-how-to-run)
- [Rebuilding WebAssembly](#-rebuilding-webassembly)
- [Design Highlights](#-design-highlights)
- [Future Scope](#-future-scope)
- [Academic Objectives](#-academic-objectives)
- [Conclusion](#-conclusion)

---

# 🚀 Project Overview

The **Mini Physics Engine** is a 2D physics simulation system designed to demonstrate the practical application of **Object-Oriented Programming in C++**.

The engine provides the fundamental components required for a simple physics simulation:

- Physics objects
- Shapes
- Forces
- Gravity
- Friction
- Collision detection
- Collision response
- Boundary constraints
- Impulse application
- Simulation state management
- Undo functionality
- Runtime statistics

The C++ backend is integrated with a browser-based frontend through **WebAssembly**.

This allows the project to combine a high-performance C++ physics engine with an interactive graphical interface.

---

# ✨ Key Features

## Physics Features

- 🌍 Gravity simulation
- 🌀 Friction
- 💥 Collision detection
- 💥 Collision response
- 🚀 Projectile motion
- 🪂 Free-fall simulation
- 💨 Velocity and acceleration
- 🧱 Static and dynamic objects
- ↔️ World boundary constraints
- 💥 Impulse application
- ⏱️ Configurable simulation timestep

## Simulation Features

- ▶️ Start / Resume
- ⏸️ Pause
- ↻ Reset
- ⏭️ Step simulation
- ↩️ Undo previous operations
- 📊 Simulation statistics
- 🔍 Object inspection
- ➕ Runtime object creation
- 🗑️ Object removal

## Web Features

- Interactive physics canvas
- Real-time simulation visualization
- Object creation interface
- Object inspector
- Telemetry information
- Physics experiments
- React-based user interface
- TypeScript-based frontend
- C++ engine running through WebAssembly

---

# 🏗️ System Architecture

The complete system consists of three primary layers:

```text
┌──────────────────────────────────────────────┐
│              PRESENTATION LAYER              │
│                                              │
│            React + TypeScript                │
│                                              │
│  Canvas • Controls • Inspector • Telemetry   │
│             Experiments                      │
└──────────────────────┬───────────────────────┘
                       │
                       │ WebAssembly API
                       ▼
┌──────────────────────────────────────────────┐
│              WASM BRIDGE LAYER               │
│                                              │
│        Emscripten Generated Code             │
│                                              │
│      physics_engine.js                       │
│      physics_engine.wasm                     │
└──────────────────────┬───────────────────────┘
                       │
                       ▼
┌──────────────────────────────────────────────┐
│               PHYSICS ENGINE                 │
│                    C++                      │
│                                              │
│  PhysicsEngine                               │
│       │                                      │
│       ▼                                      │
│     World                                    │
│    /  │  \                                   │
│   /   │   \                                  │
│Force Collision ObjectManager                 │
│                                              │
│ Vector2D • PhysicsObject • Shapes            │
│ SimulationState • Statistics                 │
└──────────────────────────────────────────────┘
