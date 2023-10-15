#!/bin/bash
#SBATCH --job-name=gridding-job
#SBATCH --partition=batch
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --output=job-gridding.out
#SBATCH --error=job-gridding.err
#SBATCH --cpus-per-task=64

./gridding_shared -i hltau_completo_uv.csv -o datosgrideados.raw -d 0.003 -N 2048 -c 3 -t 5