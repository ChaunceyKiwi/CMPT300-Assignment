#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
  if (argc != 4) {
    fprintf(stderr, "usage: s-talk [my port number] [remote machine name] [remote port number]\n");
    return 1;
  }

  int my_port_number = atoi(argv[1]);
  char* remote_machine_name = argv[2];
  int remote_port_number = atoi(argv[3]);

  /*********************************************************************
  * Set receive_socket
  */

  /* AF_INET: Address family for IPv4 */
  /* SOCK_DGRAM: Supports datagrams (connectionless, unreliable messages of a fixed maximum length) */
  int receive_socket = socket(AF_INET, SOCK_DGRAM, 0);

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(my_port_number);
  addr.sin_addr.s_addr = INADDR_ANY; /* use the IP of the local machine */
  memset(&addr.sin_zero, '\0', 8);

  if (bind(receive_socket, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
    close(receive_socket);
    fprintf(stderr, "listener: failed to bind socket\n");
    return 1;
  };

  struct hostent *remote;
  if ((remote = gethostbyname(remote_machine_name)) == NULL) {
    herror("gethostbyname");
    return 1;
  }

  /************************************************************************
  * Perform sending and receiving
  */

  socklen_t addr_len = sizeof(struct sockaddr_in);
  sendto(receive_socket, "Hello", 30, 0, (struct sockaddr *)(&(remote->h_addr)), addr_len);
  char* msg;
  recvfrom(receive_socket, msg, 30, 0, (struct sockaddr *)(&(remote->h_addr)), &addr_len);

  printf("Receive msg: %s\n", msg);

  return 0;
}
