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

LIST* ready_queues[3];          /* ready queue with three levels of priority */
LIST* send_block_queue;         /* queue of processes wating on a send operation */
LIST* receive_block_queue;      /* queue of processes wating on a receive opreation */
PID idAllocator = 0;            /* imply the next available id for a new process */
PID* currPID = NULL;            /* the PID of current running process*/
PCB* PCBTable[MAX_NUM_PROC];    /* PCBTable to look up PCB from PID */
SEM* semaphores[5];             /* Semaphore to perform synchronization */
int processCount = 0;           /* the amount of process that is currentlt running */

int main(void)
{
  displayWelcomeInfo();

  /* Initialization */
  ready_queues[0] = ListCreate();        /* queue with high priority */
  ready_queues[1] = ListCreate();        /* queue with normal priority */
  ready_queues[2] = ListCreate();        /* queue with low priority */
  send_block_queue = ListCreate();       /* queue to put process being blocked by sending message*/
  receive_block_queue = ListCreate();    /* queue to put process being blocked by receiving message*/
  currPID = &PCBTable[create(3)]->pid;   /* create and run the 'init' process */

  /* initialize all semaphore pointers */
  for (int i = 0; i < 5; i++) {
    semaphores[i] = NULL;
  }

  int semID, semVal, priority;
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
        getchar();
        printf("message: ");
        fgets(msg, MSG_LEN, stdin);
        send(pid, msg);
        break;
      case 'R': case 'r':
        receive();
        break;
      case 'Y': case 'y':
        msg = (char*) malloc(MSG_LEN * sizeof(char));
        printf("pid: ");
        scanf("%u", &pid);
        getchar();
        printf("message: ");
        fgets(msg, MSG_LEN, stdin);
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
        freeMemory();
        return 0;
      default:
        printf("Wrong command!\n");
        break;
    }
  }
}

/* Create a process and put it on the appropriate ready queue */
int create(int priority) {
  PCB* pcbPtr = createPCB(priority);
  PCBTable[pcbPtr->pid] = pcbPtr;

  /* create 'init' process */
  if (priority == 3) {
    processCount++;
    return pcbPtr->pid;
  }

  /* create normal process */
  else {
    ListPrepend(ready_queues[priority], (void*)(&pcbPtr->pid));
    processCount++;
    printf("----------------------------------------------\n");
    printf("Process #%u is successfully created\n", pcbPtr->pid);
    printf("----------------------------------------------\n\n");
    return pcbPtr->pid;
  }
}

/* Copy current runnning process and put it on the ready Q
 * corresponding to the original process' priority. Attempting
 * to Fork the "init" process should fail */
int fork() {
  if (*currPID == 0) {
    printf("----------------------------------------------\n");
    printf("Failure: 'init' process cannot be forked!\n");
    printf("----------------------------------------------\n\n");
    return 1;
  }
  PCB* newProc = copyPCB(*currPID);
  PCBTable[newProc->pid] = newProc;
  ListPrepend(ready_queues[newProc->priority], (void*)(&newProc->pid));
  processCount++;
  printf("--------------------------------------------------------\n");
  printf("Process #%u is successfully forked, resulting process #%u\n",
   *currPID, newProc->pid);
  printf("--------------------------------------------------------\n\n");
  return 0;
}

/* Kill the named process and remove it from the system */
int killProc(PID pid) {
  if (pid == 0) {
    if (processCount == 1) {
      printf("--------------------------------------------------------\n");
      printf("Target process is successfully killed\n");
      printf("All processes are gone from system, simulation terminates.\n");
      printf("--------------------------------------------------------\n\n");
      PCBTable[pid]->proc_state = EXITED;
      freeMemory();
      exit(0);
    } else {
      printf("----------------------------------------------------------------------------\n");
      printf("Failure: 'init' process can only be killed if there are no other processes!\n");
      printf("----------------------------------------------------------------------------\n\n");
      return 1;
    }
  }

  if (PCBTable[pid]->proc_state == READY) {
    PCB *result;
    for (int i = 0; i < 3; i++) {
      ListFirst(ready_queues[i]);
      result = (PCB*)ListSearch(ready_queues[i], pidIsEqual, &pid);
      if (result != NULL) {
        ListRemove(ready_queues[i]);
        processCount--;
        PCBTable[pid]->proc_state = EXITED;
        printf("--------------------------------------\n");
        printf("Process #%d is successfully killed\n", pid);
        printf("--------------------------------------\n\n");
        return 0;
      }
    }
  } else if (PCBTable[pid]->proc_state == BLOCKED) {
    killBlockedProcess(pid);
  } else if (PCBTable[pid]->proc_state == RUNNING) {
    exitProc();
  }

  return 1;
}

