#include <stdio.h>

/* create a process and put it on the appropriate ready Q */
int create(int priority) {
  return 0;
}

/* Copy current runnning process and put it on the ready Q
 * corresponding to the original process' priority. Attempting
 * to Fork the "init" process shoould fail */
int fork() {
  return 0;
}

/* kill the named process and remove it from the system */
int kill(int pid) {
  return 0;
}

/* kill currently running process */
void exit() {
}

/* time quantum of running process expires */
void quantum() {
}

/* send a message to another process - block until reply */
int send(int pid, char* msg) {
  return 0;
}

/* receive a message - block until one arrives */
void receive() {
}

/* unblocks sender and delivers reply */
int reply(int pid, char * msg) {
  return 0;
}

/* Initialize the named semaphore with the value given ID's
 * can take a value from 0 to 4. This can only be done once
 * for a semaphore - subsequent attempts result in error */
int newSemaphor(int semID, int initVal) {
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
void procinfo(int pid) {

}

/* display all process queues and their contents */
void totalInfo() {

}

int main(void)
{
  printf("Hello World!\n");
}
