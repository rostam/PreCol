#!/usr/bin/env python3
"""
Run DSJM on every matrix in input.csv, writing results to output/output_dsjm.csv.

Requires the DSJM 'gcolor' binary. Set DSJM_GCOLOR env var or pass --gcolor to
specify its location. Build DSJM from https://github.com/BenBrock/dsjm.

Usage:
    DSJM_GCOLOR=/path/to/gcolor python3 run_dsjm.py
    python3 run_dsjm.py --gcolor /path/to/gcolor [--input INPUT_CSV]
"""

import argparse
import csv
import os
import shutil
import subprocess
import sys

SCRIPTS_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.dirname(SCRIPTS_DIR)
OUTPUT_DIR = os.path.join(SCRIPTS_DIR, "output")


def find_gcolor(hint=None):
    candidates = [
        hint,
        os.environ.get("DSJM_GCOLOR"),
        shutil.which("gcolor"),
        os.path.expanduser("~/Downloads/dsjm/DSJM/examples/gcolor"),
    ]
    for c in candidates:
        if c and os.path.isfile(c) and os.access(c, os.X_OK):
            return c
    print("ERROR: DSJM 'gcolor' binary not found.")
    print("  Build DSJM and set the DSJM_GCOLOR environment variable, or pass --gcolor.")
    sys.exit(1)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--input", default=os.path.join(SCRIPTS_DIR, "input.csv"))
    parser.add_argument("--gcolor", default=None, help="Path to DSJM gcolor binary")
    args = parser.parse_args()

    gcolor = find_gcolor(args.gcolor)
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    output_csv = os.path.join(OUTPUT_DIR, "output_dsjm.csv")

    with open(args.input) as f:
        matrices = [row["Matrix"] for row in csv.DictReader(f)]

    with open(output_csv, "w", newline="") as out:
        writer = csv.writer(out)
        writer.writerow(["MatrixName", "NumOfRows", "NumOfColumns",
                         "NumOfColorsDSJM", "TimeDSJM"])

        for mtx in matrices:
            mtx_path = os.path.join(PROJECT_ROOT, mtx) if not os.path.isabs(mtx) else mtx
            result = subprocess.run(
                [gcolor, "-m", "lfo", "-i", mtx_path],
                capture_output=True, text=True,
            )
            output = result.stdout + result.stderr

            result_line = None
            for line in output.splitlines():
                parts = line.split(",")
                if (len(parts) == 7 and
                        all(p.strip().replace(".", "").lstrip("-").isdigit()
                            for p in parts[:3])):
                    result_line = parts
                    break

            if result_line is None:
                print(f"WARNING: no result line for {mtx}")
                writer.writerow([os.path.basename(mtx), "", "", "", ""])
                continue

            rows = result_line[0].strip()
            cols = result_line[1].strip()
            colors = result_line[6].strip()
            total_time = float(result_line[4].strip()) + float(result_line[5].strip())

            writer.writerow([os.path.basename(mtx), rows, cols, colors,
                             f"{total_time:.6f}"])
            print(f"{os.path.basename(mtx)}: {colors} colors, {total_time:.6f}s")

    print(f"\nResults written to {output_csv}")


if __name__ == "__main__":
    main()
