#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <resolv.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "error.h"
#include "vpn_opt.h"
#include "ssl.h"

ssl_options* init_ctx(ssl_options *ssl, options_main* opt){
    printf("init ssl\n");
    SSL_METHOD *method;

    SSL_library_init();
 
    OpenSSL_add_all_algorithms(); 
    SSL_load_error_strings();

    if ( opt->MODE == 1 ){
        method = (SSL_METHOD*)TLSv1_2_server_method(); 
    }else if ( opt->MODE == 0 ){
        method = (SSL_METHOD*)TLSv1_2_client_method(); 
    }

    ssl->ctx = SSL_CTX_new(method);

    if ( ssl->ctx == NULL ){
        vpn_error_exit("ctx1\n", 1);
    }
    if ( SSL_CTX_use_certificate_file(ssl->ctx, opt->crt, SSL_FILETYPE_PEM) <= 0 ){
        vpn_error_exit("ctx2\n", 1);
    }
    if ( SSL_CTX_use_PrivateKey_file(ssl->ctx, opt->key, SSL_FILETYPE_PEM) <= 0 ){
        vpn_error_exit("ctx3\n", 1);
    }
    if ( !SSL_CTX_check_private_key(ssl->ctx) ){
        fprintf(stderr, "Private key does not match the public certificate\n");
    }

    SSL_CTX_load_verify_locations(ssl->ctx,"keys/ca.crt",NULL);

    SSL_CTX_set_verify(ssl->ctx, SSL_VERIFY_PEER, NULL);

    return 0;
}

int new_ssl(ssl_options *ssl){  
    printf("new ssl\n");
    ssl->ssl = SSL_new(ssl->ctx);

    if ( ssl->ssl == NULL ){
        vpn_error_exit("ssl1\n", 1);
    }
    return 0;
}

int ssl_fd(ssl_options *ssl, int sockfd){
    printf("fd ssl\n");
    SSL_set_fd(ssl->ssl, sockfd);
    return 0;
}

int handshake_ssl(ssl_options *ssl, options_main* opt){
    printf("handshake\n");
    if ( opt->MODE == 1 ){
        if ( SSL_accept(ssl->ssl) != 1 ){
            vpn_error_exit("handshake fail\n", 1);
        }
        return 0;
    }else if ( opt->MODE == 0 ){
        if(SSL_connect(ssl->ssl) != 1 ){
            vpn_error_exit("handshake fail\n", 1);
        }
        return 0;
    }
    return 1;
}

int show_crt(ssl_options *ssl){   
    X509 *cert;
 
    cert = SSL_get_peer_certificate(ssl->ssl);
    if ( cert != NULL ){
        printf("Server certificates:\n");
        printf("Subject: %s\n", 
                X509_NAME_oneline(X509_get_subject_name(cert), 0, 0));
        printf("Issuer: %s\n", 
                X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0));
        X509_free(cert);
    }else{
        printf("Info: No certificates configured.\n");
    }

    return 0;
}

int read_ssl(ssl_options *ssl, uint8_t *buffer, int len){
    return SSL_read(ssl->ssl, buffer, len);
}

int write_ssl(ssl_options *ssl, uint8_t *buffer, int len){
    return SSL_write(ssl->ssl, buffer, len);
}

int end_ssl(ssl_options *ssl){
    SSL_CTX_free(ssl->ctx);
    SSL_free(ssl->ssl);
    return 0;
}
