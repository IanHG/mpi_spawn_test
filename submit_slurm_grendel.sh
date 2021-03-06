#!/bin/bash
#
#SBATCH --partition=q28
#SBATCH --job-name mytest     # name of job
#SBATCH --ntasks-per-node 3   # SLURM_NTASKS_PER_NODE
#SBATCH --nodes 2             # SLURM_JOB_NUM_NODES
#SBATCH --time 1:00:00        # max time (HH:MM:SS)
#

echo Running on $(hostname)
echo Available nodes: $SLURM_NODELIST
echo Slurm_submit_dir: $SLURM_SUBMIT_DIR
echo Start time: $(date)
echo Scratch: $SCRATCH

user='ianhg'

# Threading
export OMP_NUM_THREADS=1
export MKL_NUM_THREADS=1

# Modules
#module purge
#module load openmpi/gnu-slurm/2.0.1
ml purge
#ml openmpi/gnu-slurm/1.6.5
#ml openmpi/intel-slurm/1.10.5
ml openmpi/intel17-slurm/2.0.2

mpirun --version

# create hostfile
echo "Creating host file"
echo $SLURM_JOB_NODELIST | $SLURM_SUBMIT_DIR/parse_slurm_nodelist.py > $SLURM_SUBMIT_DIR/hostfile
sed -e 's/$/ slots=3 max_slots=20/' -i $SLURM_SUBMIT_DIR/hostfile

#Run Midas
echo "Running program"
mpirun --map-by node -n 2 --hostfile $SLURM_SUBMIT_DIR/hostfile $SLURM_SUBMIT_DIR/master $SLURM_SUBMIT_DIR/printer
