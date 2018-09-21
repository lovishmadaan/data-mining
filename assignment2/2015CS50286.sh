#!/bin/bash
if [ $1 = "-kmeans" ]
then
    ./kmeans $3 $2
elif [ $1 = "-dbscan" ]
then
    ./dbscan $4 $2 $3
else
    ./optics $4 $2 $3
    python id.py $4
fi