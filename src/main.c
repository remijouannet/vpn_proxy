#include <evp.h>
#include <aes.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#include "vpn_opt.h"
#include "utils.h"
#include "error.h"
#include "crypto.h"
#include "socket.h"
#include "ssl.h"
#include "tuntap.h"
#include "http.h"
#include "test.h"

int main(int argc, char *argv[]) {
    options_main opt;
    int sockfd;
    ssl_options ssl = {0};
	printf("starting\n");

	vpn_opt(argc, argv, &opt);

#ifdef DEBUG
    printf("debug\n");
    //test1();
    //test2(&opt);
#endif
    if (opt.tls == 1){
        init_ctx(&ssl, &opt);
    }

    create_interface(&opt);
    sockfd = create_socket(&opt);

    if (sockfd > 0 && opt.tls == 1){
        while_interface_ssl(&opt, &ssl, sockfd);
    }else{
        while_interface(&opt, sockfd);
    }

	return 0;
}
