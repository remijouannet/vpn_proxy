CFLAGSD=-I/usr/include/ -lssl -lcrypto -D DEBUG
CFLAGS=-I/usr/include/ -lssl -lcrypto
VERSION=$$(git describe --abbrev=0 --tags)
PWD=$$(pwd)

pkg: src/*.c
	cc -Wall -Werror $^ ${CFLAGS} -o pkg/vpn_proxy
release:
	scripts/github-releases.sh
docker-image:
	docker build -t vpn_proxy:$(VERSION) .
docker-build:
	docker run \
		-v $(PWD)/pkg:/app/pkg \
		vpn_proxy:$(VERSION) pkg
docker-release:
	docker run \
		-v $(PWD)/pkg:/app/pkg \
		-e "GITHUB_TOKEN=$(GITHUB_TOKEN)" \
		vpn_proxy:$(VERSION) release 
all: src/*.c
	cc -Wall -Werror $^ ${CFLAGSD} -o main 
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
	rm -f pkg/*
	rm -f src/*.o

.PHONY: pkg docker-image docker-build docker-release
