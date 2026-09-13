import type { PhysicsModule } from "./engine";

export interface ScenePreset {
  id: string;
  name: string;
  description: string;
  build: (m: PhysicsModule) => void;
}

/** Every preset is assembled purely through the existing C++ WASM API. */
export const scenePresets: ScenePreset[] = [
  {
    id: "free-fall",
    name: "FREE FALL",
    description: "Three masses released from the same height under gravity.",
    build: (m) => {
      m.reset();
      m.setGravity(9.8);
      m.disableFriction();
      m.createCircle(false, 1, 25, 55, 2, 0, 0);
      m.createCircle(false, 5, 50, 55, 2.6, 0, 0);
      m.createCircle(false, 12, 75, 55, 3.2, 0, 0);
      m.pause();
    },
  },
  {
    id: "collision-lab",
    name: "COLLISION LAB",
    description: "Two bodies approaching head-on with gravity disabled.",
    build: (m) => {
      m.reset();
      m.disableGravity();
      m.disableFriction();
      m.createCircle(false, 4, 20, 30, 3, 9, 0);
      m.createCircle(false, 8, 75, 30, 4, -5, 0);
      m.pause();
    },
  },
  {
    id: "friction-test",
    name: "FRICTION TEST",
    description: "Sliding blocks losing speed to the C++ friction model.",
    build: (m) => {
      m.reset();
      m.setGravity(9.8);
      m.setFriction(0.25);
      m.createRectangle(true, 1, 50, 2, 96, 4, 0, 0);
      m.createRectangle(false, 5, 10, 6.5, 5, 3, 14, 0);
      m.createRectangle(false, 12, 10, 12, 5, 3, 14, 0);
      m.pause();
    },
  },
  {
    id: "projectile",
    name: "PROJECTILE MOTION",
    description: "A body launched at an angle into a gravity field.",
    build: (m) => {
      m.reset();
      m.setGravity(9.8);
      m.disableFriction();
      m.createCircle(false, 2, 6, 4, 1.6, 18, 24);
      m.pause();
    },
  },
  {
    id: "empty",
    name: "EMPTY WORLD",
    description: "Clean C++ world, forces at default values.",
    build: (m) => {
      m.reset();
      m.pause();
    },
  },
];
