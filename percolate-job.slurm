#!/bin/bash
# Slurm job options (name, compute nodes, job time)
#SBATCH --job-name=percolate
#SBATCH --time=0:20:0
#SBATCH --exclusive
#SBATCH --nodes=1
#SBATCH --tasks-per-node=36
#SBATCH --cpus-per-task=1
# Replace [project] below with the project code for your MSc
# programme and course that you were told to use when you created an
# account on Cirrus within SAFE.
# Replace [student] below with your unique student budget code
# (e.g. s1234567).
#SBATCH --account=[project]-[student]
# We use the "standard" partition as we are running on CPU nodes
#SBATCH --partition=standard
# We use the "standard" QoS as our runtime is less than 4 days
#SBATCH --qos=standard

pwd
date
echo "Running Percolate on a back-end node"
./percolate -l 100
echo "Completed running Percolate on a back-end node"
