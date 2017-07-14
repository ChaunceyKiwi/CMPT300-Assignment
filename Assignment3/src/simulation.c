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

LIST* ready_queues[3];
LIST* send_block_queue;
LIST* receive_block_queue;
PID idAllocator = 0; /* imply the id of next new process */
PID* currPID = NULL;
PCB* PCBTable[MAX_NUM_PROC];
SEM* semaphores[4];
int processCount = 0;

int main(void)
{
  displayWelcomeInfo();

  ready_queues[0] = ListCreate(); /* queue with high priority */
  ready_queues[1] = ListCreate(); /* queue with normal priority */
  ready_queues[2] = ListCreate(); /* queue with low priority */

  send_block_queue = ListCreate();    /* queue to put process being blocked by sending message*/
  receive_block_queue = ListCreate(); /* queue to put process being blocked by receiing message*/

  /* create and run the 'init' process */
  currPID = &PCBTable[create(3)]->pid;

  /* initialize all semaphore pointers */
  for (int i = 0; i < 4; i++) {
    semaphores[i] = NULL;
  }

  int semID;
  int semVal;
  int priority;
  PID pid;
  char* msg;

  while(1) {
    char op = (char)getchar();
    if (op == '\n') {
      continue;
    }
    switch (op) {
      case 'C': case 'c':
        printf("priority: ");
        scanf("%d", &priority);
        create(priority);
        break;
      case 'F': case 'f':
        fork();
        break;
      case 'K': case 'k':
        printf("pid: ");
        scanf("%u", &pid);
        killProc(pid);
        break;
      case 'E': case 'e':
        exitProc();
        break;
      case 'Q': case 'q':
        quantum();
        break;
      case 'S': case 's':
        msg = (char*) malloc(MSG_LEN * sizeof(char));
        printf("pid: ");
        scanf("%u", &pid);
        printf("message: ");
        scanf("%s", msg);
        send(pid, msg);
        break;
      case 'R': case 'r':
        receive();
        break;
      case 'Y': case 'y':
        msg = (char*) malloc(MSG_LEN * sizeof(char));
        printf("pid: ");
        scanf("%u", &pid);
        printf("message: ");
        scanf("%s", msg);
        reply(pid, msg);
        break;
      case 'N': case 'n':
        printf("semID: ");
        scanf("%d", &semID);
        printf("semVal: ");
        scanf("%d", &semVal);
        newSemaphore(semID, semVal);
        break;
      case 'P': case 'p':
        printf("semID: ");
        scanf("%d", &semID);
        semaphoreP(semID);
        break;
      case 'V': case 'v':
        printf("semID: ");
        scanf("%d", &semID);
        semaphoreV(semID);
        break;
      case 'I': case 'i':
        printf("pid: ");
        scanf("%u", &pid);
        procinfo(pid);
        break;
      case 'T': case 't':
        totalInfo();
        break;
      case 'M': case 'm':
        displayMenu();
        break;
      case '!':
        return 0;
      default:
        printf("Wrong command!\n");
        break;
    }
  }
}

/* create a process and put it on the appropriate ready Q */
int create(int priority) {
  /* Try to create 'init' process */
  if (priority == 3) {
    PCB* pcbPtr = createPCB(priority);
    PCBTable[pcbPtr->pid] = pcbPtr;
    processCount++;
    return pcbPtr->pid;
  }

  /* create normal process */
  else {
    PCB* pcbPtr = createPCB(priority);
    PCBTable[pcbPtr->pid] = pcbPtr;
    ListPrepend(ready_queues[priority], (void*)(&pcbPtr->pid));
    processCount++;
    printf("Process #%u is successfully created\n", pcbPtr->pid);
    return pcbPtr->pid;
  }
}

/* Copy current runnning process and put it on the ready Q
 * corresponding to the original process' priority. Attempting
 * to Fork the "init" process should fail */
int fork() {
  if (*currPID == 0) {
    printf("Failure: 'init' process should not be forked!\n");
    return 1;
  }
  PCB* newProc = copyPCB(*currPID);
  PCBTable[newProc->pid] = newProc;
  ListPrepend(ready_queues[newProc->priority], (void*)(&newProc->pid));
  printf("Process #%u is successfully forked, resulting process #%u\n",
   *currPID, newProc->pid);
  return 0;
}

/* kill the named process and remove it from the system */
// TO-DO 'Init' process can only be killed (exit) if there are no other processes
int killProc(PID pid) {
  if (pid == 0) {
    if (processCount == 1) {
      printf("Target process is successfully killed!\n");
      printf("All processes are gone from system, simulation terminates.\n");
      exit(0);
    } else {
      printf("Failure: 'init' process can only be killed if there are no other processes!\n");
    }
  }

  PCB *result;
  for (int i = 0; i < 3; i++) {
    ListFirst(ready_queues[i]);
    result = (PCB*)ListSearch(ready_queues[i], pidIsEqual, &pid);
    if (result != NULL) {
      ListRemove(ready_queues[i]);
      processCount--;
      printf("Target process is successfully killed!\n");
      return 0;
    }
  }
  return 1;
}

