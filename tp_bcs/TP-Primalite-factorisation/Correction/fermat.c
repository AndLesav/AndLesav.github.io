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

/* pollard p-1 factorisation algorithm */
void factor_fermat(mpz_t d, const mpz_t n, gmp_randstate_t state) {
  mpz_t exp, tmp, a, q;
  mpz_inits(exp, tmp, a, q, NULL);

  mpz_urandomm(a, state, n);
  gmp_printf("a is: %Zd\n", a);
  
  mpz_set_ui(q, 1);
  mpz_set_ui(tmp, 1);
  size_t sB = mpz_sizeinbase(B, 2);

  while (mpz_cmp(q , B) == -1 && mpz_cmp_ui(tmp, 1) == 0) {
    mpz_nextprime(q, q);
    /* gmp_printf("q is: %Zd\n", q); */
    size_t sq = mpz_sizeinbase(q, 2);
    int e = sB / sq;
    /* printf("e is: %d\n", e); */
    mpz_pow_ui(exp, q, e);
    /* gmp_printf("exp is: %Zd\n", exp); */
    mpz_powm(a, a, exp, n);
    mpz_sub_ui(tmp, a, 1);
    mpz_gcd(tmp, tmp, n);
    /* gmp_printf("tmp is: %Zd\n", tmp); */
    /* gmp_printf("B is: %Zd\n", B); */
  }
  mpz_set(d, tmp);
 
  mpz_clears(exp, tmp, a, q, NULL);
}


int main(int argc, char **argv){

  mpz_t d, n, B, seed;
  mpz_inits(d, n, B, seed, NULL);

  gmp_randstate_t state;
  gmp_randinit_default(state);
  mpz_cryptrand(seed, 100);
  gmp_randseed(state, seed);
  mpz_urandomb(n, state, 200);
  mpz_set_str(n, "117827681420271584017432903522327303325344948050665323956545863", 10);

  /* mpz_set_ui(n, 637365653465547); */
  
  gmp_printf("n is: %Zd\n", n);
  mpz_set_ui(B, 1);
  mpz_set_ui(d, 1);
  while (mpz_cmp_ui(d, 1)==0) {
    mpz_mul_ui(B, B, 2);
    factor_pollard(d, n, B, state);
    gmp_printf("Divisor of n retrieved: %Zd\n", d);
  }
  gmp_printf("B is: %Zd\n", B);
    mpz_clears(d, n, B, seed, NULL);
  
  return 0;
}
