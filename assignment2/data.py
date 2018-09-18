import numpy as np
from sklearn.datasets.samples_generator import make_blobs
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

def write_file(outfile, data):
    with open(outfile, 'w') as fw:
        for point in data:
            point = list(map(str, point))
            point = (' ').join(point)
            fw.write(f'{point}\n')
        fw.close()

inputfile = 'format.txt'
outfile = 'points.txt'

X, y = make_blobs(n_samples=100000, n_features=2, centers=7, center_box=(-20.0, 20.0))
write_file(outfile, X)
plt.scatter(X[:, 0], X[:, 1], s=2)
plt.show()