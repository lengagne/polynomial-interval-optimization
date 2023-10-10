#!/bin/bash


for precision in 0.001 # for precision in  0.1 0.01 0.001
do
    for dof in `seq 2 4`
    do
        for pb in `seq 1 9`
        do
            for solveur in `seq 0 17`
            do
                for bissection_type in `seq 0 1` # for precision in  0.1 0.01 0.001
                do
        #                 sbatch .job.sh $dof $pb $precision $bissection_type $solveur
                sh  job.sh $dof $pb $precision $bissection_type $solveur
        #                 sleep 1
                done
            done
        done
    done
done
