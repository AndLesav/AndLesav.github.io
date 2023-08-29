// compile with:
// $ gcc -o  -lgmp

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gmp.h>
#include <time.h>

void mpz_cryptrand(mpz_t rop, const size_t size){
  unsigned char* buf = NULL;
  FILE* f = NULL;

  buf = malloc(size*sizeof(unsigned char));
  if(!buf)
    goto err;

  f = fopen("/dev/urandom", "r");
  if (!f) 
    goto err;

  fread(buf, size, 1, f);
  mpz_import(rop, size, 1, 1, 0, 0, buf);

 err:
  if (buf) free(buf);
  if (f) fclose(f);
}

/* binary decomposition : n = 2^s * d */
void rm_decomp(mpz_t d, int* s, const mpz_t n) {
  mpz_sub_ui(d, n, 1);
  *s = mpz_scan1(d, 0);
  mpz_fdiv_q_2exp(d, d, *s);  
}

/* test if a is a witness for n */
void rm_is_witness(int* b, const mpz_t a, const mpz_t n, const mpz_t d, const int s) {
  mpz_t x, m;
  mpz_inits(x, m, NULL);
  mpz_sub_ui(m, n, 1);
  *b = 1;
  mpz_powm(x, a, d, n);
  /* gmp_printf("a ^ d is: %Zd \n", x); */
  /* gmp_printf("m is: %Zd \n", m); */
  if ( mpz_cmp_ui(x, 1)==0 || mpz_cmp(x, m)==0) {
    *b = 0;
    /* printf("before starting squares, a is a witness? %d\n", *b); */
  } else {
    for (int i = 1; i < s; i++) {
      mpz_powm_ui(x, x, 2, n);
      if (mpz_cmp(x, m)==0) {*b = 0; break;}
      /* gmp_printf("a ^ d is: %Zd \n", x); */
      /* gmp_printf("m is: %Zd  \n", m); */
      /* printf("at this moment, a is a witness? %d\n", *b); */
    }
  }
  mpz_clears(x, m, NULL);
}

void miller_rabin(int* b, const mpz_t n, const int k) {
  mpz_t d, a;
  int s, bw;
  
  mpz_inits(d, a, NULL);
  gmp_randstate_t state;
  gmp_randinit_default(state);
  
  rm_decomp(d, &s, n);
  *b = 1;
  for (int i = 0; i < k; i++) {
    mpz_urandomm(a, state, n);
    rm_is_witness(&bw, a, n, d, s);
    if (bw==1){
      *b = 0;
      break;
    }
  }
}

int main(int argc, char **argv){
  mpz_t n, d, a;
  int s, b=0;
  s = 0;
  
  mpz_inits(n, d, a, NULL);
  gmp_randstate_t state;
  gmp_randinit_default(state);

  
  clock_t start = clock(), diff;
  for (int i =1; i< 100; i++) {
    int count = 0;
    b = 0;
    while (b == 0){
      count ++;
      /* mpz_cryptrand(n, 256/8); */
      mpz_urandomb(n, state, 256);
      rm_decomp(d, &s, n);
  
      /* gmp_printf("n is equal to : %Zd\n", n); */
      /* gmp_printf("d is equal to : %Zd\n", d); */
      /* printf("s is: %d\n", s); */

      miller_rabin(&b, n, 20);
      /* printf("n is a prime : %d\n", b); */

      /* clock_t start2 = clock(), diff2; */
      /* int bt = mpz_probab_prime_p(n, 20); */
      /* printf("n is a prime : %d\n", bt); */
      /* diff2 = clock() - start2; */
      /* int msec2 = diff * 1000 / CLOCKS_PER_SEC; */
      /* printf("Time taken %d seconds %d milliseconds \n", msec2/1000, msec2%1000); */

    }
    printf("number of integers tried: %d\n", count);
  }
  diff = clock() - start;
  int msec = diff * 1000 / CLOCKS_PER_SEC;
  printf("Time taken %d seconds %d milliseconds \n", msec/1000, msec%1000);
  
  mpz_clears(n, d, a, NULL);
  return 0;
}
