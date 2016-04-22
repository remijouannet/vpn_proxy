#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdint.h>
#include <string.h>
#include "vpn_opt.h"
#include "error.h"
#include "utils.h"

void print_usage() {
    printf("Usage: main -c -s [IP] -p [PORT]\n");
}

int vpn_opt(int argc, char *argv[], options_main* opt) {
	int option = 0;
    int port;
    int long_index = 0;

    opt->proxy = 0;

    static struct option long_options[] = {
        {"tls", no_argument,  0,  't' },
        {"proxy", no_argument, 0,  'Z' },
        {"server", required_argument, 0,  's' },
        {"client", required_argument, 0,  'c' },
        {"port", required_argument, 0,  'p' },
        {"ipproxy", required_argument, 0,  'C' },
        {"portproxy", required_argument, 0,  'P' }
    };

	while ((option = getopt_long(argc, argv,"tZs:c:p:C:P:", 
                    long_options, &long_index)) != -1) {
		switch (option) {
			case 's' : 
	            opt->MODE = 1;
                strncpy(opt->ip, optarg, 16);
				break;
			case 'c' : 
	            opt->MODE = 0;
                strncpy(opt->ip, optarg, 16);
				break;
			case 'p' :
                port = atoi(optarg);
                if(port < 1 || port > 65535) 
                    vpn_error_exit("port 1-65535", vpn_return(1));
                opt->port = atoi(optarg);
				break;
            case 'C' : 
	            opt->proxy = 1;
                strncpy(opt->ip_proxy, optarg, 16);
				break;
			case 'P' :
                port = atoi(optarg);
                if(port < 1 || port > 65535) 
                    vpn_error_exit("port 1-65535", vpn_return(1));
                opt->port_proxy = atoi(optarg);
				break;
			case 'Z' : 
	            opt->proxy = 1;
				break;
			case 't' : 
	            opt->tls = 1;
                if ( opt->MODE == 1){
                    strcpy(opt->crt, "keys/server.crt");
                    strcpy(opt->key, "keys/server.key");
                }else if( opt->MODE == 0){
                    strcpy(opt->crt, "keys/client.crt");
                    strcpy(opt->key, "keys/client.key");
                }
				break;
			default: print_usage(); 
				return 1;
		}
	}

	return 0;
}

