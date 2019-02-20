import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("measures.csv", sep=';')
df = pd.DataFrame(data=data)

colors = [(1,0,0), (0,1,0), (0,0,1)]
fig, ax = plt.subplots()

label = 'target'
ax = df.plot(ax=ax, kind='line', x='t', y='target_speed',
        c=colors[0], label=label)

label = 'left'
ax = df.plot(ax=ax, kind='line', x='t', y='measured_speed_left',
        c=colors[1], label=label)

label = 'right'
ax = df.plot(ax=ax, kind='line', x='t', y='measured_speed_right',
        c=colors[2], label=label)

plt.legend(loc='best')
plt.show()
