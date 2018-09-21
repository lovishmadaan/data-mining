#!/bin/bash
g++ -std=c++11 -o kmeans preprocess.cpp kmeans.cpp -O3
g++ -std=c++11 -o dbscan preprocess.cpp dbscan.cpp -O3
g++ -std=c++11 -o optics preprocess.cpp RangeTree.h optics.cpp -O3