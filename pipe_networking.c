#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
    mkfifo("Gandalf",0644);
    puts("Server establishing connection to client");
    int wkp = open("Gandalf",O_RDONLY);
    char pid[10];
    read(wkp,pid,HANDSHAKE_BUFFER_SIZE);
    puts("Server recieved private pipe name");
    printf("%s\n",pid);
    *to_client = open(pid,O_WRONLY);
    close(wkp);
    sprintf(pid,"%d",getpid());
    mkfifo(pid,0644);
    puts("Connection established");
    write(*to_client,pid,HANDSHAKE_BUFFER_SIZE);
    int upstream = open(pid,O_RDONLY);
    puts("Acknowledge having recieved the connection");
    return upstream;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
    char pid[10];
    sprintf(pid,"%d",getpid());
    mkfifo(pid,0644);
    int wrfd = open("Gandalf",O_WRONLY);
    puts("Client giving private pipe name to server");
    write(wrfd,pid,HANDSHAKE_BUFFER_SIZE);
    puts("Client establishing a connection with server");
    int fromfd = open(pid,O_RDONLY);
    puts("Connection established");
    char* ackwait;
    read(fromfd,ackwait,HANDSHAKE_BUFFER_SIZE);
    puts("Client recieved server private pipe");
    *to_server = open(ackwait,O_WRONLY);
    return fromfd;
}
