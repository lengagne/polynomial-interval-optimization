#!/bin/bash



for bf in `seq 0 9`                
do        
    sbatch Eval3D.sh $bf
#             sh  job3D.sh $precision $pb $bissection_mode $solveur
done
