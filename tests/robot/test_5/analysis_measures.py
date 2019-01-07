import pandas as pd
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit


data = pd.read_csv("measures_test_2.csv", sep=';')
df = pd.DataFrame(data=data)


plt.scatter(df['pwm'], df['speed_left'], c='blue', marker='.',  label='Left motor data')
plt.scatter(df['pwm'], df['speed_right'], c='red', marker='v', label='Right motor data')

plt.xlabel('PWM')
plt.ylabel('Speed (mm/s)')
plt.show()
# TODO add regression curve 

plt.plot(df['t'], df['speed_left'], 'b--',  label='Left motor')
plt.plot(df['t'], df['speed_right'], 'r-',  label='Right motor')

plt.xlabel('time')
plt.ylabel('speed (mm/s)')
plt.show()