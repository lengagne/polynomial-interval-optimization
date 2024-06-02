#!/bin/bash

#SBATCH --job-name=b2D_fast
#SBATCH --mail-user=sebastien.lengagne@uca.fr
#SBATCH --mail-type=FAIL
#SBATCH --ntasks=1
#SBATCH --ntasks-per-core=1
#SBATCH --constraint=ivy 
#SBATCH --partition=normal
#SBATCH --time=30:00



../../build/apps/Robot2D/Robot2D $1 $2 $3 $4 $5 $6
exit 0
