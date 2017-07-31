#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

void printMode(mode_t mode);
void getAndPrintUserName(uid_t uid);
void getAndPrintGroup(gid_t grpNum);
void printTime(time_t time);
void listFiles(char* dirName);
