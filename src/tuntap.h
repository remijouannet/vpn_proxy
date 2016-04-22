int create_interface(options_main* opt);
int while_interface_ssl(options_main* opt, ssl_options *ssl, int sockfd);
int while_interface(options_main* opt, int sockfd);
int send_tunnel(uint8_t *data, crypto_options *crypto_, intmax_t *len);
int recv_tunnel(uint8_t *data, crypto_options *crypto_, intmax_t *len);
int end_tunnel(crypto_options *crypto_);
