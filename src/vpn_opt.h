typedef struct options_main options_main;
struct options_main
{
    int MODE;
    int port;
    char ip[16];
    int tun;
    char ip_proxy[16];
    char crt[255];
    char key[255];
    int port_proxy;
    int proxy;
    int tls;
};

void print_usage();
int vpn_opt(int argc, char *argv[], options_main* opt);

