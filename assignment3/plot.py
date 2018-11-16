import numpy as np
import matplotlib.pyplot as plt

minSup = np.array([5, 10, 25, 50, 95])
gspan = np.array([302.4, 140, 57.4, 27.9, 8.6])
pafi = np.array([262, 140, 64.7, 30.5, 12.5])
gaston = np.array([45.5, 19.6, 6.6, 2.7, 0.3])

plt.plot(minSup, gspan, '-o', markersize=4, label='gspan')
plt.plot(minSup, pafi, '-o', markersize=4, label='fsg(pafi)')
plt.plot(minSup, gaston, '-o', markersize=4, label='gaston')
plt.xlabel('Minimum Support(%)')
plt.ylabel('Time(s)')
plt.legend()
plt.savefig('plot.png', dpi=500, bbox_inches='tight')
plt.show()