#!/usr/bin/env python3
"""
Download a curated set of matrices from SuiteSparse in Matrix Market format.

Usage:
    python3 scripts/download_matrices.py
    python3 scripts/download_matrices.py -o /path/to/output/dir
"""

import argparse
import os
import sys

SCRIPTS_DIR = os.path.dirname(os.path.abspath(__file__))
DEFAULT_OUTPUT_DIR = os.path.join(os.path.dirname(SCRIPTS_DIR),
                                  "ExampleMatrices", "FloridaSparseMatrixCollection")

# (Group, Name) pairs from the SuiteSparse collection
MATRICES = [
    ("LPnetlib", "lp_ken_11"),
    ("LPnetlib", "lp_ken_13"),
    ("LPnetlib", "lp_maros_r7"),
    ("LPnetlib", "lp_cre_d"),
    ("LPnetlib", "lp_ken_18"),
    ("Bai",      "af23560"),
    ("Shen",     "e40r0100"),
    ("vanHeukelum", "cage11"),
    ("vanHeukelum", "cage12"),
    ("LPnetlib", "lp_standata"),
    ("LPnetlib", "lp_scagr25"),
    ("LPnetlib", "lp_scagr7"),
    ("LPnetlib", "lp_stair"),
    ("LPnetlib", "lp_blend"),
    ("LPnetlib", "lp_vtp_base"),
    ("LPnetlib", "lp_agg"),
    ("LPnetlib", "lp_agg2"),
    ("LPnetlib", "lp_agg3"),
    ("LPnetlib", "lp_bore3d"),
    ("LPnetlib", "lp_israel"),
    ("LPnetlib", "lp_tuff"),
    ("LPnetlib", "lp_adlittle"),
    ("HB", "watt_2"),
    ("HB", "can_256"),
    ("HB", "can_268"),
    ("HB", "can_292"),
    ("HB", "can_634"),
    ("HB", "can_715"),
    ("HB", "can_1054"),
    ("HB", "can_1072"),
    ("HB", "impcol_c"),
    ("HB", "impcol_d"),
    ("HB", "impcol_e"),
    ("HB", "west0067"),
    ("HB", "west0381"),
    ("HB", "west0497"),
    ("HB", "gent113"),
    ("HB", "arc130"),
    ("DIMACS10", "598a"),
    ("DIMACS10", "144"),
    ("DIMACS10", "m14b"),
    ("DIMACS10", "auto"),
]

BASE_URL = "https://sparse.tamu.edu/MM/{}/{}.tar.gz"


def download_matrices(output_dir):
    try:
        import requests
    except ImportError:
        print("ERROR: 'requests' package not found. Install it with: pip install requests")
        sys.exit(1)

    import tarfile

    os.makedirs(output_dir, exist_ok=True)

    for group, name in MATRICES:
        mtx_path = os.path.join(output_dir, f"{name}.mtx")
        if os.path.exists(mtx_path):
            print(f"  skip  {name}.mtx (already present)")
            continue

        url = BASE_URL.format(group, name)
        print(f"  fetch {name}  ({url})")
        try:
            response = requests.get(url, stream=True, timeout=60)
            response.raise_for_status()
        except requests.exceptions.RequestException as e:
            print(f"  ERROR downloading {name}: {e}")
            continue

        tar_path = os.path.join(output_dir, f"{name}.tar.gz")
        with open(tar_path, "wb") as f:
            for chunk in response.iter_content(chunk_size=65536):
                f.write(chunk)

        try:
            with tarfile.open(tar_path, "r:gz") as tar:
                for member in tar.getmembers():
                    if member.name.endswith(".mtx"):
                        member.name = os.path.basename(member.name)
                        tar.extract(member, path=output_dir)
                        print(f"  saved {member.name}")
                        break
        except tarfile.TarError as e:
            print(f"  ERROR extracting {name}: {e}")
        finally:
            os.remove(tar_path)


def main():
    parser = argparse.ArgumentParser(
        description="Download SuiteSparse matrices in Matrix Market (.mtx) format."
    )
    parser.add_argument("-o", "--output", default=DEFAULT_OUTPUT_DIR,
                        help=f"Output directory (default: {DEFAULT_OUTPUT_DIR})")
    args = parser.parse_args()
    download_matrices(args.output)
    print(f"\nDone. Matrices saved to {args.output}")


if __name__ == "__main__":
    main()
