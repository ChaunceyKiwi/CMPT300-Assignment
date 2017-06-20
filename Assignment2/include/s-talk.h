/*------------------------------------------------------
 *
 *  s-talk.h
 *
 *  This file contains the declarations of functions of s-talk,
 *  the definitions global macros
 *
 *  Name         : Chauncey Liu
 *  Student ID   : 301295771
 *  SFU username : cla284
 *  Course       : CMPT 300 Operating Systems I, Summer 2017
 *  Instructor   : Harinder Khangura
 *  TA           : Amineh Dadsetan
 *
 *  Created by Chauncey on 2017-05-22.
 *  Copyright (c) 2017 Chauncey. All rights reserved.
 *
 *------------------------------------------------------
 */

#ifndef OS_ASS2_S_TALK_H_
#define OS_ASS2_S_TALK_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <pthread.h>

#include "list.h"

#define MSG_LEN 256
#define BUFFER_MAX_SIZE 30
#define UNUSED __attribute__((unused))

/* retrieves a message from sending buffer
 * sends message to the remote client */
void* sendMsg(void*);

/* receives message from the remote client
 * adds message to the receiving buffer */
void* recvMsg(void*);

/* gets input message from keyboard
 * adds the message to the sending buffer */
void* inputMsg(void*);

/* retrieves a message from receiving buffer
 * print the message to the screen */
void* outputMsg(void*);

int checkIfReady(int fds);

#endif /* OS_ASS2_S_TALK_H_ */