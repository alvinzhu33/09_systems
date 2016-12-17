compile: server.c client.c
	gcc -o server server.c
	gcc -o client client.c

server: server.c
	gcc -o server server.c
	./server

client: client.c
	gcc -o client client.c
	./client

clean:
	rm server
	rm client
