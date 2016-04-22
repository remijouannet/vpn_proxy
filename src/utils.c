#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "utils.h"

int getuser(){
    return getuid();
}

int print_hex(uint8_t *data, int l){
    int i =0;
    for (i = 0; i < l; i++){
        printf("%02X", data[i]);
    }
    printf("\n");
    return 0;
}


