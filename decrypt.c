#include "fun.h"

/************************/
// Author: Victor Franco
// Date:   06/28/22
/************************/

unsigned char*decrypt_block(unsigned char k[4],unsigned char*H,unsigned char**state) {
    unsigned char**k_ = key_schedule(k,2);
    unsigned char I[4];
    for(int i=0; i<4; i++) I[i] = H[i] ^ k_[2][i];              // KeyAddition K_2
    unsigned char j[][2] = {{I[0],I[2]},{I[1],I[3]}};
    unsigned char**j_ = shiftRow(2,2,matrixToPointer(2,2,j));   // ShiftRow
    unsigned char J[] = {j_[0][0],j_[1][0],j_[0][1],j_[1][1]};
    unsigned char K[4];
    for(int i=0; i<4; i++) K[i] = i_nibbleSub(J[i]);            // inverse NibbleSub
    unsigned char L[4];
    for(int i=0; i<4; i++) L[i] = K[i] ^ k_[1][i];              // KeyAddition K_1
    unsigned char m[][2] = {{L[0],L[2]},{L[1],L[3]}};
    unsigned char**m_ = matrix_multiplication(state,matrixToPointer(2,2,m),2,2,2,2); // MixColumn
    unsigned char M[] = {m_[0][0],m_[1][0],m_[0][1],m_[1][1]};
    unsigned char**n_ = shiftRow(2,2,m_);                       // ShiftRow
    unsigned char N[] = {n_[0][0],n_[1][0],n_[0][1],n_[1][1]};
    unsigned char O[4];
    for(int i=0; i<4; i++) O[i] = i_nibbleSub(N[i]);            // inverse NibbleSub
    unsigned char P[4];
    for(int i=0; i<4; i++) P[i] = O[i] ^ k_[0][i];              // KeyAddition K_0
    free(m_);
    free(j_);   // free memory
    free(k_);

    unsigned char*result = malloc(4*sizeof(char));
    memcpy(result,P,4); // save result
    return result;
}

unsigned char*decrypt(unsigned char k[4],unsigned char*m_encrypt,unsigned char**state,int length) {
    unsigned char*m_decrypt = malloc(length*sizeof(char));
    for(int i=0; i<length/4; i++)
        memcpy(m_decrypt+i*4,decrypt_block(k,m_encrypt+i*4,state),4);
    return m_decrypt;
}

int main(int argc, char *argv[]) {
    unsigned char m1[][2] = {{0b0011,0b0010},
                             {0b0010,0b0011}};

    unsigned char**state = matrixToPointer(2,2,m1);

    unsigned char k[] = {0b1100,0b0011,0b1111,0b0000};  // set secret key

    FILE*source  = fopen(argv[1],"rb");
    FILE*destiny = fopen(argv[2],"wb");

    int tam = 2;
    unsigned char*enc,*dec,buffer[2];

    while(tam == 2) {
        memset(buffer,0,2);
        tam = fread(buffer,1,2,source);   // get 2 bytes from encrypted file
        enc = split(buffer);              // 2 bytes to 4 nibbles
        dec = decrypt_block(k,enc,state); // decrypt
        dec = join(dec);                  // 4 nibbles to 2 bytes
        fwrite(dec,1,2,destiny);          // save 2 decrypted bytes inside the recovered file
        free(enc);
        free(dec);
    }

    free(state);
    fclose(source);
    fclose(destiny);
    return 0;
}
