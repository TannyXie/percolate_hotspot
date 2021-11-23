# MSc Programming Skills C program to Percolate

## Requirements

* GCC compiler.

To get the GCC compiler on Cirrus, run:

```console
$ module load gcc
```

---

## Compilation

Compile the source code and assemble it but do not link it (`-c` flag) and add default debug information to compiled code (`-g` flag):

```console
$ gcc -g -c arralloc.c uni.c percolate.c
```

Link the binaries into an executable file, and also link the common maths library (`-l` flag for link, `m` for maths library):

```console
$ gcc -g -o percolate arralloc.o uni.o percolate.o -lm
```

---

## Usage

To run the Percolate program, run the percolate executable file:

```console
$ ./percolate [-d DAT_FILE] [-p PGM_FILE] \
  [-l LENGTH] [-s SEED] \
  [-r RHO] [-m  MAX_CLUSTERS]
```

(where `\` denotes a line contuation character)

For example, to run with default values for the parameters:

```console
$ ./percolate
```

The program will output information on its operation. For example:

```
Rho (density): 0.300000
Map width/height: 20
Seed: 1564
Maximum number of clusters: 400
Data file: map.dat
PGM file: map.pgm
rho = 0.300000, actual density = 0.310000
Number of changes on loop 1 is 254
Number of changes on loop 2 is 237
Number of changes on loop 3 is 229
Number of changes on loop 4 is 214
Number of changes on loop 5 is 215
Number of changes on loop 6 is 193
Number of changes on loop 7 is 183
Number of changes on loop 8 is 168
Number of changes on loop 9 is 164
Number of changes on loop 10 is 151
Number of changes on loop 11 is 144
Number of changes on loop 12 is 134
Number of changes on loop 13 is 123
Number of changes on loop 14 is 113
Number of changes on loop 15 is 98
Number of changes on loop 16 is 88
Number of changes on loop 17 is 75
Number of changes on loop 18 is 64
Number of changes on loop 19 is 50
Number of changes on loop 20 is 43
Number of changes on loop 21 is 33
Number of changes on loop 22 is 19
Number of changes on loop 23 is 11
Number of changes on loop 24 is 8
Number of changes on loop 25 is 4
Number of changes on loop 26 is 2
Number of changes on loop 27 is 1
Number of changes on loop 28 is 0
Cluster DOES percolate. Cluster number: 271
Opening file map.dat
Writing data ...
...done
Closed file map.dat
Opening file map.pgm
Map has 12 clusters, maximum cluster size is 247
Displaying all clusters
Writing data ...
...done
Closed file map.pgm
```

Other examples of running the program include:

```console
$ ./percolate -r 0.4 -d map_rho0.4.dat -p map_rho0.4.pgm
$ ./percolate -l 100 -r 0
$ ./percolate -l 100 -s 123
```

### Command-line parameters

| Parameter | Description | Default Value |
| --------- |------------ | ------------- |
| `-l` | Map width/height (lentgh). length >= 1 | 20 |
| `-r` | Density (rho). 0 <= rho <= 1 | 0.3 |
| `-s` | Seed for random number generator (seed). Using the same seed allows the same random numbers to be generated every time the program is run. 0 <= seed <= 900,000,000 | 1564 |
| `-m` | Maximum number of clusters to output to the PGM file (maxClusters). If maxClusters is 1 then only the largest cluster is output. If it is 2 then the two largest are output etc. If it is (length * length) then all clusters are output. 0 <= maxClusters <= (length * length) | 400 |
| `-d` | Data file name (datFile) | map.dat |
| `-p` | PGM file name (pgmFile) | map.pgm |

### DAT output files

A plain-text data files is output. This contains the final state of the grid.

This file is plain-text so you can view it as you would any plain-text file e.g.:

```console
$ cat map.dat
```

This file does not include the halo as the use of a halo is an implementation detail.

### PGM output files

A Portable Grey Map (PGM) file is output.

This file does not include the halo as the use of a halo is an implementation detail.

This file is plain-text so you can view it as you would any plain-text file e.g.:

```console
$ cat map.pgm
```

PGM files can be viewed graphically using ImageMagick commands as follows.

Cirrus users will need first need to run:

```console
$ module load ImageMagick
```

To view a PPM file, run:

```console
$ display map.pgm
```

For more information on the PGM file format, run `man pgm` or see [PGM](http://netpbm.sourceforge.net/doc/pgm.html).

---

# A Cirrus Slurm job submission script for Percolate

`percolate-job.slurm` is a very simple job submission script for Cirrus which submits a job to a compute node to run Percolate.

To use it, you will need to edit it to:

* Replace `[project]` with the project code for your MSc programme and course that you were told to use when you created an account on Cirrus within SAFE.
* Replace `[student]` with your unique student budget code (e.g. s1234567).
* Change `./percolate -l 100` if you want to change the parameters passed to percolate.

The job requests 1 back-end node for 20 minutes. You can submit the job as follows:

```console
$ sbatch percolate-job.slurm
```

A job ID will then be displayed, for example:

```
Submitted batch job 252132
```

You can check the job's progress in the queue (if there is no entry it might already have completed):

```console
$ squeue | grep 252132
```

Once it completes you will see the Percolate output files plus a slurm-<JOB_ID>.out file:

```console
$ ls
... map.dat map.pgm slurm-252132.out ...
```

`slurm-<JOB_ID>.out` has the standard output from your script.

For more information Running Jobs on Cirrus using Slurm, see https://cirrus.readthedocs.io/en/master/user-guide/batch.html.
