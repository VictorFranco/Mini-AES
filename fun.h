#ifndef FUN_H
#define FUN_H
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

/************************/
// Author: Victor Franco
// Date:   06/28/22
/************************/

unsigned char polynomial_multi_gf2_4(unsigned char a,unsigned char b);
void showMatrix(size_t c,size_t r,unsigned char**m);
unsigned char**matrix_multiplication(unsigned char**m1,unsigned char**m2,size_t c1,size_t r1,size_t c2,size_t r2);
unsigned char**matrixToPointer(size_t c,size_t r,unsigned char m[r][c]);
void showTitle(char*title);
unsigned char nibbleSub(unsigned char input);
unsigned char**shiftRow(size_t c,size_t r,unsigned char**m);
unsigned char**key_schedule(unsigned char k_[4],int rounds);
void showArray(size_t size,unsigned char*array);
unsigned char i_nibbleSub(unsigned char input);
unsigned char*split(unsigned char buffer[2]);
unsigned char*join(unsigned char buffer[4]);

#endif
