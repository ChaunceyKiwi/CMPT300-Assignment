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

int s;
int sendBufSize;
int recvBufSize;
int status;
struct sockaddr_in addr;
struct addrinfo hints, *res;
pthread_mutex_t sendMutex;
pthread_mutex_t recvMutex;
pthread_cond_t sendBuffNotFull;
pthread_cond_t sendBuffNotEmpty;
pthread_cond_t recvBuffNotFull;
pthread_cond_t recvBuffNotEmpty;
LIST *sendList;
LIST *recvList;

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

  /* AF_INET: Address family for IPv4 */
  /* SOCK_DGRAM: Supports datagrams */
  s = socket(AF_INET, SOCK_DGRAM, 0);

  addr.sin_family = AF_INET;
  addr.sin_port = htons(my_port_number);
  addr.sin_addr.s_addr = INADDR_ANY; /* use the IP of the local machine */
  memset(&addr.sin_zero, '\0', 8);

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
  * Perform sending and receiving
  */

  status = 1; // 0 for closed, 1 for running
  sendBufSize = 0;
  recvBufSize = 0;
  pthread_mutex_init(&sendMutex, NULL);
  pthread_mutex_init(&recvMutex, NULL);
  pthread_cond_init (&sendBuffNotFull, NULL);
  pthread_cond_init (&sendBuffNotEmpty, NULL);
  pthread_cond_init (&recvBuffNotFull, NULL);
  pthread_cond_init (&recvBuffNotEmpty, NULL);

  sendList = ListCreate();
  recvList = ListCreate(); 

  pthread_t threads[4];

  /* thread1 gets input from keyboard */
  pthread_create(&threads[0], NULL, inputMsg, NULL);

  /* thread2 reveives UDP message */
  pthread_create(&threads[1], NULL, recvMsg, NULL);

  /* thread3 prints message to the screen */
  pthread_create(&threads[2], NULL, outputMsg, NULL);

  /* thread4 sends UDP message*/
  pthread_create(&threads[3], NULL, sendMsg, NULL);

  for (int i = 0; i < 4; i ++) {
    pthread_join(threads[i], NULL);
  }

  return 0;
}

void *sendMsg(UNUSED void* unused) {
  while (status) {
    pthread_mutex_lock(&sendMutex);
    while (sendBufSize == 0) {
      pthread_cond_wait(&sendBuffNotEmpty, &sendMutex);
    }
    char *msg = ListTrim(sendList); 
    if (msg[0] == '!') {
      status = 0;
    }
    sendBufSize--;
    printf("Local: %s", msg);
    sendto(s, msg, MSG_LEN, 0, res->ai_addr, res->ai_addrlen);

    pthread_cond_signal(&sendBuffNotFull);
    pthread_mutex_unlock(&sendMutex);
  }

  pthread_exit(NULL);
}

void *recvMsg(UNUSED void* unused) {
  struct sockaddr_storage their_addr;
  socklen_t addr_len = sizeof their_addr;

  while (status) {
    char buf[MSG_LEN];
    ssize_t numbytes = recvfrom(s, &buf, MSG_LEN, 0, (struct sockaddr *)&their_addr, &addr_len);
    buf[numbytes] = '\0';

    pthread_mutex_lock(&recvMutex);
    if (recvBufSize == BUFFER_MAX_SIZE) {
      pthread_cond_wait(&recvBuffNotFull, &recvMutex);
    }

    ListAppend(recvList, buf);
    recvBufSize++;

    pthread_cond_signal(&recvBuffNotEmpty);
    pthread_mutex_unlock(&recvMutex);

    if (buf[0] == '!') {
      status = 0;
    }
  }
  pthread_exit(NULL);
}

void *inputMsg(UNUSED void* unused) {
  while (status) {
    char msg[MSG_LEN];
    printf("Before fgets()\n");
    fgets(msg, sizeof msg, stdin);
    printf("After fgets()\n");
    pthread_mutex_lock(&sendMutex);
    while (sendBufSize == BUFFER_MAX_SIZE) {
      pthread_cond_wait(&sendBuffNotFull, &sendMutex);
    }
    ListAppend(sendList, msg);
    sendBufSize++;
    pthread_cond_signal(&sendBuffNotEmpty);
    pthread_mutex_unlock(&sendMutex);
    if (msg[0] == '!') {
      status = 0;
    }
  }
  pthread_exit(NULL);
}

void *outputMsg(UNUSED void* unused) {
  while (status) {
    pthread_mutex_lock(&recvMutex);
    while (recvBufSize == 0) {
      pthread_cond_wait(&recvBuffNotEmpty, &recvMutex);
    }
    char* msg = ListTrim(recvList);
    recvBufSize--;
    pthread_cond_signal(&recvBuffNotFull);
    pthread_mutex_unlock(&recvMutex);

    if (msg[0] == '!') {
      status = 0;
    }
    printf("Remote: %s", msg);
  }

  pthread_exit(NULL);
}
