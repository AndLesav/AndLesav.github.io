#! /usr/bin/python3

import sys
import socket
import oracle
from threading import *

header = "##   Welcome to Encryptor2020 ! ###\n" \
         + "We encrypted a message using a secret key. Try to recover it!\n" \
         + "1 : Padding oracle lvl  1 : error code\n" \
         + "2 : Padding oracle lvl  2 : side channel\n" \
         + "3 : Get the ciphertext (IV || CT)\n" \
         + "4 : Exit"


""" Simple class to deal with client requests """
class client(Thread):
    def __init__(self, sock, address):
        Thread.__init__(self)
        self.sock = sock
        self.addr = address
        self.cbc_oracle = oracle.Oracle()
        self.start()

    def run(self):
        self.sock.send(header.encode())
        # Client choose an option, and can submit a ciphertext as an hexadecimal string
        while True:
            # self.sock.send(b'\n>>> ')
            response = self.sock.recv(1).decode().strip()
            if response == "1" or response == "2":
                self.cbc_oracle.set_lvl(int(response))
                self.sock.send(b'Enter ciphertext (as an hex string) : ')
                ct = self.sock.recv(1024).decode().strip()
                result = self.cbc_oracle.decrypt(ct)
                self.sock.send(result)
            elif response == "3":
                self.sock.send(bytes(self.cbc_oracle.ct))
            elif response == "4":
                self.sock.send(b'Bye!\n')
                self.sock.close()
                break


def main(argv):
    serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    host, port = 'localhost', 8000

    if len(argv) == 2:
        port = int(argv[1])
    elif len(argv) >= 3:
        host, port = argv[1], int(argv[2])

    serversocket.bind((host, port))
    serversocket.listen(5)
    sys.stderr.write('Listening {}:{}\n'.format(host, port))
    while True:
        clientsocket, address = serversocket.accept()
        client(clientsocket, address)


if __name__ == '__main__':
    main(sys.argv)
