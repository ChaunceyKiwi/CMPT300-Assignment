/*------------------------------------------------------
 *
 *  simulation.c
 *
 *  This file contains the functions to implement a program
 *  which creates an interactive operating system simulation
 *  that supports a few basic O/S functions.
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

#include <stdio.h>
#include "simulation.h"

LIST *readyQueues[4];
PID idAllocator = 0; /* imply the id of next new process */
PCB *currProc = NULL;

int main(void)
{
  readyQueues[0] = ListCreate(); /* queue with high priority */
  readyQueues[1] = ListCreate(); /* queue with normal priority */
  readyQueues[2] = ListCreate(); /* queue with low priority */
  readyQueues[3] = ListCreate(); /* queue to put 'init' process */

  /* add 'init' process to queue */
  create(3);

  return 0;
}

/* create a process and put it on the appropriate ready Q */
int create(int priority) {
  PCB* pcbPtr = createPCB(priority);
  ListPrepend(readyQueues[priority], (void*)pcbPtr);
  printf("Process is successfully created, the pid is %u\n", pcbPtr->pid);
  return 0;
}

/* Copy current runnning process and put it on the ready Q
 * corresponding to the original process' priority. Attempting
 * to Fork the "init" process should fail */
int fork() {
  PCB* newProc = copyPCB(currProc);
  ListPrepend(readyQueues[newProc->priority], (void*)newProc);
  printf("Process is successfully forked, the pid is %u\n", newProc->pid);
  return 0;
}

/* kill the named process and remove it from the system */
// TO-DO 'Init' process can only be killed (exit) if there are no other processes
int killProc(PID pid) {
  PCB *result;
  for (int i = 0; i < 4; i++) {
    result = (PCB*)ListSearch(readyQueues[i], pidIsEqual, &pid);
    if (result != NULL) {
      ListRemove(readyQueues[i]);
      printf("Target process is successfully killed!\n");
      return 0;
    }
  }
  return 1;
}

/* kill currently running process */
int exitProc() {
  PCB *result;
  for (int i = 0; i < 4; i++) {
    result = (PCB*)ListSearch(readyQueues[i], pidIsEqual, &currProc->pid);
    if (result != NULL) {
      ListRemove(readyQueues[i]);
      printf("Current process is successfully exited!\n");
      return 0;
    }
  }

  return 1;
}

/* time quantum of running process expires */
void quantum() {
  if (ListCount(readyQueues[0]) != 0) {
    currProc = (PCB*)ListTrim(readyQueues[0]);
    ListPrepend(readyQueues[0], currProc);
  } else if (ListCount(readyQueues[1]) != 0) {
    currProc = (PCB*)ListTrim(readyQueues[1]);
    ListPrepend(readyQueues[1], currProc);
  } else if (ListCount(readyQueues[2]) != 0) {
    currProc = (PCB*)ListTrim(readyQueues[2]);
    ListPrepend(readyQueues[2], currProc);
  } else {
    currProc = (PCB*)ListTrim(readyQueues[3]);
    ListPrepend(readyQueues[3], currProc);
  }
  printf("Process %u gets running for a time quantum\n", currProc->pid);
}

/* send a message to another process - block until reply */
int send(PID pid, char* msg) {
  return 0;
}

/* receive a message - block until one arrives */
void receive() {
}

/* unblocks sender and delivers reply */
int reply(PID pid, char * msg) {
  return 0;
}

/* Initialize the named semaphore with the value given ID's
 * can take a value from 0 to 4. This can only be done once
 * for a semaphore - subsequent attempts result in error */
int newSemaphore(int semID, int initVal) {
  return 0;
}

/* execute the semaphore P operation on behalf of the running
 * process. You can assume semaphores IDs numbered 0 through 4 */
int semaphoreP(int semID) {
  return 0;
}

/* execuate the semaphore V operation on behalf of the running
 * process. You can assume semaphore IDs numbered 0 thgough 4 */
int semaphoreV(int semID) {
  return 0;
}

/* dump complete state information of process to screen, which
 * include process status and anything else you can think of */
void procinfo(PID pid) {
  PCB *result;
  for (int i = 0; i < 4; i++) {
    result = (PCB*)ListSearch(readyQueues[i], pidIsEqual, &pid);
    if (result != NULL) {
      printf("PID %u\n", result->pid);
      printf("Priority: %d\n", result->priority);
    }
  }
}

/* display all process queues and their contents */
void totalInfo() {
  for (int i = 0; i < 4; i++) {
    printf("Contents of ready queue with priority %d:\n", i);
    printQueue(readyQueues[i]);
  }
}

/***********************************************************
*   Helper functions implementation
*/

// TO-DO: make sure it won't over flow
PID allocateID() {
  return idAllocator++;
}

PCB* createPCB(int priority) {
  PCB* pcbPtr = (PCB*) malloc(sizeof(PCB));
  pcbPtr->pid = allocateID();
  pcbPtr->priority = priority;
  return pcbPtr;
}

PCB* copyPCB(PCB* origin) {
  PCB* pcbPtr = (PCB*) malloc(sizeof(PCB));
  pcbPtr->pid = allocateID();
  pcbPtr->priority = origin->priority;
  return pcbPtr;
}

int pidIsEqual(void* item, void* comparisonArg) {
  return ((PCB*)item)->pid == *((PID*)comparisonArg);
}

void printQueue(LIST* list) {
  ListNode* iter = list->head;
  while(iter != NULL) {
    printf("%u ", ((PCB*)iter->val)->pid);
    iter = iter->next;
  }
  printf("\n");
}
