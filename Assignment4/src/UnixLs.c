/*------------------------------------------------------
 *
 *  UnixLs.c
 *
 *  This file contains the functions to implement
 *  the file listing functions in UNIX system, which
 *  emulates ls command in UNIX system
 *
 *  Name         : Chauncey Liu
 *  Student ID   : 301295771
 *  SFU username : cla284
 *  Course       : CMPT 300 Operating Systems I, Summer 2017
 *  Instructor   : Harinder Khangura
 *  TA           : Amineh Dadsetan
 *
 *  Created by Chauncey on 2017-08-01.
 *  Copyright (c) 2017 Chauncey. All rights reserved.
 *
 *------------------------------------------------------
 */

#include "UnixLs.h"

int main(int argc, char **argv)
{
  /* flags[0/1/2] : state of -i/-l/-R flag */
  int flags[3];
  for (int i = 0; i < 3; i++) {
    flags[i] = 0;
  }

  /* read options */
  int i = 1;
  while (i < argc) {
    if (argv[i][0] == '-') {
      setFlags(argv[i], flags);
    } else {
      break;
    }
    i++;
  }

  /* If no path file is given, perform listing on current directory */
  if (i == argc) {
    listFiles(".", flags, 0);
  }

  /* If only one path file is given */
  if (i == argc - 1) {
    listFiles(argv[i], flags, 0);
    i++;
  }

  /* If more than one path files are given */
  while (i < argc) {
    listFiles(argv[i], flags, 1);
    i++;
  }

  return 0;
}

/**
 * Print information of files in the specified directory
 * @param dirName the name of directory specified
 * @param flags[0/1/2] : state of -i/-l/-R flag
 * @param printDirFlag indicate if the directory name should be print firstly
 */
void listFiles(char* dirName, int* flags, int printDirFlag) {
  /* open directory */
  DIR *dirp = opendir(dirName);
  struct dirent *dir;

  if (dirp == NULL) {
    int res = printFileInfo(dirName, ".", flags);
    if (res == 1) {
      fprintf(stderr, "UnixLs: %s: No such file or directory\n", dirName);
      exit(EXIT_FAILURE);
    }
  } else {
    /* print directory name if the printDirFlag is set */
    if (printDirFlag) {
      printf("\n%s\n", dirName);
    }

    /* first pass: printing the names of all files */
    while ((dir = readdir(dirp)) != NULL)
    {
      if (dir->d_name[0] == '.') {
        continue;
      }

      printFileInfo(dir->d_name, dirName, flags);
    }

    /* second pass: print the content of directory file */
    if (flags[2] == 1) {
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
          listFiles(path, flags, 1);
        }
      }
    }

    /* once you have finished reading a directory it needs to be closed */
    closedir(dirp);
  }
}

int printFileInfo(char* fileName, char* dirName, int* flags) {
  struct stat fileStat;
  char path[PATH_MAX_LENGTH];
  memset(path, 0, sizeof(path));
  snprintf(path, sizeof(path), "%s/%s", dirName, fileName);

  if (lstat(path, &fileStat) < 0) {
    return 1;
  }

  if (flags[0]) {
    printf("%8llu ", fileStat.st_ino);
  }

  /* Option l */
  if (flags[1]) {
    printMode(fileStat.st_mode);
    printf("%2d ", fileStat.st_nlink);
    getAndPrintUserName(fileStat.st_uid);
    getAndPrintGroup(fileStat.st_gid);
    printf("%6lld ", fileStat.st_size);
    printTime(fileStat.st_mtime);
  }

  printf("%s", fileName);

  if (flags[1]) {
    if (S_ISLNK(fileStat.st_mode)) {
      char real_path[PATH_MAX_LENGTH];
      memset(real_path, 0, sizeof(real_path));

      if (readlink(path, real_path, sizeof(real_path) - 1) < 0) {
        perror("readlink");
        return 2;
      }

      printf(" -> %s", real_path);
    }
  }

  printf("\n");
  return 0;
}

/**
 * Print the read/write/execute permission of the file
 * This function extracts information needed from mode_t structure
 * @param mode the protection information of a file
 */
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

/**
 * Print the user name of the file owner
 * The function searchs user database for a user ID
 * @param uid the id of a user
 */
/* print the user name of the file owner */
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

/**
 * Print the group name the owner belongs to
 * The function gets group database entry for a group ID
 * @param grpNum the id of a group
 */
void getAndPrintGroup(gid_t grpNum) {
  struct group *grp;
  grp = getgrgid(grpNum);

  if (grp) {
    printf("%6s  ", grp->gr_name);
  } else {
    printf("No group name for %u found\n", grpNum);
  }
}

/**
 * Print the date and the time when the file was last modified
 * This function extracts information needed from time_t structure
 * @param time time of last modification
 */
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

/**
 * Set flags with information in input
 * @param input the input to set flags
 * @param flags flags[0/1/2] : state of -i/-l/-R flag
 */
void setFlags(char* input, int* flags) {
  int flag_pos = 1;
  while(1) {
    if (input[flag_pos] == 'i') {
      flags[0] = 1;
    } else if (input[flag_pos] == 'l') {
      flags[1] = 1;
    } else if (input[flag_pos] == 'R') {
      flags[2] = 1;
    } else {
      if (input[flag_pos] != '\0' || flag_pos == 1) {
        fprintf(stderr, "UnixLs: illegal option %s\n", input);
        fprintf(stderr, "usage: ls [-ilR] [file ...]\n");
        exit(EXIT_FAILURE);
      }
      break;
    }
    flag_pos++;
  }
}
