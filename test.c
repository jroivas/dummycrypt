/* Copyright (c)2016 Jouni Roivas <jroivas@iki.fi>
 * MIT license, see LICENSE
 */
#include "dummy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef USE_SHA256
#include <openssl/sha.h>
#endif

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
    uint32_t key_len = strlen(argv[2]);
    char *m = dummy_crypt_seed(argv[1], srclen, argv[2], key_len, seed, no_seed ? 0 : 1);
    uint32_t s = dummy_crypt_size(srclen, seed);
#ifdef USE_SHA256
    printf("PreSHA256: ");
    for (uint32_t i = 0; i < s; ++i) {
        printf("%.2X ", (uint8_t)m[i]);
    }
    printf("\n");
    unsigned char *sha_key = SHA256((unsigned char*)argv[1], key_len, 0);
    for (uint32_t i = 0; i < s; ++i) {
        m[i] ^= sha_key[i % 32];
    }
#endif
    printf("Crypted  : ");
    for (uint32_t i = 0; i < s; ++i) {
        printf("%.2X ", (uint8_t)m[i]);
    }
    printf("\n");

#ifdef USE_SHA256
    for (uint32_t i = 0; i < s; ++i) {
        m[i] ^= sha_key[i % 32];
    }
#endif

    char *p = dummy_decrypt_seed(m, s, argv[2], key_len, no_seed ? seed : 0);
    //printf("Crypted  : %s\n", m);
    printf("Decrypted: %s\n", p);

    free(m);
    free(p);
}
