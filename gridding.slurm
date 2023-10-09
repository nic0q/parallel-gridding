#!/bin/bash
#SBATCH --job-name=gridding-job
#SBATCH --partition=batch
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --output=job-gridding.out
#SBATCH --error=job-gridding.err
#SBATCH --cpus-per-task=64

./gridding -i inputfle.txt -o outputfile.raw -h 64 -d arcsec