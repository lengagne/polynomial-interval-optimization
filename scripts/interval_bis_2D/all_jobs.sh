#!/bin/bash
params=(0.01 0.001)

for dof in `seq 2 4`
do
    for precision in "${params[@]}"
    do
        for bissection_type in `seq 0 1`
        do            
            for pb10 in `seq 0 1`
            do                                       
                for pb in `seq 1 9` 
                do
                    pbname=$(($((10*$pb10))+$pb));
                    for solveur in `seq 0 0`                    
                    do
                        sbatch job.sh $dof $pbname $precision $bissection_type $solveur
                    done
                done
            done
        done
    done
done

for dof in `seq 5 6`
do
    for precision in "${params[@]}"
    do
        for bissection_type in `seq 0 1`
        do            
            for pb10 in `seq 0 1`
            do                                       
                for pb in `seq 1 9` 
                do
                    pbname=$(($((10*$pb10))+$pb));
                    for solveur in `seq 0 0`                    
                    do
                        sbatch long_job.sh $dof $pbname $precision $bissection_type $solveur
                    done
                done
            done
        done        
        
    done
done

