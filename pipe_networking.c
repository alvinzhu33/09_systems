#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define MESSAGE_BUFFER_SIZE 512
#define HANDSHAKE_BUFFER_SIZE 100

//The pipe has the name of the other end in it

int server_handshake(int *dest){
  //1 Make FIFO
  //printf("1\n");
  mkfifo("from", 0644);

  //2 waits for private fifo
  //printf("2\n");
  printf("Waiting...\n");
  *dest = open("from",O_RDONLY);

  //6 gets the private fifo, remove wkp
  //printf("6\n");
  char buffer[256];
  read(*dest,buffer,sizeof(buffer));
  remove("from");

  //7 connect to client fifo, send an init message
  //printf("7\n");
  int sendMessage = open(buffer,O_WRONLY);
  char initMsg[] = "Hello client!\n";
  write(sendMessage,initMsg,sizeof(initMsg));
  //printf("buffer: %s\n",buffer);

  //10 receives init message
  //printf("10\n");
  read(*dest,buffer,sizeof(buffer));
  printf("Received: %s\n",buffer);

  //*dest = getPrivate;    //sends "from" to be read from and processed
  printf("END server_handshake\n");
  return sendMessage;    //sends pipeName to be written To (the processed message)
}

int client_handshake(int *dest){
  //3 creates private fifo
  //printf("3\n");
  char pipeName[100];
  sprintf(pipeName, "%d", getpid());
  mkfifo(pipeName, 0644);

  //4 connects to server and sends private fifo name
  //printf("4\n");
  *dest = open("from",O_WRONLY);
  //printf("pipename: %s\n",pipeName);
  write(*dest,pipeName,sizeof(pipeName));

  //5 waits for message
  //printf("5\n");
  int getMessage = open(pipeName, O_RDONLY);

  //8 gets the server init message, remove private fifo
  //printf("8\n");
  char buffer[256];
  read(getMessage,buffer,sizeof(buffer));
  printf("Received: %s\n",buffer);
  remove(pipeName);

  //9 send message to server
  //printf("9\n");
  //int sendMessage = open(pipeName,O_WRONLY);
  char initMsg[] = "Hello server!\n";
  write(*dest, initMsg, sizeof(initMsg));

  //*dest = sendPrivate;        //sends "from" to be written to
  printf("END client_handshake\n");
  return getMessage;    //sends pipeName to be read from (the processed message)
}
