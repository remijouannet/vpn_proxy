CFLAGS=-lssl -lcrypto -D DEBUG -g

all:
	cc ${CFLAGS} -Wall -Werror ssl_server.c -o ssl_server  
	cc ${CFLAGS} -Wall -Werror ssl_client.c -o ssl_client
clean:
	rm -f ssl_server
	rm -f ssl_client
	rm -f src/*.o
