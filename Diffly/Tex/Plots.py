import matplotlib.pyplot as plt
import matplotlib
import numpy as np

x=np.linspace(0, 5, 10000)
fig, ax = plt.subplots(figsize=(5, 4))
ax.set_ylim([-5, 5])

plt.plot(((np.sin(np.cos(np.sinh((x**4))))**3)-((x-1)/(x+2))))
plt.plot(((3*((np.cos(np.cos(np.sinh((x**4))))*((np.cosh((x**4))*(4*(x**3)))*(-1*np.sin(np.sinh((x**4))))))*(np.sin(np.cos(np.sinh((x**4))))**2)))-(((x+2)-(x-1))/((x+2)**2))))

plt.legend()
matplotlib.pyplot.savefig('Tex/Plots.png')
