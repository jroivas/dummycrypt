/* Copyright (c)2016 Jouni Roivas <jroivas@iki.fi>
 * MIT license, see LICENSE
 */
#include "dummy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc <= 2) {
        printf("Usage: %s msg key [n]\n", argv[0]);
        return 1;
    }
    uint32_t no_seed = 0;
    if (argc >= 4) {
        if (argv[3][0] == 'n') {
            no_seed = 1;
        }
    }

    uint32_t seed = 10;
    uint32_t srclen = strlen(argv[1]) + 1;
    char *m = dummy_crypt_seed(argv[1], srclen, argv[2], strlen(argv[2]), seed, no_seed ? 0 : 1);
    uint32_t s = dummy_crypt_size(srclen, seed);

    char *p = dummy_decrypt_seed(m, s, argv[2], strlen(argv[2]), no_seed ? seed : 0);
    //printf("Crypted  : %s\n", m);
    printf("Crypted  : ");
    for (uint32_t i = 0; i < s; ++i) {
        printf("%.2X ", (uint8_t)m[i]);
    }
    printf("\n");
    printf("Decrypted: %s\n", p);

    free(m);
    free(p);
}
