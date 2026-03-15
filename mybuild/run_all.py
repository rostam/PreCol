#!/usr/bin/env python3
"""
Master benchmark runner.
Run from the mybuild/ directory:  python3 run_all.py [--skip-precol] [--skip-julia]
                                                      [--skip-colpack] [--skip-dsjm]
                                                      [--skip-compare]
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
    print(f"{'='*60}")

def run(label, cmd, cwd=MYBUILD, timeout=3600):
    banner(label)
    print(f"  cwd : {cwd}")
    print(f"  cmd : {' '.join(cmd)}\n")
    t0 = time.time()
    result = subprocess.run(cmd, cwd=cwd, timeout=timeout)
    elapsed = time.time() - t0
    status = "OK" if result.returncode == 0 else f"FAILED (exit {result.returncode})"
    print(f"\n  [{status}]  elapsed: {elapsed:.1f}s")
    return result.returncode == 0

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

    # ── 1. PreCol ──────────────────────────────────────────────────
    if not args.skip_precol:
        results["PreCol"] = run(
            "Running PreCol",
            ["./PreCol", "input.csv", "Output/output_precol.csv"],
        )

    # ── 2. Julia ───────────────────────────────────────────────────
    if not args.skip_julia:
        results["Julia"] = run(
            "Running Julia (SparseMatrixColorings)",
            ["/home/rostam/.juliaup/bin/julia", "test_time.jl"],
        )

    # ── 3. ColPack ─────────────────────────────────────────────────
    if not args.skip_colpack:
        results["ColPack"] = run(
            "Running ColPack",
            [sys.executable, "run_colpack.py"],
        )

    # ── 4. DSJM ────────────────────────────────────────────────────
    if not args.skip_dsjm:
        results["DSJM"] = run(
            "Running DSJM",
            [sys.executable, "run_dsjm.py"],
        )

    # ── 5. Compare scripts ─────────────────────────────────────────
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
