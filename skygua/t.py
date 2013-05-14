import struct

class FAN:
	
	@staticmethod
	def encode(s):
		if type(s) is int or type(s) is long:
			if s < 30 and s >= 0: return '%c' % (s + 0x40)
			if s < 0 and s >= -32: return '%c' % (s+ 0x80)
			if s == 30: return '\x5e\x5f'
			if s > 30 and s <= 0x7fff: return '\x05' + struct.pack('h', s)
			if s > 0x7fff and s <=0x7fffffff: return '\x06' + struct.pack('l', s)
			if s > 0x7fffffff: return '\x07' + struct.pack('q', s)
	
	@staticmethod
	def decode(s):
		pass

if __name__ == '__main__':
	print type(0x80000000)
	b= FAN.encode(0x80000000)
	for e in b: print '%02x' % ord(e)
