#! /usr/bin/python3

import socket
from binascii import unhexlify, hexlify

def connect(host, port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((host, port))
    return sock


if __name__ == '__main__':
    host = '127.0.0.1'
    port = 8000

    # Exemple de connexion
    sock = connect(host, port)
    # Send client challenge
    _ = sock.send(b"AFE25CB896C23101")
    srv_chall = sock.recv(16)
    print(srv_chall)
    
    # Send client check = AES-CFB8(client_chall, key = session_key, iv = 00..00)
    # with session_key = HMAC-SHA256(srv_chall || client_chall, key = secret)
    # we don't know the session key, so we can't actually compute the valid client_check
    # _ = sock.send(client_check)

    # The client is supposed to check the servers credential, but from here, the authentication is valid for to server
    # srv_check = sock.recv(16)
    sock.close()
