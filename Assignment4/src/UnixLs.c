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

      // /* Option i */
      // if (dir->d_name[0] != '.') {
      //   printf("%llu ", fileStat.st_ino);
      //   printf("%s\n", dir->d_name);
      // }

      /* Option l, but date is not done yet */
      if (dir->d_name[0] != '.') {
        printMode(fileStat.st_mode);
        printf("%d ", fileStat.st_nlink);
        getAndPrintUserName(fileStat.st_uid);
        getAndPrintGroup(fileStat.st_gid);
        printf("%lld ", fileStat.st_size);
        printTime(fileStat.st_mtime);
        // printf("%s ", ctime(&fileStat.st_mtime));
        printf("%s\n", dir->d_name);
      }
    }

    /* once you have finished reading a directory it needs to be closed */
    closedir(dirp);
  }

  return 0;
}

void printMode(mode_t mode) {
 printf((S_ISDIR(mode)) ? "d" : "-");
 printf((mode & S_IRUSR) ? "r" : "-");
 printf((mode & S_IWUSR) ? "w" : "-");
 printf((mode & S_IXUSR) ? "x" : "-");
 printf((mode & S_IRGRP) ? "r" : "-");
 printf((mode & S_IWGRP) ? "w" : "-");
 printf((mode & S_IXGRP) ? "x" : "-");
 printf((mode & S_IROTH) ? "r" : "-");
 printf((mode & S_IWOTH) ? "w" : "-");
 printf((mode & S_IXOTH) ? "x" : "-");
 printf("  ");
}

void getAndPrintUserName(uid_t uid) {
  struct passwd *pw = NULL;
  pw = getpwuid(uid);

  if (pw) {
    printf("%s  ", pw->pw_name);
  } else {
    perror("Hmm not found???");
    printf("No name found for %u\n", uid);
  }
}

void getAndPrintGroup(gid_t grpNum) {
  struct group *grp;
  grp = getgrgid(grpNum);

  if (grp) {
    printf("%s  ", grp->gr_name);
  } else {
    printf("No group name for %u found\n", grpNum);
  }
}

void printTime(time_t time) {
  struct tm *timeinfo;
  timeinfo = localtime (&time);

  switch (timeinfo->tm_mon) {
    case 0: printf("Mon "); break;
    case 1: printf("Feb "); break;
    case 2: printf("Mar "); break;
    case 3: printf("Apr "); break;
    case 4: printf("May "); break;
    case 5: printf("Jun "); break;
    case 6: printf("Jul "); break;
    case 7: printf("Aug "); break;
    case 8: printf("Sep "); break;
    case 9: printf("Oct "); break;
    case 10: printf("Nov "); break;
    case 11: printf("Dec "); break;
    default: printf("Error"); break;
  }

  printf("%d ", timeinfo->tm_mday);
  printf("%d ", timeinfo->tm_year + 1900);
  printf("%d:%d ", timeinfo->tm_hour, timeinfo->tm_min);
}
