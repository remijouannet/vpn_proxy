struct crypto_options
{
    uint8_t key[32];
    uint8_t salt[32];
    EVP_CIPHER_CTX en;
    EVP_CIPHER_CTX de;
};
typedef struct crypto_options crypto_options;

uint8_t *aes_encrypt(EVP_CIPHER_CTX *e, uint8_t *plaintext, intmax_t *len);
uint8_t *aes_decrypt(EVP_CIPHER_CTX *e, uint8_t *ciphertext, intmax_t *len);
intmax_t init_crypto(crypto_options *crypto_);
intmax_t end_crypto(crypto_options *crypto_);
