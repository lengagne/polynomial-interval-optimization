#!/bin/bash

#SBATCH --job-name=job1
#SBATCH --mail-user=sebastien.lengagne@uca.fr
#SBATCH --mail-type=FAIL
#SBATCH --ntasks=1
#SBATCH --ntasks-per-core=1
#SBATCH --exclusive
#SBATCH --constraint=ivy 

../build/apps/Robot2D/Robot2D $1 $2 $3 $4
exit 0
