#!/usr/bin/env python3
"""
Wrapper that runs ./PreCol one matrix at a time so progress is visible
and a stuck matrix doesn't block the rest.
"""

import csv
import os
import subprocess
import sys
import tempfile
import time

SCRIPT_DIR  = os.path.dirname(os.path.abspath(__file__))
PRECOL      = os.path.join(SCRIPT_DIR, "./PreCol")
INPUT_CSV   = os.path.join(SCRIPT_DIR, "input.csv")
OUTPUT_CSV  = os.path.join(SCRIPT_DIR, "Output/output_precol.csv")
TIMEOUT_SEC = 120   # kill a single matrix run after this many seconds

os.makedirs(os.path.dirname(OUTPUT_CSV), exist_ok=True)

# Read all rows from the input CSV
with open(INPUT_CSV) as f:
    reader = csv.DictReader(f)
    fieldnames = reader.fieldnames
    rows = list(reader)

output_header_written = False

with open(OUTPUT_CSV, "w", newline="") as out_f:
    out_writer = None  # initialised after first successful run

    for i, row in enumerate(rows, 1):
        matrix = row["Matrix"]
        name   = os.path.basename(matrix)
        print(f"[{i}/{len(rows)}] PreCol  {name} ...", end="  ", flush=True)

        # Write a temp single-row CSV for this matrix
        with tempfile.NamedTemporaryFile(
            mode="w", suffix=".csv", delete=False, dir=SCRIPT_DIR
        ) as tmp:
            tmp_path = tmp.name
            writer = csv.DictWriter(tmp, fieldnames=fieldnames)
            writer.writeheader()
            writer.writerow(row)

        tmp_out = tmp_path + ".out.csv"

        t0 = time.time()
        try:
            result = subprocess.run(
                [PRECOL, tmp_path, tmp_out],
                cwd=SCRIPT_DIR,
                timeout=TIMEOUT_SEC,
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

print(f"\nResults written to {OUTPUT_CSV}")
