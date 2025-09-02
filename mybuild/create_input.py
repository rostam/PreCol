# file: create_matrix_csv.py

import os
import csv
import argparse

def create_matrix_csv(directory_path, output_csv_path):
    """
    Scans a directory for files and creates a CSV with their paths and
    predefined constant values for the other columns.

    Args:
        directory_path (str): The path to the folder to scan for files.
        output_csv_path (str): The path where the output CSV file will be saved.
    """
    # --- You can customize the constant values in this dictionary ---
    CSV_VALUES = {
        'COLORING_ALGORITHM': 'D2Columns',
        'COLORING_ORDERING': 'LargestFirstOrderingDegrees',
        'KIND_OF_SPARSIFICATION': 'Full',
        'BLOCK_SIZE': 0,
        'INDEPENDENT_SET_ALGORITHM': 'Best',
        'ALPHA_FOR_BALANCED_COLORING': 1,
        'RHO_FOR_INDEPENDENT_SET_ALGORITHM': 1 # Set to 1 for consistency with your example
    }

    # Define the header for the CSV file based on your requirements
    header = [
        'Matrix',
        'COLORING_ALGORITHM',
        'COLORING_ORDERING',
        'KIND_OF_SPARSIFICATION',
        'BLOCK_SIZE',
        'INDEPENDENT_SET_ALGORITHM',
        'ALPHA_FOR_BALANCED_COLORING',
        'RHO_FOR_INDEPENDENT_SET_ALGORITHM'
    ]

    try:
        # Get a sorted list of all *files* in the specified directory
        # os.listdir() gets all entries, we filter to include only files
        files = sorted([f for f in os.listdir(directory_path) if os.path.isfile(os.path.join(directory_path, f))])

        if not files:
            print(f"Warning: No files found in the directory '{directory_path}'.")
            return

        # Open the output CSV file in write mode
        with open(output_csv_path, 'w', newline='', encoding='utf-8') as csvfile:
            writer = csv.writer(csvfile)

            # Write the header row
            writer.writerow(header)

            # Iterate over each file found in the directory
            for filename in files:
                # Construct the full path for the 'Matrix' column
                # Using forward slashes for cross-platform consistency in the CSV
                matrix_path = os.path.join(directory_path, filename).replace('\\', '/')

                # Create the row data using the file path and the constant values
                row = [
                    matrix_path,
                    CSV_VALUES['COLORING_ALGORITHM'],
                    CSV_VALUES['COLORING_ORDERING'],
                    CSV_VALUES['KIND_OF_SPARSIFICATION'],
                    CSV_VALUES['BLOCK_SIZE'],
                    CSV_VALUES['INDEPENDENT_SET_ALGORITHM'],
                    CSV_VALUES['ALPHA_FOR_BALANCED_COLORING'],
                    CSV_VALUES['RHO_FOR_INDEPENDENT_SET_ALGORITHM']
                ]

                # Write the row to the CSV file
                writer.writerow(row)

        print(f"✅ Success! CSV file created at: {output_csv_path}")
        print(f"Processed {len(files)} files from '{directory_path}'.")

    except FileNotFoundError:
        print(f"❌ Error: The directory '{directory_path}' does not exist.")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")


def main():
    """Main function to parse command-line arguments and run the script."""
    parser = argparse.ArgumentParser(
        description="Reads all files from a folder and creates a CSV file with specified columns.",
        formatter_class=argparse.RawTextHelpFormatter
    )

    parser.add_argument(
        '-d', '--directory',
        type=str,
        required=True,
        help="Path to the directory containing the matrix files."
    )

    parser.add_argument(
        '-o', '--output',
        type=str,
        default='output.csv',
        help="Name for the output CSV file (default: output.csv)."
    )

    args = parser.parse_args()
    create_matrix_csv(args.directory, args.output)


if __name__ == '__main__':
    main()