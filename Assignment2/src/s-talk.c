/*------------------------------------------------------
 *
 *  s-talk.c
 *
 *  This file contains the functions to implement
 *  a chatting program that enables someone at one
 *  terminal (or Xterm) to communicate with someone
 *  at another terminal.
 *
 *  Name         : Chauncey Liu
 *  Student ID   : 301295771
 *  SFU username : cla284
 *  Course       : CMPT 300 Operating Systems I, Summer 2017
 *  Instructor   : Harinder Khangura
 *  TA           : Amineh Dadsetan
 *
 *  Created by Chauncey on 2017-06-19.
 *  Copyright (c) 2017 Chauncey. All rights reserved.
 *
 *------------------------------------------------------
 */

#include "s-talk.h"

int s;                           /* file descriptor that refers to the endpoint for communication  */
struct addrinfo *res;            /* stores information about address of the remote process */
pthread_t threads[4];            /* the pthreads environment will run four threads */
LIST *sendList;                  /* buffer list to store the messages not sent yet */
LIST *recvList;                  /* buffer list to store the messages not printed yet */
pthread_mutex_t sendMutex;       /* mutex for modifying sendList */
pthread_mutex_t recvMutex;       /* mutex for modifying recvList */
pthread_cond_t sendBuffNotFull;  /* condition variable to imply sendList is not full*/
pthread_cond_t sendBuffNotEmpty; /* condition variable to imply sendList is not empty */
pthread_cond_t recvBuffNotFull;  /* condition variable to imply recvList is not full */
pthread_cond_t recvBuffNotEmpty; /* condition variable ti imply recvList is not empty */

