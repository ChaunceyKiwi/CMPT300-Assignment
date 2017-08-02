#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>

#define PATH_MAX_LENGTH 256

void printMode(mode_t mode);
void getAndPrintUserName(uid_t uid);
void getAndPrintGroup(gid_t grpNum);
void printTime(time_t time);
void listFiles(char* dirName, int flag_i, int flag_l, int flag_R, int printDirFlag);
