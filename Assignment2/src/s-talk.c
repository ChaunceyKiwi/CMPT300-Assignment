#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include "list.h"

#define MESSAGE_LENGTH 256
#define BUFFER_MAX_SIZE 30

int sendBufSize = 0;
int recvBufSize = 0;
int status = 1; // 0 for closed, 1 for running
int s;
struct sockaddr_in addr;
struct addrinfo hints, *res;
pthread_mutex_t sendMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t recvMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t sendBuffNotFull = PTHREAD_COND_INITIALIZER;
pthread_cond_t sendBuffNotEmpty = PTHREAD_COND_INITIALIZER;
pthread_cond_t recvBuffNotFull = PTHREAD_COND_INITIALIZER;
pthread_cond_t recvBuffNotEmpty = PTHREAD_COND_INITIALIZER;
LIST *sendList;
LIST *recvList;

void *sendMessage() {
  while (status) {
    pthread_mutex_lock(&sendMutex);
    while (sendBufSize == 0) {
      pthread_cond_wait(&sendBuffNotEmpty, &sendMutex);
    }
    char *str = ListTrim(sendList); 
    if (str[0] == '!') {
      status = 0;
    }
    sendBufSize--;
    sendto(s, str, MESSAGE_LENGTH, 0, res->ai_addr, res->ai_addrlen);

    pthread_cond_signal(&sendBuffNotFull);
    pthread_mutex_unlock(&sendMutex);
  }

  pthread_exit(NULL);
}

void *recvMessage() {
  struct sockaddr_storage their_addr;
  socklen_t addr_len = sizeof their_addr;

  while (status) {
    char buf[MESSAGE_LENGTH];
    int numbytes = recvfrom(s, &buf, MESSAGE_LENGTH, 0, (struct sockaddr *)&their_addr, &addr_len);
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

void *inputMessage() {
  while (status) {
    char message[MESSAGE_LENGTH];
    fgets(message, sizeof message, stdin);
    pthread_mutex_lock(&sendMutex);
    while (sendBufSize == BUFFER_MAX_SIZE) {
      pthread_cond_wait(&sendBuffNotFull, &sendMutex);
    }
    ListAppend(sendList, message);
    sendBufSize++;
    pthread_cond_signal(&sendBuffNotEmpty);
    pthread_mutex_unlock(&sendMutex);
    if (message[0] == '!') {
      status = 0;
    }
  }
  pthread_exit(NULL);
}

void *outputMessage() {
  while (status) {
    pthread_mutex_lock(&recvMutex);
    while (recvBufSize == 0) {
      pthread_cond_wait(&recvBuffNotEmpty, &recvMutex);
    }
    char* message = ListTrim(recvList);
    recvBufSize--;
    pthread_cond_signal(&recvBuffNotFull);
    pthread_mutex_unlock(&recvMutex);

    if (message[0] == '!') {
      status = 0;
    }
    printf("%s", message);
  }

  pthread_exit(NULL);
}

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

  sendList = ListCreate();
  recvList = ListCreate(); 

  pthread_t threads[4];

  /* thread1 get input from keyboard */
  pthread_create(&threads[0], NULL, inputMessage, NULL);

  /* thread2 reveive UDP message */
  pthread_create(&threads[1], NULL, recvMessage, NULL);

  /* thread3 print message to the screen */
  pthread_create(&threads[2], NULL, outputMessage, NULL);

  /* thread4 send UDP message*/
  pthread_create(&threads[3], NULL, sendMessage, NULL);

  for (int i = 0; i < 4; i ++) {
    pthread_join(threads[i], NULL);
  }

  return 0;
}
