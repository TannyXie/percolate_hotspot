#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include "arralloc.h"
#include "uni.h"
#include "percolate.h"

int** map;
int loop, nchange, old;
long int counter_p1, counter_p2, counter_p3;
int argc;
char** argv;

void update(int i, int j) {
  old = map[i][j];
  if (map[i-1][j] > map[i][j]) map[i][j] = map[i-1][j];
  if (map[i+1][j] > map[i][j]) map[i][j] = map[i+1][j];
  if (map[i][j-1] > map[i][j]) map[i][j] = map[i][j-1];
  if (map[i][j+1] > map[i][j]) map[i][j] = map[i][j+1];
  if (map[i][j] != old)
    {
      nchange++;
    }
}

int msec = 10, trigger = 10, round = 20;

void print_counter(){
  clock_t before = clock();
  FILE* handle;
  handle = fopen("Counter_p.txt","w+");
  int r = 0;
  while(r < round){
    clock_t diff = clock() - before;
    while(diff < trigger){
      diff = clock() - before;
    }
    fprintf(handle, "Counter1:%8ld, Counter2:%8ld, Counter3:%8ld\n", counter_p1, counter_p2, counter_p3);
    ++r;
  }
  fclose(handle);
}

int main_step();

int main(int argc_m, char* argv_m[])
{
  argc = argc_m;
  argv = argv_m;
  pthread_t timer_pid, mainstep_pid;
  pthread_create(&timer_pid, NULL, main_step, NULL);
  pthread_create(&mainstep_pid, NULL, print_counter, NULL);
  pthread_join(&timer_pid, NULL);
  pthread_join(&mainstep_pid, NULL);
  return 0;
}

