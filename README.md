# Dummy crypt

Sometimes one does not need real cryptographically strong encryption,
but just easy way to obfuscate data.

Sometimes implemetation of a real cryptographically secure algorithm
may need effort, extra code, space or usage of 3rd party libraries.

This is a simple crypto, which suits most of the needs for just obfuscating files.
Cracking it should not be that trivial if used properly.
For example for obfuscating save file in a game, this is "good enough".

To achieve best security level use long enough seed and long enough key.
On optimal case key is as long as the encrypted data (or a bit longer).

## Details

 - Key is the encryption key. It's used as-is, and looped if necessary.
 - Seed is random seed, looped as well.
 - Plaintext is data to be encrypted

Algorithm works like:

 1. Generate seed
 2. Write seed (and seed length) to output buffer
 3. Go thorough message one byte at time
 4. `encrypted_character = ((message_character ^ seed_char) + key_char) % 256`
 5. Get next message character, seed character and key character

Decryption is almost the same:

 1. Read seed (and seed length) from message
 2. Go thorough message one byte at time
 3. `message_character = ((encrypted_character - key_char) ^ seed_char) % 256`

### Analysis

Algorithm uses two simple ways to encrypt message:

 1. XOR with random seed
 2. Plus modulus 256

First of all [XOR](https://en.wikipedia.org/wiki/XOR_cipher) is known simple crypt itself, with known weaknesess.
We use it to preprocess the plaintext.

This preprocessed data is feed to the real cipher, which is simply: `(a + b) % 256`
It's basically same as [substitution cipher](https://en.wikipedia.org/wiki/Substitution_cipher)
similar to [ROT-13](https://en.wikipedia.org/wiki/ROT13)
but we use all 8-bit values (256), and variable substitution.

Strength lies in seed size combined with key size. Bigger seed and key results better security.
By exposing seed, it's possible to easy cracking of this code.
However how this is build: first XOR then plus and modulus, causes cracking to be harder.

REMARK: This should not EVER be considered as cryptographically safe.
Use something like [AES](https://en.wikipedia.org/wiki/Advanced_Encryption_Standard) instead.
This is meant for simple, easy and fast encryptions, where breach of the data is not that crucial.

## Building

To build C version you need [meson](http://mesonbuild.com/) and [ninja](https://ninja-build.org/).

After that:

    mkdir build
    cd build
    meson ..
    ninja

To test C version:

    ./dummy_crypt "Message to be encrypted" "Your key!"

To test Python version:

    ./dummy.py "Message to be encrypted" "Your key!"
