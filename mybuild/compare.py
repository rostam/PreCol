import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
# Load CSVs
df1 = pd.read_csv('output.csv')  # Replace it with the actual path to the first CSV
df2 = pd.read_csv('OutputTable.csv')  # Replace it with the actual path to the second CSV

# Strip whitespace and standardize
df1['Matrix'] = df1['Matrix'].astype(str).str.strip()
df2['Matrix'] = df2['Matrix'].astype(str).str.strip()

# Merge
merged = pd.merge(df1, df2, on='Matrix', suffixes=('_Original', '_New'))

# Compute differences
merged['ColorDifference'] = merged['NumOfColors'] - merged['NumberOfColors']
merged['AbsoluteDifference'] = merged['ColorDifference'].abs()
merged['RelativeDifference'] = (
        merged['AbsoluteDifference'] / merged[['NumOfColors', 'NumberOfColors']].max(axis=1)
)

# Flag significant differences
THRESHOLD_ABS = 10
THRESHOLD_REL = 0.10
merged['SignificantDifference'] = (
        (merged['AbsoluteDifference'] > THRESHOLD_ABS) |
        (merged['RelativeDifference'] > THRESHOLD_REL)
)

# Print significant differences
print("Significant Differences:")
print(merged[merged['SignificantDifference']][[
    'Matrix', 'NumOfColors', 'NumberOfColors', 'ColorDifference', 'RelativeDifference'
]])

# Save merged table
merged.to_csv('merged_comparison_cleaned.csv', index=False)

# ---- Visualization ----

# Prepare data for plotting with new labels
melted = merged.melt(
    id_vars=['Matrix', 'SignificantDifference'],
    value_vars=['NumOfColors', 'NumberOfColors'],
    var_name='Source',
    value_name='ColorCount'
)

# Create a mapping for the labels
label_map = {
    'NumOfColors': 'Number of Colors from PreCol',
    'NumberOfColors': 'Number of Colors from ColPack'
}

# Apply the mapping to the Source column
melted['Source'] = melted['Source'].map(label_map)

# Shorten matrix names for x-axis
melted['MatrixShort'] = melted['Matrix'].apply(lambda x: x.split('/')[-1].replace('.mtx', ''))

# Plot
plt.figure(figsize=(14, 6))
sns.barplot(
    data=melted,
    x='MatrixShort',
    y='ColorCount',
    hue='Source',
    palette=['#4C72B0', '#55A868']
)

# Highlight significant matrices
for idx, row in merged.iterrows():
    if row['SignificantDifference']:
        plt.gca().get_xticklabels()[idx].set_color('red')

plt.xticks(rotation=45, ha='right')
plt.title('Comparison of Color Counts (PreCol vs ColPack)')
plt.tight_layout()
plt.legend(title='Color Count Source')
plt.grid(axis='y')
plt.savefig('color_comparison_plot.png', dpi=300)
plt.show()