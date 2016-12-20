#include "pipe_networking.c"

int client_handshake(int *dest);
int server_handshake(int *dest);

int server_handshake1 (char *buffer);
int server_handshake2 (char *buffer, int from_client);
