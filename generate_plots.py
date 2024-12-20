
import pandas as pd
import matplotlib.pyplot as plt
import json

# Read data from JSON
with open('exchange_data.json', 'r') as f:
    data = json.load(f)

# Convert to DataFrame
df = pd.DataFrame({
    'date': pd.to_datetime(data['dates']),
    'integrated_lacunarity': data['integrated_lacunarity'],
    'rolling_corr': data['rolling_corr']
})

# Calculate smoothed averages
window_size = 30
df['smoothed_lacunarity'] = df['integrated_lacunarity'].rolling(window=window_size, min_periods=1, center=True).mean()
df['smoothed_correlation'] = df['rolling_corr'].rolling(window=window_size, min_periods=1, center=True).mean()

# Set style and create figure with 4 subplots
plt.style.use('seaborn-v0_8-whitegrid')
fig, (ax1, ax2, ax3, ax4) = plt.subplots(4, 1, figsize=(12, 20))

# Plot 1: Correlation Values
ax1.plot(df['date'], df['rolling_corr'], '-', color='blue', label='Correlation')
ax1.set_title('Rolling Correlation Over Time')
ax1.set_xlabel('Time')
ax1.set_ylabel('Correlation Value')
ax1.grid(True, alpha=0.3)
ax1.legend(loc='upper right')

# Plot 2: Integrated Lacunarity
ax2.plot(df['date'], df['integrated_lacunarity'], '-', color='green', label='Lacunarity (Power Law)')
ax2.set_title('Lacunarity Coefficient Over Time')
ax2.set_xlabel('Time')
ax2.set_ylabel('Lacunarity (Power Law)')
ax2.grid(True, alpha=0.3)
ax2.legend(loc='upper right')

# Plot 3: Overlap of both metrics
ax3.plot(df['date'], df['rolling_corr'], '-', color='blue', label='Correlation')
ax3.plot(df['date'], df['integrated_lacunarity'], '-', color='green', label='Lacunarity (Power Law)')
ax3.set_title('Overlap of Correlation and Lacunarity Coefficients Over Time')
ax3.set_xlabel('Time')
ax3.set_ylabel('Value')
ax3.grid(True, alpha=0.3)
ax3.legend(loc='upper right')

# Plot 4: Smoothed averages
ax4.plot(df['date'], df['smoothed_correlation'], '-', color='blue', linewidth=2, label='Smoothed Correlation')
ax4.plot(df['date'], df['smoothed_lacunarity'], '-', color='green', linewidth=2, label='Smoothed Lacunarity')
ax4.set_title('Smoothed Averages (30-day window)')
ax4.set_xlabel('Time')
ax4.set_ylabel('Value')
ax4.grid(True, alpha=0.3)
ax4.legend(loc='upper right')

# Rotate x-axis labels for better readability
for ax in [ax1, ax2, ax3, ax4]:
    ax.tick_params(axis='x', rotation=45)

# Adjust layout and save
plt.tight_layout()
plt.savefig('exchange_analysis.png', dpi=300, bbox_inches='tight')
plt.close()
