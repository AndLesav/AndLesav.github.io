#! /usr/bin/python3

import socket
from binascii import unhexlify, hexlify
import time


def connect(host, port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((host, port))
    return sock


if __name__ == '__main__':
    host = '127.0.0.1'
    port = 8000

    sock = connect(host, port)
    header = sock.recv(4096).decode()
    print(header)
    time.sleep(1)

    sock.send(b'3')
    iv_ct = sock.recv(2048).decode().split('\n')[0]
    print(iv_ct)
    print("We have IV=0x{} and CT=0X{}".format(iv_ct[:32], iv_ct[32:]))

    iv = unhexlify(iv_ct[:32])
    ct = unhexlify(iv_ct[32:])

    sock.send(b'1')
    sock.recv(2048).decode()

    sock.send(bytes(iv_ct, "ascii"))
    print(sock.recv(2048).decode())

    sock.send(b'4')

    sock.close()
