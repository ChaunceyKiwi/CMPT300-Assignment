#include "UnixLs.h"

int main(int argc, char **argv)
{
  int flag_i = 0;
  int flag_l = 0;
  int flag_R = 0;

  int i = 1;

  /* read options */
  while (i < argc) {
    if (argv[i][0] == '-') {

      int flag_pos = 1;
      while (argv[i][flag_pos] == 'i' ||
             argv[i][flag_pos] == 'l' ||
             argv[i][flag_pos] == 'R' ) {

        if (argv[i][flag_pos] == 'i') {
          flag_i = 1;
        } else if (argv[i][flag_pos] == 'l') {
          flag_l = 1;
        } else {
          flag_R = 1;
        }

        flag_pos++;
    }

    } else {
      break;
    }
    i++;
  }

  if (i == argc) {
    listFiles(".", flag_i, flag_l, flag_R);
  }

  /* If only one path file is given */
  if (i == argc - 1) {
    listFiles(argv[i], flag_i, flag_l, flag_R);
    i++;
  }

  /* If more than one path files are given */
  while (i < argc) {
    printf("\n%s\n", argv[i]);
    listFiles(argv[i], flag_i, flag_l, flag_R);
    i++;
  }

  return 0;
}

void listFiles(char* dirName, int flag_i, int flag_l, int flag_R) {
  if (flag_R) {
    printf("\n%s\n", dirName);
  }

  /* open directory */
  DIR *dirp = opendir(dirName);
  struct dirent *dir;

  if (dirp) {
    /* first pass: printing the names of all files */
    while ((dir = readdir(dirp)) != NULL)
    {
      if (dir->d_name[0] == '.') {
        continue;
      }

      struct stat fileStat;
      char path[PATH_MAX_LENGTH];
      memset(path, 0, sizeof(path));
      snprintf(path, sizeof(path), "%s/%s", dirName, dir->d_name);

      if (lstat(path, &fileStat) < 0) {
        printf("Error!!!!\n");
        return;
      }

      if (flag_i) {
        printf("%llu ", fileStat.st_ino);
      }

      /* Option l, but date is not done yet */
      if (flag_l) {
        printMode(fileStat.st_mode);
        printf("%2d ", fileStat.st_nlink);
        getAndPrintUserName(fileStat.st_uid);
        getAndPrintGroup(fileStat.st_gid);
        printf("%6lld ", fileStat.st_size);
        printTime(fileStat.st_mtime);
      }

      printf("%s", dir->d_name);

      if (flag_l) {
        if (S_ISLNK(fileStat.st_mode)) {
          char real_path[PATH_MAX_LENGTH];
          memset(real_path, 0, sizeof(real_path));

          if (readlink(path, real_path, sizeof(real_path) - 1) < 0) {
            perror("readlink");
            return;
          }

          printf(" -> %s", real_path);
        }
      }

      printf("\n");
    }

    if (flag_R == 1) {
      /* second pass: print the content of directory file */
      rewinddir(dirp);
      while ((dir = readdir(dirp)) != NULL)
      {
        /* Skipped over all hidden files */
        if (dir->d_name[0] == '.') {
          continue;
        }

        struct stat fileStat;
        char path[PATH_MAX_LENGTH];
        memset(path, 0, sizeof(path));
        snprintf(path, sizeof(path), "%s/%s", dirName, dir->d_name);

        if (lstat(path, &fileStat) < 0) {
          printf("Error!!!!\n");
          return;
        }

        if(S_ISDIR(fileStat.st_mode)) {
          listFiles(path, flag_i, flag_l, flag_R);
        }
      }
    }

    /* once you have finished reading a directory it needs to be closed */
    closedir(dirp);
  }
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
    printf("%8s  ", pw->pw_name);
  } else {
    perror("Hmm not found???");
    printf("No name found for %u\n", uid);
  }
}

void getAndPrintGroup(gid_t grpNum) {
  struct group *grp;
  grp = getgrgid(grpNum);

  if (grp) {
    printf("%6s  ", grp->gr_name);
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

  printf("%2d ", timeinfo->tm_mday);
  printf("%d ", timeinfo->tm_year + 1900);
  printf("%02d:%02d ", timeinfo->tm_hour, timeinfo->tm_min);
}
