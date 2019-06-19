import pandas as pd
import matplotlib.pyplot as plt
plt.style.use('seaborn-whitegrid')


# data = pd.read_csv("measures_empty.csv", sep=';')
# df = pd.DataFrame(data=data)


# plt.plot(df['pwm'], df['speed_left'], c='blue',  label='left motor')
# plt.plot(df['pwm'], df['speed_right'], c='red', label='right motor')

# plt.xlabel('PWM')
# plt.ylabel('Linear velocity (cm/s)')
# plt.legend()
# plt.savefig('motors_empty.png')
# plt.show()









data = pd.read_csv("measures_empty.csv", sep=';')
df_empty = pd.DataFrame(data=data)
df_empty.columns = ['pwm', 'speed_left_empty', 'speed_right_empty']
data = pd.read_csv("measures_full.csv", sep=';')
df_full = pd.DataFrame(data=data)
df_full = df_full.drop(['pwm'], axis=1)
df_full.columns = ['speed_left_full', 'speed_right_full']
df = pd.concat([df_empty, df_full], axis=1)


plt.plot(df['pwm'], df['speed_left_full'], c='blue',  label='left motor (fully charged)')
plt.plot(df['pwm'], df['speed_right_full'], c='red', label='right motor (fully charged)')
plt.plot(df['pwm'], df['speed_left_empty'], '--', c='blue',  label='left motor (50% charge)')
plt.plot(df['pwm'], df['speed_right_empty'], '--', c='red', label='right motor (50% charge)')

plt.xlabel('PWM', fontsize=13)
plt.ylabel('Linear velocity (cm/s)', fontsize=13)
plt.legend(fontsize=12)
plt.tick_params(axis='both', which='major', labelsize=11)
plt.savefig('motors_merged.png')
plt.show()