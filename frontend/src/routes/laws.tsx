import { createFileRoute } from "@tanstack/react-router";
import { useEffect, useState } from "react";
import { Pause, Play, RotateCcw, StepForward } from "lucide-react";

import { WorldCanvas } from "@/components/physics/WorldCanvas";
import { ActionButton, Panel, Readout, SegmentedControl } from "@/components/physics/ui";
import { TelemetryBar } from "@/components/simulator/TelemetryBar";
import { experiments } from "@/lib/physics/experiments";
import { useSimulation } from "@/lib/physics/useSimulation";
import { cn } from "@/lib/utils";

export const Route = createFileRoute("/laws")({
  head: () => ({
    meta: [
      { title: "Physics Laws — Interactive Demonstrations" },
      {
        name: "description",
        content:
          "Ten interactive demonstrations of Newton's laws, momentum, energy, friction and projectile motion, all solved by a C++ physics engine.",
      },
      { property: "og:title", content: "Physics Laws — Interactive Demonstrations" },
      {
        property: "og:description",
        content: "Run ten guided mechanics experiments driven by a real C++ physics engine.",
      },
    ],
  }),
  component: LawsPage,
});

function LawsPage() {
  const sim = useSimulation();
  const [activeId, setActiveId] = useState(experiments[0]!.id);
  const active = experiments.find((e) => e.id === activeId) ?? experiments[0]!;

  useEffect(() => {
    if (sim.status !== "ready") return;
    sim.pause();
    sim.call((m) => active.build(m));
    // Rebuild the C++ world whenever the experiment changes.
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [activeId, sim.status]);

  const disabled = sim.status !== "ready";

  return (
    <main className="mx-auto flex w-full max-w-[1700px] flex-1 flex-col gap-3 p-3 sm:p-4">
      <h1 className="sr-only">Physics laws demonstrations</h1>
      <TelemetryBar state={sim.state} status={sim.status} />

      <div className="grid min-h-0 flex-1 gap-3 xl:grid-cols-[300px_minmax(0,1fr)_320px]">
        <Panel title="EXPERIMENT LIBRARY" subtitle={`${experiments.length} guided demonstrations`}>
          <div className="space-y-2">
            {experiments.map((e, i) => (
              <button
                key={e.id}
                type="button"
                onClick={() => setActiveId(e.id)}
                className={cn(
                  "w-full rounded-md border px-3 py-2 text-left transition-all",
                  e.id === activeId
                    ? "border-primary/50 bg-primary/10"
                    : "border-border bg-background/40 hover:bg-accent",
                )}
              >
                <div className="label-micro">
                  {String(i + 1).padStart(2, "0")} · {e.law}
                </div>
                <div className="tech mt-1 text-[11px] tracking-[0.12em] text-foreground">{e.name}</div>
                <p className="mt-1 text-[11px] leading-snug text-muted-foreground">{e.description}</p>
              </button>
            ))}
          </div>
        </Panel>

        <div className="flex min-h-0 flex-col gap-3">
          <div className="glass flex flex-wrap items-center gap-2 rounded-xl px-3 py-2.5">
            <ActionButton
              tone="primary"
              disabled={disabled}
              onClick={() => (sim.running ? sim.pause() : sim.run())}
            >
              {sim.running ? <Pause className="size-3.5" /> : <Play className="size-3.5" />}
              {sim.running ? "PAUSE" : "RUN EXPERIMENT"}
            </ActionButton>
            <ActionButton disabled={disabled} onClick={sim.stepOnce}>
              <StepForward className="size-3.5" /> STEP
            </ActionButton>
            <ActionButton
              disabled={disabled}
              onClick={() => {
                sim.pause();
                sim.call((m) => active.build(m));
              }}
            >
              <RotateCcw className="size-3.5" /> RESTART
            </ActionButton>
            <div className="ml-auto w-48">
              <SegmentedControl
                size="sm"
                value={sim.speed}
                onChange={sim.setSpeed}
                options={[
                  { label: "0.25×", value: 0.25 },
                  { label: "0.5×", value: 0.5 },
                  { label: "1×", value: 1 },
                  { label: "2×", value: 2 },
                ]}
              />
            </div>
          </div>

          <div className="glass min-h-[400px] flex-1 overflow-hidden rounded-xl">
            <WorldCanvas state={sim.state} selectedId={null} showGrid showVectors />
          </div>
        </div>

        <div className="scroll-thin flex max-h-[calc(100vh-9rem)] flex-col gap-3 overflow-y-auto xl:max-h-none">
          <Panel title={active.name} subtitle={active.law}>
            <div className="space-y-3">
              <div className="tech rounded-md border border-primary/30 bg-primary/5 px-3 py-2 text-[12px] text-primary">
                {active.formula}
              </div>
              <p className="text-[11px] leading-relaxed text-muted-foreground">{active.explanation}</p>
            </div>
          </Panel>

          <Panel title="LIVE MEASUREMENTS" subtitle="Read directly from the C++ state">
            <div className="grid grid-cols-2 gap-2">
              {active.readouts(sim.state).map((r, i) => (
                <Readout key={`${r.label}-${i}`} label={r.label} value={r.value} unit={r.unit ?? ""} />
              ))}
            </div>
          </Panel>
        </div>
      </div>
    </main>
  );
}
