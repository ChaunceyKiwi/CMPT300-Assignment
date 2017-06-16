#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

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
  * Set receive_socket
  */

  /* AF_INET: Address family for IPv4 */
  /* SOCK_DGRAM: Supports datagrams (connectionless, unreliable messages of a fixed maximum length) */
  int s = socket(AF_INET, SOCK_DGRAM, 0);

  struct sockaddr_in addr;
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
  * Set send_socket
  */
  int infoRes;
  struct addrinfo hints, *res;
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

  char buf[30];
  sendto(s, "Hello!", 30, 0, res->ai_addr, res->ai_addrlen);
  
  struct sockaddr_storage their_addr;
  socklen_t addr_len = sizeof their_addr;
  int numbytes = recvfrom(s, &buf, 30, 0, (struct sockaddr *)&their_addr, &addr_len);

  printf("listener: packet is %d bytes long\n", numbytes);
  buf[numbytes] = '\0';
  printf("listener: packet contains \"%s\"\n", buf);

  close(s);

  return 0;
}
