from hashlib import sha256

def truncated_sha256(m, n_bytes):
    mask = 1 << (n_bytes*8)
    m_trunc = int(m % mask).to_bytes(n_bytes, 'big')
    digest = sha256(m_trunc).digest()[-n_bytes:]
    return int(digest.hex(), 16)

def sha256_32(m):
    return truncated_sha256(m, 4)
