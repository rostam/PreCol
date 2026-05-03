#!/usr/bin/env python3
"""
Master benchmark runner — runs PreCol, ColPack, Julia, and DSJM in parallel,
then generates comparison plots.

Usage (from any directory):
    python3 scripts/run_all.py [options]

Options:
    --skip-precol    skip PreCol benchmark
    --skip-julia     skip Julia benchmark
    --skip-colpack   skip ColPack benchmark
    --skip-dsjm      skip DSJM benchmark
    --skip-compare   skip comparison plots
"""

import argparse
import os
import shutil
import subprocess
import sys
import time

SCRIPTS_DIR = os.path.dirname(os.path.abspath(__file__))
OUTPUT_DIR  = os.path.join(SCRIPTS_DIR, "output")

HEARTBEAT_INTERVAL = 10


def banner(text):
    print(f"\n{'='*60}")
    print(f"  {text}")
    print(f"{'='*60}", flush=True)


def find_julia():
    candidates = [
        shutil.which("julia"),
        os.path.expanduser("~/.juliaup/bin/julia"),
        "/usr/local/bin/julia",
        "/usr/bin/julia",
    ]
    for c in candidates:
        if c and os.path.isfile(c) and os.access(c, os.X_OK):
            return c
    print("WARNING: julia binary not found; skipping Julia benchmark.")
    return None


def run(label, cmd, cwd=SCRIPTS_DIR, timeout=3600):
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
    banner(f"Launching {len(tools)} tools in parallel")
    print(f"  {'LABEL':<12}  COMMAND", flush=True)
    print(f"  {'-'*12}  {'-'*44}")
    for label, cmd, _ in tools:
        print(f"  {label:<12}  {' '.join(cmd)}", flush=True)

    t0 = time.time()
    procs = {}
    for label, cmd, cwd in tools:
        procs[label] = (subprocess.Popen(cmd, cwd=cwd), time.time())
    print(f"\n  All {len(tools)} processes started.\n", flush=True)

    results = {}
    last_heartbeat = time.time()

    while procs:
        for label in list(procs):
            p, t_start = procs[label]
            if p.poll() is not None:
                elapsed = time.time() - t_start
                ok = p.returncode == 0
                status = "DONE  ✓" if ok else f"FAILED ✗ (exit {p.returncode})"
                print(f"  [{status}]  {label:<12}  {elapsed:.1f}s", flush=True)
                results[label] = ok
                del procs[label]

        if procs and time.time() - last_heartbeat >= HEARTBEAT_INTERVAL:
            still = ", ".join(
                f"{lbl} ({time.time()-ts:.0f}s)"
                for lbl, (_, ts) in procs.items()
            )
            print(f"  [running]  {still}", flush=True)
            last_heartbeat = time.time()

        if procs:
            time.sleep(1)

    return results


def main():
    parser = argparse.ArgumentParser(
        description="Run all coloring benchmarks and comparison plots."
    )
    parser.add_argument("--skip-precol",  action="store_true")
    parser.add_argument("--skip-julia",   action="store_true")
    parser.add_argument("--skip-colpack", action="store_true")
    parser.add_argument("--skip-dsjm",    action="store_true")
    parser.add_argument("--skip-compare", action="store_true")
    args = parser.parse_args()

    os.makedirs(OUTPUT_DIR, exist_ok=True)
    results = {}

    # ── Phase 1: run all tools in parallel ────────────────────────
    tools = []
    if not args.skip_precol:
        tools.append(("PreCol",
                       [sys.executable, os.path.join(SCRIPTS_DIR, "run_precol.py")],
                       SCRIPTS_DIR))

    if not args.skip_julia:
        julia = find_julia()
        if julia:
            tools.append(("Julia",
                           [julia, os.path.join(SCRIPTS_DIR, "test_time.jl")],
                           SCRIPTS_DIR))

    if not args.skip_colpack:
        tools.append(("ColPack",
                       [sys.executable, os.path.join(SCRIPTS_DIR, "run_colpack.py")],
                       SCRIPTS_DIR))

    if not args.skip_dsjm:
        tools.append(("DSJM",
                       [sys.executable, os.path.join(SCRIPTS_DIR, "run_dsjm.py")],
                       SCRIPTS_DIR))

    if tools:
        results.update(run_parallel(tools))

    # ── Phase 2: comparison plots (run after phase 1) ─────────────
    if not args.skip_compare:
        for script in ["compare.py", "compare_relative.py"]:
            script_path = os.path.join(SCRIPTS_DIR, script)
            if os.path.exists(script_path):
                results[script] = run(
                    f"Running {script}",
                    [sys.executable, script_path],
                    cwd=SCRIPTS_DIR,
                )

    # ── Summary ───────────────────────────────────────────────────
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
