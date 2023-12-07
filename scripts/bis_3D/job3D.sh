#!/bin/bash

#SBATCH --job-name=bis_3D
#SBATCH --mail-user=sebastien.lengagne@uca.fr
#SBATCH --mail-type=FAIL
#SBATCH --ntasks=1
#SBATCH --ntasks-per-core=1
#SBATCH --exclusive
#SBATCH --constraint=ivy 
# #SBATCH --partition=long 

cd ..
../build/apps/Robot3D/Robot3D $1 $2 $3 $4
exit 0
