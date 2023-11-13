#!/bin/bash

for precision in 0.001 # for precision in  0.1 0.01 0.001
do
    for bissection_type in `seq 0 1`
    do
        for dof in `seq 2 6`
        do
    #         for pb10 in `seq 0 1`
            for pb10 in `seq 0`
            do         
    #             for pb in `seq 1 9`
                for pb in `seq 1 1`
                pbname=$(($((10*$pb10))+$pb));
#                 echo $pbname;
                do
                    for solveur in `seq 0 17`                    
                    do
                            sbatch job.sh $dof $pbname $precision $bissection_type $solveur
#                     sh  job.sh $dof $pbname $precision $bissection_type $solveur
                    done
                done
            done
        done
    done
done
