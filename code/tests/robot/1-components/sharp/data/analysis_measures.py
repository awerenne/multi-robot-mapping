import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
plt.style.use('seaborn-whitegrid')

data = pd.read_csv("measures.csv", sep=';')
df = pd.DataFrame(data=data)
df['t'] = df['t']/1000
df['voltage'] = df['voltage']*5
df.plot(x='t', y='voltage', legend=False)
plt.axhline(y=2, linewidth=2, color='r', linestyle='--')
plt.xlabel('Time [seconds]', fontsize=13)
plt.ylabel('Voltage [V]', fontsize=13)
plt.tick_params(axis='both', which='major', labelsize=11)
plt.savefig('sharp-flow.png')
plt.show()




