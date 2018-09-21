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

points = read_file('final_data.txt')

def read_file2(inputfile, points):
    data = []
    clusterID = 0
    a = np.ones((100000, 1))
    colors = ['red', 'blue', 'yellow', 'black', 'green', 'grey', 'cyan', 'magenta']
    with open(inputfile, 'r') as fr:
        for line in fr:
            line = line.strip()
            if line[0] == '#':
                data = np.array(data).astype(int)
                plt.scatter(points[data, 0], points[data, 1], s=0.1)
                clusterID += 1
                data = []
            else:
                a[int(line)] -= 1
                data.append(int(line))
        data = np.array(data).astype(int)
        plt.scatter(points[data, 0], points[data, 1], s=0.1)
        clusterID += 1
        data = []
    for i in range(100000):
        if(a[i] != 0):
            print("Wrong %d" %i)
    fr.close()
    plt.show()

read_file2('dbscan.txt', points)