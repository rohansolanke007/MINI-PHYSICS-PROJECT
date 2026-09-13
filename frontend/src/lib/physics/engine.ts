/**
 * Thin typed bridge to the EXISTING C++ physics engine compiled to
 * WebAssembly with Emscripten (public/physics_engine.js + .wasm).
 *
 * No physics is implemented here. Every simulation operation is delegated
 * to the global Emscripten `Module` object exactly as described in
 * API_CONTRACT.md.
 */

export interface PhysicsModule {
  reset(): void;
  pause(): void;
  resume(): void;
  isPaused(): boolean;

  setTimeStep(dt: number): void;
  getTimeStep(): number;

  setGravity(g: number): void;
  disableGravity(): void;
  setFriction(coefficient: number): void;
  disableFriction(): void;

  createCircle(
    isStatic: boolean,
    mass: number,
    x: number,
    y: number,
    radius: number,
    vx: number,
    vy: number,
  ): number;
  createRectangle(
    isStatic: boolean,
    mass: number,
    x: number,
    y: number,
    width: number,
    height: number,
    vx: number,
    vy: number,
  ): number;
  createTriangle(
    isStatic: boolean,
    mass: number,
    x: number,
    y: number,
    base: number,
    height: number,
    vx: number,
    vy: number,
  ): number;

  removeObject(id: number): boolean;
  setObjectPosition(id: number, x: number, y: number): void;
  setObjectVelocity(id: number, vx: number, vy: number): void;
  applyImpulse(id: number, fx: number, fy: number): void;

  step(count: number): void;
  undo(): boolean;

  getStateJSON(): string;
}

export interface Vec2 {
  x: number;
  y: number;
}

export interface PhysicsObjectState {
  id: number;
  kind: string;
  isStatic: boolean;
  shape: "Circle" | "Rectangle" | "Triangle" | string;
  shapeType: number;
  shapeParams: number[];
  mass: number;
  pos: Vec2;
  vel: Vec2;
  acc: Vec2;
  radius: number;
}

export interface CollisionEvent {
  a: number;
  b: number;
  penetration: number;
}

export interface PhysicsState {
  time: number;
  timeStep: number;
  paused: boolean;
  gravity: number;
  friction: number;
  gravityEnabled: boolean;
  frictionEnabled: boolean;
  boundaryMin: Vec2;
  boundaryMax: Vec2;
  stats: {
    objects: number;
    dynamic: number;
    static: number;
    kineticEnergy: number;
    collisions: number;
  };
  recentCollisions: CollisionEvent[];
  objects: PhysicsObjectState[];
}

declare global {
  interface Window {
    Module?: Partial<PhysicsModule> & Record<string, unknown>;
  }
}

let loader: Promise<PhysicsModule> | null = null;

export function loadPhysicsEngine(): Promise<PhysicsModule> {
  if (typeof window === "undefined") {
    return Promise.reject(new Error("The C++ engine can only load in the browser."));
  }
  if (loader) return loader;

  loader = new Promise<PhysicsModule>((resolve, reject) => {
    const existing = window.Module;
    if (existing && typeof existing.getStateJSON === "function") {
      resolve(existing as PhysicsModule);
      return;
    }

    // Emscripten reads the global `Module` object when physics_engine.js runs.
    window.Module = {
      locateFile: (path: string) => `/${path}`,
      onRuntimeInitialized: function () {
        window.dispatchEvent(new Event("physics-engine-ready"));
        resolve(window.Module as unknown as PhysicsModule);
      },
    } as NonNullable<Window["Module"]>;

    const script = document.createElement("script");
    script.src = "/physics_engine.js";
    script.async = true;
    script.onerror = () => reject(new Error("Could not load physics_engine.js"));
    document.head.appendChild(script);
  });

  return loader;
}

export function readState(module: PhysicsModule): PhysicsState {
  return JSON.parse(module.getStateJSON()) as PhysicsState;
}

/** Emscripten throws numeric pointers for C++ exceptions; normalise to text. */
export function engineErrorMessage(error: unknown): string {
  if (error instanceof Error) return error.message;
  if (typeof error === "string") return error;
  if (typeof error === "number") return "The C++ engine rejected that operation.";
  return "Unknown engine error.";
}
