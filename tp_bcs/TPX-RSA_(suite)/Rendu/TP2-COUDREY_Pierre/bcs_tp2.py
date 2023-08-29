#!/usr/bin/env sage
# Auteur : Pierre COUDREY, M2 Cybersecurite, 2020/2021
import binascii
from sage.all import *


def int_to_ascii(x):
    return binascii.unhexlify(hex(x).replace('0x', '').replace('L', '')).decode()


def decrypt_c1():
    e = 65537
    # Variables for exercice 1.1
    n1 = 0xd2a0399be4db182811c7b222c374360e684f5693f835f2477d5e1696e368f7efc1c03a4aeec85b9753a70d2db4e85623293852fa54de24bd1dbdb490af5e5fcc67777f6f1e3823e2db06c071e0b9cf44a6e8c26d10026fe5a58dfa27fe02eda5bbda5f1bf227de2741753d7cef849d054622dee7a2cf45297bffeeb1212cc9e5a23bf68b6e761a5b0342944eb208a64baad7830afc368b5da7d8b34246a880db3c8c7b11f8c014ecbdd8e7c577c2edb162231e0853a619adfb8159660c956fca3d91ceb44e7ad1aed74905bddddcdb46a8e50c339eb3bdedb2cf5276433bf7df47d06c2af42c0ef3905743bdc2dc7dff3bed96de977c3dda28f2fb17e02744c1
    c1 = 0x3af0ce642453b6e91a56cc6a105a7b3890e83e4c2830ac3b98c170c3b6caa629a155bd0e6e419f3d07d4e285c69e483dbc2abbaa80f080bdf883ce6cd76a95e2ee302a7b766bffd3afa50ad66333f5faa1f78b12f018b30394843b8d000f956f74982af3fbe4ac51aedd71899a7942cd5a7b6ce6dbfebfa4eac2b7a8b38852d5639bd5902d2ff3f2ecb06ff74414f86b571ab069b3510567e7fd356a73a70c9c41d675623510ad51024c03b1a97131c18dd6fcd393bb14657258cb86123f4651bc257d7db2e09d8e40a17fdbb327bb803155f6904ae258f457d504984dd3614bf5e2237f3114fe93633bf7a630dc6777a5f7199a0f6c82fabf5f29965adc2b5d
    p = sqrt(n1)
    phi = (p-1)*p
    d = inverse_mod(e, phi)
    flag = power_mod(c1, d, n1)
    return int_to_ascii(flag)


def pollard(n, bound):
    a = 2
    for i in range(2, bound):
        a = power_mod(a, i, n)
        d = gcd(a-1, n)
        if 1 < d < n:
            return d


def decrypt_c2():
    # Variable for exercice 1.2
    e = pow(2, 16) + 1
    B = 150  # Bound for Pollard p-1 algorithm
    n2 = 0x7db7520e7eafe6b649919a766295a8c582d5a1492e754696a05035b492cc6b0180c9b741282137c674121f40c74a85edbd3d25c795f065db3a2de0d005b9a4f4e0cf6f37aa200f206fb870ef6f3cefe456c655ae3f61272d927955ab74b3c4fad8cd0d5c68d9c14e8a2b86443524b82a03691014df1a4ea48ff5dc8c74f5b5e406e36f62bb97dd5db44fa522ee0d05a194d
    c2 = 0x175335d7d9c6b204ccca4c90b43b3817acc7fa3ec259e999c4c30baf65b8187424f3fc14a7ed45961cac716a539ba1cbe1afbdb773dac45df1d264c21e9a13ef88b7339672e42e86a2c20507ed0813a0a742936f5246e04e6ed0edcfae3555629f4d0b460659a0fe9de5a2d1cbb849adacb1d29e2ded2d214923bd68e4c67b5927540dde12f0d1807d4665b59d8bab37c13

    p, q = 0, 0
    r = n2
    while True:
        p = pollard(n2, B)
        r = r//p
        if is_prime(r):
            q = r
            break

    phi = (p - 1) * (q - 1)
    d = inverse_mod(e, phi)
    flag = power_mod(c2, d, n2)
    return int_to_ascii(flag)


def decryption_oracle():
    # https://crypto.stackexchange.com/questions/2323/how-does-a-chosen-plaintext-attack-on-rsa-work
    n = 456378902858290907415273676326459758501863587455889046415299414290812776158851091008643992243505529957417209835882169153356466939122622249355759661863573516345589069208441886191855002128064647429111920432377907516007825359999
    e = 65537
    ct = 41662410494900335978865720133929900027297481493143223026704112339997247425350599249812554512606167456298217619549359408254657263874918458518753744624966096201608819511858664268685529336163181156329400702800322067190861310616

    c_a = power_mod(2, e, n)
    c_b = c_a * ct

    # nc = Netcat('challenge01.root-me.org', 51031)
    # nc.read()
    # nc.write(bytes(str(c_b), 'utf-8'))
    # nc.write(bytes("\n", 'utf-8'))
    # nc.read()
    # nc.close()

    c_bd = 1458414996286361336171022694278618963923904557163518886908519150023343172644758356895532151523030

    flag = (c_bd // 2) % n
    return int_to_ascii(flag)


def main():
    print("=" * 10 + "Challenge 1" + "=" * 10)
    print(decrypt_c1())
    print("=" * 10 + "Challenge 2" + "=" * 10)
    print(decrypt_c2())
    print("=" * 10 + "Challenge decryption oracle" + "=" * 10)
    print(decryption_oracle())


if __name__ == "__main__":
    main()