/* Kill currently running process */
int exitProc() {
  if (*currPID == 0) {
    if (processCount == 1) {
      printf("----------------------------------------------------------\n");
      printf("Target process is successfully killed\n");
      printf("All processes are gone from system, simulation terminates.\n");
      printf("----------------------------------------------------------\n\n");
      freeMemory();
      exit(0);
    } else {
      printf("---------------------------------------------------------------------------\n");
      printf("Failure: 'init' process can only be killed if there are no other processes!\n");
      printf("---------------------------------------------------------------------------\n");
      return 1;
    }
  }

  PCBTable[*currPID]->proc_state = EXITED;
  printf("--------------------------------------------\n");
  printf("Time quantum of process #%u expires\n", *currPID);
  currPID = NULL;
  processCount--;
  printf("Current process is successfully exited\n");
  quantum();
  return 0;
}

/* Time quantum of running process expires */
void quantum() {
  if (currPID != NULL) {
    printf("--------------------------------------------\n");
    /* skip 'init' process since it is not on any queue */
    if (*currPID != 0) {
      PCB* currProc = PCBTable[*currPID];
      demotePriority(*currPID);
      ListPrepend(ready_queues[currProc->priority], (void*)currPID);
    }
    PCBTable[*currPID]->proc_state = READY;
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
  currProc->proc_state = RUNNING;
  if (currProc->print_proc_message == 1 && strlen(currProc->proc_message) != 0) {
    printf("Receive message: %s", currProc->proc_message);
    strcpy(currProc->proc_message, "");
    currProc->print_proc_message = 0;
  }
  printf("--------------------------------------------\n\n");
}

/* Send a message to another process - block until reply */
int send(PID pid, char* msg) {
  /* send message and block sender */
  strcpy(PCBTable[pid]->proc_message, msg);
  printf("--------------------------------------------\n");
  printf("Message sent to the process #%u\n", pid);

  /* unblock receiver if the receiver is blocked on receiving */
  ListFirst(receive_block_queue);
  PID* result = (PID*)ListSearch(receive_block_queue, pidIsEqual, &pid);
  if (result != NULL) {
    PCB* targetPCB = PCBTable[pid];
    ListPrepend(ready_queues[targetPCB->priority], ListRemove(receive_block_queue)); /* unblock receiver */
    printf("Process #%u is unblocked by process #%u\n", pid, *currPID);
    targetPCB->print_proc_message = 1;
  }

  /* block sender only if it is not 'init' process */
  if (*currPID != 0) {
    ListPrepend(send_block_queue, (void*)currPID);
    PCBTable[*currPID]->proc_state = BLOCKED;
    printf("Process #%u blocked on sending message\n", *currPID);
    printf("Time quantum of process #%u expires\n", *currPID);
    currPID = NULL;
    quantum();
  } else {
    printf("--------------------------------------------\n\n");
  }

  return 0;
}

/* Receive a message - block until one arrives */
void receive() {
  PCB* currProc = PCBTable[*currPID];
  if (strlen(currProc->proc_message) != 0) {
    printf("--------------------------------------------\n");
    printf("Receive message: %s", currProc->proc_message);
    printf("--------------------------------------------\n\n");
    strcpy(currProc->proc_message, "");  /* empty string */
  } else if (*currPID != 0) {
    ListPrepend(receive_block_queue, (void*)currPID); /* block itself */
    PCBTable[*currPID]->proc_state = BLOCKED;
    printf("--------------------------------------------\n");
    printf("Process #%u blocked on receiving message\n", *currPID);
    printf("Time quantum of process #%u expires\n", *currPID);
    currPID = NULL;
    quantum();
  }
}

/* Unblocks sender and delivers reply */
int reply(PID pid, char* msg) {
  PID* result;
  PCB* targetPCB = PCBTable[pid];

  /* unblock sender if the sender is blocked on sending  */
  ListFirst(send_block_queue);
  result = (PID*)ListSearch(send_block_queue, pidIsEqual, &pid);
  if (result != NULL) {
    strcpy(targetPCB->proc_message, msg);
    ListPrepend(ready_queues[targetPCB->priority], ListRemove(send_block_queue));
    targetPCB->print_proc_message = 1;
    printf("--------------------------------------------\n");
    printf("Message replied to the process #%u\n", pid);
    printf("Process #%u is unblocked by process #%u\n", pid, *currPID);
    printf("--------------------------------------------\n\n");
    return 0;
  }

  /* unblock receiver if the receiver is blocked on receiving */
  ListFirst(receive_block_queue);
  result = (PID*)ListSearch(receive_block_queue, pidIsEqual, &pid);
  if (result != NULL) {
    strcpy(targetPCB->proc_message, msg);
    ListPrepend(ready_queues[targetPCB->priority], ListRemove(receive_block_queue));
    targetPCB->print_proc_message = 1;
    printf("--------------------------------------------\n");
    printf("Message replied to the process #%u\n", pid);
    printf("Process #%u is unblocked by process #%u\n", pid, *currPID);
    printf("--------------------------------------------\n\n");
    return 0;
  }

  return 0;
}

/* Initialize the named semaphore with the value given ID's
 * can take a value from 0 to 4. This can only be done once
 * for a semaphore - subsequent attempts result in error */
int newSemaphore(int semID, int semVal) {
  /* initialize semaphore only if it has not been initialied yet */
  if (semaphores[semID] == NULL) {
    SEM* semaphore = (SEM*) malloc(sizeof(SEM));
    semaphore->val = semVal;
    semaphore->plist = ListCreate();
    semaphores[semID] = semaphore;
    printf("--------------------------------------------\n");
    printf("Semaphores #%d is initialized with value %d\n", semID, semaphore->val);
    printf("--------------------------------------------\n\n");
  } else {
    printf("--------------------------------------------\n");
    printf("Failure: Semaphores #%d has already been initialized!\n ", semID);
    printf("--------------------------------------------\n\n");
  }

  return 0;
}

/* Execute the semaphore P operation on behalf of the running
 * process. You can assume semaphores IDs numbered 0 through 4 */
int semaphoreP(int semID) {
  /* since 'init' will never be blocked, treat it as nothing happen */
  if (*currPID != 0) {
    (semaphores[semID]->val)--;
    if (semaphores[semID]->val < 0) {
      /* add current process to plist of semaphore */
      ListPrepend(semaphores[semID]->plist, (void*)currPID); /* block itself */
      PCBTable[*currPID]->proc_state = BLOCKED;
      printf("--------------------------------------------\n");
      printf("Process #%u blocked on semaphore #%d\n", *currPID, semID);
      printf("Time quantum of process #%u expires\n", *currPID);
      currPID = NULL;
      quantum();
    }
  }
  return 0;
}

/* Execute the semaphore V operation on behalf of the running
 * process. You can assume semaphore IDs numbered 0 through 4 */
int semaphoreV(int semID) {
  (semaphores[semID]->val)++;
  if (semaphores[semID]->val <= 0) {
    PID* procID = ListTrim(semaphores[semID]->plist);
    ListPrepend(ready_queues[PCBTable[*procID]->priority], (void*)procID);
    PCBTable[*procID]->proc_state = READY;
    printf("--------------------------------------------\n");
    printf("Process #%u is unblocked by process #%u\n", *procID, *currPID);
    printf("--------------------------------------------\n\n");
  }
  return 0;
}

/* Dump complete state information of process to screen, which
 * include process status and anything else you can think of */
void procinfo(PID pid) {
  PCB *result = PCBTable[pid];
  if (result != NULL) {
    printf("--------------------------------------------\n");
    printf("Information of process #%u:\n", result->pid);
    printf("PID: %u\n", result->pid);
    printf("Priority: %d\n", result->priority);
    if (result->proc_state == RUNNING) {
      printf("State: runnning\n");
    } else if (result->proc_state == READY) {
      printf("State: ready\n");
    } else if (result->proc_state == BLOCKED) {
      printf("State: blocked\n");
    } else {
      printf("State: exited\n");
    }
  } else {
    printf("Process does not exist!\n");
  }
  printf("--------------------------------------------\n\n");
}

/* Display all process queues and their contents */
void totalInfo() {
  printf("--------------------------------------------------\n");
  for (int i = 0; i < 3; i++) {
    printf("Contents of ready queue with priority %d:\n", i);
    printQueue(ready_queues[i]);
  }
  printf("Contents of queue of process waiting on a send:\n");
  printQueue(send_block_queue);
  printf("Contents of queue of process waiting on a receive:\n");
  printQueue(receive_block_queue);

  for (int i = 0; i < 5; i++) {
    if (semaphores[i] != NULL) {
      printf("Semaphore #%d with value %d\n", i, semaphores[i]->val);
      printf("The processes wating on this semaphore: \n");
      printQueue(semaphores[i]->plist);
    }
  }
  printf("--------------------------------------------------\n\n");
}

/***********************************************************
*   Helper functions implementation
*/

/* Demote priority to perform round robin with priority */
void demotePriority(PID pid) {
  if (PCBTable[pid]->priority == 0 || PCBTable[pid]->priority == 1) {
    (PCBTable[pid]->priority)++;
  }
}

/* Allocate PID by incrementing a unsigned integer */
PID allocateID() {
  return idAllocator++;
}

PCB* createPCB(int priority) {
  PCB* pcbPtr = (PCB*) malloc(sizeof(PCB));
  pcbPtr->pid = allocateID();
  pcbPtr->priority = priority;
  pcbPtr->proc_state = RUNNING;
  strcpy(pcbPtr->proc_message, "");
  pcbPtr->print_proc_message = 0;
  return pcbPtr;
}

PCB* copyPCB(PID originID) {
  PCB* originProc = PCBTable[originID];
  PCB* pcbPtr = (PCB*) malloc(sizeof(PCB));
  pcbPtr->pid = allocateID();
  pcbPtr->priority = originProc->priority;
  pcbPtr->proc_state = originProc->proc_state;
  strcpy(pcbPtr->proc_message, originProc->proc_message);
  pcbPtr->print_proc_message = originProc->print_proc_message;
  return pcbPtr;
}

int pidIsEqual(void* item, void* comparisonArg) {
  return *((PID*)item) == *((PID*)comparisonArg);
}

void killBlockedProcess(PID pid) {
  PID* result;
  PCBTable[pid]->proc_state = EXITED;

  /* Check processes that are blocked on sending, kill it if found */
  ListFirst(send_block_queue);
  result = (PID*)ListSearch(send_block_queue, pidIsEqual, &pid);
  if (result != NULL) {
    ListRemove(send_block_queue);
    processCount--;
    printf("Process #%d is successfully killed\n", pid);
    return;
  }

  /* Check processes that are blocked on receiving, kill it if found */
  ListFirst(receive_block_queue);
  result = (PID*)ListSearch(receive_block_queue, pidIsEqual, &pid);
  if (result != NULL) {
    ListRemove(receive_block_queue);
    processCount--;
    printf("Process #%d is successfully killed\n", pid);
    return;
  }

  /* Check processes that are blocked on semaphore, kill it if found */
  for (int i = 0; i < 5; i++) {
    if (semaphores[i] != NULL) {
      ListFirst(semaphores[i]->plist);
      result = (PID*)ListSearch(semaphores[i]->plist, pidIsEqual, &pid);
      if (result != NULL) {
        ListRemove(semaphores[i]->plist);
        processCount--;
        printf("Process #%d is successfully killed\n", pid);
        return;
      }
    }
  }
}

void printQueue(LIST* list) {
  ListNode* iter = list->tail;
  while(iter != NULL) {
    printf("%u ", *((PID*)iter->val));
    iter = iter->prev;
  }
  printf("\n");
}

void freeItem(void* item) {
  if (item != NULL) {
    free(item);
  }
}

void freeMemory() {
  ListFree(ready_queues[0], freeItem);
  ListFree(ready_queues[1], freeItem);
  ListFree(ready_queues[2], freeItem);
  ListFree(send_block_queue, freeItem);
  ListFree(receive_block_queue, freeItem);

  for (int i = 0; i < 5; i++) {
    if (semaphores[i] != NULL) {
      ListFree(semaphores[i]->plist, freeItem);
    }
  }
  printf("All lists allocated are freed. Ready to exit.\n");
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
