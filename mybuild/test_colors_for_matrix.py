import numpy as np
from scipy.io import mmread
from collections import defaultdict
import csv

def check_column_compression(matrix_file, column_colors):
    """
    Check if columns with the same color can be compressed together without overlap.

    Parameters:
    ------------
    matrix_file : str
        Path to the matrix in .mtx (Matrix Market) format.
    column_colors : list[int]
        List of colors assigned to columns. Length must equal the number of columns in matrix.

    Returns:
    --------
    dict
        Dictionary mapping color -> True/False (compressible or not).
    """

    # Load sparse matrix
    A = mmread(matrix_file).tocsr()
    n_rows, n_cols = A.shape

    print(len(column_colors), n_cols)
    if len(column_colors) > 60000:
        return None
    if len(column_colors) != n_cols:
        raise ValueError(f"Length of column_colors must match number of columns in the matrix {matrix_file}.")

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


# Example usage:
if __name__ == "__main__":
    input_csv = "input.csv"
    with open(input_csv, mode='r') as in_csv:
        reader = csv.DictReader(in_csv)
        for row in reader:
            matrix_path = row["Matrix"]
            with open("colors_out/"+matrix_path+"colors", mode='r') as colors_file:
                column_colors = colors_file.read().split("-")[:-1]
                print(matrix_path)
                result = check_column_compression(matrix_path, column_colors)
                if result is None:
                    continue
                print("Compression possible by color:")
                for color, ok in result.items():
                    if not ok:
                        print(f"Matrix {matrix_path}, Color {color}: {'Yes' if ok else 'No'}")
