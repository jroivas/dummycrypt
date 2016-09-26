/* Copyright (c)2016 Jouni Roivas <jroivas@iki.fi>
 * MIT license, see LICENSE
 */

#ifndef __DUMMY_H
#define __DUMMY_H

#include <stdint.h>

char *dummy_crypt_seed(const char *src, uint32_t len, const char *key, uint32_t key_len, uint32_t seed_len, uint8_t provide_seed_len);
char *dummy_crypt(const char *src, uint32_t len, const char *key, uint32_t key_len, uint32_t seed_len);
uint32_t dummy_crypt_size_seed(uint32_t len, uint32_t seed_len, uint32_t provide_seed_len);
uint32_t dummy_crypt_size(uint32_t len, uint32_t seed_len);
char *dummy_decrypt_seed(const char *src, uint32_t len, const char *key, uint32_t key_len, uint32_t seed_len);
char *dummy_decrypt(const char *src, uint32_t len, const char *key, uint32_t key_len);

#endif
