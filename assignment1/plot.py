import numpy as np
import matplotlib.pyplot as plt
import subprocess
from subprocess import Popen, PIPE
import sys

cuts = np.array([1, 5, 10, 25, 50, 90])
apriori = []
fptree = []
for cut in cuts:
    cmd1 = ['./apriori', sys.argv[1], 'apriori' + str(cut) + '.txt', str(cut)]
    result1 = subprocess.Popen(cmd1, stdout=subprocess.PIPE)
    out1 = result1.stdout.read()
    apriori.append(out1)
    cmd2 = ['./fptree', sys.argv[1], 'fptree' + str(cut) + '.txt', str(cut)]
    result2 = subprocess.Popen(cmd2, stdout=subprocess.PIPE)
    out2 = result2.stdout.read()
    fptree.append(out2)
    # print(out1, out2)
apriori = np.array(apriori)
fptree = np.array(fptree)

plt.plot(cuts, apriori, '-o', markersize = 5, label = 'Apriori')
plt.plot(cuts, fptree, '-o', markersize = 5, label = 'FP Tree')
plt.xlabel('Support Threshold(%)')
plt.ylabel('Running Time(s)')
plt.legend()
plt.show()