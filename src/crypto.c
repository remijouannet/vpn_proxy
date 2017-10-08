#include <openssl/evp.h>
#include <openssl/aes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "error.h"
#include "utils.h"
#include "crypto.h"

intmax_t aes_init(uint8_t *key_data, intmax_t key_data_len, 
        uint8_t *salt, EVP_CIPHER_CTX *e_ctx, EVP_CIPHER_CTX *d_ctx){

    intmax_t i, nrounds = 5;
    uint8_t key[32], iv[32];
  
    i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), 
        salt, key_data, key_data_len, nrounds, key, iv);

    if (i != 32) {
        printf("Key size is %d bits - should be 256 bits\n", (int)i);
        return -1;
    }

    EVP_CIPHER_CTX_init(e_ctx);
    EVP_EncryptInit_ex(e_ctx, EVP_aes_256_cbc(), NULL, key, iv);
    EVP_CIPHER_CTX_init(d_ctx);
    EVP_DecryptInit_ex(d_ctx, EVP_aes_256_cbc(), NULL, key, iv);

    return 0;
}

uint8_t *aes_encrypt(EVP_CIPHER_CTX *e, uint8_t *plaintext, intmax_t *len){
    intmax_t c_len = *len + AES_BLOCK_SIZE - 1, f_len = 0;
    uint8_t *ciphertext = malloc(c_len);

    EVP_EncryptInit_ex(e, NULL, NULL, NULL, NULL);

    EVP_EncryptUpdate(e, ciphertext, (int *)&c_len, plaintext, *len);

    EVP_EncryptFinal_ex(e, ciphertext+c_len, (int *)&f_len);

    *len = c_len + f_len;

    return ciphertext;
}

uint8_t *aes_decrypt(EVP_CIPHER_CTX *e, uint8_t *ciphertext, intmax_t *len){
    intmax_t f_len = 0;
    intmax_t p_len = *len;
    uint8_t *plaintext = malloc(*len);
  
    EVP_DecryptInit_ex(e, NULL, NULL, NULL, NULL);

    EVP_DecryptUpdate(e, plaintext, (int *)&p_len, ciphertext, *len);

    EVP_DecryptFinal_ex(e, plaintext+p_len, (int *)&f_len);

    *len = p_len + f_len;

    return plaintext;
}

intmax_t init_crypto(crypto_options *crypto_){
    intmax_t key_data_len;
    uint8_t *input = (unsigned char*)"lol";
    uint8_t *plaintext;
    uint8_t *ciphertext;
    intmax_t olen, len;

    key_data_len = strlen((const char *)crypto_->key);
  
    if (aes_init(crypto_->key, key_data_len, 
                crypto_->salt, &(crypto_->en), &(crypto_->de))){
        printf("Couldn't initialize AES cipher\n");
        return -1;
    }

    olen = len = strlen((const char *)input)+1;
    
    ciphertext = aes_encrypt(&(crypto_->en), input, &len);
    plaintext = aes_decrypt(&(crypto_->de), ciphertext, &len);

    if (memcmp(plaintext, input, olen)){ 
        printf("FAIL: enc/dec failed for \"%s\"\n", input);
        return -1;
    }else{
        printf("OK: enc/dec ok for \"%s\"\n", plaintext);
        return -1;
    }
    
    free(ciphertext);
    free(plaintext);

    return 0;
}
 
intmax_t end_crypto(crypto_options *crypto_){
    EVP_CIPHER_CTX_cleanup(&(crypto_->en));
    EVP_CIPHER_CTX_cleanup(&(crypto_->de));
    return 0;
}
