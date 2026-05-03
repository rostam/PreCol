#!/usr/bin/env python3
"""
Generate color-count and runtime comparison charts across PreCol, ColPack, Julia, and DSJM.
Reads CSV files from output/, writes plots to output/figures/.

Usage:
    python3 scripts/compare.py
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

# Normalize join key to bare filename
for df in [df_precol, df_colpack, df_julia]:
    df["Matrix"] = df["Matrix"].astype(str).str.strip()
    df["Matrix_key"] = df["Matrix"].apply(lambda x: x.split("/")[-1])

df_dsjm = df_dsjm.rename(columns={"MatrixName": "Matrix_key"})
df_dsjm["Matrix_key"] = df_dsjm["Matrix_key"].astype(str).str.strip()

merged = (df_precol
          .merge(df_colpack, on="Matrix_key", suffixes=("_PreCol", "_ColPack"))
          .merge(df_julia[["Matrix_key", "NumberOfColorsJulia", "TimeJulia"]], on="Matrix_key")
          .merge(df_dsjm[["Matrix_key", "NumOfColorsDSJM", "TimeDSJM"]], on="Matrix_key"))

merged["Diff_PreCol_vs_ColPack"]  = merged["NumOfColors"] - merged["NumberOfColorsColpack"]
merged["Diff_PreCol_vs_Julia"]    = merged["NumOfColors"] - merged["NumberOfColorsJulia"]
merged["Diff_ColPack_vs_Julia"]   = merged["NumberOfColorsColpack"] - merged["NumberOfColorsJulia"]
merged["Diff_PreCol_vs_DSJM"]     = merged["NumOfColors"] - merged["NumOfColorsDSJM"]
merged["Diff_ColPack_vs_DSJM"]    = merged["NumberOfColorsColpack"] - merged["NumOfColorsDSJM"]
merged["Diff_Julia_vs_DSJM"]      = merged["NumberOfColorsJulia"] - merged["NumOfColorsDSJM"]

merged["Matrix_short"] = merged["Matrix_key"].str.replace(".mtx", "", regex=False)
merged.to_csv(os.path.join(OUTPUT_DIR, "merged_comparison_all.csv"), index=False)

# ── Plot 1: color count comparison ────────────────────────────────
colors_data = merged.melt(
    id_vars=["Matrix_short"],
    value_vars=["NumOfColors", "NumberOfColorsColpack", "NumberOfColorsJulia", "NumOfColorsDSJM"],
    var_name="Source",
    value_name="ColorCount",
)
colors_data["Source"] = colors_data["Source"].map({
    "NumOfColors": "PreCol",
    "NumberOfColorsColpack": "ColPack",
    "NumberOfColorsJulia": "Julia",
    "NumOfColorsDSJM": "DSJM",
})

plt.figure(figsize=(14, 6))
sns.barplot(data=colors_data, x="Matrix_short", y="ColorCount", hue="Source")
plt.xticks(rotation=45, ha="right")
plt.title("Number of Colors — PreCol vs ColPack vs Julia vs DSJM")
plt.legend(title="Tool")
plt.grid(axis="y")
plt.tight_layout()
plt.savefig(os.path.join(FIGURES_DIR, "comparison_colors_all.png"), dpi=150,
            bbox_inches="tight")
print("Saved comparison_colors_all.png")

# ── Plot 2: coloring time comparison ──────────────────────────────
time_data = merged.melt(
    id_vars=["Matrix_short"],
    value_vars=["Time", "ColoringTimeColpack", "TimeJulia", "TimeDSJM"],
    var_name="Source",
    value_name="Time (s)",
)
time_data["Source"] = time_data["Source"].map({
    "Time": "PreCol",
    "ColoringTimeColpack": "ColPack",
    "TimeJulia": "Julia",
    "TimeDSJM": "DSJM",
})

plt.figure(figsize=(14, 6))
sns.barplot(data=time_data, x="Matrix_short", y="Time (s)", hue="Source")
plt.xticks(rotation=45, ha="right")
plt.title("Coloring Time — PreCol vs ColPack vs Julia vs DSJM")
plt.legend(title="Tool")
plt.grid(axis="y", which="both", linestyle="--", alpha=0.6)
plt.tight_layout()
plt.savefig(os.path.join(FIGURES_DIR, "comparison_times_all.png"), dpi=150,
            bbox_inches="tight")
print("Saved comparison_times_all.png")

plt.figure(figsize=(14, 6))
sns.barplot(data=time_data, x="Matrix_short", y="Time (s)", hue="Source")
plt.yscale("log")
plt.xticks(rotation=45, ha="right")
plt.title("Coloring Time (log scale) — PreCol vs ColPack vs Julia vs DSJM")
plt.legend(title="Tool")
plt.grid(axis="y", which="both", linestyle="--", alpha=0.6)
plt.tight_layout()
plt.savefig(os.path.join(FIGURES_DIR, "comparison_times_log.png"), dpi=150,
            bbox_inches="tight")
print("Saved comparison_times_log.png")
