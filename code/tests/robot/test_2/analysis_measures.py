import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("measures.csv", sep=';')
df = pd.DataFrame(data=data)

fig, ax = plt.subplots()

for key, grp in df.groupby(['light_on','f']):
    if key[0] == 1 and key[1] == 20:
        ax = grp.plot(ax=ax, kind='line', x='t', y='voltage',
            c=(1,0,0), label="Light on - 20 Hz")
    elif key[0] == 1 and key[1] == 50:
        ax = grp.plot(ax=ax, kind='line', x='t', y='voltage',
            c=(1,1,0), label="Light on - 50 Hz")
    elif key[0] == 0 and key[1] == 20:
        ax = grp.plot(ax=ax, kind='line', x='t', y='voltage',
            c=(0,1,0), label="Light on - 20 Hz")
    elif key[0] == 0 and key[1] == 50:
        ax = grp.plot(ax=ax, kind='line', x='t', y='voltage',
            c=(0,1,1), label="Light on - 50 Hz")

plt.legend(loc='best')
plt.show()