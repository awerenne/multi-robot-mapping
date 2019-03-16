import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
plt.style.use('seaborn-whitegrid')

x = np.arange(0,3,0.01)
plt.plot(x, np.tanh(2*3.1415*x), c='red',  label=r'$\tanh(2\pi x)$')
plt.plot(x, np.tanh(3.1415*x), c='green',  label=r'$\tanh(\frac{\pi}{2} x)$')
plt.plot(x, np.tanh(3.1415/2*x), c='brown',  label=r'$\tanh(\frac{\pi}{4} x)$')
plt.ylim([0,1.5])
plt.xlabel('Time [seconds]', fontsize=13)
plt.ylabel(r"$\zeta(t)$", fontsize=13)
plt.legend(fontsize=12)
plt.tick_params(axis='both', which='major', labelsize=11)
plt.savefig('zeta.png')
plt.show()








