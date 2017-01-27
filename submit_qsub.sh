#!/bin/bash
#
#PBS -l nodes=2:ppn=4
#PBS -N mpi_spawn_test
#PBS -q qexp
#PBS -j oe
#PBS -l walltime=1:00:00
#

SCRDIR=/scratch/$PBS_JOBID

#Run Midas
#cd $SCRDIR
cd /home/ian/programming/mpi/spawn
cat $PBS_NODEFILE | uniq > hostfile
sed -e 's/$/ slots=1 max_slots=20/' -i hostfile
mpirun --bynode -n 2 --hostfile hostfile /home/ian/programming/mpi/spawn/master /home/ian/programming/mpi/spawn/printer
