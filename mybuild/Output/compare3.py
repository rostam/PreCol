import os
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from matplotlib.pyplot import pause

FIGURES = os.path.join(os.path.dirname(os.path.abspath(__file__)), "figures")
os.makedirs(FIGURES, exist_ok=True)

df_precol = pd.read_csv('output_precol.csv')
df_colpack = pd.read_csv('output_colpack.csv')
df_julia = pd.read_csv('output_julia.csv')
df_dsjm = pd.read_csv('output_dsjm.csv')

# Standardize Matrix column: strip whitespace and use bare filename as join key
for df in [df_precol, df_colpack, df_julia]:
    df['Matrix'] = df['Matrix'].astype(str).str.strip()
    df['Matrix_key'] = df['Matrix'].apply(lambda x: x.split('/')[-1])

df_dsjm = df_dsjm.rename(columns={'MatrixName': 'Matrix_key'})
df_dsjm['Matrix_key'] = df_dsjm['Matrix_key'].astype(str).str.strip()

merged = df_precol.merge(df_colpack, on="Matrix_key", suffixes=('_PreCol', '_ColPack'))
merged = merged.merge(df_julia[['Matrix_key', 'NumberOfColorsJulia', 'TimeJulia']], on="Matrix_key")
merged = merged.merge(df_dsjm[['Matrix_key', 'NumOfColorsDSJM', 'TimeDSJM']], on="Matrix_key")

merged['Diff_PreCol_vs_ColPack'] = merged['NumOfColors'] - merged['NumberOfColorsColpack']
merged['Diff_PreCol_vs_Julia'] = merged['NumOfColors'] - merged['NumberOfColorsJulia']
merged['Diff_ColPack_vs_Julia'] = merged['NumberOfColorsColpack'] - merged['NumberOfColorsJulia']
merged['Diff_PreCol_vs_Dsjm'] = merged['NumOfColors'] - merged['NumOfColorsDSJM']
merged['Diff_ColPack_vs_Dsjm'] = merged['NumberOfColorsColpack'] - merged['NumOfColorsDSJM']
merged['Diff_Julia_vs_Dsjm'] = merged['NumberOfColorsJulia'] - merged['NumOfColorsDSJM']

merged['AbsDiff_PreCol_vs_ColPack'] = merged['Diff_PreCol_vs_ColPack'].abs()
merged['AbsDiff_PreCol_vs_Julia'] = merged['Diff_PreCol_vs_Julia'].abs()
merged['AbsDiff_ColPack_vs_Julia'] = merged['Diff_ColPack_vs_Julia'].abs()
merged['AbsDiff_PreCol_vs_Dsjm'] = merged['Diff_PreCol_vs_Dsjm'].abs()
merged['AbsDiff_ColPack_vs_Dsjm'] = merged['Diff_ColPack_vs_Dsjm'].abs()
merged['AbsDiff_Julia_vs_Dsjm'] = merged['Diff_Julia_vs_Dsjm'].abs()

merged.to_csv("merged_comparison_all.csv", index=False)

# ---- Visualization ----
# Matrix_key is already the bare filename (e.g. "144.mtx")
merged['Matrix_short'] = merged['Matrix_key'].str.replace('.mtx', '', regex=False)

# 1. Color counts comparison
colors_data = merged.melt(
    id_vars=['Matrix_short'],
    value_vars=['NumOfColors', 'NumberOfColorsColpack', 'NumberOfColorsJulia', 'NumOfColorsDSJM'],
    var_name='Source',
    value_name='ColorCount'
)

color_label_map = {
    'NumOfColors': 'PreCol',
    'NumberOfColorsColpack': 'ColPack',
    'NumberOfColorsJulia': 'Julia',
    'NumOfColorsDSJM': 'DSJM'
}
colors_data['Source'] = colors_data['Source'].map(color_label_map)

plt.figure(figsize=(14, 6))
sns.barplot(
    data=colors_data,
    x='Matrix_short',
    y='ColorCount',
    hue='Source'
)
plt.xticks(rotation=45, ha='right')
plt.title("Comparison of Number of Colors (PreCol vs ColPack vs Julia vs DSJM)")
plt.legend(title="Source")
plt.grid(axis='y')
plt.tight_layout()
plt.savefig(os.path.join(FIGURES, "comparison_colors_all.png"), dpi=300, bbox_inches="tight")
plt.show()


# 2. Coloring time comparison
time_data = merged.melt(
    id_vars=['Matrix_short'],
    value_vars=['Time', 'ColoringTimeColpack', 'TimeJulia', 'TimeDSJM'],
    var_name='Source',
    value_name='Time (seconds)'
)

time_label_map = {
    'Time': 'PreCol',
    'ColoringTimeColpack': 'ColPack',
    'TimeJulia': 'Julia',
    'TimeDSJM': 'DSJM'
}

time_data['Source'] = time_data['Source'].map(time_label_map)


plt.figure(figsize=(14, 6))
sns.barplot(
    data=time_data,
    x='Matrix_short',
    y='Time (seconds)',
    hue='Source'
)
#plt.yscale('log')  # <-- Make the y-axis logarithmic
plt.xticks(rotation=45, ha='right')
plt.title("Comparison of Coloring Times (PreCol vs ColPack vs Julia vs DSJM)")
plt.legend(title="Source")
plt.grid(axis='y', which='both', linestyle='--', alpha=0.6)  # Log-friendly grid
plt.tight_layout()
plt.savefig(os.path.join(FIGURES, "comparison_times_all_log.png"), dpi=300, bbox_inches="tight")
plt.show()
