#CA
openssl req -config openssl.conf -x509 -nodes -days 365 -newkey rsa:4096 -keyout ca.key -out ca.crt

#Server
openssl req -config openssl_server.conf -nodes -new -newkey rsa:4096 -out server.csr -keyout server.key

openssl x509 -req  -in server.csr -out server.crt -CA ca.crt -CAkey ca.key -CAcreateserial -CAserial ca.srl

#Client
openssl req -config openssl_client.conf -nodes -new -newkey rsa:4096 -out client.csr -keyout client.key

openssl x509 -req -in client.csr -out client.crt -CA ca.crt -CAkey ca.key
