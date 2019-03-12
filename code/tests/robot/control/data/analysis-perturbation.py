import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
plt.style.use('seaborn-whitegrid')

data = pd.read_csv("perturbation.csv", sep=';')
df = pd.DataFrame(data=data)

# Full view
plt.plot(df['t'], df['err_line'], c='blue')
plt.ylim([-2500,2500])
plt.xlabel('t', fontsize=12)
plt.ylabel('error line', fontsize=12)
plt.legend(fontsize=12)
plt.tick_params(axis='both', which='major', labelsize=11)
plt.savefig('perturbation.png')
plt.show()








