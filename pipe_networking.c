#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define MESSAGE_BUFFER_SIZE 512
#define HANDSHAKE_BUFFER_SIZE 100

int server_handshake(int *dest){
  //1 Make FIFO
  //printf("1\n");
  printf("[SERVER] Making...\n");
  mkfifo("from", 0644);

  //2 waits for private fifo
  //printf("2\n");
  printf("[SERVER] Waiting...\n");
  *dest = open("from",O_RDONLY);

  //6 gets the private fifo, remove wkp
  //printf("6\n");
  char buffer[256];
  read(*dest,buffer,sizeof(buffer));
  printf("[SERVER] Received pipe: %s\n", buffer);
  remove("from");

  //7 connect to client fifo, send an init message
  //printf("7\n");
  int sendMessage = open(buffer,O_WRONLY);
  char initMsg[] = "Hello client!\n";
  write(sendMessage,initMsg,sizeof(initMsg));

  //10 receives init message
  //printf("10\n");
  read(*dest,buffer,sizeof(buffer));
  printf("[SERVER] Received: %s\n",buffer);

  //printf("[SERVER] Handshake Complete!\n");
  return sendMessage;    //sends pipeName to be written To (the processed message)
}

int client_handshake(int *dest){
  //3 creates private fifo
  //printf("3\n");
  char pipeName[100];
  sprintf(pipeName, "%d", getpid());
  printf("[CLIENT] Making private FIFO: %s\n", pipeName);
  mkfifo(pipeName, 0644);

  //4 connects to server and sends private fifo name
  //printf("4\n");
  *dest = open("from",O_WRONLY);
  printf("[CLIENT] Waiting...\n");
  write(*dest,pipeName,sizeof(pipeName));

  //5 waits for message
  //printf("5\n");
  int getMessage = open(pipeName, O_RDONLY);

  //8 gets the server init message, remove private fifo
  //printf("8\n");
  char buffer[256];
  read(getMessage,buffer,sizeof(buffer));
  printf("[CLIENT] Received pipe: %s\n",buffer);
  remove(pipeName);

  //9 send message to server
  //printf("9\n");
  char initMsg[] = "Hello server!\n";
  write(*dest, initMsg, sizeof(initMsg));

  //printf("[CLIENT] Handshake Complete!\n");
  return getMessage;    //sends pipeName to be read from (the processed message)
}

int server_handshake1 (char *buffer) {
	mkfifo ("from", 0644);
	//printf ("[SERVER] Making...\n");

  int from_client = open ("from", O_RDONLY);
	//printf ("[SERVER] Connecting...\n");

  read (from_client, buffer, MESSAGE_BUFFER_SIZE);
	printf ("[SERVER] Pipe received: %s\n",buffer);

  remove ("from");
	return from_client;
}

int server_handshake2 (char *buffer, int from_client) {
	int to_client = open(buffer, O_WRONLY);
  strncpy (buffer, "Hello client!", MESSAGE_BUFFER_SIZE);
	write(to_client,buffer,MESSAGE_BUFFER_SIZE);

  read(from_client,buffer,MESSAGE_BUFFER_SIZE);
	printf("[SERVER %d] Received: %s\n", getpid()-1, buffer);

  //printf("[SERVER] Handshake Complete!\n");
  return to_client;
}
