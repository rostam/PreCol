using SparseMatrixColorings
using MatrixMarket
using SparseArrays
using CSV
using DataFrames

function color_mtx_file(filepath::String)
    println("\n==============================")
    println("Loading matrix from: $filepath")
    A = mmread(filepath)

    println("Matrix size: ", size(A), ", nnz = ", nnz(A))

    problem = ColoringProblem(; structure = :nonsymmetric, partition = :column)
    algo    = GreedyColoringAlgorithm(; decompression = :direct)

    # Measure time
    t = @elapsed result = coloring(A, problem, algo)

    ncolors = maximum(column_colors(result))

    # Report results
    println("Coloring took $(round(t, digits=4)) seconds")
    println("Number of colors used: $ncolors")

    return (rows=size(A,1), cols=size(A,2), nnz=nnz(A), time=t, ncolors=ncolors)
end

# Run from CSV and save results
function run_from_csv(input_csv::String, output_csv::String)
    df = CSV.read(input_csv, DataFrame)

    results = DataFrame(
        Matrix=String[],
        Rows=Int[],
        Cols=Int[],
        NNZ=Int[],
        TimeJulia=Float64[],
        NumberOfColorsJulia=Int[]
    )

    for row in eachrow(df)
        filepath = row.Matrix
        try
            stats = color_mtx_file(filepath)
            push!(results, (
                filepath,
                stats.rows,
                stats.cols,
                stats.nnz,
                stats.time,
                stats.ncolors
            ))
        catch e
            @warn "Failed to process $filepath" exception=(e, catch_backtrace())
        end
    end

    CSV.write(output_csv, results)
    println("\n✅ Results written to $output_csv")
end

# Example usage
run_from_csv("input.csv", "Output/output_julia.csv")
