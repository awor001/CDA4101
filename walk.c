#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <mpi.h>


int listDirectories (char *basePath, const int root, int rank, int *count)
{
   int i;
   char path[100];
   struct dirent *dp;
   DIR *dir = opendir(basePath);

   // printf("%s\n", basePath);
   //fflush(stdout);
   if (!dir)
      return 1;

   while ((dp = readdir(dir)) != NULL)
   {
      if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
      {
         printf("dir#%d , ", ++(*count));
         printf("proc# %d) ", rank);
         for(i = 0; i <= root; i++)
            printf("|");

         printf("|-%s\n", dp->d_name);
         fflush(stdout);

         strcpy(path, basePath);

         strcat(path, "/");

         strcat(path, dp->d_name);

         listDirectories(path, root + 1, rank, count);
      }
}
   closedir(dir);

   return 0;
}

int main (int argc, char *argv[])
{
   
   char pathList[100][100];
   char path[100] = "."; //This is hardcoded to your current directory.
 
   double elapsed_time;
   struct dirent *dp;

   int rank;
   int number_processes;

   int count = 0;
   int globalCount = 0;
     
   DIR * dir = opendir(path);
   int i = 0;
   int j;
   while ((dp = readdir(dir)) != NULL)
   {
      if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) //
      strcpy(pathList[i], dp->d_name);
      
      i++;
   }
   closedir(dir);
     
   
   MPI_Init (&argc, &argv);
   MPI_Comm_rank (MPI_COMM_WORLD, &rank);
   MPI_Comm_size (MPI_COMM_WORLD, &number_processes);
   MPI_Barrier (MPI_COMM_WORLD);
   elapsed_time = - MPI_Wtime();
   
   printf("j: %d, rank: %d, processes: %d\n", j, rank, number_processes);
   fflush(stdout);
   for (j = rank + 2; j <= 61; j += number_processes) // you might have to change 61 depending on how many folders in current you have
   {                                                  // this range is 60 for 60 folders; don't touch the 2
      strcpy(path, "./");
      strcat(path, pathList[j]);
    
      printf("%d) |-%s\n", rank, pathList[j]);
 
      listDirectories(path, 0, rank, &count);
   }

   MPI_Reduce (&count, &globalCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
   
   elapsed_time += MPI_Wtime();
   if(!rank)
   {
      printf("Execution time: %8.6f\n\n", elapsed_time);
      fflush(stdout);
   }
   printf("Rank: %d, count: %d\n", rank, count);

   MPI_Finalize();
   printf("Rank : %d, count: %d\n", rank, count); 
   if (!rank)
      printf("Number of total directories: %d\n", globalCount);
   return 0;
}