int main(int argc, char *argv[])
{
  if (argc != 4) {
    fprintf(stderr, "usage: s-talk [my port number] [remote machine name] [remote port number]\n");
    exit(1);
  }

  int my_port_number = atoi(argv[1]);
  char* remote_machine_name = argv[2];
  char* remote_port_number = argv[3];

  /*********************************************************************
  * Set local socket
  */

  struct sockaddr_in addr; /* used to identify an Internet host and a service */
  struct addrinfo hints; /* specify criteria of socket addresses returned */

  /* AF_INET: Address family for IPv4 */
  /* SOCK_DGRAM: Supports datagrams */
  s = socket(AF_INET, SOCK_DGRAM, 0);

  /* Make recvfrom() non-blcking*/
  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;
  if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout) == -1)  {
    perror("setsockopt");
    exit(1);
  } 

  addr.sin_family = AF_INET;
  addr.sin_port = htons(my_port_number);
  addr.sin_addr.s_addr = INADDR_ANY; /* use the IP of the local machine */
  memset(&addr.sin_zero, '\0', 8);

  // Associate the socket created with the local machine
  if (bind(s, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
    close(s);
    fprintf(stderr, "listener: failed to bind socket\n");
    return 1;
  };

  /***********************************************************************
  * Get remote address
  */

  int infoRes;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  if ((infoRes = getaddrinfo(remote_machine_name, remote_port_number, &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(infoRes));
    return 1;
  }

  /***********************************************************************
  * Perform chatting task
  */

  /* initialization */
  sendList = ListCreate();
  recvList = ListCreate();
  pthread_mutex_init(&sendMutex, NULL);
  pthread_mutex_init(&recvMutex, NULL);
  pthread_cond_init (&sendBuffNotFull, NULL);
  pthread_cond_init (&sendBuffNotEmpty, NULL);
  pthread_cond_init (&recvBuffNotFull, NULL);
  pthread_cond_init (&recvBuffNotEmpty, NULL);

  /* thread1 is created to get input from keyboard */
  pthread_create(&threads[0], NULL, inputMsg, NULL);

  /* thread2 is created to receive message */
  pthread_create(&threads[1], NULL, recvMsg, NULL);

  /* thread3 is created to print message to the screen */
  pthread_create(&threads[2], NULL, outputMsg, NULL);

  /* thread4 is created to send message */
  pthread_create(&threads[3], NULL, sendMsg, NULL);

  /* wait until all threads are completed */
  for (int i = 0; i < 4; i ++) {
    pthread_join(threads[i], NULL);
  }

  /* free all objects */
  pthread_mutex_destroy(&sendMutex);
  pthread_mutex_destroy(&recvMutex);
  pthread_cond_destroy(&sendBuffNotFull);
  pthread_cond_destroy(&sendBuffNotEmpty);
  pthread_cond_destroy(&recvBuffNotFull);
  pthread_cond_destroy(&recvBuffNotEmpty);
  ListFree(sendList, freeItem);
  ListFree(recvList, freeItem);
  close(s);

  return 0;
}


/**
 * Thread1 gets input message from keyboard
 * adds the message to the sending buffer
 * @param unused the argument is not used,
 * macros UNUSED is used to suppress the warning
 * @return if returns, the effect shall be as if
 * there was an implicit call to pthread_exit()
 */
void* inputMsg(UNUSED void* unused) {
  char *msg = (char*) malloc(MSG_LEN * sizeof(char));
  fd_set read_fd_set;
  struct timeval timeout;
  int result;

  /* Non-blocking reading */
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;

  while (1) {
    char temp[MSG_LEN];

    /* Initialize fd_set to watch stdin */
    FD_ZERO(&read_fd_set);
    FD_SET(0, &read_fd_set);

    // check if keyboard input is available
    result = select(1, &read_fd_set, NULL, NULL, &timeout);

    usleep(WAITING_INTERVAL);

    if (result == -1) {
      perror("select()");
    } 

    else if (result) {
      fgets(temp, sizeof(temp), stdin);
      strcat(msg, temp);
      if (msg[strlen(msg)-1] == '\n') {
        pthread_mutex_lock(&sendMutex);

        /* wait until the sending buffer is not full */
        while (ListCount(sendList) == BUFFER_MAX_SIZE) {
          pthread_cond_wait(&sendBuffNotFull, &sendMutex);
        }

        ListPrepend(sendList, msg);

        /* resume suspended process if any */
        pthread_cond_signal(&sendBuffNotEmpty);

        pthread_mutex_unlock(&sendMutex);
        if (msg[0] == '!' && msg[1] == '\n') {
          break;
        } 
        
        msg = (char*) malloc(MSG_LEN * sizeof(char));
      }
    }

    // do nothing if keyboard input is not available 
    else {} 
  }

  pthread_exit(NULL);
}


/**
 * Thread2 receives message from the remote client
 * and adds message to the receiving buffer
 * @param unused the argument is not used,
 * macros UNUSED is used to suppress the warning
 * @return if returns, the effect shall be as if
 * there was an implicit call to pthread_exit()
 */

void* recvMsg(UNUSED void* unused) {
  struct sockaddr_storage their_addr;
  socklen_t addr_len = sizeof their_addr;
  char *msg = (char*) malloc(MSG_LEN * sizeof(char));
  fd_set read_fd_set;
  struct timeval timeout;
  int result;

  /* Non-blocking reading */
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;

  while (1) {
    char temp[MSG_LEN];

    /* Initialize fd_set to watch stdin */
    FD_ZERO(&read_fd_set);
    FD_SET(s, &read_fd_set);

    // check if message from remote process is available
    result = select(s+1, &read_fd_set, NULL, NULL, &timeout);
    usleep(WAITING_INTERVAL);

    if (result == -1) {
      perror("select()");
    } 

    else if (result) {
      ssize_t numbytes = recvfrom(s, temp, MSG_LEN, 0, (struct sockaddr *)&their_addr, &addr_len);
  
      /* keep accumulating messages until it end with new line */
      if (numbytes != -1) {
        strcat(msg, temp);
      }

      if (strlen(msg) > 0 && msg[strlen(msg)-1] == '\n') {
        pthread_mutex_lock(&recvMutex);

        /* wait until the receiving buffer is not full */
        if (ListCount(recvList) == BUFFER_MAX_SIZE) {
          pthread_cond_wait(&recvBuffNotFull, &recvMutex);
        }

        ListPrepend(recvList, msg);

        /* resume suspended process if any */
        pthread_cond_signal(&recvBuffNotEmpty);

        pthread_mutex_unlock(&recvMutex);

        if (msg[0] == '!' && msg[1] == '\n') {
          break;
        } 
        
        msg = (char*) malloc(MSG_LEN * sizeof(char));
      }
    } 
    // do nothing if no message is available 
    else {}
  }

  pthread_exit(NULL);
}


/**
 * Thread3 retrieves a message from receiving buffer
 * print the message to the screen
 * @param unused the argument is not used,
 * macros UNUSED is used to suppress the warning
 * @return if returns, the effect shall be as if
 * there was an implicit call to pthread_exit()
 */
void* outputMsg(UNUSED void* unused) {
  while (1) {
    pthread_mutex_lock(&recvMutex);

    /* wait until the receiving buffer is not empty */
    while (ListCount(recvList) == 0) {
      pthread_cond_wait(&recvBuffNotEmpty, &recvMutex);
    }

    char* msg = ListTrim(recvList);

    /* resume suspended process if any */
    pthread_cond_signal(&recvBuffNotFull);

    pthread_mutex_unlock(&recvMutex);

    if (msg[0] == '!' && msg[1] == '\n') {
      printf("Session is terminated by remote request\n");
      usleep(WAITING_INTERVAL);
      pthread_cancel(threads[0]);
      pthread_cancel(threads[3]);
      free(msg);
      break;
    }
    printf("Remote: %s", msg);
    free(msg);
  }

  pthread_exit(NULL);
}


/**
 * Thread4 retrieves a message from sending buffer
 * sends message to the remote client
 * @param unused the argument is not used,
 * macros UNUSED is used to suppress the warning
 * @return if returns, the effect shall be as if
 * there was an implicit call to pthread_exit()
 */
void* sendMsg(UNUSED void* unused) {
  while (1) {
    pthread_mutex_lock(&sendMutex);

    /* wait until the sending buffer is not empty */
    while (ListCount(sendList) == 0) {
      pthread_cond_wait(&sendBuffNotEmpty, &sendMutex);
    }

    char *msg = ListTrim(sendList);
    sendto(s, msg, MSG_LEN, 0, res->ai_addr, res->ai_addrlen);

    /* resume suspended process if any */
    pthread_cond_signal(&sendBuffNotFull);

    pthread_mutex_unlock(&sendMutex);

    if (msg[0] == '!' && msg[1] == '\n') {
      printf("Session is terminated by local request\n");
      usleep(WAITING_INTERVAL);
      pthread_cancel(threads[1]);
      pthread_cancel(threads[2]);
      free(msg);
      break;
    }

    free(msg);
  }

  pthread_exit(NULL);
}

void freeItem(void* item) {
  if (item != NULL) {
    free(item);
  }
}
