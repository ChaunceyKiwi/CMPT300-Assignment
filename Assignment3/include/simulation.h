/*------------------------------------------------------
 *
 *  simulation.h
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
#include <string.h>
#include "list.h"

#define MSG_LEN 40
#define MAX_NUM_PROC 200

typedef unsigned int PID;

enum state {RUNNING, READY, BLOCKED, EXITED};
typedef struct PCB_ {
  PID pid;                    /* the id of process */
  int priority;               /* priority of the process */
  enum state proc_state;      /* the state of the process */
  char proc_message[MSG_LEN]; /* messages sent or replied to this process */
  int print_proc_message;     /* imply if the proc_message should be displayed */
} PCB;

typedef struct semaphore_ {
  int val;     /* the value of the semaphores */
  LIST* plist; /* processes blocked on this semaphore */
} SEM;

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
int newSemaphore(int semID, int semVal);

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

/* demote priority to perform round robin with priority */
void demotePriority(PID pid);

/* allocate PID by incrementing a unsigned integer */
PID allocateID();

void killBlockedProcess(PID pid);
PCB* createPCB(int priority);
PCB* copyPCB(PID originID);
int pidIsEqual(void* item, void* comparisonArg);
void printQueue(LIST* list);
void displayWelcomeInfo();
void displayMenu();
void freeItem(void* item);
void freeMemory();

#endif /* OS_ASS3_SIMULATION_H_ */
