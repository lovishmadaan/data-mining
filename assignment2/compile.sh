#!/bin/bash
g++-7 -std=c++11 -o kmeans preprocess.cpp kmeans.cpp -O3
g++-7 -std=c++11 -o dbscan preprocess.cpp dbscan.cpp -O3
g++-7 -std=c++11 -o optics preprocess.cpp RangeTree.h optics.cpp -O3