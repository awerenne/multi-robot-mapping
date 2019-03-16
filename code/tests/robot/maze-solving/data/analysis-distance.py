import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
plt.style.use('seaborn-whitegrid')

data = pd.read_csv("measures_distance.csv", sep=';')
df = pd.DataFrame(data=data)
df['d'] /= 2
df['err'] = abs(df['ground_truth']-df['d'])
a = df.groupby('ground_truth', as_index='ground_truth')
b = a.err.mean().values 
c = a.ground_truth.mean().values
e = a.err.std().values
b /= c  # normalize
e /= c
plt.bar(c, b, yerr=e)
plt.savefig('hist-distance.png')
plt.show()


sns.distplot(df.loc[df['ground_truth'] == 17.5]['d'], hist=False, rug=True)
sns.distplot(df.loc[df['ground_truth'] == 22.5]['d'], hist=False, rug=True)
sns.distplot(df.loc[df['ground_truth'] == 27.5]['d'], hist=False, rug=True)
plt.savefig('distrib.png')
plt.show()


plt.scatter(df['err']/df['ground_truth'], df['mse']/1250)
plt.savefig('err-mse.png')
plt.show()









