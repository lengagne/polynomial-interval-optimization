#!/bin/bash

for precision in 0.001 # for precision in  0.1 0.01 0.001
do
    for dof in `seq 2 4`
    do
        for pb10 in `seq 0 1`
        do
            for pb in `seq 1 9`
            do
                pbname=$(($((10*$pb10))+$pb));
#                 echo $pbname;
                for solveur in `seq 0 17`
                do
#                     for bissection_type in `seq 0 2`
                    for bissection_type in `seq 0 1`
                    do
                            sbatch job.sh $dof $pbname $precision $bissection_type $solveur
#                     sh  job.sh $dof $pbname $precision $bissection_type $solveur
                    done
                done
            done
        done
    done
done
