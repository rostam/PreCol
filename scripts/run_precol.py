#!/usr/bin/env python3
"""
Run PreCol on every row of input.csv, one matrix at a time, writing
results incrementally to output/output_precol.csv.

Usage:
    python3 run_precol.py [--input INPUT_CSV] [--timeout SECONDS]
"""

import argparse
import csv
import os
import subprocess
import sys
import tempfile
import time

SCRIPTS_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.dirname(SCRIPTS_DIR)
OUTPUT_DIR = os.path.join(SCRIPTS_DIR, "output")

TIMEOUT_SEC = 120


def find_precol():
    candidates = [
        os.path.join(PROJECT_ROOT, "cmake-build-release", "PreCol"),
        os.path.join(PROJECT_ROOT, "mybuild", "PreCol"),
        os.path.join(PROJECT_ROOT, "build", "PreCol"),
    ]
    for c in candidates:
        if os.path.isfile(c) and os.access(c, os.X_OK):
            return c
    print("ERROR: PreCol binary not found. Build the project first (cmake + make).")
    print("  Searched:", "\n    ".join(candidates))
    sys.exit(1)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--input", default=os.path.join(SCRIPTS_DIR, "input.csv"),
                        help="Input CSV file (default: scripts/input.csv)")
    parser.add_argument("--timeout", type=int, default=TIMEOUT_SEC,
                        help="Per-matrix timeout in seconds (default: 120)")
    args = parser.parse_args()

    precol = find_precol()
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    output_csv = os.path.join(OUTPUT_DIR, "output_precol.csv")

    with open(args.input) as f:
        reader = csv.DictReader(f)
        fieldnames = reader.fieldnames
        rows = list(reader)

    output_header_written = False

    with open(output_csv, "w", newline="") as out_f:
        out_writer = None

        for i, row in enumerate(rows, 1):
            matrix = row["Matrix"]
            name = os.path.basename(matrix)
            print(f"[{i}/{len(rows)}] PreCol  {name} ...", end="  ", flush=True)

            with tempfile.NamedTemporaryFile(
                mode="w", suffix=".csv", delete=False, dir=SCRIPTS_DIR
            ) as tmp:
                tmp_path = tmp.name
                writer = csv.DictWriter(tmp, fieldnames=fieldnames)
                writer.writeheader()
                writer.writerow(row)

            tmp_out = tmp_path + ".out.csv"

            t0 = time.time()
            try:
                result = subprocess.run(
                    [precol, tmp_path, tmp_out],
                    cwd=PROJECT_ROOT,
                    timeout=args.timeout,
                    stdout=subprocess.DEVNULL,
                    stderr=subprocess.DEVNULL,
                )
                elapsed = time.time() - t0

                if result.returncode == 0 and os.path.exists(tmp_out):
                    with open(tmp_out) as res_f:
                        res_reader = csv.DictReader(res_f)
                        result_rows = list(res_reader)

                    if result_rows:
                        if not output_header_written:
                            out_writer = csv.DictWriter(out_f, fieldnames=res_reader.fieldnames)
                            out_writer.writeheader()
                            output_header_written = True
                        for r in result_rows:
                            out_writer.writerow(r)
                        out_f.flush()
                        colors = result_rows[0].get("NumOfColors", "?")
                        print(f"{colors} colors  {elapsed:.2f}s", flush=True)
                    else:
                        print(f"FAILED (empty output)  {elapsed:.2f}s", flush=True)
                else:
                    print(f"FAILED (exit {result.returncode})  {elapsed:.2f}s", flush=True)

            except subprocess.TimeoutExpired:
                elapsed = time.time() - t0
                print(f"TIMEOUT after {elapsed:.0f}s — skipped", flush=True)

            finally:
                os.unlink(tmp_path)
                if os.path.exists(tmp_out):
                    os.unlink(tmp_out)

    print(f"\nResults written to {output_csv}")


if __name__ == "__main__":
    main()
