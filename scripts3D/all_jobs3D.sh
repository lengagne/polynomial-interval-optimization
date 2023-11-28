#!/bin/bash


for precision in 0.001 # for precision in  0.1 0.01 0.001
do
    for bissection_mode in `seq 0 1`
    do
        for pb10 in `seq 0 1`
        do
            for pb in `seq 1 8`
            do
                pbname=$(($((10*$pb10))+$pb));  
                for solveur in `seq 0 17`                
                do        
                sbatch job3D.sh $precision $pbname $bissection_mode $solveur
    #             sh  job3D.sh $precision $pb $bissection_mode $solveur
                done
            done
        done
    done
done

# for precision in 0.001 # for precision in  0.1 0.01 0.001
# do
#     for bissection_mode in `seq 0 1`
#     do
#         for pb10 in `seq 0 1`
#         do
#             for pb in `seq 5 8`
#             do
#                 pbname=$(($((10*$pb10))+$pb));  
#                 for solveur in `seq 0 17`                
#                 do        
#                 sbatch job3D_long.sh $precision $pbname $bissection_mode $solveur
#     #             sh  job3D.sh $precision $pb $bissection_mode $solveur
#                 done
#             done
#         done
#     done
# done
