import type { ReactNode } from "react";
import { cn } from "@/lib/utils";

export function Panel({
  title,
  subtitle,
  actions,
  children,
  className,
  bodyClassName,
}: {
  title?: string;
  subtitle?: string;
  actions?: ReactNode;
  children: ReactNode;
  className?: string;
  bodyClassName?: string;
}) {
  return (
    <section className={cn("glass flex min-h-0 flex-col rounded-xl", className)}>
      {(title || actions) && (
        <header className="flex items-center justify-between gap-2 border-b border-border px-3.5 py-2.5">
          <div>
            {title && <h2 className="tech text-[11px] tracking-[0.18em]">{title}</h2>}
            {subtitle && <p className="label-micro mt-1 normal-case">{subtitle}</p>}
          </div>
          {actions}
        </header>
      )}
      <div className={cn("scroll-thin min-h-0 flex-1 overflow-y-auto p-3.5", bodyClassName)}>
        {children}
      </div>
    </section>
  );
}

export function Field({
  label,
  suffix,
  children,
}: {
  label: string;
  suffix?: string;
  children: ReactNode;
}) {
  return (
    <label className="block">
      <span className="label-micro flex items-center justify-between">
        {label}
        {suffix && <em className="not-italic opacity-70">{suffix}</em>}
      </span>
      <span className="mt-1 block">{children}</span>
    </label>
  );
}

export function NumberInput({
  value,
  onChange,
  step = 0.5,
  min,
  max,
}: {
  value: number;
  onChange: (value: number) => void;
  step?: number;
  min?: number;
  max?: number;
}) {
  return (
    <input
      type="number"
      value={Number.isFinite(value) ? value : ""}
      step={step}
      min={min}
      max={max}
      onChange={(e) => onChange(e.target.value === "" ? NaN : Number(e.target.value))}
      className="tech h-8 w-full rounded-md border border-input bg-background/60 px-2 text-[12px] text-foreground outline-none transition-colors focus:border-primary/60 focus:ring-2 focus:ring-ring/30"
    />
  );
}

export function SegmentedControl<T extends string | number>({
  options,
  value,
  onChange,
  size = "md",
}: {
  options: { label: string; value: T }[];
  value: T;
  onChange: (value: T) => void;
  size?: "sm" | "md";
}) {
  return (
    <div className="flex gap-1 rounded-md border border-border bg-background/40 p-1">
      {options.map((option) => (
        <button
          key={String(option.value)}
          type="button"
          onClick={() => onChange(option.value)}
          className={cn(
            "tech flex-1 rounded px-2 tracking-[0.1em] transition-all",
            size === "sm" ? "py-1 text-[9px]" : "py-1.5 text-[10px]",
            option.value === value
              ? "bg-primary text-primary-foreground shadow-[0_0_16px_-4px_var(--primary)]"
              : "text-muted-foreground hover:bg-accent hover:text-foreground",
          )}
        >
          {option.label}
        </button>
      ))}
    </div>
  );
}

export function ActionButton({
  children,
  onClick,
  tone = "default",
  className,
  disabled,
  type = "button",
}: {
  children: ReactNode;
  onClick?: () => void;
  tone?: "default" | "primary" | "danger" | "ghost";
  className?: string;
  disabled?: boolean;
  type?: "button" | "submit";
}) {
  return (
    <button
      type={type}
      onClick={onClick}
      disabled={disabled}
      className={cn(
        "tech inline-flex items-center justify-center gap-1.5 rounded-md px-3 py-2 text-[10px] tracking-[0.14em] transition-all disabled:cursor-not-allowed disabled:opacity-40",
        tone === "primary" &&
          "bg-primary text-primary-foreground hover:brightness-110 hover:shadow-[0_0_22px_-6px_var(--primary)]",
        tone === "default" &&
          "border border-border bg-secondary/60 text-foreground hover:border-primary/40 hover:bg-accent",
        tone === "danger" &&
          "border border-destructive/40 text-destructive hover:bg-destructive/10",
        tone === "ghost" && "text-muted-foreground hover:bg-accent hover:text-foreground",
        className,
      )}
    >
      {children}
    </button>
  );
}

export function Readout({
  label,
  value,
  unit,
  accent,
}: {
  label: string;
  value: string;
  unit?: string;
  accent?: string;
}) {
  return (
    <div className="rounded-md border border-border bg-background/40 px-2.5 py-2">
      <div className="label-micro">{label}</div>
      <div className="tech mt-1 text-[13px] font-medium" style={accent ? { color: accent } : undefined}>
        {value}
        {unit && <span className="ml-1 text-[9px] text-muted-foreground">{unit}</span>}
      </div>
    </div>
  );
}
