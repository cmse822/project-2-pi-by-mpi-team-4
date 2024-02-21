#!/bin/bash
########## Define Resources Needed with SBATCH Lines ##########
 
#SBATCH --time=00:30:00             # limit of wall clock time - how long the job will run (same as -t)
#SBATCH --cpus-per-task=1           # number of CPUs (or cores) per task (same as -c)
#SBATCH --mem=2G                    # memory required per node - amount of memory (in bytes)
#SBATCH --job-name par_pi_job      # you can give your job a name for easier identification (same as -J)
 
# Standard output and error to file
# %x: job name, %j: job ID
#SBATCH --output=%x-%j.SLURMout
########## Command Lines to Run ##########

# module purge
# module load GCC
# module load OpenMPI
# # module load HDF5
# module load Python
# module load git
#SBATCH --constraint=amr

# cd ./                 ### change to the directory where your code is located

darts=(1e3 1e6 1e9)
processors=(1 2 4 8 16 32)

for d in "${darts[@]}"; do
    for p in "${processors[@]}"; do
        srun -n $p --time=1:00:00 par_pi_calc_Q3.exe $d >> output.txt
    done
done
# scontrol show job $SLURM_JOB_ID     ### write job information to output file
