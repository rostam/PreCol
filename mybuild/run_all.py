#!/usr/bin/env python3
"""
Master benchmark runner.
Run from the mybuild/ directory:  python3 run_all.py [--skip-precol] [--skip-julia]
                                                      [--skip-colpack] [--skip-dsjm]
                                                      [--skip-compare]

The four tools run in parallel; compare scripts run after all finish.
"""

import argparse
import os
import subprocess
import sys
import time

MYBUILD = os.path.dirname(os.path.abspath(__file__))
OUTPUT  = os.path.join(MYBUILD, "Output")

def banner(text):
    print(f"\n{'='*60}")
    print(f"  {text}")
    print(f"{'='*60}", flush=True)

def run(label, cmd, cwd=MYBUILD, timeout=3600):
    """Run a command, wait for it, return (ok, elapsed)."""
    banner(label)
    print(f"  cwd : {cwd}")
    print(f"  cmd : {' '.join(cmd)}\n", flush=True)
    t0 = time.time()
    result = subprocess.run(cmd, cwd=cwd, timeout=timeout)
    elapsed = time.time() - t0
    status = "OK" if result.returncode == 0 else f"FAILED (exit {result.returncode})"
    print(f"\n  [{status}]  elapsed: {elapsed:.1f}s", flush=True)
    return result.returncode == 0

def run_parallel(tools):
    """
    Launch all tools simultaneously, stream their output, wait for all.
    tools: list of (label, cmd, cwd) tuples.
    Returns dict {label: ok}.
    """
    banner(f"Launching {len(tools)} tools in parallel")
    procs = []
    t0 = time.time()
    for label, cmd, cwd in tools:
        print(f"  starting : {label}")
        print(f"    cmd    : {' '.join(cmd)}", flush=True)
        p = subprocess.Popen(cmd, cwd=cwd)
        procs.append((label, p))

    print(f"\n  Waiting for all {len(tools)} processes to finish...", flush=True)
    results = {}
    for label, p in procs:
        p.wait()
        elapsed = time.time() - t0
        ok = p.returncode == 0
        status = "OK" if ok else f"FAILED (exit {p.returncode})"
        print(f"  [{status}]  {label}  (wall time so far: {elapsed:.1f}s)", flush=True)
        results[label] = ok
    return results

def main():
    parser = argparse.ArgumentParser(description="Run all coloring benchmarks and comparisons.")
    parser.add_argument("--skip-precol",  action="store_true")
    parser.add_argument("--skip-julia",   action="store_true")
    parser.add_argument("--skip-colpack", action="store_true")
    parser.add_argument("--skip-dsjm",    action="store_true")
    parser.add_argument("--skip-compare", action="store_true")
    args = parser.parse_args()

    os.makedirs(OUTPUT, exist_ok=True)
    results = {}

    # ── Phase 1: run all four tools in parallel ────────────────────
    tools = []
    if not args.skip_precol:
        tools.append(("PreCol",  ["./PreCol", "input.csv", "Output/output_precol.csv"], MYBUILD))
    if not args.skip_julia:
        tools.append(("Julia",   ["/home/rostam/.juliaup/bin/julia", "test_time.jl"],   MYBUILD))
    if not args.skip_colpack:
        tools.append(("ColPack", [sys.executable, "run_colpack.py"],                    MYBUILD))
    if not args.skip_dsjm:
        tools.append(("DSJM",    [sys.executable, "run_dsjm.py"],                       MYBUILD))

    if tools:
        results.update(run_parallel(tools))

    # ── Phase 2: compare scripts (sequential, depend on phase 1) ───
    if not args.skip_compare:
        for script in ["compare.py", "compare2.py", "compare3.py"]:
            results[script] = run(
                f"Running {script}",
                [sys.executable, script],
                cwd=OUTPUT,
            )

    # ── Summary ────────────────────────────────────────────────────
    banner("Summary")
    all_ok = True
    for name, ok in results.items():
        mark = "✓" if ok else "✗"
        print(f"  {mark}  {name}")
        if not ok:
            all_ok = False
    print()
    sys.exit(0 if all_ok else 1)

if __name__ == "__main__":
    main()
