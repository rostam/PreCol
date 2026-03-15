#!/usr/bin/env python3
import csv
import os
import subprocess

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
GCOLOR = os.path.expanduser("~/Downloads/dsjm/DSJM/examples/gcolor")
INPUT_CSV = os.path.join(SCRIPT_DIR, "input.csv")
OUTPUT_CSV = os.path.join(SCRIPT_DIR, "Output/output_dsjm.csv")

os.makedirs(os.path.dirname(OUTPUT_CSV), exist_ok=True)

with open(INPUT_CSV) as f:
    matrices = [row["Matrix"] for row in csv.DictReader(f)]

with open(OUTPUT_CSV, "w", newline="") as out:
    writer = csv.writer(out)
    writer.writerow(["MatrixName", "NumOfRows", "NumOfColumns", "NumOfColorsDSJM", "TimeDSJM"])

    for mtx in matrices:
        mtx_path = os.path.join(SCRIPT_DIR, mtx)
        result = subprocess.run(
            [GCOLOR, "-m", "lfo", "-i", mtx_path],
            capture_output=True, text=True
        )
        output = (result.stdout + result.stderr)

        result_line = None
        for line in output.splitlines():
            parts = line.split(",")
            if len(parts) == 7 and all(p.strip().replace(".", "").lstrip("-").isdigit() for p in parts[:3]):
                result_line = parts
                break

        if result_line is None:
            print(f"WARNING: no result line found for {mtx}")
            writer.writerow([os.path.basename(mtx), "", "", "", ""])
            continue

        rows = result_line[0].strip()
        cols = result_line[1].strip()
        colors = result_line[6].strip()
        ordering_time = float(result_line[4].strip())
        coloring_time = float(result_line[5].strip())
        total_time = ordering_time + coloring_time

        writer.writerow([os.path.basename(mtx), rows, cols, colors, f"{total_time:.6f}"])
        print(f"{os.path.basename(mtx)}: {colors} colors, {total_time:.6f}s")

print(f"\nResults written to {OUTPUT_CSV}")
