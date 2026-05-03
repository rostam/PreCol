#!/usr/bin/env python3
"""
Verify that the coloring produced by PreCol is valid: columns sharing a color
must not share any non-zero row (distance-2 coloring check).

Reads input.csv and the corresponding colors files written by PreCol.

Usage:
    python3 scripts/test_colors.py
    python3 scripts/test_colors.py --input scripts/input.csv --colors-dir /path/to/colors
"""

import argparse
import csv
import os
from collections import defaultdict

SCRIPTS_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.dirname(SCRIPTS_DIR)


def check_column_compression(matrix_file, column_colors):
    """Return dict mapping color -> True (compressible) / False (overlap detected)."""
    try:
        from scipy.io import mmread
    except ImportError:
        print("ERROR: scipy not found. Install with: pip install scipy")
        raise SystemExit(1)

    A = mmread(matrix_file).tocsr()
    n_rows, n_cols = A.shape

    if len(column_colors) != n_cols:
        raise ValueError(
            f"column_colors length {len(column_colors)} != matrix columns {n_cols} "
            f"for {matrix_file}"
        )

    color_groups = defaultdict(list)
    for j, c in enumerate(column_colors):
        color_groups[c].append(j)

    result = {}
    for color, cols in color_groups.items():
        seen_rows = set()
        valid = True
        for j in cols:
            rows_with_nonzero = set(A[:, j].nonzero()[0])
            if seen_rows & rows_with_nonzero:
                valid = False
                break
            seen_rows |= rows_with_nonzero
        result[color] = valid
    return result


def main():
    parser = argparse.ArgumentParser(
        description="Validate PreCol column coloring for each matrix in input.csv."
    )
    parser.add_argument("--input", default=os.path.join(SCRIPTS_DIR, "input.csv"),
                        help="Input CSV (default: scripts/input.csv)")
    parser.add_argument("--colors-dir", default=os.path.join(SCRIPTS_DIR, "colors_out"),
                        help="Directory containing '<matrix>.colors' files")
    args = parser.parse_args()

    if not os.path.exists(args.input):
        print(f"ERROR: input CSV not found: {args.input}")
        raise SystemExit(1)

    with open(args.input, newline="") as f:
        rows = list(csv.DictReader(f))

    all_ok = True
    for row in rows:
        matrix_path = row["Matrix"]
        if not os.path.isabs(matrix_path):
            matrix_path = os.path.join(PROJECT_ROOT, matrix_path)

        matrix_name = os.path.basename(matrix_path)
        colors_file = os.path.join(args.colors_dir, matrix_name + ".colors")

        if not os.path.exists(colors_file):
            print(f"  SKIP  {matrix_name}  (colors file not found: {colors_file})")
            continue

        with open(colors_file) as cf:
            raw = cf.read().strip()
        column_colors = [int(c) for c in raw.split("-") if c.strip()]

        if len(column_colors) > 60000:
            print(f"  SKIP  {matrix_name}  (too large: {len(column_colors)} columns)")
            continue

        print(f"  checking {matrix_name} ...")
        try:
            result = check_column_compression(matrix_path, column_colors)
        except (ValueError, Exception) as e:
            print(f"  ERROR {matrix_name}: {e}")
            all_ok = False
            continue

        invalid = [c for c, ok in result.items() if not ok]
        if invalid:
            print(f"  FAIL  {matrix_name}: invalid colors {invalid}")
            all_ok = False
        else:
            print(f"  OK    {matrix_name}: {len(result)} colors, all valid")

    raise SystemExit(0 if all_ok else 1)


if __name__ == "__main__":
    main()
