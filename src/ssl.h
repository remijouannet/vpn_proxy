struct ssl_options
{
    SSL *ssl;
    SSL_CTX *ctx;
};
typedef struct ssl_options ssl_options;

ssl_options *init_ctx(ssl_options *ssl, options_main* opt);
int new_ssl(ssl_options *ssl);
int ssl_fd(ssl_options *ssl, int sockfd);
int handshake_ssl(ssl_options *ssl, options_main* opt);
int show_crt(ssl_options *ssl);
int read_ssl(ssl_options *ssl, uint8_t *buffer, int len);
int write_ssl(ssl_options *ssl, uint8_t *buffer, int len);
int end_ssl(ssl_options *ssl);
