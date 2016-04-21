#include <evp.h>
#include <aes.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <net/if.h>
#include <linux/if_tun.h>
#include <stdint.h>

#include "error.h"
#include "utils.h"
#include "vpn_opt.h"
#include "crypto.h"
#include "http.h"
#include "ssl.h"
#include "tuntap.h"

#define BUFFER 9000

int create_interface(options_main* opt){
    int fd, i;
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(struct ifreq));

    //if ((opt->tun = open("/dev/net/tun",O_RDWR)) < 0)
    //    vpn_error_exit("tuntap", errno);

    ifr.ifr_flags = IFF_TUN | IFF_MULTI_QUEUE | IFF_NO_PI;
    strncpy(ifr.ifr_name, "vpn%d", IFNAMSIZ);
    
    //if (ioctl(opt->tun, TUNSETIFF, (void *)&ifr) < 0) vpn_error_exit("ioctl", errno);
    for (i = 0; i < 2; i++) {
          fd = open("/dev/net/tun", O_RDWR);
          ioctl(fd, TUNSETIFF, (void *)&ifr);
          opt->tuns[i] = fd;
    }

    printf("Allocated interface %s. Configure and use it\n", ifr.ifr_name);
    
    return 0;
}

int while_interface_ssl(options_main* opt, ssl_options *ssl, int sockfd){
    printf("sockfd %d\n", sockfd);
    printf("tuntap %d\n", opt->tun);

    uint8_t buffer[BUFFER] = {0};
    int len;
	fd_set fdset;
	 
    new_ssl(ssl);
    ssl_fd(ssl, sockfd);
    handshake_ssl(ssl, opt);
    show_crt(ssl);

    sleep(3);

    printf("end init ssl\n");

    while(1){
        len = 0;
		FD_ZERO(&fdset);
		FD_SET(sockfd, &fdset);
		FD_SET(opt->tun, &fdset);
		
        if (select(sockfd+opt->tun+1, &fdset, NULL, NULL, NULL) < 0) 
            vpn_error_exit("select", errno);
        
        memset(&buffer, 0, sizeof(buffer));

        if (FD_ISSET(opt->tun, &fdset)) {
			len = read(opt->tun, buffer, sizeof(buffer));

#ifdef DEBUG
            printf(">%d\n", len);
            print_hex(buffer, 200);
#endif

            if (len < 0){ 
                end_ssl(ssl);
                close(sockfd);
                vpn_error_exit("read", errno);
            }

	        if( write_ssl(ssl, buffer, sizeof(buffer)) < 0){
                end_ssl(ssl);
                close(sockfd);
                vpn_error_exit("send error" , errno);
                break;
            }
		} else if(FD_ISSET(sockfd, &fdset)) {
            len = read_ssl(ssl, buffer, sizeof(buffer));

#ifdef DEBUG
            printf("<%d\n", len);
            print_hex(buffer, 200);
#endif

            if( len <= 0 ){
                end_ssl(ssl);
                close(sockfd);
                vpn_error_exit("recv error", errno);
                break;
            }

            if (write(opt->tun, buffer, len) < 0){ 
                end_ssl(ssl);
                close(sockfd);
                vpn_error_exit("write", errno);
            }
		}
    }
    return 0;
}

int while_interface(options_main* opt, int sockfd){
    printf("sockfd %d\n", sockfd);
    printf("tuntap %d\n", opt->tun);

    uint8_t buffer[BUFFER] = {0};
    intmax_t len;
    intmax_t len1;
	fd_set fdset;
    crypto_options crypto_ = {{0}};

    memcpy(crypto_.key, "aeeazfafqdfeeeeeeaeaeaeaeae", 32);
    memcpy(crypto_.salt, "aeeazfafqdfeeeeee", 32);
    init_crypto(&crypto_);
    
    sleep(3);
	
    while(1){
        len = 0;
        len1 = 0;
		FD_ZERO(&fdset);
		FD_SET(sockfd, &fdset);
		FD_SET(opt->tun, &fdset);
		
        if (select(sockfd+opt->tun+1, &fdset, NULL, NULL, NULL) < 0) 
            vpn_error_exit("select", errno);
        
        memset(&buffer, 0, sizeof(buffer));

        if (FD_ISSET(opt->tun, &fdset)) {
            //add_http_header(opt, buffer, &len);

			len1 = read(opt->tun, buffer+len, sizeof(buffer)-len);

#ifdef DEBUG
            printf(">%s\n", buffer);
#endif

            if (len1 < 0){ 
                end_crypto(&crypto_);
                vpn_error_exit("read", errno);
            }

            send_tunnel(buffer+len, &crypto_, &len1);

            len = len1 + len;

            //add_http_footer(opt, buffer, &len);
		
	        if( send(sockfd, buffer, len, 0) < 0){
                end_crypto(&crypto_);
                close(sockfd);
                vpn_error_exit("send error" , errno);
                break;
            }
		} else if(FD_ISSET(sockfd, &fdset)) {
            len = recv(sockfd, buffer, sizeof(buffer), 0);

#ifdef DEBUG
            printf("<%s\n", buffer);
#endif

            if( len <= 0 ){
                end_crypto(&crypto_);
                close(sockfd);
                vpn_error_exit("recv error", errno);
                break;
            }

            //remove_http_header(opt, buffer, &len);
            //remove_http_footer(opt, buffer, &len);
            recv_tunnel(buffer, &crypto_, &len);
	
			if (write(opt->tun, buffer, len) < 0){ 
                end_crypto(&crypto_);
                vpn_error_exit("write", errno);
            }
		}
    }
    return 0;
}

int send_tunnel(uint8_t *data, crypto_options *crypto_, intmax_t *len){
    uint8_t *ciphertext;
    
    ciphertext = aes_encrypt(&(crypto_->en), data, len);
    memcpy(data, ciphertext, *len);
    free(ciphertext);

    return 0;
}

int recv_tunnel(uint8_t *data, crypto_options *crypto_, intmax_t *len){
    uint8_t *plaintext;

    plaintext = aes_decrypt(&(crypto_->de), data, len);
    memcpy(data, plaintext, *len);
    free(plaintext);

    return 0;
}
