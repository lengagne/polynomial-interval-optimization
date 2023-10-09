#!/bin/bash


for precision in 0.001 # for precision in  0.1 0.01 0.001
do
for pb in `seq 1 4`
    do
        for solveur in `seq 0 17`
        do
            sbatch job3D.sh $precision $pb $solveur
#             sh  job3D.sh $precision $pb $solveur
        done
    done
done
