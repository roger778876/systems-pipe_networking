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
  open("server_fifo", O_RDONLY, 0644);
  int cpd;
  read("server_fifo", &cpd, sizeof(int));
  
  // removing server's FIFO
  remove("server_fifo");
  
  // sending acknowledgement to client
  open(cpd, 0_WRONLY, 0644);
  

  return 0;
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
  int READ = 0;
  int WRITE = 1;
  int client_pipe[2];
  pipe(client_pipe);
  close(client_pipe[WRITE]);
  
  // connecting to server
  open("server_fifo", 0_WRONLY, 0644);
  
  // sending private pipe descriptor
  int cpd = client_pipe[WRITE];
  write("server_fifo", cpd, sizeof(int));
  
  

  return client_pipe[READ];
}
