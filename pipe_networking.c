#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int server_handshake(int *dest){

  mkfifo("from", 0644);

  int fd = open("from", O_RDONLY);
  int fd2 = open("to", O_WRONLY);
  dest = fd;

  return fd2;
}

int client_handshake(int *dest){
  char *pipeName = "to";
  //sprintf(pipeName, "%d", getpid());
  mkfifo(pipeName, 0644);
  
  int fd = open(pipeName, O_WRONLY);
  int fd2 = open("from",O_RDONLY);
  dest = fd;

  return fd2;
}