int main_step()
{
  int length;
  float rho;
  int seed;
  int maxClusters;
  char* datFile;
  char* pgmFile;

  length = 20;
  rho  = 0.30;
  seed = 1564;
  datFile = "map.dat";
  pgmFile = "map.pgm";
  maxClusters = length * length;
  counter_p1 = counter_p2 = counter_p3 = 0;

  int opt;
  int exit_code = -1;
  while ((opt = getopt(argc, argv, ":hd:p:l:s:m:r:")) != -1)
    {
      switch (opt)
	{
	case 'h':
	  exit_code = 0;
	  break;
	case 'd':
	  datFile = optarg;
	  break;
	case 'p':	
	  pgmFile = optarg;
	  break;
	case 'l':	
	  length = atoi(optarg);
	  break;
	case 's':
	  seed = atoi(optarg);
	  break;
	case 'm':
	  maxClusters = atoi(optarg);
	  break;
	case 'r':
	  rho = atof(optarg);
	  break;
	case ':':
	  printf("Missing value!\n");
	  exit_code = 1;
	  break;
	case '?':
	  printf("Unknown option: %c\n", optopt);
	  exit_code = 1;
	  break;
	}
    }
  for(; optind < argc; optind++){
    printf("Extra argument: %s\n", argv[optind]);
    exit_code = 1;
  }
  if (exit_code != -1)
    {
      printf("Usage: percolate [-d DAT_FILE] [-p PGM_FILE] [-l LENGTH] [-s SEED] [-r RHO]\n");
      exit(exit_code);
    }
  if (length < 1)
    {
      printf("length (%d) must be >= 1\n", length);
      exit(1);
    }
  if ((rho < 0) || (rho > 1))
    {
      printf("rho (%f) must be >= 0 and <= 1\n", rho);
      exit(1);
    }
  if ((seed < 0) || (seed > 900000000))
    {
      printf("seed (%d) must be >= 0 and <= 900000000\n", seed);
      exit(1);
    }
  if ((maxClusters < 0) || (maxClusters > length * length))
    {
      printf("maxClusters (%d) must be >= 0 and <= %d\n", maxClusters, length * length);
      exit(1);
    }
  printf("Rho (density): %f\n", rho);
  printf("Map width/height: %d\n", length);
  printf("Seed: %d\n", seed);
  printf("Maximum number of clusters: %d\n", maxClusters);
  printf("Data file: %s\n", datFile);
  printf("PGM file: %s\n", pgmFile);

  map = (int**)arralloc(sizeof(int), 2, length + 2, length + 2);

  rinit(seed);

  int nfill;
  int i, j;
  float r;
  for (i=0; i<length+2; i++)
    {
      for (j=0; j<length+2; j++)
	{
	  map[i][j] = 0;
	}
    }
  nfill = 0;
  for (i=1; i<=length; i++)
    {
      for (j=1; j<=length; j++)
	{
	  r=random_uniform();
	  if(r > rho)
	    {
	      nfill++;
	      map[i][j]=1;
	    }
	}
    }
  printf("rho = %f, actual density = %f\n",
	 rho, 1.0 - ((double) nfill)/((double) length*length) );

  nfill = 0;
  for (i=1; i<=length; i++)
    {
      for (j=1; j<=length; j++)
	{
	  if (map[i][j] != 0)
	    {
	      nfill++;
	      map[i][j] = nfill;
	    }
	}
    }

  loop = 1;
  nchange = 1;
  while (nchange > 0)
    {
      nchange = 0;
      for (i=1; i<=length; i++)
	{
	  for (j=1; j<=length; j++)
	    {
	      if (map[i][j] != 0)
		{
      update(i, j);
		}
	    }
	}
      printf("Number of changes on loop %d is %d\n", loop, nchange);
      loop++;
    }

  int itop, ibot, percclusternum;
  int percs = 0;
  percclusternum = 0;
  for (itop=1; itop<=length; itop++)
    {
      if (map[1][itop] > 0)
	{
	  for (ibot=1; ibot<=length; ibot++)
	    {
	      if (map[1][itop] == map[length][ibot])
		{
		  percs = 1;
		  percclusternum = map[1][itop];
		}
	    }
	}
    }
  if (percs)
    {
      printf("Cluster DOES percolate. Cluster number: %d\n", percclusternum);
    }
  else
    {
      printf("Cluster DOES NOT percolate\n");
    }

  printf("Opening file %s\n", datFile);
  FILE *fp;
  fp = fopen(datFile, "w");
  printf("Writing data ...\n");
  for (i=1; i<=length; i++)
    {
      for (j=1; j<=length; j++)
	{
	  fprintf(fp, " %4d", map[i][j]);
	}
      fprintf(fp,"\n");
    }
  printf("...done\n");
  fclose(fp);
  printf("Closed file %s\n", datFile);

  int ncluster, maxsize;
  struct cluster *clustlist;
  int colour;
  int *rank;
  clustlist = (struct cluster*)arralloc(sizeof(struct cluster), 1, length*length);
  rank = (int*)arralloc(sizeof(int), 1, length*length);
  for (i=0; i < length*length; i++)
    {
      rank[i] = -1;
      clustlist[i].size = 0;
      clustlist[i].id   = i+1;
    }
  for (i=1;i<=length; i++)
    {
      for (j=1; j<=length; j++)
	{
	  if (map[i][j] != 0)
	    {
	      ++(clustlist[map[i][j]-1].size);
	    }
	}
    }
  percsort(clustlist, length*length);
  maxsize = clustlist[0].size;
  for (ncluster=0; ncluster < length*length && clustlist[ncluster].size > 0; ncluster++);
  if (maxClusters > ncluster)
    {
      maxClusters = ncluster;
    }
  for (i=0; i < ncluster; i++)
    {
      rank[clustlist[i].id - 1] = i;
    }
  printf("Opening file %s\n", pgmFile);
  fp = fopen(pgmFile, "w");
  printf("Map has %d clusters, maximum cluster size is %d\n",
	 ncluster, maxsize);
  if (maxClusters == 1)
    {
      printf("Displaying the largest cluster\n");
    }
  else if (maxClusters == ncluster)
    {
      printf("Displaying all clusters\n");
    }
  else
    {
      printf("Displaying the largest %d clusters\n", maxClusters);
    }
  printf("Writing data ...\n");
  fprintf(fp, "P2\n");
  if (maxClusters > 0)
    {
      fprintf(fp, "%d %d\n%d\n", length, length, maxClusters);
    }
  else
    {
      fprintf(fp, "%d %d\n%d\n", length, length, 1);
    }
  for (i=1; i<=length; i++)
    {
      for (j=1; j<=length; j++)
	{
	  if (map[i][j] > 0)
	    {
	      colour = rank[map[i][j]-1];
	      if (colour >= maxClusters)
		{
		  colour = maxClusters;
		}
	    }
	  else
	    {
	      colour = maxClusters;
	    }
	  fprintf(fp, " %4d", colour);
	}
      fprintf(fp,"\n");
    }
  printf("...done\n");
  fclose(fp);
  printf("Closed file %s\n", pgmFile);
  free(clustlist);
  free(rank);

  free(map);
}

static int clustcompare(const void *p1, const void *p2)
{
  int size1, size2, id1, id2;

  size1 = ((struct cluster *) p1)->size;
  size2 = ((struct cluster *) p2)->size;

  id1   = ((struct cluster *) p1)->id;
  id2   = ((struct cluster *) p2)->id;

  if (size1 != size2)
    {
      return(size2 - size1);
    }
  else
    {
      return(id2 - id1);
    }
}

void percsort(struct cluster *list, int n)
{
  qsort(list, (size_t) n, sizeof(struct cluster), clustcompare);
}
