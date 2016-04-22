#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "error.h"

void vpn_log_err(char *msg, char *error) {
    time_t timer;
    struct tm* tm_info;
    char time_f[28];

    time(&timer);
    tm_info = localtime(&timer);
    strftime(time_f, 28, "%d/%b/%Y %H:%M:%S %z", tm_info);

    fprintf(stderr, "%s ERROR %s %s \n", time_f, msg, error);
}

void vpn_log_out(char *msg) {
    time_t timer;
    struct tm* tm_info;
    char time_f[28];

    time(&timer);
    tm_info = localtime(&timer);
    strftime(time_f, 28, "%d/%b/%Y %H:%M:%S %z", tm_info);

    fprintf(stderr, "%s INFO %s \n", time_f, msg);
}

void vpn_error(char *msg) {
    vpn_log_err(msg, strerror(errno));
}

void vpn_error_exit(char *msg, int code) {
    vpn_error(msg);
    exit(code);
}

int vpn_return(int return_) {
    errno = return_;        
    return return_;
}


