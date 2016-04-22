#CFLAGS=-I/usr/include/openssl -lssl -lcrypto -D DEBUG
CFLAGS=-I/usr/include/openssl -lssl -lcrypto

all: src/*.c
	cc ${CFLAGS} -Wall -Werror $^ -o main 
run:
	./main
run_client:
	./test/test_client.sh ${IPSERVER} ${IPPROXY} 8859
run_client2:
	./test/test_client2.sh ${IPSERVER} ${IPPROXY} 8859
run_server:
	./test/test_server.sh 172.17.0.0
clean:
	rm -f main
	rm -f src/*.o
scp:
	scp -P 80 -r src/ debian@vm1.remijouannet.com:/home/debian/git/vpntohttp/
