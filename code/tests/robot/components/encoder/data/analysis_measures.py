import pandas as pd
import matplotlib.pyplot as plt
plt.style.use('seaborn-whitegrid')


data = pd.read_csv("measures.csv", sep=';')
df = pd.DataFrame(data=data)


plt.plot(df['pwm'], df['speed_left'], c='blue',  label='left')
plt.plot(df['pwm'], df['speed_right'], c='red', label='right')

plt.xlabel('PWM')
plt.ylabel('Speed (cm/s)')
plt.legend()
plt.show()