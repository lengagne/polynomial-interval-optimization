#!/bin/bash

#SBATCH --job-name=job_3D
#SBATCH --mail-user=sebastien.lengagne@uca.fr
#SBATCH --mail-type=FAIL
#SBATCH --ntasks=1
#SBATCH --ntasks-per-core=1
#SBATCH --exclusive
# #SBATCH --constraint=ivy 
# #SBATCH --partition=long 

# ../build/apps/Robot3D/Eval3D $1 
exit 0
