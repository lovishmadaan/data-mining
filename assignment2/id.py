import numpy as np
import matplotlib.pyplot as plt
import sys

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
points = read_file(sys.argv[1])
data = read_file('optics.txt')

RPlot = (data[:, 0] ** 0.5)
RPoints = data[:, 1].astype(int)
RPoints = points[RPoints]

plt.plot(np.array(range(data.shape[0])), RPlot, 'black')
plt.show()

# ordering = data[:, 1].astype(int)

# labels = (-2 * np.ones((100000, 1)))
# labels = labels.astype(int)

# labels[ordering[0:33001]] = 0
# labels[ordering[34490:46028]] = 1
# labels[ordering[46346:59100]] = 2
# labels[ordering[59425:79585]] = 3
# labels[ordering[79652:84345]] = 4
# labels[ordering[84530:92091]] = 5
# labels[ordering[92220:99825]] = 6

# plt.scatter(points[:, 0], points[:, 1], c=labels[:, 0],s=0.1)
# plt.show()