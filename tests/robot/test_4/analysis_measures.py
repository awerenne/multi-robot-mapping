import pandas as pd
import matplotlib.pyplot as plt

frequency_slave;arrival_time_step_master;seq_number

data = pd.read_csv("measures_test_2.csv", sep=';')
df = pd.DataFrame(data=data)
min_frequency = df['frequency_slave'].min()
min_time = df.loc[df['frequency_slave'] == min_frequency].value_counts() * 1000/frequency # get minimum scale

fig, ax = plt.subplots()

for frequency, grp in df.groupby('frequency_slave'):
    sub_grp = grp.loc[grp['seq_number']*1000/frequency <= min_time]  # filter to be on same scale
    sub_grp['t'] = sub_grp['seq_number']*1000/frequency
    if frequency == 10:
        ax = sub_grp.plot.scatter(ax=ax, x='t', y='arrival_time_step_master',
            c=(1,0,0), label="10 Hz")
    elif frequency == 50:
        ax = sub_grp.plot.scatter(ax=ax, x='t', y='arrival_time_step_master',
            c=(0,1,0), label="50 Hz")
    elif frequency == 100:
        ax = sub_grp.plot.scatter(ax=ax, x='t', y='arrival_time_step_master',
            c=(0,0,1), label="100 Hz")

plt.legend(loc='best')
plt.show()


