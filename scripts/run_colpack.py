#!/usr/bin/env python3
"""
Run ColPack on every matrix in input.csv, writing results to output/output_colpack.csv.

Requires the ColPack binary to be on PATH or in the project build directory.

Usage:
    python3 run_colpack.py [--input INPUT_CSV] [--colpack PATH]
"""

import argparse
import csv
import os
import re
import shutil
import subprocess
import sys

SCRIPTS_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.dirname(SCRIPTS_DIR)
OUTPUT_DIR = os.path.join(SCRIPTS_DIR, "output")

ORDERING_MAP = {
    "LargestFirstOrderingDegrees": "LARGEST_FIRST",
    "SLO": "SMALLEST_LAST",
    "NaturalOrdering": "NATURAL",
    "IDO": "INCIDENCE_DEGREE",
}

METHOD_MAP = {
    "D2RestrictedColumns": "COLUMN_PARTIAL_DISTANCE_TWO",
    "D2RestrictedRows": "ROW_PARTIAL_DISTANCE_TWO",
    "D2Columns": "COLUMN_PARTIAL_DISTANCE_TWO",
    "D2Rows": "ROW_PARTIAL_DISTANCE_TWO",
}


def find_colpack(hint=None):
    candidates = [hint] if hint else []
    candidates += [
        shutil.which("ColPack"),
        os.path.join(PROJECT_ROOT, "mybuild", "ColPack"),
        os.path.join(PROJECT_ROOT, "cmake-build-release", "ColPack"),
    ]
    for c in candidates:
        if c and os.path.isfile(c) and os.access(c, os.X_OK):
            return c
    print("ERROR: ColPack binary not found.")
    print("  Install ColPack (https://github.com/CSCsw/ColPack) and ensure it is on PATH,")
    print("  or pass --colpack /path/to/ColPack.")
    sys.exit(1)


def parse_colpack_output(output_text):
    ordering = method = coloring_time = num_colors = "N/A"
    for line in output_text.splitlines():
        if line.startswith("order:"):
            ordering = line.split(":")[1].strip()
        elif line.startswith("methd:"):
            method = line.split(":")[1].strip()
        elif "order+color time" in line:
            coloring_time = line[line.rindex("+") + 1:].strip()
        elif "number of colors" in line:
            m = re.search(r"number of colors: (\d+)", line)
            if m:
                num_colors = m.group(1)
    return ordering, method, coloring_time, num_colors


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--input", default=os.path.join(SCRIPTS_DIR, "input.csv"))
    parser.add_argument("--colpack", default=None, help="Path to ColPack binary")
    args = parser.parse_args()

    colpack = find_colpack(args.colpack)
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    output_csv = os.path.join(OUTPUT_DIR, "output_colpack.csv")

    with open(args.input) as in_csv, open(output_csv, "w", newline="") as out_csv:
        reader = csv.DictReader(in_csv)
        writer = csv.writer(out_csv)
        writer.writerow(["Matrix", "ExitCode", "Ordering", "Method",
                         "ColoringTimeColpack", "NumberOfColorsColpack"])

        for row in reader:
            matrix_path = row["Matrix"]
            method = METHOD_MAP.get(row["COLORING_ALGORITHM"], "DISTANCE_TWO")
            ordering = ORDERING_MAP.get(row["COLORING_ORDERING"], "LARGEST_FIRST")

            cmd = [colpack, "-f", matrix_path, "-o", ordering,
                   "-m", method, "-nT", "1", "-v"]
            print(f"Running: {' '.join(cmd)}")

            try:
                result = subprocess.run(cmd, capture_output=True, text=True,
                                        timeout=120, cwd=PROJECT_ROOT)
                exit_code = result.returncode
                ordering_out, method_out, t, colors = parse_colpack_output(result.stdout)
                writer.writerow([matrix_path, exit_code, ordering_out, method_out, t, colors])
            except subprocess.TimeoutExpired:
                writer.writerow([matrix_path, "TIMEOUT", "N/A", "N/A", "N/A", "N/A"])
            except Exception as e:
                writer.writerow([matrix_path, "ERROR", "N/A", "N/A", "N/A", str(e)])

    print(f"\nResults written to {output_csv}")


if __name__ == "__main__":
    main()
