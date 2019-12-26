import numpy as np
import sys

inputfile = sys.argv[1]
outputfile = sys.argv[2]

lines = []
labels = {}
with open(inputfile, 'r') as fr:
    for line in fr:
        line = line.strip().split()
        if len(line) == 1 and line[0][0] <= 'Z' and line[0][0] >= 'A' and line[0] not in labels:
            labels[line[0]] = len(labels)
        lines.append(line)
fr.close()
print(lines[:10])
print(labels)

index = 0
l = len(lines)
with open(outputfile, 'w') as fw:
    while index < l:
        fw.write(f't # {lines[index][0][1 : ]}\n')
        index += 1
        vert = int(lines[index][0])
        index += 1
        for i in range(vert):
            fw.write(f'v {i} {labels[lines[index][0]]}\n')
            index += 1
        edges = int(lines[index][0])
        index += 1
        for i in range(edges):
            # print(lines[index])
            fw.write(f'e {lines[index][0]} {lines[index][1]} {lines[index][2]}\n')
            index += 1
fw.close()