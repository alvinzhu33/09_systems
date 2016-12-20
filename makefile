compile: fserver.c client.c
	gcc -o server fserver.c
	gcc -o client client.c

server: server.c
	gcc -o server server.c
	./server

fserver: fserver.c
	gcc -o server fserver.c
	./server

client: client.c
	gcc -o client client.c
	./client

clean:
	rm server
	rm client

run: compile
		./server
		./client
