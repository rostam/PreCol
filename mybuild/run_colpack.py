import csv
import subprocess
import re

input_csv = "input.csv"
output_csv = "Output/output_colpack.csv"
colpack_executable = "./ColPack"


# Mapping CSV values to command-line options
ordering_map = {
    "LargestFirstOrderingDegrees": "LARGEST_FIRST",
    "SLO": "SMALLEST_LAST"
}


method_map = {
    "D2RestrictedColumns": "COLUMN_PARTIAL_DISTANCE_TWO",
    "D2RestrictedRows": "ROW_PARTIAL_DISTANCE_TWO",
    "D2Columns": "COLUMN_PARTIAL_DISTANCE_TWO",
    "D2Rows": "ROW_PARTIAL_DISTANCE_TWO",
}

# Function to extract metrics from ColPack output
def parse_colpack_output(output_text):
    print(output_text)
    ordering = method = coloring_time = num_colors = "N/A"

    for line in output_text.splitlines():
        if line.startswith("order:"):
            ordering = line.split(":")[1].strip()
        elif line.startswith("methd:"):
            method = line.split(":")[1].strip()
        elif "order+color time" in line:
            coloring_time = line[line.rindex("+")+1:]
            # match = re.search(r"order\+color time = ([0-9.]+)", line)
            # if match:
            #     coloring_time = match.group(1)
        elif "number of colors" in line:
            match = re.search(r"number of colors: (\d+)", line)
            if match:
                num_colors = match.group(1)
    return ordering, method, coloring_time, num_colors

# Open output CSV
with open(output_csv, mode='w', newline='') as out_csv:
    writer = csv.writer(out_csv)
    writer.writerow(["Matrix", "ExitCode", "Ordering", "Method", "ColoringTimeColpack", "NumberOfColorsColpack"])

    # Read input CSV
    with open(input_csv, mode='r') as in_csv:
        reader = csv.DictReader(in_csv)
        for row in reader:
            matrix_path = row["Matrix"]
            method = method_map.get(row["COLORING_ALGORITHM"], "DISTANCE_TWO")
            ordering = ordering_map.get(row["COLORING_ORDERING"], "LARGEST_FIRST")

            cmd = [
                colpack_executable,
                "-f", matrix_path,
                "-o", ordering,
                "-m", method,
                "-v"
            ]

            print(f"Running: {' '.join(cmd)}")

            try:
                result = subprocess.run(cmd, capture_output=True, text=True, timeout=120)
                exit_code = result.returncode
                ordering_out, method_out, time, colors = parse_colpack_output(result.stdout)
                writer.writerow([matrix_path, exit_code, ordering_out, method_out, time, colors])

            except Exception as e:
                writer.writerow([matrix_path, "ERROR", "N/A", "N/A", "N/A", str(e)])
