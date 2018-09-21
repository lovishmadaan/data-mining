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

# def write_file(outfile, data):
#     with open(outfile, 'w') as fw:
#         for point in data:
#             point = list(map(str, point))
#             point = (' ').join(point)
#             fw.write(f'{point}\n')
#         fw.close()

# X, y = make_blobs(n_samples=1000, n_features=2, centers=3, center_box=(-25.0, 25.0))

# write_file('points2.txt', X)

#Plot the results
data = read_file('final_data.txt')
labels = read_file('test.txt')
plt.scatter(data[:, 0], data[:, 1], c=labels[:, 0], s=0.1)
plt.show()