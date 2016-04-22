# vpn_proxy

* build it and generate certificate/key

```
keys/gen.sh

docker pull remijouannet/docker-tinyproxy 

./docker.sh build
```

* in one terminal

```
./docker.sh run_s
```

* in another terminal

```
./docker.sh run_c

```

* in another terminal if you want a second client

```
./docker.sh run_c2
```


