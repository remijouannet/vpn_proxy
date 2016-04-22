#include <errno.h>

void vpn_log_err(char *msg, char *error);
void vpn_log_out(char *msg);
void vpn_error(char *msg);
void vpn_error_exit(char *msg, int code);
int vpn_return(int return_);

