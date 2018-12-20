#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
    puts("Server establishing connection to client");
    int wkp = open("Gandalf",O_RDONLY);
    char* pipename;
    read(wkp,pipename,HANDSHAKE_BUFFER_SIZE);
    puts("Server recieved private pipe name");
    close(wkp);
    int upstream = open(pipename,O_WRONLY);
    char pid[10];
    sprintf(pid,"%d",getpid());
    mkfifo(pid,0644);
    puts("Connection established");
    char* ack = pid;
    open(pid,O_RDONLY);
    puts("Acknowledging having recieved the connection");
    write(upstream,ACK,HANDSHAKE_BUFFER_SIZE);
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
    puts("Connection established");
    char* ackbuff = "ipepay";
    puts("Client giving private pipe name to server");
    write(wrfd,ackbuff,HANDSHAKE_BUFFER_SIZE);
    puts("Client establishing a connection with server");
    int fromfd = open(pid,O_RDONLY);
    char* ackwait;
    read(fromfd,ackwait,HANDSHAKE_BUFFER_SIZE);
    *to_server = open(ackwait,O_WRONLY);
    return fromfd;
}
