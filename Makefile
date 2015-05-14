CC=gcc

all: server client

server: server.o
	$(CC) server.o -o server
client: client.o 
	$(CC) client.o -o client
server.o: main.c
	$(CC) main.c -c -D SERVER -o server.o
client.o: main.c
	$(CC) main.c -c	-D CLIENT -o client.o
clean:
	rm -rf *.o server client
