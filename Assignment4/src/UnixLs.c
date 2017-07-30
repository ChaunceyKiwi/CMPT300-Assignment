#include "UnixLs.h"

int main(void)
{
  /* open directory */
  DIR *dirp = opendir(".");
  struct dirent *dir;

  if (dirp) {
    while ((dir = readdir(dirp)) != NULL)
    {
      struct stat fileStat;
      if (stat(dir->d_name, &fileStat) < 0) {
        return 1;
      }

      if (dir->d_name[0] != '.') {
        printf("%llu ", fileStat.st_ino);
        printf("%s\n", dir->d_name);
        // printf("File size: %lld bytes\t", fileStat.st_size);
        // printf("Number of Links: %d\n", fileStat.st_nlink);
      }
    }

    /* once you have finished reading a directory it needs to be closed */
    closedir(dirp);
  }

  return 0;
}
