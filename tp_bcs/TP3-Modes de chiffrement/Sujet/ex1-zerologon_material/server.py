#! /usr/bin/python3

import sys
import socket
from threading import *
from binascii import unhexlify, hexlify
from Crypto.Hash import HMAC, SHA256
from cfb8 import CFB8


def KDF(secret, data):
	h = HMAC.new(secret, digestmod=SHA256)
	h.update(data)
	return h.digest()

""" Simple class to deal with client requests """
class client(Thread):
	def __init__(self, socket, address, secret):
		Thread.__init__(self)
		self.sock = socket
		self.addr = address
		self.secret = secret
		self.start()


	def recieve_cli_challenge(self):
		self.cli_chall = unhexlify(self.sock.recv(1024).decode().strip())
		# Generate a random server challenge
		with open('/dev/urandom', 'rb') as fp:
			self.srv_chall = fp.read(8)

		# Compute the session key from the secret and the challenges
		self.session_key = KDF(self.secret, self.cli_chall + self.srv_chall)

	def recieve_cli_cred(self):
		# print("waiting for cli creds")
		# cli_creds = AES-CFB8(cli_chall, key = session_key, iv = 00..00)
		cli_creds = unhexlify(self.sock.recv(1024).decode().strip())
		# print("recieved ", cli_creds)

		# Check creds
		aes = CFB8(self.session_key, bytes(16))
		if aes.decrypt(cli_creds) != self.cli_chall:
			# print("Expected ", aes.encrypt(self.cli_chall), " but got ", cli_creds)
			raise ValueError("Credential do not match")

	def send_srv_chall(self):
		self.sock.send(self.srv_chall)
		# print("challenge sent")
		return 

	def send_srv_cred(self):
		aes = CFB8(self.session_key, bytes(16))
		srv_cred = aes.encrypt(self.srv_chall)
		self.sock.send(srv_cred + b'You are successful authenticated!')
		print("Authentication successful!")

	def run(self):
		# Client choose an option, and can submit a ciphertext as an hexadecimal string
		while True:
			try:
				self.recieve_cli_challenge()
				self.send_srv_chall()
				self.recieve_cli_cred()
				self.send_srv_cred()
			except Exception as e:
				# print("Exception ", e)
				self.sock.send(b'Authentication failed')
			finally:	
				self.sock.close()
				break


def main(argv):
	serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	host, port = 'localhost', 8000

	if len(argv) == 2:
		port = int(argv[1])
	elif len(argv) >= 3:
		host, port = argv[1], int(argv[2])

	# Generate the secret known bye the server only
	with open('/dev/urandom', 'rb') as fp:
		secret = fp.read(16)

	serversocket.bind((host, port))
	serversocket.listen(256)
	sys.stderr.write('Listening {}:{}\n'.format(host, port))
	while True:
		clientsocket, address = serversocket.accept()
		client(clientsocket, address, secret)

if __name__ == '__main__':
	main(sys.argv) 
