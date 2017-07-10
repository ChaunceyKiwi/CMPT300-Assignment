/*------------------------------------------------------
 *
 *  s-talk.h
 *
 *  This file contains the declarations of functions of simulation,
 *  and the definitions of global macros
 *
 *  Name         : Chauncey Liu
 *  Student ID   : 301295771
 *  SFU username : cla284
 *  Course       : CMPT 300 Operating Systems I, Summer 2017
 *  Instructor   : Harinder Khangura
 *  TA           : Amineh Dadsetan
 *
 *  Created by Chauncey on 2017-07-09.
 *  Copyright (c) 2017 Chauncey. All rights reserved.
 *
 *------------------------------------------------------
 */

#ifndef OS_ASS3_SIMULATION_H_
#define OS_ASS3_SIMULATION_H_

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#define MSG_LEN 256

typedef unsigned int PID;

typedef struct PCB_ {
  PID pid;
  int priority;
  char proc_message[MSG_LEN];
} PCB;

/* create a process and put it on the appropriate ready Q */
int create(int priority);

/* Copy current runnning process and put it on the ready Q
 * corresponding to the original process' priority. Attempting
 * to Fork the "init" process should fail */
int fork();

/* kill the named process and remove it from the system */
int killProc(PID pid);

/* kill currently running process */
int exitProc();

/* time quantum of running process expires */
void quantum();

/* send a message to another process - block until reply */
int send(PID pid, char* msg);

/* receive a message - block until one arrives */
void receive();

/* unblocks sender and delivers reply */
int reply(PID pid, char * msg);

/* Initialize the named semaphore with the value given ID's
 * can take a value from 0 to 4. This can only be done once
 * for a semaphore - subsequent attempts result in error */
int newSemaphore(int semID, int initVal);

/* execute the semaphore P operation on behalf of the running
 * process. You can assume semaphores IDs numbered 0 through 4 */
int semaphoreP(int semID);

/* execuate the semaphore V operation on behalf of the running
 * process. You can assume semaphore IDs numbered 0 thgough 4 */
int semaphoreV(int semID);

/* dump complete state information of process to screen, which
 * include process status and anything else you can think of */
void procinfo(PID pid);

/* display all process queues and their contents */
void totalInfo();


/***********************************************************
*   Helper functions declarations
*/

PID allocateID();
PCB* createPCB(int priority);
PCB* copyPCB(PCB* origin);
int pidIsEqual(void* item, void* comparisonArg);

#endif /* OS_ASS3_SIMULATION_H_ */
