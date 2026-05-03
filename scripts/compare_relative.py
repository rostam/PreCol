#!/usr/bin/env python3
"""
Generate coloring-time comparison chart with times normalized relative to Julia.
Reads CSV files from output/, writes plot to output/figures/relative_times.png.

Usage:
    python3 scripts/compare_relative.py
"""

import os
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

SCRIPTS_DIR = os.path.dirname(os.path.abspath(__file__))
OUTPUT_DIR  = os.path.join(SCRIPTS_DIR, "output")
FIGURES_DIR = os.path.join(OUTPUT_DIR, "figures")
os.makedirs(FIGURES_DIR, exist_ok=True)


def load_csv(name):
    path = os.path.join(OUTPUT_DIR, name)
    if not os.path.exists(path):
        raise FileNotFoundError(f"Expected output file not found: {path}\n"
                                f"Run the benchmark scripts first.")
    return pd.read_csv(path)


df_precol  = load_csv("output_precol.csv")
df_colpack = load_csv("output_colpack.csv")
df_julia   = load_csv("output_julia.csv")
df_dsjm    = load_csv("output_dsjm.csv")

for df in [df_precol, df_colpack, df_julia]:
    df["Matrix_key"] = df["Matrix"].astype(str).apply(lambda x: x.split("/")[-1])

df_dsjm = df_dsjm.rename(columns={"MatrixName": "Matrix_key"})
df_dsjm["Matrix_key"] = df_dsjm["Matrix_key"].astype(str).str.strip()

df_merged = (df_julia[["Matrix_key", "TimeJulia"]]
             .merge(df_precol[["Matrix_key", "Time"]].rename(columns={"Time": "TimePrecol"}),
                    on="Matrix_key")
             .merge(df_colpack[["Matrix_key", "ColoringTimeColpack"]]
                    .rename(columns={"ColoringTimeColpack": "TimeColpack"}),
                    on="Matrix_key")
             .merge(df_dsjm[["Matrix_key", "TimeDSJM"]], on="Matrix_key"))

df_merged["PreCol / Julia"]  = df_merged["TimePrecol"]  / df_merged["TimeJulia"]
df_merged["ColPack / Julia"] = df_merged["TimeColpack"]  / df_merged["TimeJulia"]
df_merged["DSJM / Julia"]    = df_merged["TimeDSJM"]     / df_merged["TimeJulia"]

df_plot = df_merged.melt(
    id_vars="Matrix_key",
    value_vars=["PreCol / Julia", "ColPack / Julia", "DSJM / Julia"],
    var_name="Method",
    value_name="Relative Time",
)

plt.figure(figsize=(10, 6))
sns.barplot(data=df_plot, x="Matrix_key", y="Relative Time", hue="Method")
plt.yscale("log")
plt.axhline(y=1.0, color="gray", linestyle="--", linewidth=1, label="Julia baseline")
plt.xticks(rotation=45, ha="right")
plt.title("Coloring Time Relative to Julia (lower = faster)")
plt.ylabel("Relative Time (tool / Julia)")
plt.xlabel("Matrix")
plt.tight_layout()
plt.savefig(os.path.join(FIGURES_DIR, "relative_times.png"), dpi=150, bbox_inches="tight")
print("Saved relative_times.png")
