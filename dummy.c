/* Copyright (c)2016 Jouni Roivas <jroivas@iki.fi>
 * MIT license, see LICENSE
 */

#include "dummy.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

#ifndef __linux
static uint8_t __rand_initted = 0;
void __init_rand()
{
    if (__rand_initted == 0) {
        srand(time(NULL));
        __rand_initted = 1;
    }
}
#endif

uint8_t __rand_byte()
{
#ifdef __linux
    // Non-optimal to open and close all the time
    FILE *randf = fopen("/dev/random", "r");
    uint8_t r = fgetc(randf);
    fclose(randf);
    return r;
#else
    // Stupid fallback
    __init_rand();
    return rand() % 256;
#endif
}

void __gen_seed(char *buf, uint32_t len)
{
    for (uint32_t i = 0; i < len; ++i) {
        buf[i] = __rand_byte();
    }
}

uint32_t dummy_crypt_size(uint32_t len, uint32_t seed_len)
{
    return (len + 1 + seed_len);
}

char *dummy_crypt_seed(const char *src, uint32_t len, const char *key, uint32_t key_len, uint32_t seed_len, uint8_t provide_seed_len)
{
    assert(src != NULL);
    assert(key != NULL);
    assert(len > 0);
    assert(key_len > 0);
    assert(seed_len > 0);

    char *seed = malloc(seed_len);
    char *res = calloc(1, len + 1 + seed_len);
    assert(seed != NULL);
    assert(res != NULL);

    // Initialize seed
    __gen_seed(seed, seed_len);

    uint32_t i = 0;
    uint32_t si = 0;
    uint32_t ki = 0;

    seed_len %= 256;
    if (provide_seed_len == 1) {
        // Write seed to buffer
        res[i++] = seed_len;
    }
    for (si = 0; si < seed_len; si++) {
        res[i++] = seed[si];
    }

    si = 0;
    // Crypt
    for (uint32_t mi = 0; mi < len; mi++) {
        uint32_t d = (src[mi] ^ seed[si]);
        d += key[ki];
        res[i++] = d % 256;

        ki = (ki + 1) % key_len;
        si = (si + 1) % seed_len;
    }

    free(seed);

    return res;
}

char *dummy_crypt(const char *src, uint32_t len, const char *key, uint32_t key_len, uint32_t seed_len)
{
    return dummy_crypt_seed(src, len, key, key_len, seed_len, 1);
}

char *dummy_decrypt_seed(const char *src, uint32_t len, const char *key, uint32_t key_len, uint32_t seed_len)
{
    assert(src != NULL);
    assert(key != NULL);
    assert(len > 0);
    assert(key_len > 0);

    uint32_t i = 0;
    uint32_t si = 0;
    uint32_t ki = 0;

    if (seed_len == 0) {
        seed_len = src[i++];
    }
    assert(seed_len > 0);
    assert((len - 1 - seed_len) > 0);

    char *seed = malloc(seed_len);
    assert(seed != NULL);

    for (si = 0; si < seed_len; si++) {
        seed[si] = src[i++];
    }

    char *res = calloc(1, len - 1 - seed_len);
    assert(res != NULL);

    si = 0;
    // Decrypt
    for (uint32_t mi = 0; mi < len - seed_len - 1; mi++) {
        uint32_t d = src[i++];
        if (d < key[ki]) {
            d = 256 - (key[ki] - d);
        } else {
            d -= key[ki];
        }
        d ^= seed[si];
        res[mi] = d % 256;

        ki = (ki + 1) % key_len;
        si = (si + 1) % seed_len;
    }

    free(seed);
    return res;
}

char *dummy_decrypt(const char *src, uint32_t len, const char *key, uint32_t key_len)
{
    return dummy_decrypt_seed(src, len, key, key_len, 0);
}
