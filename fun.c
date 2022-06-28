#include "fun.h"

/************************/
// Author: Victor Franco
// Date:   06/28/22
/************************/

unsigned char polynomial_multi_gf2_4(unsigned char a,unsigned char b) {
    unsigned char n = 4;
    unsigned char r = 0b0011;
    unsigned char aux = b;
    unsigned char check = 0;
    unsigned char res = a & 1 ? b : 0x00;
    for(int i=1; i<n+1; i++) {
        check = aux & 1<<(n-1) ? 1 : 0;
        aux <<= 1;
        aux &= (1<<n)-1;
        aux = check    ? aux ^ r   : aux;
        res = a & 1<<i ? res ^ aux : res;
    }
    return res;
}

void showMatrix(size_t c,size_t r,unsigned char**m) {
    for(int i=0; i<r; i++) {
        printf("%13s","");
        for(int j=0; j<c; j++) printf("%02X ",m[i][j]);
        printf("\n");
    }
}

void showArray(size_t size,unsigned char*array) {
    for(int i=0; i<size; i++) printf("%02X ",array[i]);
    printf("\n");
}

unsigned char**matrix_multiplication(unsigned char**m1,unsigned char**m2,size_t c1,size_t r1,size_t c2,size_t r2) {
    if(c1 != r2) return NULL;
    unsigned char**result = malloc(r1*sizeof(char*));
    for(int i=0; i<c2; i++) result[i] = malloc(c2*sizeof(char));
    for(int r=0; r<r1; r++) {
        for(int c=0; c<c2; c++) {
            unsigned char res = 0x00;
            for(int i=0; i<c1; i++) res ^= polynomial_multi_gf2_4(m1[r][i],m2[i][c]);
            result[r][c] = res;
        }
    }
    return result;
}

unsigned char**matrixToPointer(size_t c,size_t r,unsigned char m[r][c]) {
    unsigned char**result = malloc(r*sizeof(char*));
    for(int i=0; i<r; i++) result[i] = m[i];
    return result;
}

void showTitle(char*title) {
    int size = 30;
    int padding = (size+strlen(title))/2;
    for(int i=0; i<size; i++) printf("-");
    printf("\n%*s\n",padding,title);
    for(int i=0; i<size; i++) printf("-");
    printf("\n");
}

unsigned char nibbleSub(unsigned char input) {
    unsigned char outputs[] = {0b1110,0b0100,0b1101,0b0001,0b0010,0b1111,0b1011,0b1000,0b0011,0b1010,0b0110,0b1100,0b0101,0b1001,0b0000,0b0111};
    input &= 0x0F;
    return outputs[input];
}

unsigned char i_nibbleSub(unsigned char input) {
    unsigned char outputs[] = {0b1110,0b0011,0b0100,0b1000,0b0001,0b1100,0b1010,0b1111,0b0111,0b1101,0b1001,0b0110,0b1011,0b0010,0b0000,0b0101};
    input &= 0x0F;
    return outputs[input];
}


unsigned char**shiftRow(size_t c,size_t r,unsigned char**m) {
    unsigned char**result = malloc(r*sizeof(char*));
    for(int i=0; i<r; i++) result[i] = malloc(c * sizeof(int));
    result[0] = m[0];
    result[1][0] = m[1][1];
    result[1][1] = m[1][0];
    return result;
}

unsigned char**key_schedule(unsigned char k_[4],int rounds) {
    unsigned char**k = malloc((rounds+1)*sizeof(char*));
    for(int i=0; i<rounds+1; i++) k[i] = malloc(4 * sizeof(int));
    unsigned char w[4*(rounds+1)];
    w[0]  = k_[0];
    w[1]  = k_[1];
    w[2]  = k_[2];
    w[3]  = k_[3];
    memcpy(k[0],w+0,4);
    for(int i=0; i<rounds; i++) {
        w[i*4+4]  = w[i*4+0]^nibbleSub(w[i*4+3])^(i+1);
        w[i*4+5]  = w[i*4+1]^w[i*4+4];
        w[i*4+6]  = w[i*4+2]^w[i*4+5];
        w[i*4+7]  = w[i*4+3]^w[i*4+6];
        memcpy(k[i+1],w+(i+1)*4,4);
    }
    return k;
}

unsigned char*split(unsigned char buffer[2]) {
    unsigned char*result = malloc(4*sizeof(char));
    unsigned char aux[] = {
        buffer[0]>>4,
        0x0F&buffer[0],
        buffer[1]>>4,
        0x0F&buffer[1]
    };
    memcpy(result,aux,4);
    return result;
}

unsigned char*join(unsigned char buffer[4]) {
    unsigned char*result = malloc(4*sizeof(char));
    unsigned char aux[] = {
        (buffer[0]<<4)+(0x0F&buffer[1]),
        (buffer[2]<<4)+(0x0F&buffer[3]),
    };
    memcpy(result,aux,2);
    return result;
}
