# Mini-AES
![](https://img.shields.io/badge/c99-004283?style=for-the-badge&logo=c&logoColor=white&labelColor=101010)

This program is an implementation of the [Mini-AES](https://piazza.com/class_profile/get_resource/ixlc30gojpe5fs/iyv0273azwtz4) published in Cryptologia by Raphael Chung-Wei Phan in 2002.

Mini-AES has parameters significantly reduced while preserving its original structure.
It is meant to be a purely educational cipher and is **not secure** for actual applications. 
Mini-AES encrypts files in blocks of 16 bits (2 bytes) and the secret key has the same size.

## Encryption
```bash
gcc encrypt.c fun.c -o encrypt.out
./encrypt.out source.pdf encrypted_file
```

## Desencryption
```bash
gcc decrypt.c fun.c -o decrypt.out
./decrypt.out encrypted_file recovered_file.pdf
```
## Compiler support
* C99
* C11
* C17 or more
