# CMPT300 ASSIGNMENT3 Documentation

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
- Used to achieve higher time efficiency to get a PCB from PID
- Trade-off between increasing space efficiency and decreasing time efficiency

## Semaphore structure
- int val: the value of the semaphores
- LIST* plist: a list of processes waiting on this semaphore

## About 'init' process
- 'init' process runs only when no other processes are ready to execute
- 'init' process will never block, thus:
  - it will not be blocked when performing receive() and there is no message available
  - do nothing when performs semaphoreP since it will not block itself
- 'init' process cannot be killed or exited unless it is the last process in the system
- once init process is killed/exited, the simulation terminates

## Description of scheduling algorithm
- Round robin is applied to each priority ready queue, only when the ready queue with a higher priority is empty will the job in the lower priority queue be consider to enter running state.
- To prevent starvation, after each time quantum the running process will be put at the end of the ready queue which has a lower priority. For example, a process with high priority will be put at normal priority queue after its time quantum expires.

## Policy on IPC
- Send
  - After message is sent to named process, block sender until it get reply.
  - The source of reply can be from any process.
  - If the target process is already blocked on receiving, unblock it.

- Receive
  - If the message has already been sent, the receiving process displays the message immediately and continue executing.
  - If the message has not been sent yet, the receiving process will block itself. Once there is another process sends message to it, receiving process will be unblocked and be put at ready queue. The next time the process get running it will display the message.
  - Only one message can be sent to a process at a time, otherwise a second message would just
  overwrite the former one.

- Reply
  - All processes are able to reply, even if they has not been sent to.
  - The process running 'reply' will never block itself.
  - If the target is a process blocked on sending message, unblock it.
  - If the target is a process blocked on receiving message, unblock it.

## Policy on Semaphore
- We choose FIFO (first in, first out) to unblock the process. For example, when semaphoreV is performed and try to unblock a process that is blocked on that semaphore, it will choose the process which enters the process list of that semaphore firstly.
