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

LIST* readyQueues[4];
LIST* blockQueue;
PID idAllocator = 0; /* imply the id of next new process */
PID* currPID = NULL;
PCB* PCBTable[MAX_NUM_PROC];

int main(void)
{
  readyQueues[0] = ListCreate(); /* queue with high priority */
  readyQueues[1] = ListCreate(); /* queue with normal priority */
  readyQueues[2] = ListCreate(); /* queue with low priority */
  readyQueues[3] = ListCreate(); /* queue to put 'init' process */
  blockQueue = ListCreate();     /* queue to put process being blocked */

  /* add 'init' process to queue */
  create(3);
  quantum();

  int priority;
  int pid;

  while(1) {
    char op = getchar();
    switch (op) {
      case 'C':
        scanf("%d", &priority);
        create(priority);
        break;
      case 'F':
        fork();
        break;
      case 'K':
        scanf("%u", &pid);
        killProc(pid);
        break;
      case 'E':
        exitProc();
        break;
      case 'Q':
        quantum();
        break;
      case 'S': break;
      case 'R': break;
      case 'Y': break;
      case 'N': break;
      case 'P': break;
      case 'V': break;
      case 'I':
        scanf("%u", &pid);
        procinfo(pid);
        break;
      case 'T':
        totalInfo();
        break;
      case '!':
        return 0;
    }
  }

  return 0;
}

/* create a process and put it on the appropriate ready Q */
int create(int priority) {
  PCB* pcbPtr = createPCB(priority);
  PCBTable[pcbPtr->pid] = pcbPtr;
  ListPrepend(readyQueues[priority], (void*)(&pcbPtr->pid));
  if (pcbPtr->pid != 0) {
    printf("Process #%u is successfully created\n", pcbPtr->pid);
  }
  return 0;
}

/* Copy current runnning process and put it on the ready Q
 * corresponding to the original process' priority. Attempting
 * to Fork the "init" process should fail */
int fork() {
  PCB* newProc = copyPCB(*currPID);
  PCBTable[newProc->pid] = newProc;
  ListPrepend(readyQueues[newProc->priority], (void*)(&newProc->pid));
  printf("Process #%u is successfully forked, resulting process #%u\n",
   *currPID, newProc->pid);
  return 0;
}

/* kill the named process and remove it from the system */
// TO-DO 'Init' process can only be killed (exit) if there are no other processes
int killProc(PID pid) {
  PCB *result;
  for (int i = 0; i < 4; i++) {
    ListFirst(readyQueues[i]);
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
    ListFirst(readyQueues[i]);
    result = (PCB*)ListSearch(readyQueues[i], pidIsEqual, currPID);
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
  if (currPID != NULL) {
    printf("Time quantum of process #%u expires\n", *currPID);
  }

  if (ListCount(readyQueues[0]) != 0) {
    currPID = (PID*)ListTrim(readyQueues[0]);
    ListPrepend(readyQueues[0], (void*)currPID);
  } else if (ListCount(readyQueues[1]) != 0) {
    currPID = (PID*)ListTrim(readyQueues[1]);
    ListPrepend(readyQueues[1], (void*)currPID);
  } else if (ListCount(readyQueues[2]) != 0) {
    currPID = (PID*)ListTrim(readyQueues[2]);
    ListPrepend(readyQueues[2], (void*)currPID);
  } else {
    currPID = (PID*)ListTrim(readyQueues[3]);
    ListPrepend(readyQueues[3], (void*)currPID);
  }

  printf("Time quantum of process #%u starts\n", *currPID);
}

/* send a message to another process - block until reply */
int send(PID pid, char* msg) {
  for (int i = 0; i < 4; i++) {
    ListFirst(readyQueues[i]);
    PID* result = (PID*)ListSearch(readyQueues[i], pidIsEqual, currPID);
    if (result != NULL) {
      printf("Message sent to the process #%u\n", pid);
      ListPrepend(blockQueue, (void*)ListRemove(readyQueues[i])); /* block sender */
      quantum();
      return 0;
    }
  }

  /* store the message in the PCB */
  strcpy(PCBTable[pid]->proc_message, msg);
  return 0;
}

/* receive a message - block until one arrives */
void receive() {
  PCB* currProc = PCBTable[*currPID];
  if (strlen(currProc->proc_message) != 0) {
    printf("Receive message: %s\n", currProc->proc_message);
    strcpy(currProc->proc_message, "");  /* empty string */
  } else {
    for (int i = 0; i < 4; i++) {
      ListFirst(readyQueues[i]);
      PID* result = (PID*)ListSearch(readyQueues[i], pidIsEqual, currPID);
      if (result != NULL) {
        printf("Process #%u blocked on receiving message\n", *currPID);
        ListPrepend(blockQueue, (void*)ListRemove(readyQueues[i])); /* block receiver */
        quantum();
      }
    }
  }
}

/* unblocks sender and delivers reply */
int reply(PID pid, char* msg) {
  ListFirst(blockQueue);
  PID* result = (PID*)ListSearch(blockQueue, pidIsEqual, &pid);
  PCB* targetPCB = PCBTable[pid];

  if (result != NULL) {
    strcpy(targetPCB->proc_message, msg);
    ListPrepend(readyQueues[targetPCB->priority], (void*)ListRemove(blockQueue)); /* unblock sender */
    printf("Message replied to the process #%u!\n", pid);
    quantum();
    return 0;
  }

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
  PCB *result = PCBTable[pid];

  if (result != NULL) {
    printf("PID %u\n", result->pid);
    printf("Priority: %d\n", result->priority);
  } else {
    printf("Process does not exist!\n");
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

PCB* copyPCB(PID originID) {
  PCB* originProc = PCBTable[originID];
  PCB* pcbPtr = (PCB*) malloc(sizeof(PCB));
  pcbPtr->pid = allocateID();
  pcbPtr->priority = originProc->priority;
  strcpy(pcbPtr->proc_message, originProc->proc_message);
  return pcbPtr;
}

int pidIsEqual(void* item, void* comparisonArg) {
  return *((PID*)item) == *((PID*)comparisonArg);
}

void printQueue(LIST* list) {
  ListNode* iter = list->head;
  while(iter != NULL) {
    printf("%u ", *((PID*)iter->val));
    iter = iter->next;
  }
  printf("\n");
}
