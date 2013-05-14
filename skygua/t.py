import struct

class FAN:
	
	@staticmethod
	def encode(s):
		if type(s) is int:
			if s < 30 and s >= 0: return '%c' % (s + 0x40)
			if s < 0 and s >= -32: return '%c' % (s+ 0x80)
			if s == 30: return '\x5e\x5f'
			if s > 30 and s <= 0x7fff: return '\x05' + struct.pack('h', s)
			if s > 0x7fff: return '\x06' + struct.pack('l', s)
	
	@staticmethod
	def decode(s):
		pass

if __name__ == '__main__':
	b= FAN.encode(0x8000)
	for e in b: print '%02x' % ord(e)
