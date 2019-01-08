## Synopsis

A C implementation of common cryptographic algorithms. This is, by no means, meant to be used in production
as its mostly just for learning.

## Contents
- Ciphers
  - AES
- Pseudo Random Number Generators
  - Linear Congruential Generator
  - Middle Square Weyl Sequence (TODO: Randomly Generate Seeds)

## AES
This implementation only supports 128 bit keys, though in the future I plan on adding support
for 192 and 256 bit keys as well. In order to use this library, they key has to be expanded
and packed into an AES_KEY struct. This is then passed to encrypt/decrypt along with the input
message that needs to be exactly 128 bits. There is also support for AES CBC as long as the
input is a multiple of 128 (padding is not automatically added).

#### Usage
```
// Packs AES_KEY with the expanded round keys used for encryption and decryption
aes_set_key(key, &aes_key, 16);

// Encrypts plaintext and copys the result to ciphertext
aes_encrypt(plaintext, ciphertext, &aes_key);

// Decrypts ciphertext copying the result to plaintext
aes_decrypt(ciphertext, plaintext, &aes_key);

// Loops over each block of plaintext encrypting and XORing with the previous block
aes_encrypt_cbc(plaintext, ciphertext, sizeof(ciphertext), &aes_key, iv);
    
```

#### Invaluable Resources
https://en.wikipedia.org/wiki/Advanced_Encryption_Standard
https://en.wikipedia.org/wiki/Rijndael_key_schedule
https://github.com/B-Con
https://github.com/kokke/tiny-AES-c

## Author

2018 Matthew Emerson

## License

Released under MIT License.