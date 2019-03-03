import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
plt.style.use('seaborn-whitegrid')
sns.set()

data = pd.read_csv("measures.csv", sep=';')
df = pd.DataFrame(data=data)
df['t'] = df['t']/1000
df['voltage'] = df['voltage']*5
df.plot(x='t', y='voltage', legend=False)
plt.axhline(y=2, linewidth=2, color='r', linestyle='--')
plt.xlabel('Time [seconds]')
plt.ylabel('Voltage [V]')
plt.savefig('sharp-flow.png')
plt.show()




