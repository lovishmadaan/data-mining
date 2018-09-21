import numpy as np
import matplotlib.pyplot as plt

def read_file(inputfile):
    data = []
    with open(inputfile, 'r') as fr:
        for line in fr:
            line = line.strip().split(' ')
            data.append(list(map(float, line)))
    fr.close()
    data = np.array(data)
    return data

#Plot the results
# data = read_file('points.txt')
data = read_file('out_optics.txt')

der = [0]
for i in range(1, data.shape[0]):
    der.append(data[i][0] - data[i - 1][0])
der = np.array(der)

dder = [0]
for i in range(1, der.shape[0]):
    dder.append(der[i] - der[i - 1])

plt.scatter(np.arange(data.shape[0]), data[:, 0], marker=".")
plt.show()