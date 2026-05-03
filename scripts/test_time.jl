using SparseMatrixColorings
using MatrixMarket
using SparseArrays
using CSV
using DataFrames

const SCRIPTS_DIR = @__DIR__
const OUTPUT_DIR  = joinpath(SCRIPTS_DIR, "output")

function color_mtx_file(filepath::String)
    println("\n==============================")
    println("Loading matrix from: $filepath")
    A = mmread(filepath)

    println("Matrix size: ", size(A), ", nnz = ", nnz(A))

    problem = ColoringProblem(; structure = :nonsymmetric, partition = :column)
    algo    = GreedyColoringAlgorithm(; decompression = :direct)

    t = @elapsed result = coloring(A, problem, algo)

    ncolors = maximum(column_colors(result))

    println("Coloring took $(round(t, digits=4)) seconds")
    println("Number of colors used: $ncolors")

    return (rows=size(A,1), cols=size(A,2), nnz=nnz(A), time=t, ncolors=ncolors)
end

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
        if !isabspath(filepath)
            filepath = joinpath(dirname(SCRIPTS_DIR), filepath)
        end
        try
            stats = color_mtx_file(filepath)
            push!(results, (
                row.Matrix,
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

    mkpath(dirname(output_csv))
    CSV.write(output_csv, results)
    println("\nResults written to $output_csv")
end

run_from_csv(
    joinpath(SCRIPTS_DIR, "input.csv"),
    joinpath(OUTPUT_DIR,  "output_julia.csv")
)
