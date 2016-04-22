#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>

#include "vpn_opt.h"
#include "socket.h"
#include "http.h"
#include "error.h"

int create_socket(options_main* opt) {
    if(opt->MODE == 1)
        return socket_server(opt);
    else if(opt->MODE == 0)
        return socket_client(opt);
    else
        vpn_error_exit("no mode", vpn_return(1));

    return 0;
}

int socket_client(options_main* opt) {
    printf("socket_client\n");
    
    int sockfd;
    int tun = 0;
    struct sockaddr_in serv_addr;

    memset(&serv_addr, '\0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;

    if ( opt->proxy == 1 ){
        inet_pton(AF_INET, opt->ip_proxy, &serv_addr.sin_addr);
        serv_addr.sin_port = htons(opt->port_proxy);
    }else{
        inet_pton(AF_INET, opt->ip, &serv_addr.sin_addr);
        serv_addr.sin_port = htons(opt->port);
    }
    printf("opt proxy %d\n", opt->proxy);
   
    printf("connect opt %s:%d\n", opt->ip, opt->port);
    printf("connect %s:%d\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        vpn_error_exit("ERROR opening socket", errno);
    
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        vpn_error_exit("ERROR connecting", errno);

    opt->tun = opt->tuns[tun];
    tun += 1;
 
    if(opt->proxy == 1){
        printf("send proxy %s\n", opt->ip_proxy);
        uint8_t buffer[1500] = {0};
        intmax_t len = 0;

        len += sprintf((char *)buffer+len, 
                "CONNECT %s:%d HTTP/1.1\r\n\r\n", 
                opt->ip, opt->port); 

#ifdef DEBUG
        printf(">%s\n", buffer);
#endif
		
        if( send(sockfd, buffer, len, 0) < 0){
            close(sockfd);
            vpn_error_exit("send error" , errno);
        }

        len = recv(sockfd, buffer, sizeof(buffer), 0);

#ifdef DEBUG
        printf("<%s\n", buffer);
#endif

    }
   
    return sockfd;
}

int socket_server(options_main* opt){
    int sockfd, new_fd, pid, tun = 0;
    struct sockaddr_in my_addr;
    struct sockaddr their_addr;
    unsigned int sin_size;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))==-1)
        vpn_error_exit("create socket error", errno);

    memset(&my_addr, '0', sizeof(my_addr));
    memset(&their_addr, '0', sizeof(their_addr));

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(opt->port);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))==-1)
            vpn_error_exit("bind socket error", errno);

    if(listen(sockfd, 10)==-1)
        vpn_error_exit("listen", errno);

    while(1){
        sin_size = sizeof(struct sockaddr);

        printf("server is run %i\n", opt->port);

        if((new_fd = accept(sockfd, &their_addr, &sin_size)) < 0)
            vpn_error_exit("accept", errno);
          
        printf("socket tuntap %d\n", opt->tuns[tun]);

        opt->tun = opt->tuns[tun];
        tun += 1;
        
        pid = fork();

        if(pid == 0){
            printf("fork success %i\n", getpid());

            struct timeval timeout;      
            timeout.tv_sec = 5;

            setsockopt(new_fd, IPPROTO_TCP, SO_RCVTIMEO, 
                    (char *)&timeout, sizeof(timeout));

            setsockopt(new_fd, IPPROTO_TCP, SO_SNDTIMEO, 
                    (char *)&timeout, sizeof(timeout));

            setsockopt(new_fd, SOL_SOCKET, SO_RCVTIMEO, 
                    (char *)&timeout, sizeof(timeout));
           /* 
            if( opt->proxy == 2){
                uint8_t buffer[1500] = {0};
                intmax_t len = 0;

                printf("receive proxy\n");
                len = recv(new_fd, buffer, sizeof(buffer), 0);

                if ( len > 0) {
                    add_http_header(opt, buffer, &len);
                    len += sprintf((char *)buffer+len, "yolooooooooo"); 
                    add_http_footer(opt, buffer, &len);

#ifdef DEBUG
                    printf(">%s\n", buffer);
#endif
		
                    if( send(new_fd, buffer, len, 0) < 0){
                        close(sockfd);
                        vpn_error_exit("send error" , errno);
                    }
                }
            }else{
                printf("no proxy\n");
            }*/

            return new_fd;
        }else{
            printf("close socket %i\n", getpid());
            close(new_fd);
        }
    }
    printf("close sockfd %i\n", getpid());
    close(sockfd);
}


