import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
# Load CSVs
df1 = pd.read_csv('output.csv')  # Replace with the actual path to first CSV
df2 = pd.read_csv('OutputTable.csv')  # Replace with the actual path to second CSV

# Strip whitespace and standardize
df1['Matrix'] = df1['Matrix'].astype(str).str.strip()
df2['Matrix'] = df2['Matrix'].astype(str).str.strip()

# Merge
merged = pd.merge(df1, df2, on='Matrix', suffixes=('_Original', '_New'))

# Compute differences for colors
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

# Create shorter matrix names
merged['Matrix'] = merged['Matrix'].apply(lambda x: x.split('/')[-1].replace('.mtx', ''))

# Create two subplots
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(14, 12))

# 1. Colors Comparison Plot
colors_data = merged.melt(
    id_vars=['Matrix', 'SignificantDifference'],
    value_vars=['NumOfColors', 'NumberOfColors'],
    var_name='Source',
    value_name='ColorCount'
)

# Create a mapping for the color labels
color_label_map = {
    'NumOfColors': 'Number of Colors from PreCol',
    'NumberOfColors': 'Number of Colors from ColPack'
}

colors_data['Source'] = colors_data['Source'].map(color_label_map)

# Plot colors
sns.barplot(
    data=colors_data,
    x='Matrix',
    y='ColorCount',
    hue='Source',
    palette=['#4C72B0', '#55A868'],
    ax=ax1
)

# Highlight significant matrices in colors plot
for idx, row in merged.iterrows():
    if row['SignificantDifference']:
        ax1.get_xticklabels()[idx].set_color('red')

ax1.set_xticklabels(ax1.get_xticklabels(), rotation=45, ha='right')
ax1.set_title('Comparison of Color Counts (PreCol vs ColPack)')
ax1.legend(title='Color Count Source')
ax1.grid(axis='y')

# 2. Time Comparison Plot
time_data = merged.melt(
    id_vars=['Matrix'],
    value_vars=['ColoringTime', 'Time'],
    var_name='Source',
    value_name='Time (seconds)'
)

# Create a mapping for the time labels
time_label_map = {
    'ColoringTime': 'Coloring Time from PreCol',
    'Time': 'Coloring Time from ColPack'
}

time_data['Source'] = time_data['Source'].map(time_label_map)

# Plot times
sns.barplot(
    data=time_data,
    x='Matrix',
    y='Time (seconds)',
    hue='Source',
    palette=['#4C72B0', '#55A868'],
    ax=ax2
)

ax2.set_xticklabels(ax2.get_xticklabels(), rotation=45, ha='right')
ax2.set_title('Comparison of Coloring Times (PreCol vs ColPack)')
ax2.legend(title='Time Measurement Source')
ax2.grid(axis='y')

# Adjust layout
plt.tight_layout()
plt.savefig('comparison_plot.png', dpi=300, bbox_inches='tight')
plt.show()

# Print time comparison statistics
print("\nTime Comparison Statistics:")
time_stats = merged[['Matrix', 'ColoringTime', 'Time']].copy()
time_stats['TimeDifference'] = time_stats['ColoringTime'] - time_stats['Time']
time_stats['RelativeTimeDiff'] = time_stats['TimeDifference'] / time_stats[['ColoringTime', 'Time']].max(axis=1)
print(time_stats[['Matrix', 'ColoringTime', 'Time', 'TimeDifference', 'RelativeTimeDiff']])