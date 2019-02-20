import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("measures.csv", sep=';')
df = pd.DataFrame(data=data)

default_parameters = {
    # 'num_samples_avg': 4,
    # 'dimming_level': 0,
    'f': 100,
    'light_on': 1,
    'h': 10
}

# Plot position in function of time
# Filter only the default parameters 
sub_sample = df
for key, value in default_parameters.items():
    sub_sample = sub_sample.loc[sub_sample[key] == value]
sub_sample.plot(x='t', y='position')
plt.show()

# Plot lines of each sensor in function of time
colors = [(1,0,0), (0,1,0), (0,0,1), (1,1,0), (0,1,1), (0.75,0.75,0.75)]
fig, ax = plt.subplots()
for i in range(6):
    y_column = 'vector_' + str(i)
    label = 'sensor ' + str(i)
    ax = sub_sample.plot(ax=ax, kind='line', x='t', y=y_column,
            c=colors[i], label=label)
plt.legend(loc='best')
plt.show()


# Todo:
# Analyze each parameter (num_samp, dim_lvl, frq, light, height) separately 
# while the others stay fixed to the default best values