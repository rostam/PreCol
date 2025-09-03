import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from matplotlib.pyplot import pause

df_precol = pd.read_csv('output_precol.csv')
df_colpack = pd.read_csv('output_colpack.csv')
df_julia = pd.read_csv('output_julia.csv')

# Standardize column names
df_precol['Matrix'] = df_precol['Matrix'].astype(str).str.strip()
df_colpack['Matrix'] = df_colpack['Matrix'].astype(str).str.strip()
df_julia['Matrix'] = df_julia['Matrix'].astype(str).str.strip()

merged = df_precol.merge(df_colpack, on="Matrix", suffixes=('_PreCol', '_ColPack'))
merged = merged.merge(df_julia[['Matrix', 'NumberOfColorsJulia', 'TimeJulia']], on="Matrix")

merged['Diff_PreCol_vs_ColPack'] = merged['NumOfColors'] - merged['NumberOfColorsColpack']
merged['Diff_PreCol_vs_Julia'] = merged['NumOfColors'] - merged['NumberOfColorsJulia']
merged['Diff_ColPack_vs_Julia'] = merged['NumberOfColorsColpack'] - merged['NumberOfColorsJulia']

merged['AbsDiff_PreCol_vs_ColPack'] = merged['Diff_PreCol_vs_ColPack'].abs()
merged['AbsDiff_PreCol_vs_Julia'] = merged['Diff_PreCol_vs_Julia'].abs()
merged['AbsDiff_ColPack_vs_Julia'] = merged['Diff_ColPack_vs_Julia'].abs()

merged.to_csv("merged_comparison_all.csv", index=False)

# ---- Visualization ----
# Simplify matrix names
merged['Matrix_short'] = merged['Matrix'].apply(lambda x: x.split('/')[-1].replace('.mtx', ''))

# 1. Color counts comparison
colors_data = merged.melt(
    id_vars=['Matrix_short'],
    value_vars=['NumOfColors', 'NumberOfColorsColpack', 'NumberOfColorsJulia'],
    var_name='Source',
    value_name='ColorCount'
)

color_label_map = {
    'NumOfColors': 'PreCol',
    'NumberOfColorsColpack': 'ColPack',
    'NumberOfColorsJulia': 'Julia'
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
plt.title("Comparison of Number of Colors (PreCol vs ColPack vs Julia)")
plt.legend(title="Source")
plt.grid(axis='y')
plt.tight_layout()
plt.savefig("comparison_colors_all.png", dpi=300, bbox_inches="tight")
plt.show()


# 2. Coloring time comparison
time_data = merged.melt(
    id_vars=['Matrix_short'],
    value_vars=['Time', 'ColoringTimeColpack', 'TimeJulia'],
    var_name='Source',
    value_name='Time (seconds)'
)

time_label_map = {
    'Time': 'PreCol',
    'ColoringTimeColpack': 'ColPack',
    'TimeJulia': 'Julia'
}

time_data['Source'] = time_data['Source'].map(time_label_map)

plt.figure(figsize=(14, 6))
sns.barplot(
    data=time_data,
    x='Matrix_short',
    y='Time (seconds)',
    hue='Source'
)
plt.xticks(rotation=45, ha='right')
plt.title("Comparison of Coloring Times (PreCol vs ColPack vs Julia)")
plt.legend(title="Source")
plt.grid(axis='y')
plt.tight_layout()
plt.savefig("comparison_times_all.png", dpi=300, bbox_inches="tight")
plt.show()

