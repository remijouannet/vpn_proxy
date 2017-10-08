FROM ubuntu:16.04
MAINTAINER Remi Jouannet "remijouannet@gmail.com"

# Install Deps
RUn apt-get update
RUN apt-get install -y bzip2 expect git wget build-essential libssl-dev tcpdump nmap iputils-ping bash

# Clean
RUN apt-get clean && rm -fr /var/lib/apt/lists/* /tmp/* /var/tmp/*

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
