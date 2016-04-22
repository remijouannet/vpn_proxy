FROM alpine:3.3
MAINTAINER Rémi Jouannet "remijouannet@gmail.com"


RUN apk update
RUN apk upgrade
RUN apk add openssl-dev gcc make iputils 
RUN apk add libc-dev linux-headers syslinux-dev bash
RUN apk add tcpdump nmap-nping

COPY . /app
WORKDIR /app

RUN make all

EXPOSE 8080:8080
EXPOSE 80:80
EXPOSE 443:443

ENTRYPOINT ["make"]
