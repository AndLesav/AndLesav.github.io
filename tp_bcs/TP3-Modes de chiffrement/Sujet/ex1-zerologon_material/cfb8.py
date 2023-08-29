from Crypto.Cipher import AES

class CFB8(object):
	def __init__(self, key, iv):
		self.iv = iv
		self.key = key
		self.block_size = 16

	def encrypt(self, data):
		ct = bytearray()
		payload = bytearray(self.iv + data)
		aes = AES.new(self.key, AES.MODE_ECB)
		for i in range(len(data)):
			temp = aes.encrypt(payload[i:i+16])
			payload[i+16] ^= temp[0]
		return bytes(payload[16:])

	def decrypt(self, data):
		return self.encrypt(data)