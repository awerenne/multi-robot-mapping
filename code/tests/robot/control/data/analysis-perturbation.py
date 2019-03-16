import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
plt.style.use('seaborn-whitegrid')

data = pd.read_csv("perturbation.csv", sep=';')
df = pd.DataFrame(data=data)
df['err_line'] = df['err_line']/1250

# Full view
colors = ['blue', 'red']
for i in range(2):
    new_df = df.loc[df['test_nb'] == i]
    u = list(new_df['t'].values)
    u = [y - x for x, y in zip(u, u[1:])]
    u.append(0)
    v = list(new_df['err_line'].values)
    v = [y - x for x, y in zip(v, v[1:])]
    v.append(0)
    plt.quiver(new_df['t'], new_df['err_line'], u, v, color=colors[i])
plt.ylim([-3,3])
plt.xlabel('Time [seconds]', fontsize=13)
plt.ylabel('Error line [cm]', fontsize=13)
plt.legend(fontsize=12)
plt.tick_params(axis='both', which='major', labelsize=11)
plt.savefig('perturbation.png')
plt.show()








