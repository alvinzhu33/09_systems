#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>


//The pipe has the name of the other end in it

int server_handshake(int *dest){

  //1
  mkfifo("from", 0644);

  printf("Waiting for message\n");
  int getPrivate = open("from",O_RDONLY);

  //2 waits for private fifo
  printf("Waiting\n");
  char buffer[256];
  read(getPrivate,buffer,sizeof(buffer));
  
  //6 gets the private fifo, remove wkp
  remove("from");
  
  //7 connect to client fifo, send an init message
  int sendMessage = open(buffer,O_WRONLY);
  char *initMsg = "Hello there\n";
  write(sendMessage,initMsg,sizeof(initMsg));
  
  
  *dest = sendMessage;
  return getPrivate;
  
}

int client_handshake(int *dest){
  //3 private fifo
  char pipeName[100];
  sprintf(pipeName, "%d", getpid());  
  mkfifo(pipeName, 0644);

  //4 connects to server and sends private fifo name
  int sendPrivate = open("from",O_WRONLY);
  write(sendPrivate,pipeName,sizeof(pipeName));

  //5 waits for message
  int getStartMessage = open(pipeName, O_RDONLY);
  
  //8 gets the server init message, remove private fifo
  remove(pipeName);

  //9 send message to server
  char *toSend = "Hello back";
  write(sendPrivate,toSend,sizeof(toSend));
  
  //to_server is where the message is, for the server to read
  //WR_ONLY

  //from_server is the processed message, where the server wrote to
  //RD_ONLY
  //

  *dest = getStartMessage;
  return sendPrivate;
  
}
