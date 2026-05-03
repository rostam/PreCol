#!/usr/bin/env python3
"""
Scan a directory of .mtx files and generate an input.csv for the benchmark scripts.

Usage:
    python3 scripts/create_input.py -d ExampleMatrices/FloridaSparseMatrixCollection
    python3 scripts/create_input.py -d /path/to/matrices -o scripts/input.csv
"""

import argparse
import csv
import os

SCRIPTS_DIR = os.path.dirname(os.path.abspath(__file__))

# Default benchmark parameters — edit here or override via a custom script
CSV_DEFAULTS = {
    "COLORING_ALGORITHM":              "D2Columns",
    "COLORING_ORDERING":               "LargestFirstOrderingDegrees",
    "KIND_OF_SPARSIFICATION":          "Full",
    "BLOCK_SIZE":                      0,
    "INDEPENDENT_SET_ALGORITHM":       "Best",
    "ALPHA_FOR_BALANCED_COLORING":     0,
    "RHO_FOR_INDEPENDENT_SET_ALGORITHM": 1.0,
}

HEADER = [
    "Matrix",
    "COLORING_ALGORITHM",
    "COLORING_ORDERING",
    "KIND_OF_SPARSIFICATION",
    "BLOCK_SIZE",
    "INDEPENDENT_SET_ALGORITHM",
    "ALPHA_FOR_BALANCED_COLORING",
    "RHO_FOR_INDEPENDENT_SET_ALGORITHM",
]


def create_input_csv(directory, output_path):
    try:
        files = sorted(
            f for f in os.listdir(directory)
            if os.path.isfile(os.path.join(directory, f)) and f.endswith(".mtx")
        )
    except FileNotFoundError:
        print(f"ERROR: directory not found: {directory}")
        raise SystemExit(1)

    if not files:
        print(f"WARNING: no .mtx files found in {directory}")
        return

    with open(output_path, "w", newline="", encoding="utf-8") as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(HEADER)
        for filename in files:
            matrix_path = os.path.join(directory, filename).replace("\\", "/")
            writer.writerow([
                matrix_path,
                CSV_DEFAULTS["COLORING_ALGORITHM"],
                CSV_DEFAULTS["COLORING_ORDERING"],
                CSV_DEFAULTS["KIND_OF_SPARSIFICATION"],
                CSV_DEFAULTS["BLOCK_SIZE"],
                CSV_DEFAULTS["INDEPENDENT_SET_ALGORITHM"],
                CSV_DEFAULTS["ALPHA_FOR_BALANCED_COLORING"],
                CSV_DEFAULTS["RHO_FOR_INDEPENDENT_SET_ALGORITHM"],
            ])

    print(f"Created {output_path}  ({len(files)} matrices)")


def main():
    parser = argparse.ArgumentParser(
        description="Generate input.csv for PreCol benchmarks from a directory of .mtx files."
    )
    parser.add_argument("-d", "--directory", required=True,
                        help="Directory containing .mtx files")
    parser.add_argument("-o", "--output",
                        default=os.path.join(SCRIPTS_DIR, "input.csv"),
                        help="Output CSV path (default: scripts/input.csv)")
    args = parser.parse_args()
    create_input_csv(args.directory, args.output)


if __name__ == "__main__":
    main()
