#!/bin/bash

#SBATCH --job-name=Eval3D
#SBATCH --mail-user=sebastien.lengagne@uca.fr
#SBATCH --mail-type=ALL
#SBATCH --ntasks=1
#SBATCH --ntasks-per-core=1
#SBATCH --exclusive
# #SBATCH --constraint=ivy 
#SBATCH --partition=fast

../build/apps/Robot3D/Eval3D $1 
exit 0
