/*------------------------------------------------------
 *
 *  UnixLs.h
 *
 *  This file contains the declarations of functions
 *  and global variables
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

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>

#define PATH_MAX_LENGTH 4096 /* maximum length allowed for a path name */

/* Print information of files in the specified directory */
void listFiles(char* dirName, int* flags, int printDirFlag);

/* Print information of file on specified path */
void printFileInfo(char* fileName, char* dirName, int* flags);

/* Print the read/write/execute permission of the file */
void printMode(mode_t mode);

/* Print the user name of the file owner */
void getAndPrintUserName(uid_t uid);

/* Print the group name the owner belongs to */
void getAndPrintGroup(gid_t grpNum);

/* Print the date and the time when the file was last modified */
void printTime(time_t time);

/* Set flags with information in input */
void setFlags(char* input, int* flags);

/* Identify if a file is directory file or ordinary file */
int isDirectory(char* filePath);
