from hashlib import sha256

def hash_to_int(m):
	return int(sha256(m.encode()).digest().hex(), 16)

######################################
#           Exercice 2-3             #
######################################

# NIST's P-256 parameters
p = 2**256 - 2**224 + 2**192 + 2**96 - 1 #  prime defining the field
# y^ = x^3 + ax + b
a = -3
b = 41058363725152142129326129780047268409114441015993725554835256314039467401291
Gx = 0x6B17D1F2E12C4247F8BCE6E563A440F277037D812DEB33A0F4A13945D898C296
Gy = 0x4FE342E2FE1A7F9B8EE7EB4A7C0F9E162BCE33576B315ECECBB6406837BF51F5
n = 115792089210356248762697446949407573529996955224135760342422259061068512044369 # G's order

# Create the curve and the generator on sage
P256 = EllipticCurve(Zmod(p), [a, b])
G = P256([Gx, Gy])

# Public key to verify the signatures 
P = P256(0x7453873d3b072c061d03c3d09aba344b6a25f406a3dad40b0281025824bda4a3, 0x9e41b56ecaaf199fbb6d33fb2d748ec454c128aa7be74a1998cf57de34b7988b)


# Exercice 3.1
m1 = "Message important"
r1 = 0x62286e65a6703cdff07b19dba56c34c84d9be61edfac86b347d7fc337615a3bf
s1 = 0xc4926a0932e3d1f6aeec3c6a55926aaa73911c2b7a1325b2cc2e45ed70271578
k1 = 0x7963d7dadb13b6c53864c267dad2e94e52c00e41eaabd03c6f5c559ef912722b

# Exercice 3.2
m2_1 = "Deux messages différents, signés utilisant le même nonce "
r2_1 = 0xa6b2cbe4982361d0132b720c698ae5cb866271aa3f39b564018848a3bb7fbe07
s2_1 = 0x700f22bbdaa2027936bff58819fa674a2ff83ff120d18990b6cddc933b87fd89
m2_2 = "sont facilement reconnaissables par leur signature. "
r2_2 = 0xd76446168b18bc500812599efb6e37e9bf4467614f943aa55901ef1f3a1a57ef
s2_2 = 0x947016dd00edd5bab0a2c91dbeff335bbe4b22449c91cb20a3766ed80aa32bd1
m2_3 = "Il est alors possible de retrouver une information secrète..."
r2_3 = 0xa6b2cbe4982361d0132b720c698ae5cb866271aa3f39b564018848a3bb7fbe07
s2_3 = 0x1437dd0ebd91efcebe8bf7d93c8a4a5ba3b7a900bf1a1a37da20a3a1e7a4d8fa
