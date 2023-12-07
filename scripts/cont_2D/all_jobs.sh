#!/bin/bash

for precision in 0.01 0.001 0.0001
do
    for bissection_type in `seq 0 1`
    do
        for dof in `seq 2 4`
        do
            for pb10 in `seq 0 1`
            do         
                for pb in `seq 1 9`               
                do
                    pbname=$(($((10*$pb10))+$pb));
                    for solveur in `seq 9 17`                    
                    do
                            sbatch job.sh $dof $pbname $precision $bissection_type $solveur
                    done
                done
            done
        done
    done
done


for precision in 0.01 0.001 0.0001
do
    for bissection_type in `seq 0 1`
    do
        for dof in `seq 5 8`
        do
            for pb10 in `seq 0 1`
            do         
                for pb in `seq 1 9`                
                do
                    pbname=$(($((10*$pb10))+$pb));
                    for solveur in `seq 9 17`                    
                    do
                            sbatch job_long.sh $dof $pbname $precision $bissection_type $solveur
                    done
                done
            done
        done
    done
done
