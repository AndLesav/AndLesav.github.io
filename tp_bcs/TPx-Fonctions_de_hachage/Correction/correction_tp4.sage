from hashlib import sha256

def truncated_sha256(m, n_bytes):
    mask = 1 << (n_bytes*8)
    m_trunc = int(m & mask).to_bytes(n_bytes, 'big')
    digest = sha256(m_trunc).digest()[-n_bytes:]
    return int(digest.hex(), 16)

def sha256_32(m):
    return truncated_sha256(m, 4)

def sha256_8(m):
    return truncated_sha256(m, 1)

# Cf. Wikipedia
def floyd(f, x0):
    # Main phase of algorithm: finding a repetition x_i = x_2i.
    # The hare moves twice as quickly as the tortoise and
    # the distance between them increases by 1 at each step.
    # Eventually they will both be inside the cycle and then,
    # at some point, the distance between them will be
    # divisible by the period λ.
    tortoise = f(x0) # f(x0) is the element/node next to x0.
    hare = f(f(x0))
    while tortoise != hare:
        tortoise = f(tortoise)
        hare = f(f(hare))

    # At this point the tortoise position, ν, which is also equal
    # to the distance between hare and tortoise, is divisible by
    # the period λ. So hare moving in circle one step at a time, 
    # and tortoise (reset to x0) moving towards the circle, will 
    # intersect at the beginning of the circle. Because the 
    # distance between them is constant at 2ν, a multiple of λ,
    # they will agree as soon as the tortoise reaches index μ.

    # Find the position μ of first repetition.    
    mu = 0
    tortoise = x0
    while tortoise != hare:
        tortoise = f(tortoise)
        hare = f(hare)   # Hare and tortoise move at same speed
        mu += 1
 
    # Find the length of the shortest cycle starting from x_μ
    # The hare moves one step at a time while tortoise is still.
    # lam is incremented until λ is found.
    lam = 1
    hare = f(tortoise)
    while tortoise != hare:
        hare = f(hare)
        lam += 1
 
    return lam, mu

def find_collision(hash_func):
    x_0 = randint(1, 0x100000000)
    # lam is the cycle length
    # mu is the length of the tail: the number of iteration before finding the cycle
    # mu-1 iteration bring us juste before the loop and lam + mu brings us to the start of the loop
    lam, mu = floyd(hash_func, x_0)

    x = x_0
    for i in range(mu-1):
        x = hash_func(x)
    y = x
    for i in range(lam):
        y = hash_func(y)
    if (x != y  and hash_func(x) == hash_func(y)):
        print("Found a collision!")
        return x, y
    
    print("No collision found...")
    return

find_collision(sha256_32)
