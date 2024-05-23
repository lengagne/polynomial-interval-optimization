#!/bin/bash

#SBATCH --job-name=b3D
#SBATCH --mail-user=sebastien.lengagne@uca.fr
#SBATCH --mail-type=FAIL
#SBATCH --ntasks=1
#SBATCH --ntasks-per-core=1
#SBATCH --constraint=ivy 
#SBATCH --partition=normal

../../build/apps/Robot3D/Robot3D $1 $2 $3 $4 $5
exit 0
