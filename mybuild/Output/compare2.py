import os
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

FIGURES = os.path.join(os.path.dirname(os.path.abspath(__file__)), "figures")
os.makedirs(FIGURES, exist_ok=True)

# Read CSVs
df_precol = pd.read_csv('output_precol.csv')
df_colpack = pd.read_csv('output_colpack.csv')
df_julia = pd.read_csv('output_julia.csv')
df_dsjm = pd.read_csv('output_dsjm.csv')

for df in [df_precol, df_colpack, df_julia]:
    df['Matrix_key'] = df['Matrix'].astype(str).apply(lambda x: x.split('/')[-1])

df_dsjm = df_dsjm.rename(columns={'MatrixName': 'Matrix_key'})
df_dsjm['Matrix_key'] = df_dsjm['Matrix_key'].astype(str).str.strip()

# Select relevant columns and rename for merging clarity
df_precol_sel = df_precol[['Matrix_key', 'Time']].rename(columns={'Time': 'TimePrecol'})
df_colpack_sel = df_colpack[['Matrix_key', 'ColoringTimeColpack']].rename(columns={'ColoringTimeColpack': 'TimeColpack'})
df_julia_sel = df_julia[['Matrix_key', 'TimeJulia']]
df_dsjm_sel = df_dsjm[['Matrix_key', 'TimeDSJM']]

# Merge all on Matrix_key
df_merged = df_julia_sel.merge(df_precol_sel, on='Matrix_key', how='inner') \
    .merge(df_colpack_sel, on='Matrix_key', how='inner') \
    .merge(df_dsjm_sel, on='Matrix_key', how='inner')

# Compute relative times (relative to Julia)
df_merged['Precol/Julia'] = df_merged['TimePrecol'] / df_merged['TimeJulia']
df_merged['Colpack/Julia'] = df_merged['TimeColpack'] / df_merged['TimeJulia']
df_merged['Dsjm/Julia'] = df_merged['TimeDSJM'] / df_merged['TimeJulia']

# Reshape for plotting
df_plot = df_merged.melt(id_vars='Matrix_key',
                         value_vars=['Precol/Julia', 'Colpack/Julia', 'Dsjm/Julia'],
                         var_name='Method',
                         value_name='RelativeTime')

# Plot
plt.figure(figsize=(10, 6))
sns.barplot(data=df_plot, x='Matrix_key', y='RelativeTime', hue='Method')
plt.yscale('log')
plt.xticks(rotation=45, ha='right')
plt.title('Coloring Time Relative to Julia')
plt.ylabel('Relative Time (Method / Julia)')
plt.xlabel('Matrix (filename)')
plt.tight_layout()
plt.savefig(os.path.join(FIGURES, "relative_times.png"), dpi=300, bbox_inches="tight")
plt.show()
