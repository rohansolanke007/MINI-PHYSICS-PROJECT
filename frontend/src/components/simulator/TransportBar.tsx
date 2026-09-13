import { Camera, Grid3x3, Pause, Play, RotateCcw, Square, StepForward, Undo2, Move } from "lucide-react";
import { ActionButton, SegmentedControl } from "@/components/physics/ui";
import type { Simulation } from "@/lib/physics/useSimulation";
import { cn } from "@/lib/utils";

export function TransportBar({
  sim,
  showGrid,
  showVectors,
  onToggleGrid,
  onToggleVectors,
  onSnapshot,
}: {
  sim: Simulation;
  showGrid: boolean;
  showVectors: boolean;
  onToggleGrid: () => void;
  onToggleVectors: () => void;
  onSnapshot: () => void;
}) {
  const disabled = sim.status !== "ready";
  return (
    <div className="glass flex flex-wrap items-center gap-2 rounded-xl px-3 py-2.5">
      <ActionButton onClick={sim.undo} disabled={disabled}>
        <Undo2 className="size-3.5" /> UNDO
      </ActionButton>
      <ActionButton
        tone="primary"
        disabled={disabled}
        onClick={() => (sim.running ? sim.pause() : sim.run())}
      >
        {sim.running ? <Pause className="size-3.5" /> : <Play className="size-3.5" />}
        {sim.running ? "PAUSE" : "RUN"}
      </ActionButton>
      <ActionButton onClick={sim.stepOnce} disabled={disabled}>
        <StepForward className="size-3.5" /> STEP
      </ActionButton>
      <ActionButton onClick={sim.stop} disabled={disabled}>
        <Square className="size-3.5" /> STOP
      </ActionButton>

      <div className="ml-1 flex items-center gap-2">
        <span className="label-micro">SPEED</span>
        <div className="w-44">
          <SegmentedControl
            size="sm"
            value={sim.speed}
            onChange={sim.setSpeed}
            options={[
              { label: "0.5×", value: 0.5 },
              { label: "1×", value: 1 },
              { label: "2×", value: 2 },
              { label: "4×", value: 4 },
            ]}
          />
        </div>
      </div>

      <div className="ml-auto flex flex-wrap items-center gap-2">
        <ActionButton onClick={onToggleGrid} className={cn(showGrid && "border-primary/50 text-primary")}>
          <Grid3x3 className="size-3.5" /> GRID
        </ActionButton>
        <ActionButton
          onClick={onToggleVectors}
          className={cn(showVectors && "border-primary/50 text-primary")}
        >
          <Move className="size-3.5" /> VECTORS
        </ActionButton>
        <ActionButton onClick={onSnapshot} disabled={disabled}>
          <Camera className="size-3.5" /> SNAPSHOT
        </ActionButton>
        <ActionButton tone="danger" onClick={sim.resetWorld} disabled={disabled}>
          <RotateCcw className="size-3.5" /> RESET
        </ActionButton>
      </div>
    </div>
  );
}
