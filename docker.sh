#!/bin/bash

APP0="vpntohttp"
APP1="vpntohttp_server"
APP2="vpntohttp_client"
APP3="proxy"
APP4="vpntohttp_client2"

build_() {
    docker build -t $APP0 .
}

run_server() {
    docker stop $(docker ps -a | grep $APP1 | cut -d' ' -f1) > /dev/null 2>&1;
    docker rm $(docker ps -a | grep $APP1 | cut -d' ' -f1) > /dev/null 2>&1;

    docker stop $(docker ps -a | grep $APP3 | cut -d' ' -f1) > /dev/null 2>&1;
    docker rm $(docker ps -a | grep $APP3 | cut -d' ' -f1) > /dev/null 2>&1;

    docker run \
        --name $APP3 \
        -p 127.0.0.1:8859:8859 \
        -d remijouannet/docker-tinyproxy;

    docker run \
        --cap-add=NET_ADMIN \
        --device='/dev/net/tun' \
        --name $APP1 \
        -p 127.0.0.1:443:443 \
        -i -t $APP0 run_server;
}

run_client() {
    docker stop $(docker ps -a | grep $APP2 | cut -d' ' -f1) > /dev/null 2>&1;
    docker rm $(docker ps -a | grep $APP2 | cut -d' ' -f1) > /dev/null 2>&1;

    ipserver=$(docker inspect \
        --format '{{ .NetworkSettings.IPAddress }}' $APP1)
    ipproxy=$(docker inspect \
        --format '{{ .NetworkSettings.IPAddress }}' $APP3)

    docker run \
        --cap-add=NET_ADMIN \
        --device='/dev/net/tun' \
        --name $APP2 \
        -e IPSERVER=$ipserver \
        -e IPPROXY=$ipproxy \
        -i -t $APP0 run_client;

    docker stop $(docker ps -a | grep $APP3 | cut -d' ' -f1) > /dev/null 2>&1;
    docker rm $(docker ps -a | grep $APP3 | cut -d' ' -f1) > /dev/null 2>&1;
}

run_client2() {
    docker stop $(docker ps -a | grep $APP4 | cut -d' ' -f1) > /dev/null 2>&1;
    docker rm $(docker ps -a | grep $APP4 | cut -d' ' -f1) > /dev/null 2>&1;

    ipserver=$(docker inspect \
        --format '{{ .NetworkSettings.IPAddress }}' $APP1)
    ipproxy=$(docker inspect \
        --format '{{ .NetworkSettings.IPAddress }}' $APP3)

    docker run \
        --cap-add=NET_ADMIN \
        --device='/dev/net/tun' \
        --name $APP4 \
        -e IPSERVER=$ipserver \
        -e IPPROXY=$ipproxy \
        -i -t $APP0 run_client2;

    docker stop $(docker ps -a | grep $APP3 | cut -d' ' -f1) > /dev/null 2>&1;
    docker rm $(docker ps -a | grep $APP3 | cut -d' ' -f1) > /dev/null 2>&1;
}

stop_() {
    docker stop $APP1
    docker stop $APP2
    docker stop $APP3
    docker stop $APP4
}

help="build : build or rebuild the DockerFile\n
run_s : the container after building\n
run_c : the container after building\n
run_c2 : the container after building\n
stop : stop the container\n"
case $1 in
    "build" )
        build_ ;;
    "run_s" )
        run_server ;;
    "run_c" )
        run_client ;;
    "run_c2" )
        run_client2 ;;
    "restart" )
        restart_ ;;
    "stop" )
        stop_ ;;
    *)
        echo -e $help ;;
esac
