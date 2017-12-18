#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  // creating server's FIFO
  mkfifo("server_fifo", 0644);
  
  // reading client message
  int sf = open("server_fifo", O_RDONLY, 0644);
  char message[BUFFER_SIZE];
  read(sf, message, BUFFER_SIZE);
  
  // removing server's FIFO
  remove("server_fifo");
  
  // sending acknowledgement to client
  *to_client = open(message, O_WRONLY, 0644);
  write(*to_client, message, BUFFER_SIZE);

  // receiving second client message
  read(sf, message, BUFFER_SIZE);

  printf("Server finished!\n");

  return sf;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  // creating client's private pipe
  char message[BUFFER_SIZE] = "clientpipe";
  mkfifo(message, 0644);
  
  // connecting to server
  *to_server = open("server_fifo", O_WRONLY, 0644);
  
  // sending private pipe descriptor
  write(*to_server, message, BUFFER_SIZE);

  // receiving server acknowledgement
  int cp = open(message, O_RDONLY, 0644);
  read(cp, message, BUFFER_SIZE);

  // removing client pipe
  remove(message);

  // sending acknowledgement message
  write(*to_server, message, BUFFER_SIZE);

  printf("Client finished!\n");

  return cp;
}
