"""
Created on Mar 17, 2012

@author: Amy X Zhang
axz@mit.edu 
http://people.csail.mit.edu


Demo of OPTICS Automatic Clustering Algorithm
https://github.com/amyxzhang/OPTICS-Automatic-Clustering

"""

import numpy as np
import matplotlib.pyplot as plt
# import OpticsClusterArea as OP
from itertools import *
import AutomaticClustering as AutoC

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
data = read_file('out_optics2.txt')

points = read_file('dataset.txt')

RPlot = (data[:, 0] ** 0.5)
RPoints = data[:, 1].astype(int)
RPoints = points[RPoints]

plt.plot(np.array(range(data.shape[0])), RPlot)
plt.show()

#hierarchically cluster the data
rootNode = AutoC.automaticCluster(RPlot, RPoints)

#print Tree (DFS)
AutoC.printTree(rootNode, 0)

#graph reachability plot and tree
AutoC.graphTree(rootNode, RPlot)

#array of the TreeNode objects, position in the array is the TreeNode's level in the tree
array = AutoC.getArray(rootNode, 0, [0])

#get only the leaves of the tree
leaves = AutoC.getLeaves(rootNode, [])

#graph the points and the leaf clusters that have been found by OPTICS
fig = plt.figure()
ax = fig.add_subplot(111)

ax.plot(points[:,0], points[:,1], 'y.')
colors = cycle('gmkrcbgrcmk')
for item, c in zip(leaves, colors):
    node = []
    for v in range(item.start,item.end):
        node.append(RPoints[v])
    node = np.array(node)
    ax.plot(node[:,0],node[:,1], c+'o', ms=5)

plt.savefig('Graph2.png', dpi=None, facecolor='w', edgecolor='w',
    orientation='portrait', papertype=None, format=None,
    transparent=False, bbox_inches=None, pad_inches=0.1)
plt.show()