/* kill currently running process */
int exitProc() {
  if (*currPID == 0) {
    if (processCount == 1) {
      printf("Target process is successfully killed!\n");
      printf("All processes are gone from system, simulation terminates.\n");
      exit(0);
    } else {
      printf("Failure: 'init' process can only be killed if there are no other processes!\n");
    }
  }

  PCB *result;
  for (int i = 0; i < 3; i++) {
    ListFirst(ready_queues[i]);
    result = (PCB*)ListSearch(ready_queues[i], pidIsEqual, currPID);
    if (result != NULL) {
      ListRemove(ready_queues[i]);
      processCount--;
      printf("Current process is successfully exited!\n");
      return 0;
    }
  }

  return 1;
}

/* time quantum of running process expires */
void quantum() {
  if (currPID != NULL) {
    /* skip 'init' process since it is not on any queue */
    if (*currPID != 0) {
      PCB* currProc = PCBTable[*currPID];
      ListPrepend(ready_queues[currProc->priority], (void*)currPID);
    }
    printf("Time quantum of process #%u expires\n", *currPID);
  }

  if (ListCount(ready_queues[0]) != 0) {
    currPID = (PID*)ListTrim(ready_queues[0]);
  } else if (ListCount(ready_queues[1]) != 0) {
    currPID = (PID*)ListTrim(ready_queues[1]);
  } else if (ListCount(ready_queues[2]) != 0) {
    currPID = (PID*)ListTrim(ready_queues[2]);
  } else {
    currPID = &PCBTable[0]->pid;
  }

  printf("Time quantum of process #%u starts\n", *currPID);

  PCB* currProc = PCBTable[*currPID];
  if (currProc->print_proc_message == 1 && strlen(currProc->proc_message) != 0) {
    printf("Receive message: %s\n", currProc->proc_message);
    currProc->print_proc_message = 0;
  }
}

/* send a message to another process - block until reply */
int send(PID pid, char* msg) {
  /* unblock receiver if the receiver is blocked on receiving */
  ListFirst(receive_block_queue);
  PID* result = (PID*)ListSearch(receive_block_queue, pidIsEqual, &pid);
  if (result != NULL) {
    PCB* targetPCB = PCBTable[pid];
    ListPrepend(ready_queues[targetPCB->priority], ListRemove(receive_block_queue)); /* unblock receiver */
    targetPCB->print_proc_message = 1;
  }

  /* send message and block sender */
  strcpy(PCBTable[pid]->proc_message, msg);
  printf("Message sent to the process #%u\n", pid);
  ListPrepend(send_block_queue, (void*)currPID); /* block itself */
  printf("Time quantum of process #%u expires\n", *currPID);
  currPID = NULL;
  quantum();
  return 0;
}

/* receive a message - block until one arrives */
void receive() {
  PCB* currProc = PCBTable[*currPID];
  if (strlen(currProc->proc_message) != 0) {
    printf("Receive message: %s\n", currProc->proc_message);
    strcpy(currProc->proc_message, "");  /* empty string */
  } else {
    ListPrepend(receive_block_queue, (void*)currPID); /* block itself */
    printf("Process #%u blocked on receiving message\n", *currPID);
    printf("Time quantum of process #%u expires\n", *currPID);
    currPID = NULL;
    quantum();
  }
}

/* unblocks sender and delivers reply */
int reply(PID pid, char* msg) {
  ListFirst(send_block_queue);
  PID* result = (PID*)ListSearch(send_block_queue, pidIsEqual, &pid);
  PCB* targetPCB = PCBTable[pid];

  /* reply to a process that is blocked on sending */
  if (result != NULL) {
    strcpy(targetPCB->proc_message, msg);
    ListPrepend(ready_queues[targetPCB->priority], ListRemove(send_block_queue)); /* unblock sender */
    targetPCB->print_proc_message = 1;
    printf("Message replied to the process #%u!\n", pid);
    return 0;
  }

  /* reply to a running process */
  else {
    strcpy(targetPCB->proc_message, msg);
    return 0;
  }
}

/* Initialize the named semaphore with the value given ID's
 * can take a value from 0 to 4. This can only be done once
 * for a semaphore - subsequent attempts result in error */
