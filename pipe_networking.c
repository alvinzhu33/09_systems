#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int client_handshake(int *dest){
  char pipeName[10];
  sprintf(pipeName, "%d", getpid());
  mkfifo(pipeName, 0644);
  
  int fd = open(pipeName, O_RDONLY);

  close(fd);
  return fd;
}

int server_handshake(int *dest){
  char pipeName[10];
  sprintf(pipeName, "%d", getpid());
  mkfifo(pipeName, 0644);

  int fd = open(pipeName, O_WRONLY);
  
  close(fd);
  return fd;
}
