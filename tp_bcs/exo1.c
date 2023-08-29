#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gmp.h>

typedef struct {
  mpz_t n;
  mpz_t e;
} RSA_PK;

typedef struct {
  mpz_t n;
  mpz_t d;
} RSA_SK;



/* use /dev/urandom to generate random number of the given size */
void mpz_cryptrand(mpz_t rop, size_t size) {
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

/* question 1a : generating prime  */
void get_prime(mpz_t prime, const size_t len) {
  mpz_cryptrand(prime, (size_t) (len+7)/8); /* generating random number */
  mpz_nextprime(prime, prime);
}

void get_modulus(mpz_t n, const size_t len) {
  mpz_t p, q;
  mpz_init(p); mpz_init(q);
  get_prime(p, len);
  get_prime(q, len);
  mpz_mul(n, p, q);
}


/* extended gcd with one value computed
 expect positive integers to be given as input */
void extended_euclide(mpz_t d, mpz_t u, mpz_t v, const mpz_t a, const mpz_t b) {
  mpz_t s, s_old, r, r_old, tmp, q;

  mpz_inits(s, s_old, r, r_old, q, tmp, NULL);
  mpz_set_ui(s_old, 1);
  mpz_set(r, b);
  mpz_set(r_old, a);
  
  while (mpz_sgn(r) != 0) {
    /* first is euclidean divion : r_old = q * r + r_new */
    mpz_fdiv_qr(q, tmp, r_old, r);
    mpz_set(r_old, r);
    mpz_set(r, tmp);

    /* then is recursive formula for s */
    mpz_set(tmp, s);
    mpz_mul(s, q, s);
    mpz_sub(s, s_old, s);
    mpz_set(s_old, tmp);    
  }
  
  mpz_set(d, r_old);
  mpz_set(u, s_old);
  mpz_set(v, d);
  mpz_submul(v, u, a);
  mpz_cdiv_q(v, v, b);

  if (mpz_sgn(d) == -1) {
    mpz_neg(d, d);
    mpz_neg(u, u);
    mpz_neg(v, v);
  }
  mpz_clears(r, r_old, s, s_old, q, tmp, NULL);
}

/* set u to a^(-1) mod n, if it exists ie gcd(a, n)=1 */
void modular_inverse(mpz_t u, const mpz_t a, mpz_t n) {
  mpz_t d, v, t;
  mpz_inits(d, v, t, NULL);

  extended_euclide(d, t, v, a, n);
  if (mpz_cmp_ui(d, 1) == 0) {
    mpz_set(u, t);
  }
  mpz_clears(d, v, t, NULL);
}

/* textbook RSA keygen with e = 2^16+1 */
void RSA_keygen(RSA_PK *pk, RSA_SK *sk, const size_t len) {
  mpz_t p, q, phi;
  mpz_inits(p, q, phi, NULL);
  mpz_set_ui(pk->e, 65537);

  /* generating p, q such that gcd(e, phi(pq)) = 1 */
  while (mpz_sgn(sk->d) == 0) {
    get_prime(p, (int) len/2);
    get_prime(q, (int) len/2);

    mpz_mul(pk->n, p, q);
    mpz_set(sk->n, pk->n);

    mpz_sub(phi, pk->n, p);
    mpz_sub(phi, phi, q);
    mpz_add_ui(phi, phi, 1);

    modular_inverse(sk->d, pk->e, phi);
  }
  mpz_mod(sk->d, sk->d, phi);

  /* if (mpz_sgn(sk->d) == -1) { */
  /*   mpz_add(sk->d, sk->d, phi); */
  /* } */
  
  mpz_clears(p, q, phi, NULL);
}

/* compute pow := m^e mod n  (use function from gmp) */
void modular_exp(mpz_t pow, const mpz_t m, const mpz_t e, const mpz_t n) {
    mpz_powm_sec (pow, m, e, n);
}


/* convert string m to int using gmp function */
void string2int(mpz_t out, const char *m, const size_t len) {
  mpz_import (out, len, 1, 1, 0, 0, m);
}

/* convert string m to int using gmp function */
void int2string(char *out, size_t *len, const mpz_t m) {
  mpz_export (out, len, 1, sizeof(*out), 0, 0, m);
}


void RSA_encrypt (mpz_t c, const char *m, const RSA_PK pk) {

  size_t len = strlen(m);
  
  /* convert string m to int */
  string2int(c, m, len);
  gmp_printf("m int before encryption is: %Zd\n", c);

  /* encrypt m using modular exponentiation */
  modular_exp(c, c, pk.e, pk.n);
    
}


void RSA_decrypt (char **m_r, size_t *len, const mpz_t c, const RSA_SK *sk) {
  mpz_t tmp;
  mpz_init(tmp);
  
  /* decryption using modular exponentiation and the secret key */
  modular_exp(tmp, c, sk->d, sk->n);
  gmp_printf("m int recovered is: %Zd\n", tmp);
  
  *m_r = malloc((mpz_sizeinbase(tmp, 2) + 7) / 8 + 1);
  
  /* conversion back into string */
  int2string(*m_r, len, tmp);
  
  mpz_clear(tmp);
}


int main(int argc, char **argv) {


  /* mpz_t random_num, prime, modulus, a, b, u, v, d; */
  /* mpz_init(random_num); */
  /* mpz_init(modulus); */
  /* mpz_inits(a, b, u, v, d, NULL); */
  
  /* /\* test for random number *\/ */
  /* mpz_cryptrand(random_num, 32); */
  /* gmp_printf("random number: %Zd\n", random_num); */
  /* mpz_clear(random_num); */

  /* /\* test for random prime *\/ */
  /* mpz_init(prime); */
  /* get_prime(prime, 1024); */
  /* gmp_printf("random prime: %Zd\n", prime); */
  /* mpz_clear(prime); */

  /* /\* test for modulus *\/ */
  /* get_modulus(modulus, 1024); */
  /* gmp_printf("random modulus: %Zd\n", modulus); */
  /* mpz_clear(modulus); */

  /* /\* test for extended gcd *\/ */
  /* mpz_cryptrand(a, 5); */
  /* mpz_cryptrand(b, 5); */
  /* gmp_printf("random elements: %Zd\t%Zd\n", a, b); */
  /* extended_euclide(d, u, v, a, b); */
  /* gmp_printf("output of extended gcd is: %Zd\t%Zd\t%Zd\n", d, u, v); */
  /* mpz_clears(a, b, u, v, d, NULL); */

  /* test for keygen  */

  
  RSA_PK pk;
  RSA_SK sk;
  mpz_inits(pk.n, pk.e, sk.n, sk.d, NULL);
  
  RSA_keygen(&pk, &sk, 1024);
  gmp_printf("e is: %Zd\n", pk.e);
  gmp_printf("n is: %Zd\n", pk.n);
  gmp_printf("d is: %Zd\n", sk.d);

  
  char m[] = "word";


  mpz_t c;
  size_t len;

  mpz_init(c);
  
  RSA_encrypt (c, m, pk);
  gmp_printf("c = 0x%Zx\n", c);
  char* m_r;
  RSA_decrypt (&m_r, &len, c, &sk);
  printf("m recovered: %s\n", m_r);
  free(m_r);
  mpz_clears(pk.n, pk.e, sk.n, sk.d, c,  NULL);

  return 0;
}


 /* compile with: */
 /* $ gcc -o gmp_rand mpz_rand.c -lgmp */

