#!/bin/bash

#SBATCH --job-name=bis_3D
#SBATCH --mail-user=sebastien.lengagne@uca.fr
#SBATCH --mail-type=FAIL
#SBATCH --ntasks=1
#SBATCH --ntasks-per-core=1
#SBATCH --constraint=ivy 

../../build/apps/Robot3D/Robot3D $1 $2 $3 $4 $5
exit 0
