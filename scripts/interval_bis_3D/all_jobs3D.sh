#!/bin/bash

params=(0.01 0.001)

for precision in "${params[@]}"
do
    for bissection_mode in `seq 0 1`
    do
        for pb10 in `seq 0 1`
        do
            for pb in `seq 1 4`
            do
                pbname=$(($((10*$pb10))+$pb));  
                for solveur in `seq 0 0`                
                do        
                sbatch job3D.sh $precision $pbname $bissection_mode $solveur
                done
            done
        done
    done
done

for precision in "${params[@]}"
do
    for bissection_mode in `seq 0 1`
    do
        for pb10 in `seq 0 1`
        do
            for pb in `seq 5 8`
            do
                pbname=$(($((10*$pb10))+$pb));  
                for solveur in `seq 0 0`                
                do        
                sbatch job3D_long.sh $precision $pbname $bissection_mode $solveur
                done
            done
        done
    done
done

