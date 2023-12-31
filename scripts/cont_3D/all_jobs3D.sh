#!/bin/bash


for precision in $1
do
    for bissection_mode in `seq 0 1`
    do
        for pb10 in `seq 0 1`
        do
            for pb in `seq 1 8`
            do
                pbname=$(($((10*$pb10))+$pb));  
                for solveur in `seq 9 17`                
                do        
                sbatch job3D.sh $precision $pbname $bissection_mode $solveur
#                 sbatch job3D_long.sh $precision $pbname $bissection_mode $solveur
                done
            done
        done
    done
done
