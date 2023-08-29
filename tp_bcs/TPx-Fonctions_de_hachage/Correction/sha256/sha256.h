#ifndef DEF_SHA256
    #define DEF_SHA256
    
    #include <stdint.h>

    #define SHA256_BLOCK_SIZE 64
    #define SHA256_DIGEST_SIZE 32
    
    #define ROTR(x,n) ( ((x)>>(n)) | ((x)<<(32-(n))) )
    #define SHR(x,n) ((x) >> (n))
    #define CH(x,y,z) ((x & y)^(~x & z))
    #define MAJ(x,y,z) ((x & y)^(x & z)^(y & z))
    #define SUM_0(x) ((ROTR(x,2)) ^ (ROTR(x,13)) ^ (ROTR(x,22)))
    #define SUM_1(x) ((ROTR(x,6)) ^ (ROTR(x,11)) ^ (ROTR(x,25)))
    #define SIGMA_0(x) ((ROTR(x,7)) ^ (ROTR(x,18)) ^ (SHR(x,3)))
    #define SIGMA_1(x) ((ROTR(x,17)) ^ (ROTR(x,19)) ^ (SHR(x,10)))
    #define BYTES_2_WORD(a,b,c,d)   (((a) << 24) |\
                                    ((b) << 16) |\
                                    ((c) <<  8) |\
                                     (d))           
                                     
    
    typedef struct{
        // block of data to process (512 bits for SHA256)
        uint8_t     block[SHA256_BLOCK_SIZE]; 
        // hash value
        uint32_t    h[8];        
        // Number of bytes added to the block (between 0 and 64)
        uint8_t     blockLength;  
        // Total number of bits processed so far
        uint64_t    bitLength;              
    } sha256_state;


    void sha256_init(sha256_state*);
    void sha256_update(sha256_state*, const char*, const uint64_t);
    void sha256_final(sha256_state*, char*);
#endif