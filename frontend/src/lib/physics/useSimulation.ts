import { useCallback, useEffect, useRef, useState } from "react";
import { toast } from "sonner";
import {
  engineErrorMessage,
  loadPhysicsEngine,
  readState,
  type PhysicsModule,
  type PhysicsState,
} from "./engine";

export type EngineStatus = "loading" | "ready" | "error";

/**
 * Drives the C++ WebAssembly engine. The C++ world is the single source of
 * truth: this hook only forwards commands and polls Module.getStateJSON().
 */
export function useSimulation() {
  const moduleRef = useRef<PhysicsModule | null>(null);
  const [status, setStatus] = useState<EngineStatus>("loading");
  const [error, setError] = useState<string | null>(null);
  const [state, setState] = useState<PhysicsState | null>(null);
  const [running, setRunning] = useState(false);
  const [speed, setSpeed] = useState(1);

  const runningRef = useRef(false);
  const speedRef = useRef(1);
  speedRef.current = speed;

  const refresh = useCallback(() => {
    const m = moduleRef.current;
    if (!m) return;
    try {
      setState(readState(m));
    } catch (e) {
      console.error(e);
    }
  }, []);

  /** Safely invoke an engine operation and surface C++ exceptions to the UI. */
  const call = useCallback(
    <T,>(fn: (m: PhysicsModule) => T): T | undefined => {
      const m = moduleRef.current;
      if (!m) {
        toast.error("The C++ engine is still loading.");
        return undefined;
      }
      try {
        const result = fn(m);
        setState(readState(m));
        return result;
      } catch (e) {
        toast.error(engineErrorMessage(e));
        return undefined;
      }
    },
    [],
  );

  useEffect(() => {
    let cancelled = false;
    loadPhysicsEngine()
      .then((m) => {
        if (cancelled) return;
        moduleRef.current = m;
        try {
          m.pause();
        } catch {
          /* ignore */
        }
        setStatus("ready");
        setState(readState(m));
      })
      .catch((e) => {
        if (cancelled) return;
        setError(engineErrorMessage(e));
        setStatus("error");
      });
    return () => {
      cancelled = true;
    };
  }, []);

  // Animation loop: advances the C++ engine, never simulates in JS.
  useEffect(() => {
    if (status !== "ready") return;
    let frame = 0;
    let last = performance.now();
    let accumulator = 0;

    const tick = (now: number) => {
      const elapsed = Math.min((now - last) / 1000, 0.25);
      last = now;
      const m = moduleRef.current;
      if (m) {
        if (runningRef.current) {
          const dt = m.getTimeStep() || 0.05;
          accumulator += (elapsed * speedRef.current) / dt;
          const steps = Math.min(8, Math.floor(accumulator));
          if (steps > 0) {
            accumulator -= steps;
            try {
              m.step(steps);
            } catch (e) {
              runningRef.current = false;
              setRunning(false);
              toast.error(engineErrorMessage(e));
            }
          }
        }
        try {
          setState(readState(m));
        } catch {
          /* ignore transient read errors */
        }
      }
      frame = requestAnimationFrame(tick);
    };

    frame = requestAnimationFrame(tick);
    return () => cancelAnimationFrame(frame);
  }, [status]);

  useEffect(() => {
    return () => {
      // Leave the shared engine paused when a view unmounts.
      try {
        moduleRef.current?.pause();
      } catch {
        /* ignore */
      }
      runningRef.current = false;
    };
  }, []);

  const run = useCallback(() => {
    call((m) => m.resume());
    runningRef.current = true;
    setRunning(true);
  }, [call]);

  const pause = useCallback(() => {
    runningRef.current = false;
    setRunning(false);
    call((m) => m.pause());
  }, [call]);

  const stepOnce = useCallback(() => {
    runningRef.current = false;
    setRunning(false);
    call((m) => {
      m.resume();
      m.step(1);
      m.pause();
    });
  }, [call]);

  const stop = useCallback(() => {
    runningRef.current = false;
    setRunning(false);
    call((m) => m.pause());
  }, [call]);

  const resetWorld = useCallback(() => {
    runningRef.current = false;
    setRunning(false);
    call((m) => {
      m.reset();
      m.pause();
    });
  }, [call]);

  const undo = useCallback(() => {
    const ok = call((m) => m.undo());
    if (ok === false) toast.message("Nothing left to undo in the C++ world.");
  }, [call]);

  return {
    module: moduleRef,
    status,
    error,
    state,
    running,
    speed,
    setSpeed,
    call,
    refresh,
    run,
    pause,
    stepOnce,
    stop,
    resetWorld,
    undo,
  };
}

export type Simulation = ReturnType<typeof useSimulation>;
