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

  printf("Waiting...\n");
  int getPrivate = open("from",O_RDONLY);

  //2 waits for private fifo
  char buffer[256];
  read(getPrivate,buffer,sizeof(buffer));
  
  //6 gets the private fifo, remove wkp
  remove("from");
  
  //7 connect to client fifo, send an init message
  int sendMessage = open(buffer,O_WRONLY);
  
  char *initMsg = "Hello client!\n";
  write(sendMessage,initMsg,15);
  
  //printf("buffer: %s\n",buffer);
  
  *dest = getPrivate;    //sends "from" to be read from and processed
  return sendMessage;    //sends pipeName to be written To (the processed message)
  
}

int client_handshake(int *dest){
  //3 private fifo
  char pipeName[100];
  sprintf(pipeName, "%d", getpid());  
  mkfifo(pipeName, 0644);

  //4 connects to server and sends private fifo name
  
  int sendPrivate = open("from",O_WRONLY);
  
  //printf("pipename: %s\n",pipeName);
  
  write(sendPrivate,pipeName,sizeof(pipeName));

  //5 waits for message
  int getMessage = open(pipeName, O_RDONLY);
  
  //8 gets the server init message, remove private fifo
  char buffer[256];
  read(getMessage,buffer,sizeof(buffer));
  printf("Received: %s\n",buffer);
  remove(pipeName);


  //Why does this mess it up?!!
  //9 send message to server
  //char *toSend = "Hello server!\n";
  //write(sendPrivate,toSend,15);
  

  *dest = sendPrivate;        //sends "from" to be written to
  return getMessage;    //sends pipeName to be read from (the processed message)
  
}
