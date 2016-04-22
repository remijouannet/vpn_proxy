#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>

#include "utils.h"
#include "vpn_opt.h"
#include "http.h"

int add_http_header(options_main* opt, uint8_t *data, intmax_t *len){
    if(opt->MODE == 1){
        return add_http_server_header(opt, data, len);
    }else if(opt->MODE == 0){
        return add_http_client_header(opt, data, len);
    }else{
        return 0;
    }
}

int remove_http_header(options_main* opt, uint8_t *data, intmax_t *len){
    if(opt->MODE == 0){
        return remove_http_server_header(opt, data, len);
    }else if(opt->MODE == 1){
        return remove_http_client_header(opt, data, len);
    }else{
        return 0;
    }
}

int add_http_client_header(options_main* opt, uint8_t *data, intmax_t *len){
    *len = 0;
    *len += sprintf((char *)data+*len, "POST http://%s:%d/ HTTP/1.1\r\n", opt->ip, opt->port);
    *len += sprintf((char *)data+*len, "Host: %s:%d\r\n", opt->ip, opt->port);
    *len += sprintf((char *)data+*len, "Proxy-Connection: keep-alive\r\n");
    *len += sprintf((char *)data+*len, "Connection: keep-alive\r\n");
    *len += sprintf((char *)data+*len, "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/49.0.2623.112 Safari/537.36\r\n");
    *len += sprintf((char *)data+*len, "Content-Type: multipart/form-data;"
           " boundary=----7d81b516112482\r\n\r\n");
    *len += sprintf((char *)data+*len, "----7d81b516112482\r\n");
    *len += sprintf((char *)data+*len, "Content-Disposition: form-data; "
            "name=\"Filedata\"; filename=\"lol.jpg\"\r\n");
    *len += sprintf((char *)data+*len, "Content-Type: image/jpeg\r\n\r\n");
    return 0;
}

int remove_http_client_header(options_main* opt, uint8_t *data, intmax_t *len){
    int len_header = len_http_client_header(opt);

    memmove(data, data+len_header, *len); 
    *len -= len_header;
    return 0;
}

int add_http_server_header(options_main* opt, uint8_t *data, intmax_t *len){
    *len = 0;
    *len += sprintf((char *)data+*len, "HTTP/1.1 200 OK\r\n");
    *len += sprintf((char *)data+*len, "Connection: keep-alive\r\n");
    *len += sprintf((char *)data+*len, "Content-Type: text/plain\r\n\r\n");
    return 0;
}

int remove_http_server_header(options_main* opt, uint8_t *data, intmax_t *len){
    int len_header = len_http_server_header(opt); 

    memmove(data, data+len_header, *len); 
    *len -= len_header;
    return 0;
}


int add_http_footer(options_main* opt, uint8_t *data, intmax_t *len){
    if(opt->MODE == 1){
        return add_http_server_footer(opt, data, len);
    }else if(opt->MODE == 0){
        return add_http_client_footer(opt, data, len);
    }else{
        return 0;
    }
}

int remove_http_footer(options_main* opt, uint8_t *data, intmax_t *len){
    if(opt->MODE == 0){
        return remove_http_server_footer(opt, data, len);
    }else if(opt->MODE == 1){
        return remove_http_client_footer(opt, data, len);
    }else{
        return 0;
    }
}

int add_http_client_footer(options_main* opt, uint8_t *data, intmax_t *len){
    *len += sprintf((char *)data+*len, "\r\n----7d81b516112482--\r\n");
    return 0;
}

int remove_http_client_footer(options_main* opt, uint8_t *data, intmax_t *len){
    int len_footer = len_http_client_footer(opt); 

    memset(data+(*len-len_footer), '\0', (size_t)*len-len_footer);
    *len -= len_footer;
    return 0;
}

int add_http_server_footer(options_main* opt, uint8_t *data, intmax_t *len){
    return 0;
}

int remove_http_server_footer(options_main* opt, uint8_t *data, intmax_t *len){
    return 0;
}

int len_http_server_header(options_main* opt){
    uint8_t buffer[500] = {0};
    intmax_t len = 0;

    add_http_server_header(opt, buffer, &len);

    return len;
}

int len_http_client_header(options_main* opt){
    uint8_t buffer[500] = {0};
    intmax_t len = 0;

    add_http_client_header(opt, buffer, &len);

    return len;
}

int len_http_server_footer(options_main* opt){
    uint8_t buffer[500] = {0};
    intmax_t len = 0;

    add_http_server_footer(opt, buffer, &len);

    return len;
}

int len_http_client_footer(options_main* opt){
    uint8_t buffer[500] = {0};
    intmax_t len = 0;

    add_http_client_footer(opt, buffer, &len);

    return len;
}


