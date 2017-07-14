## Process control block (PCB) structure
- PID pid: process ID
- int priority: 0 as high, 1 as normal, 2 as low
- enum state proc_state: running, ready, blocked, exited
- char proc_message[MSG_LEN]: a string storing any messages that another process send or reply to this process
int print_proc_message;
- print_proc_message: an integer to imply if the proc_message should be displayed and cleared the next time the process run

## Queues
- 3 ready queues, one for each type of priority (0 as high, 1 as normal, 2 as low)
- a queue of process waiting on a send operation
- a queue of process waiting on a receive operation
- each queue is a list of process id PID

## PCB Table
- PCB table is an array of PCB pointer
- Used to achieve higher efficiency to get a PCB from PID
- Trade-off between increasing space complexity and decreasing time complexity

## Semaphore structure
- int val: the value of the semaphores
- LIST* plist: a list of processes waiting on this semaphore

## Description of scheduling algorithm
- Round robin is applied to each priority ready queue, only when ready queue with high priority is empty will the job in the lower priority queue be consider to enter running state
- To prevent starvation, after each time quantum the running process will be put at the end of the ready queue which has a lower priority. For example, a process with high priority will be put at normal priority queue after its time quantum expires.

## Policy on IPC
- Only one message can be sent to a process at a time, otherwise a second message would just
overwrite the former one.
