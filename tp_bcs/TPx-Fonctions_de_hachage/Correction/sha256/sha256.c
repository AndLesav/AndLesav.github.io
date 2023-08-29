#include <stdio.h>
#include <string.h>
#include "sha256.h"

const uint32_t K[64] ={
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};


void sha256_transform(sha256_state  *ctx){
    uint32_t w[64], a, b, c, d, e, f, g, h, tmp1, tmp2;
    uint8_t i, j; // Counters

    for(i=0, j=0 ; i<16 ; i++, j+=4)
        w[i] =  BYTES_2_WORD(ctx->block[j], ctx->block[j+1], ctx->block[j+2], ctx->block[j+3]);
    for( ; i<64 ; i++)
        w[i] = SIGMA_1(w[i-2]) + w[i-7] + SIGMA_0(w[i-15]) + w[i-16];

    a = ctx->h[0]; b = ctx->h[1]; c = ctx->h[2]; d = ctx->h[3]; 
    e = ctx->h[4]; f = ctx->h[5]; g = ctx->h[6]; h = ctx->h[7];

    for( i=0 ; i<64 ; i++ ){
        tmp1 = h + SUM_1(e) + CH(e,f,g) + K[i] + w[i];
        tmp2 = SUM_0(a) + MAJ(a,b,c);
        h = g;
        g = f;
        f =e;
        e = d + tmp1;
        d = c;
        c = b;
        b = a;
        a = tmp1 + tmp2;
    }

    ctx->h[0] += a;
    ctx->h[1] += b;
    ctx->h[2] += c;
    ctx->h[3] += d; 
    ctx->h[4] += e;
    ctx->h[5] += f;
    ctx->h[6] += g;
    ctx->h[7] += h;
}


void sha256_init(sha256_state *ctx){
    ctx->h[0] = 0x6a09e667;    
    ctx->h[1] = 0xbb67ae85;
    ctx->h[2] = 0x3c6ef372;
    ctx->h[3] = 0xa54ff53a;
    ctx->h[4] = 0x510e527f;
    ctx->h[5] = 0x9b05688c;
    ctx->h[6] = 0x1f83d9ab;
    ctx->h[7] = 0x5be0cd19;    

    ctx->blockLength = 0;
    ctx->bitLength = 0;
}

void sha256_update(sha256_state *ctx, const char *data, const uint64_t size){
    uint64_t i;

    for( i=0 ; i<size ; i++ ){
        ctx->block[ctx->blockLength] = data[i];
        ctx->blockLength++;
        if( ctx->blockLength == SHA256_BLOCK_SIZE ){
            sha256_transform(ctx);
            ctx->blockLength = 0;
            ctx->bitLength += (SHA256_BLOCK_SIZE << 3);
        }
    }
}

void sha256_final(sha256_state *ctx, char* hash){
    uint8_t i = ctx->blockLength;

    ctx->bitLength += ctx->blockLength * 8;

    if( ctx->blockLength < 56 ){
        ctx->block[i++] = 0x80;
        while( i<56 )
            ctx->block[i++] = 0;
    }
    else{
        ctx->block[i++] = 0x80;
        while( i<64 )
            ctx->block[i++] = 0;
        sha256_transform(ctx);
        memset(ctx->block, 0, 56);
    }

    // Add the length of the message (in bits) at on the last 8 bytes of the block
    ctx->block[56] = (ctx->bitLength & 0xff00000000000000) >> 56; 
    ctx->block[57] = (ctx->bitLength & 0xff000000000000) >> 48; 
    ctx->block[58] = (ctx->bitLength & 0xff0000000000) >> 40; 
    ctx->block[59] = (ctx->bitLength & 0xff00000000) >> 32; 
    ctx->block[60] = (ctx->bitLength & 0xff000000) >> 24; 
    ctx->block[61] = (ctx->bitLength & 0xff0000) >> 16; 
    ctx->block[62] = (ctx->bitLength & 0xff00) >> 8; 
    ctx->block[63] = (ctx->bitLength & 0xff) ;  
    sha256_transform(ctx);
    
    for( i = 0 ; i<8 ; i++){
        hash[i*4  ] = (ctx->h[i] & 0xff000000) >> 24;
        hash[i*4+1] = (ctx->h[i] & 0xff0000) >> 16;
        hash[i*4+2] = (ctx->h[i] & 0xff00) >> 8;
        hash[i*4+3] = (ctx->h[i] & 0xff);
    }
}

void test_sha256() {
    char hash[SHA256_DIGEST_SIZE] = "";
    sha256_state sha256;

    char test1[] = "abc";
    char result1[SHA256_DIGEST_SIZE] = {
        0xba,0x78,0x16,0xbf,0x8f,0x01,0xcf,0xea,0x41,0x41,0x40,0xde,0x5d,0xae,0x22,0x23,
        0xb0,0x03,0x61,0xa3,0x96,0x17,0x7a,0x9c,0xb4,0x10,0xff,0x61,0xf2,0x00,0x15,0xad};

    char test2[] = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
    char result2[SHA256_DIGEST_SIZE] = {
        0x24,0x8d,0x6a,0x61,0xd2,0x06,0x38,0xb8,0xe5,0xc0,0x26,0x93,0x0c,0x3e,0x60,0x39,
        0xa3,0x3c,0xe4,0x59,0x64,0xff,0x21,0x67,0xf6,0xec,0xed,0xd4,0x19,0xdb,0x06,0xc1};

    sha256_init(&sha256);
    sha256_update(&sha256, test1, strlen(test1));
    sha256_final(&sha256, hash);
    if( !memcmp(hash, result1, SHA256_DIGEST_SIZE) )
        printf("Test 1: OK\n");
    else
        printf("Test 1: NOK\n");


    sha256_init(&sha256);
    sha256_update(&sha256, test2, strlen(test2));
    sha256_final(&sha256, hash);
    if( !memcmp(hash, result2, SHA256_DIGEST_SIZE) )
        printf("Test 2: OK\n");
    else
        printf("Test 2: NOK\n");

}


int main(int argc, char ** argv){
    test_sha256();    
    
    return 0;
}