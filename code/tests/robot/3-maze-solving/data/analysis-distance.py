import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
import random
plt.style.use('seaborn-whitegrid')

data = pd.read_csv("measures_distance.csv", sep=';')
df = pd.DataFrame(data=data)
df['err'] = abs(df['ground_truth']-df['d'])

# Select only one part of the data (each ground truth has same number of samples)
gt = df.ground_truth.unique()
size_chunk = 7
for target in gt:
    sub_df = df.loc[df['ground_truth'] == target]
    n_to_remove = len(sub_df) - size_chunk
    to_remove = random.sample(set(sub_df.d), n_to_remove)
    for item in to_remove:
        idx = df[df.d == item].iloc[0].name 
        df = df.drop([idx])


# a = df.groupby('ground_truth', as_index='ground_truth')
# b = a.err.mean().values 
# c = a.ground_truth.mean().values
# e = a.err.std().values
# b /= c  # normalize
# e /= c
# # plt.bar(c, b, yerr=e)
# plt.bar(c, b)
# plt.xlim([15,40])
# plt.xlabel('Ground truth distance [cm]', fontsize=13)
# plt.ylabel('Relative error %', fontsize=13)
# plt.tick_params(axis='both', which='major', labelsize=10)
# plt.savefig('hist-distance.png')
# plt.show()

# measures = []
# for target in gt:
#     measures.append(df.loc[df['ground_truth'] == target]['d'].mean())
# plt.plot(gt, gt, linestyle='--', marker='*', color='b')
# plt.scatter(gt, measures, color='red')
# plt.savefig('ground-vers-measured.png')
# plt.show()

for target in gt:
    # SET BANDWITH PARAMETERS LARGER TO CONTROL SMOOTHER FIT 
    sns.distplot(df.loc[df['ground_truth'] == target]['d'], hist=False, rug=True)
plt.xlabel('Measured distance [cm]', fontsize=13)
plt.tick_params(axis='both', which='major', labelsize=10)
plt.savefig('distrib.png')
plt.show()


# plt.scatter(df['err']/df['ground_truth'], df['mse']/1250)
# plt.savefig('err-mse.png')
# plt.show()









