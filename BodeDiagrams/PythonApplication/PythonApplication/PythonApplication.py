import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

df_bode = pd.read_csv("bode_data.csv")
df_poles = pd.read_csv("poles_data.csv")

plt.figure(figsize=(12, 8))

# Bode Diagrams
plt.subplot(2, 2, 1)
plt.plot(df_bode['w'], df_bode['Magnitude(dB)'])
plt.xscale('log')
plt.ylabel('Magnitude (dB)')
plt.title('Bode Diagram - Magnitude')
plt.grid(True, which='both')

plt.subplot(2, 2, 2)
plt.plot(df_bode['w'], df_bode['Phase(degree)'])
plt.xscale('log')
plt.xlabel('Frequency (rad/s)')
plt.ylabel('Phase (degree)')
plt.title('Bode Diagram - Phase')
plt.grid(True, which='both')

# Poles
plt.subplot(2, 2, (3, 4))
plt.plot(df_poles['real'], df_poles['imaginary'], 'rx', markersize=10, label='Poles')

plt.axhline(y=0, color='blue', linestyle='-', alpha=0.5, linewidth=1.5)
plt.axvline(x=0, color='blue', linestyle='-', alpha=0.5, linewidth=1.5)
plt.grid(True, color='gray', linestyle='--', alpha=0.7)
plt.xlabel('Real')
plt.ylabel('Imaginary')
plt.title('Pole-Zero Plot')
plt.axis('equal')  
plt.legend()

plt.gca().set_facecolor('#f0f0f0')

plt.tight_layout()
plt.show()
