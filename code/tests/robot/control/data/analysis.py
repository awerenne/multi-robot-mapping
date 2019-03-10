import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
plt.style.use('seaborn-whitegrid')

data = pd.read_csv("pid-speed.csv", sep=';')
df = pd.DataFrame(data=data)

# Full view
# plt.plot(df['seq_number'], df['progress_speed'], c='red',  label='progress speed')
# plt.plot(df['seq_number'], df['measured_speed'], c='blue', label='measured speed')
# plt.ylim([5,9])
# plt.xlabel('t')
# plt.ylabel('Linear velocity (cm/s)')
# plt.legend()
# plt.savefig('pid_speed_normal.png')
# plt.show()


# Find period
# df = df[df.seq_number >= 590]
# df['seq_number'] = (df['seq_number']-590)%400
# plt.scatter(df['seq_number'], df['progress_speed'], c='blue',  label='progress speed')
# plt.ylim([5,9])
# plt.xlabel('t')
# plt.ylabel('Linear velocity (cm/s)')
# plt.legend()
# plt.show()


# Shaded with variance plot

discretize_factor = 20
df = df[df.seq_number >= 590]
df['seq_number'] = (df['seq_number']-590)%400
df['discrete_seq_number'] = pd.qcut(df['seq_number'],discretize_factor,
        labels=np.arange(discretize_factor))
Y = df.groupby('discrete_seq_number')['progress_speed'].mean() 
y = df.groupby('discrete_seq_number')['measured_speed'].mean() 
err = df.groupby('discrete_seq_number')['measured_speed'].sem() 

plt.plot(np.arange(discretize_factor), Y, c='red',  label='progress speed')
plt.plot(np.arange(discretize_factor), y, c='blue', label='progress speed')
plt.fill_between(np.arange(discretize_factor), y-2*err, y+2*err)
plt.ylim([5,9])
plt.xlabel('t')
plt.ylabel('Linear velocity (cm/s)')
plt.legend()
plt.savefig('pid_speed_shaded.png')
plt.show()








