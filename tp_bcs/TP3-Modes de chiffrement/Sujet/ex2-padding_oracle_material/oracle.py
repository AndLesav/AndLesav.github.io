from Crypto.Cipher import AES
# from Crypto.Util.Padding import pad, unpad
from binascii import unhexlify, hexlify
import time

def pad(data_to_pad, block_size):
	"""Apply a custom padding. The padding will look like
	00
	0001
	000102
	00010203
	...
	000102030405060708090A0B0C0D0E0F
	Args:
	  data_to_pad (byte string):
		The data that needs to be padded.
	  block_size (integer):
		The block boundary to use for padding. The output length is guaranteed
		to be a multiple of :data:`block_size`.
	Return:
	  byte string : the original data with the appropriate padding added at the end.
	"""
	padding_len = block_size-len(data_to_pad)%block_size
	padding = bytes([i for i in range(padding_len)])
	return data_to_pad + padding


def unpad(padded_data, block_size, style='pkcs7'):
	"""Remove standard padding.
	Args:
	  padded_data (byte string):
		A piece of data with padding that needs to be stripped.
	  block_size (integer):
		The block boundary to use for padding. The input length
		must be a multiple of :data:`block_size`.
	Return:
		byte string : data without padding.
	Raises:
	  ValueError: if the padding is incorrect.
	"""

	pdata_len = len(padded_data)
	if pdata_len % block_size:
		raise ValueError("Input data is not padded")
	padding_len = padded_data[-1]+1
	if padding_len<1 or padding_len>min(block_size, pdata_len):
		raise ValueError("Padding is incorrect.")
	for i in range(1, padding_len+1):
		if padded_data[-i] != padding_len-i:
			raise ValueError("Padding is incorrect.")
	return padded_data[:-padding_len]

class Oracle(object):
	"""docstring for Oracle"""
	def __init__(self):
		with open('/dev/urandom', 'rb') as fp:
			self.key = fp.read(16)
		
		# Encrypt the secret data using a newly generated key
		aes = AES.new(self.key, AES.MODE_CBC)
		data = b"................Haha this is a super secret message! You will never find it!"
		ct = aes.encrypt(pad(data, AES.block_size))

		iv = hexlify(aes.iv)
		iv = bytes("0"*(len(iv)%2), "ascii") + iv
		ct = hexlify(ct)
		ct = bytes("0"*(len(ct)%2), "ascii") + ct
		self.ct = iv + ct
		self.lvl = 1

	def set_lvl(self, lvl):
		self.lvl = lvl
		
	def decrypt(self, data):
		if len(data) % 16 != 0:
			return b"AES work on 16 bytes blocks..."
		if len(data) < 16:
			return b"CBC mode need an IV and at least one block to operate..."

		iv = unhexlify(data[:32])
		ct = unhexlify(data[32:])

		aes = AES.new(self.key, AES.MODE_CBC, iv)
		pt = aes.decrypt(ct)
		
		try:
			pt = unpad(pt, AES.block_size)
		except ValueError:
			if self.lvl == 1:
				return b'Decryption failed'
			else:
				return b'Who knows if it was a success ?'

		if self.lvl == 1:
				return b'Decryption successful'
		time.sleep(0.1)
		return b'Who knows if it was a success ?'