import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
plt.style.use('seaborn-whitegrid')

data = pd.read_csv("measures_distance.csv", sep=';')
df = pd.DataFrame(data=data)
df['err'] = abs(df['ground_truth']-df['d'])
a = df.groupby('ground_truth', as_index='ground_truth')
b = a.err.mean().values 
c = a.ground_truth.mean().values
e = a.err.std().values
b /= c  # normalize
e /= c
plt.bar(c, b, yerr=e)
# plt.hist(df['d'], color='skyblue', label='measured speed')
# plt.xlabel('Time [seconds]', fontsize=13)
# plt.ylabel('Linear velocity [cm/s]', fontsize=13)
# plt.legend(fontsize=12)
# plt.tick_params(axis='both', which='major', labelsize=11)
plt.savefig('hist-distance.png')
plt.show()


sns.distplot(df['d'], hist=False, rug=True)
sns.distplot(df['to_remove'], hist=False, rug=True)
plt.show()


plt.scatter(df['err']/df['ground_truth'], df['mse']/1250)
plt.show()









