#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

int main()
{
   struct timeval t1, t2;
   double elapsedTime;
   char path[100];
    
   // Input path from user
   printf("Enter path to list files: ");
   scanf("%s", path);
   
   gettimeofday(&t1, NULL);
   listDirectories(path, 0);
   
   gettimeofday(&t2, NULL);
   elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;
   elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;
   printf("Elapsed time: %lf\n", elapsedTime/1000);  
   return 0;
}
   
    
int listDirectories(char *basePath, const int root)
{
   int i;
   char path[1000];
   struct dirent *dp;
   DIR *dir = opendir(basePath);
    
   // Unable to open directory stream
   if (!dir)
      return 1;
    
   while ((dp = readdir(dir)) != NULL)
   {
      if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
      {
         for(i = 0; i <= root; i++)
            printf("| ");
 
         printf("|-%s\n", dp->d_name);

         strcpy(path, basePath);
         strcat(path, "/");
         strcat(path, dp->d_name);

         listDirectories(path, root + 1);
      }  
   }
   closedir(dir);
   return 0;
}
