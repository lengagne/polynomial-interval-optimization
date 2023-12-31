#!/bin/bash

#SBATCH --job-name=cont2D_long
#SBATCH --mail-user=sebastien.lengagne@uca.fr
#SBATCH --mail-type=FAIL
#SBATCH --ntasks=1
#SBATCH --ntasks-per-core=1
#SBATCH --exclusive
#SBATCH --constraint=ivy 
#SBATCH --partition=long

cd ..
../build/apps/Robot2D/Robot2D $1 $2 $3 $4 $5
exit 0
