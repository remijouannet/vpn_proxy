FROM alpine:3.3
MAINTAINER Remi Jouannet "remijouannet@gmail.com"

RUN apk update && apk upgrade
RUN apk --no-cache add openssl-dev gcc make iputils libc-dev \
        linux-headers syslinux-dev bash tcpdump nmap-nping wget


# Install github-releases
RUN wget https://github.com/aktau/github-release/releases/download/v0.7.2/linux-amd64-github-release.tar.bz2
RUN tar xvf linux-amd64-github-release.tar.bz2
RUN mv bin/linux/amd64/github-release /bin/ && rm -rf bin/linux && rm linux-amd64-github-release.tar.bz2

COPY . /app
WORKDIR /app

RUN make all

EXPOSE 80:80
EXPOSE 443:443

ENTRYPOINT ["make"]
