import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
plt.style.use('seaborn-whitegrid')

data = pd.read_csv("measures.csv", sep=';')
df = pd.DataFrame(data=data)


# ------------
params = {
    'num_samples_avg': 10,
    'f': 50,
    'mode': "manual"
}
sub_sample = df
for key, value in params.items():
    sub_sample = sub_sample.loc[sub_sample[key] == value]
sub_sample.plot(x='t', y='position')
plt.show()


# ------------
sub_sample.boxplot(column=['v0', 'v1', 'v2', 'v3', 'v4', 'v5'])
plt.show()


# ------------
params = {
    'num_samples_avg': 6,
    'f': 40,
    'mode': "manual"
}
sub_sample = df
for key, value in params.items():
    sub_sample = sub_sample.loc[sub_sample[key] == value]
sub_sample.plot(x='t', y='position')
plt.show()


# # ------------
# params = {
#     'num_samples_avg': 10,
#     'f': 50,
#     'mode': "automatic"
# }
# sub_sample = df
# for key, value in params.items():
#     sub_sample = sub_sample.loc[sub_sample[key] == value]
# sub_sample.plot(x='t', y='position')
# plt.show()


# # ------------
# sub_sample.boxplot(column=['v0', 'v1', 'v2', 'v3', 'v4', 'v5'])
# plt.show()













