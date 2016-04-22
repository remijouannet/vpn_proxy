#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <evp.h>
#include <aes.h>

int aes_init(unsigned char *key_data, int key_data_len, 
        unsigned char *salt, EVP_CIPHER_CTX *e_ctx, EVP_CIPHER_CTX *d_ctx)
{
  int i, nrounds = 5;
  unsigned char key[32], iv[32];
  
  i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), 
          salt, key_data, key_data_len, nrounds, key, iv);

  if (i != 32) {
    printf("Key size is %d bits - should be 256 bits\n", i);
    return -1;
  }

  EVP_CIPHER_CTX_init(e_ctx);
  EVP_EncryptInit_ex(e_ctx, EVP_aes_256_cbc(), NULL, key, iv);
  EVP_CIPHER_CTX_init(d_ctx);
  EVP_DecryptInit_ex(d_ctx, EVP_aes_256_cbc(), NULL, key, iv);

  return 0;
}

unsigned char *aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext, int *len)
{
  int c_len = *len + AES_BLOCK_SIZE, f_len = 0;
  unsigned char *ciphertext = malloc(c_len);

  EVP_EncryptInit_ex(e, NULL, NULL, NULL, NULL);

  EVP_EncryptUpdate(e, ciphertext, &c_len, plaintext, *len);

  EVP_EncryptFinal_ex(e, ciphertext+c_len, &f_len);

  *len = c_len + f_len;
  return ciphertext;
}

unsigned char *aes_decrypt(EVP_CIPHER_CTX *e, unsigned char *ciphertext, int *len)
{
  int p_len = *len, f_len = 0;
  unsigned char *plaintext = malloc(p_len);
  
  EVP_DecryptInit_ex(e, NULL, NULL, NULL, NULL);
  EVP_DecryptUpdate(e, plaintext, &p_len, ciphertext, *len);
  EVP_DecryptFinal_ex(e, plaintext+p_len, &f_len);

  *len = p_len + f_len;
  return plaintext;
}

int main(int argc, char **argv)
{
  EVP_CIPHER_CTX en, de;

  unsigned int salt[] = {12345, 54321};
  unsigned char *key_data;
  int key_data_len, i;
  char *input[] = {"a", "abcd", "this is a test", "this is a bigger test", 
                   "\nWho are you ?\nI am the 'Doctor'.\n'Doctor' who ?\nPrecisely!",
                   NULL};
  
  key_data = (unsigned char *)argv[1];
  key_data_len = strlen(argv[1]);
  
  if (aes_init(key_data, key_data_len, (unsigned char *)&salt, &en, &de)) {
    printf("Couldn't initialize AES cipher\n");
    return -1;
  }

  for (i = 0; input[i]; i++) {
    char *plaintext;
    unsigned char *ciphertext;
    int olen, len;
    
    olen = len = strlen(input[i])+1;
    
    ciphertext = aes_encrypt(&en, (unsigned char *)input[i], &len);
    plaintext = (char *)aes_decrypt(&de, ciphertext, &len);

    if (strncmp(plaintext, input[i], olen)) 
      printf("FAIL: enc/dec failed for \"%s\"\n", input[i]);
    else 
      printf("OK: enc/dec ok for \"%s\"\n", plaintext);
    
    free(ciphertext);
    free(plaintext);
  }

  EVP_CIPHER_CTX_cleanup(&en);
  EVP_CIPHER_CTX_cleanup(&de);

  return 0;
}
  