int newSemaphore(int semID, int semVal) {
  /* initialize semaphore only if it has not been initialied yet */
  if (semaphores[semID] == NULL) {
    SEM* semaphore = (SEM*) malloc(sizeof(SEM));
    semaphore->state = 1;
    semaphore->val = semVal;
    semaphore->plist = ListCreate();
    semaphores[semID] = semaphore;
    printf("Semaphores #%d is initialized with value %d\n", semID, semaphore->val);
  }
  return 0;
}

/* execute the semaphore P operation on behalf of the running
 * process. You can assume semaphores IDs numbered 0 through 4 */
int semaphoreP(int semID) {
  (semaphores[semID]->val)--;

  if (semaphores[semID]->val < 0) {
    /* add current process to plist of semaphore */
    for (int i = 0; i < 3; i++) {
      ListFirst(ready_queues[i]);
      PID* result = (PID*)ListSearch(ready_queues[i], pidIsEqual, currPID);
      if (result != NULL) {
        printf("Process #%u blocked on semaphore #%d\n", *currPID, semID);
        ListPrepend(semaphores[semID]->plist, ListRemove(ready_queues[i])); /* block itself */
        quantum();
        return 0;
      }
    }
  }
  return 0;
}

/* execute the semaphore V operation on behalf of the running
 * process. You can assume semaphore IDs numbered 0 through 4 */
int semaphoreV(int semID) {
  (semaphores[semID]->val)++;

  if (semaphores[semID]->val <= 0) {
    PID* procID = ListTrim(semaphores[semID]->plist);
    ListPrepend(ready_queues[PCBTable[*procID]->priority], (void*)procID);
    printf("Process #%u is unblocked by process #%u", *procID, *currPID);
  }

  return 0;
}

/* dump complete state information of process to screen, which
 * include process status and anything else you can think of */
void procinfo(PID pid) {
  PCB *result = PCBTable[pid];

  if (result != NULL) {
    printf("\nInformation of process #%u:\n", result->pid);
    printf("PID %u\n", result->pid);
    printf("Priority: %d\n\n", result->priority);
  } else {
    printf("Process does not exist!\n");
  }
}

/* display all process queues and their contents */
void totalInfo() {
  for (int i = 0; i < 3; i++) {
    printf("Contents of ready queue with priority %d:\n", i);
    printQueue(ready_queues[i]);
  }
  printf("Contents of queue of process waiting on a send:\n");
  printQueue(send_block_queue);
  printf("Contents of queue of process waiting on a receive:\n");
  printQueue(receive_block_queue);
}

/***********************************************************
*   Helper functions implementation
*/

PID allocateID() {
  return idAllocator++;
}

PCB* createPCB(int priority) {
  PCB* pcbPtr = (PCB*) malloc(sizeof(PCB));
  pcbPtr->pid = allocateID();
  pcbPtr->priority = priority;
  pcbPtr->print_proc_message = 0;
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
  ListNode* iter = list->tail;
  while(iter != NULL) {
    printf("%u ", *((PID*)iter->val));
    iter = iter->prev;
  }
  printf("\n");
}

void displayWelcomeInfo() {
  printf("\n");
  printf("----------------------------------------------------------------------\n");
  printf("|          Welcome to Interactive Operating System Simulation        |\n");
  printf("|            Type 'M' to know more about commands available          |\n");
  printf("----------------------------------------------------------------------\n\n");
}

void displayMenu() {
  printf("\n");
  printf("-----------------------------------------------------------------------\n");
  printf("|    ***********************************************                  |\n");
  printf("|             Menu for operations available                           |\n");
  printf("|             Command: Character -> Action                            |\n");
  printf("|    ***********************************************                  |\n");
  printf("|                                                                     |\n");
  printf("|        Create: 'C' -> Create a process                              |\n");
  printf("|          Fork: 'F' -> Fork a process                                |\n");
  printf("|          Kill: 'K' -> Kill a named process                          |\n");
  printf("|          Exit: 'E' -> Kill current process                          |\n");
  printf("|       Quantum: 'Q' -> Time quantum of running process expires       |\n");
  printf("|          Send: 'S' -> Send a message to another process             |\n");
  printf("|       Receive: 'R' -> Receive a message                             |\n");
  printf("|         Reply: 'Y' -> Unblock sender and delivers reply             |\n");
  printf("| New Semaphore: 'N' -> Initialize the named semaphore                |\n");
  printf("|   Semaphore P: 'P' -> Execute the semaphore P on current process    |\n");
  printf("|   Semaphore V: 'V' -> Execute the semaphore V on current process    |\n");
  printf("|      Procinfo: 'I' -> Dump information of process to screen         |\n");
  printf("|     Totalinfo: 'T' -> Display all process queues                    |\n");
  printf("|          Menu: 'M' -> Display the menu for all commands             |\n");
  printf("-----------------------------------------------------------------------\n\n");
}
