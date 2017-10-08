#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <netinet/ip.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <string.h>

#include "vpn_opt.h"
#include "utils.h"
#include "error.h"
#include "crypto.h"
#include "ssl.h"
#include "tuntap.h"
#include "socket.h"
#include "http.h"
#include "test.h"

int test1() {
    crypto_options crypto_ = {{0}};

    memcpy(crypto_.key, "aeeazfafqdfeeeeeeaeaeaeaeae", 32);
    memcpy(crypto_.salt, "aeeazfafqdfeeeeee", 32);
    init_crypto(&crypto_);
    
    intmax_t len;
    uint8_t *input = (unsigned char*)"lol2";
    uint8_t *plaintext;
    uint8_t *ciphertext;

    len = 16;
    printf("%ju \n", len); 
    print_hex(input, len);
    ciphertext = aes_encrypt(&(crypto_.en), input, &len);

    printf("%ju \n", len); 
    print_hex(ciphertext, len);
    plaintext = aes_decrypt(&(crypto_.de), ciphertext, &len);

    printf("%ju \n", len); 
    print_hex(plaintext, len);

    end_crypto(&crypto_);
    return 0;
}

int test2(void *p) {
    options_main *opt = p;

    uint8_t bufferr[1000] = {0};
    intmax_t len_test;

    add_http_header(opt, bufferr, &len_test);
    len_test += sprintf((char *)bufferr+len_test, "yoloooooooooo");
    add_http_footer(opt, bufferr, &len_test);
    printf("%s\n", bufferr);

    if(opt->MODE == 1){
        opt->MODE = 0;
    }else{
        opt->MODE = 1;
    }

    printf("#######################################\n");

    remove_http_header(opt, bufferr, &len_test);
    remove_http_footer(opt, bufferr, &len_test);
    printf("%s\n", bufferr);

    return 0;
}
