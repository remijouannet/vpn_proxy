int add_http_header(options_main* opt, uint8_t *data, intmax_t *len);
int remove_http_header(options_main* opt, uint8_t *data, intmax_t *len);

int add_http_footer(options_main* opt, uint8_t *data, intmax_t *len);
int remove_http_footer(options_main* opt, uint8_t *data, intmax_t *len);

int remove_http_server_header(options_main* opt, uint8_t *data, intmax_t *len);
int add_http_server_header(options_main* opt, uint8_t *data, intmax_t *len);
int remove_http_client_header(options_main* opt, uint8_t *data, intmax_t *len);
int add_http_client_header(options_main* opt, uint8_t *data, intmax_t *len);

int remove_http_server_footer(options_main* opt, uint8_t *data, intmax_t *len);
int add_http_server_footer(options_main* opt, uint8_t *data, intmax_t *len);
int remove_http_client_footer(options_main* opt, uint8_t *data, intmax_t *len);
int add_http_client_footer(options_main* opt, uint8_t *data, intmax_t *len);

int len_http_server_header(options_main* opt);
int len_http_client_header(options_main* opt);

int len_http_server_footer(options_main* opt);
int len_http_client_footer(options_main* opt);
