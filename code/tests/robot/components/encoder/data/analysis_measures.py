import pandas as pd
import matplotlib.pyplot as plt
plt.style.use('seaborn-whitegrid')


data = pd.read_csv("measures.csv", sep=';')
df = pd.DataFrame(data=data)


plt.plot(df['pwm'], df['speed_left'], c='blue',  label='left motor')
plt.plot(df['pwm'], df['speed_right'], c='red', label='right motor')

plt.xlabel('PWM')
plt.ylabel('Linear velocity (cm/s)')
plt.legend()
plt.savefig('motors.png')
plt.show()