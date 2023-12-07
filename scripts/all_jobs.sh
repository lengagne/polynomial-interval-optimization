#!/bin/bash

for precision in 0.001 0.01 0.0001
do

    cd bis_3D
    sh all_jobs3D.sh $precision
    cd ..

    cd bis_2D
    sh all_jobs.sh $precision
    cd ..
    
    cd cont_2D
    sh all_jobs.sh $precision
    cd ..
    
    cd cont_3D
    sh all_jobs3D.sh $precision
    cd ..
    
done
