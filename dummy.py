#!/usr/bin/env python
# Copyright (c)2016 Jouni Roivas <jroivas@iki.fi>
#  MIT license, see LICENSE

import random
import sys

def dummy_crypt(msg, key, seed=None, seed_size=6):
    """
    >>> dummy_crypt('a', 'b', seed='1234')
    '\\x041234\\xb2'
    >>> dummy_crypt('a', 'b', seed='1234') == '\\x041234\\xb2'
    True
    >>> dummy_crypt('a', 'b', seed='2234') != '\\x041234\\xb2'
    True
    >>> dummy_crypt('a', 'a', seed='1234') != '\\x041234\\xb2'
    True
    >>> dummy_crypt('a', 'c', seed='1234') != '\\x041234\\xb2'
    True
    >>> dummy_crypt('a', 'c', seed='1234') != dummy_crypt('a', 'c')
    True
    >>> dummy_crypt('a', 'b', seed='1') != dummy_crypt('a', 'b', seed='1234')
    True
    >>> dummy_crypt('a', 'b', seed='1')
    '\\x011\\xb2'
    >>> dummy_crypt('a', 'b', seed='1') != dummy_crypt('a', 'b', seed='1234')
    True
    >>> a = dummy_crypt('a', 'b', seed='1')[2:]
    >>> b = dummy_crypt('a', 'b', seed='1234')[5:]
    >>> a == b  # FIXME a known weakness...
    True
    """
    res = ''
    if seed is None:
        seed = ''
        for i in xrange(seed_size):
            seed += chr(random.randint(0, 255))
    res += chr(len(seed))
    res += seed

    key_idx = 0
    seed_idx = 0

    for m in msg:
        res += chr(((ord(m) ^ ord(seed[seed_idx])) + ord(key[key_idx])) % 256)
        key_idx = (key_idx + 1) % len(key)
        seed_idx = (seed_idx + 1) % len(seed)

    return res

def dummy_decrypt(msg, key):
    """
    >>> dummy_decrypt('\\x041234\\xb2', 'b')
    'a'
    >>> dummy_decrypt('\\x041234\\xb2', 'b') == 'a'
    True
    >>> dummy_decrypt('\\x041234\\xb2', 'a') != 'a'
    True
    >>> dummy_decrypt('\\x041234\\xb2', 'c') != 'a'
    True
    >>> key = 'DAdn0AdiO%EeZDEkare273'
    >>> dummy_decrypt(dummy_crypt('12345', key), key)
    '12345'
    >>> dummy_decrypt(dummy_crypt('msg', key), key) == 'msg'
    True
    >>> dummy_decrypt(dummy_crypt('msg', key, seed='13h132'), key) == 'msg'
    True
    """
    res = ''

    seed_len = ord(msg[0])
    seed = msg[1:seed_len+1]
    msg = msg[seed_len+1:]

    key_idx = 0
    seed_idx = 0
    for m in msg:
        res += chr(((ord(m) - ord(key[key_idx])) % 256) ^ ord(seed[seed_idx]))
        key_idx = (key_idx + 1) % len(key)
        seed_idx = (seed_idx + 1) % len(seed)

    return res

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print('Usage: %s message key' % (sys.argv[0]))
        sys.exit(1)
    msg = sys.argv[1]
    key = sys.argv[2]

    e = dummy_crypt(msg, key)
    p = dummy_decrypt(e, key)

    print (e)
    print (p)
