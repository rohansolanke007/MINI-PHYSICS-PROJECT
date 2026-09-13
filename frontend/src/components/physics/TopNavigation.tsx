import { Link, useRouterState } from "@tanstack/react-router";
import { Atom } from "lucide-react";
import { cn } from "@/lib/utils";

export function TopNavigation() {
  const pathname = useRouterState({ select: (s) => s.location.pathname });
  const isLaws = pathname.startsWith("/laws");

  const modes = [
    { label: "SIMULATOR", to: "/", active: !isLaws },
    { label: "PHYSICS LAWS", to: "/laws", active: isLaws },
  ] as const;

  return (
    <header className="sticky top-0 z-30 border-b border-border bg-background/70 backdrop-blur-xl">
      <div className="mx-auto flex h-14 w-full items-center gap-4 px-4 sm:px-6">
        <Link to="/" className="flex items-center gap-2.5">
          <span className="grid size-8 place-items-center rounded-lg border border-primary/40 bg-primary/10 text-primary">
            <Atom className="size-4" />
          </span>
          <span className="leading-tight">
            <span className="block text-[13px] font-semibold tracking-wide">PHYSICS LAB</span>
            <span className="label-micro">Interactive C++ Physics Engine</span>
          </span>
        </Link>

        <nav className="ml-auto flex items-center gap-1 rounded-lg border border-border bg-background/50 p-1">
          {modes.map((mode) => (
            <Link
              key={mode.to}
              to={mode.to}
              className={cn(
                "tech rounded-md px-3 py-1.5 text-[10px] tracking-[0.16em] transition-all sm:px-4",
                mode.active
                  ? "bg-primary text-primary-foreground shadow-[0_0_20px_-6px_var(--primary)]"
                  : "text-muted-foreground hover:bg-accent hover:text-foreground",
              )}
            >
              {mode.label}
            </Link>
          ))}
        </nav>
      </div>
    </header>
  );
}
