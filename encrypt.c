#include "fun.h"

/************************/
// Author: Victor Franco
// Date:   06/28/22
/************************/

unsigned char*encrypt_block(unsigned char k[4],unsigned char*p,unsigned char**state) {
    unsigned char**k_ = key_schedule(k,2);
    unsigned char A[4];
    for(int i=0; i<4; i++) A[i] = p[i] ^ k_[0][i];              // KeyAddition K_0
    unsigned char B[4];
    for(int i=0; i<4; i++) B[i] = nibbleSub(A[i]);              // NibbleSub
    unsigned char c[][2] = {{B[0],B[2]},{B[1],B[3]}};
    unsigned char**c_ = shiftRow(2,2,matrixToPointer(2,2,c));   // ShiftRow
    unsigned char C[] = {c_[0][0],c_[1][0],c_[0][1],c_[1][1]};
    unsigned char**d = matrix_multiplication(state,c_,2,2,2,2); // MixColumn
    unsigned char D[] = {d[0][0],d[1][0],d[0][1],d[1][1]};
    unsigned char E[4];
    for(int i=0; i<4; i++) E[i] = D[i] ^ k_[1][i];              // KeyAddition K_1
    unsigned char F[4];
    for(int i=0; i<4; i++) F[i] = nibbleSub(E[i]);              // NibbleSub
    unsigned char g[][2] = {{F[0],F[2]},{F[1],F[3]}};
    unsigned char**g_ = shiftRow(2,2,matrixToPointer(2,2,g));   // ShiftRow
    unsigned char G[] = {g_[0][0],g_[1][0],g_[0][1],g_[1][1]};
    unsigned char H[4];
    for(int i=0; i<4; i++) H[i] = G[i] ^ k_[2][i];              // KeyAddition K_2
    free(c_);
    free(d);
    free(g_);   // free memory
    free(k_);

    unsigned char*result = malloc(4*sizeof(char));
    memcpy(result,H,4); // save result
    return result;
}

unsigned char*encrypt(unsigned char k[4],unsigned char*p,unsigned char**state,int length) {
    unsigned char*m_encrypt = malloc(length*sizeof(char));
    for(int i=0; i<length/4; i++)
        memcpy(m_encrypt+i*4,encrypt_block(k,p+i*4,state),4);
    return m_encrypt;
}

int main(int argc, char *argv[]) {
    unsigned char m1[][2] = {{0b0011,0b0010},
                             {0b0010,0b0011}};

    unsigned char**state = matrixToPointer(2,2,m1);

    unsigned char k[] = {0b1100,0b0011,0b1111,0b0000};  // set secret key

    FILE*source  = fopen(argv[1],"rb");
    FILE*destiny = fopen(argv[2],"wb");

    int tam = 2;
    unsigned char*p,*enc,buffer[2];

    while(tam == 2) {
        memset(buffer,0,2);
        tam = fread(buffer,1,2,source); // get 2 bytes from source file
        p   = split(buffer);            // 2 bytes to 4 nibbles
        enc = encrypt_block(k,p,state); // encrypt
        enc = join(enc);                // 4 nibbles to 2 bytes
        fwrite(enc,1,2,destiny);        // save 2 encrypted bytes inside the encrypted file
        free(p);
        free(enc);
    }

    free(state);
    fclose(source);
    fclose(destiny);
    return 0;
}
