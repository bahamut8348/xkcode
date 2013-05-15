import struct

class FAN:
	
	@staticmethod
	def encode(s):
		if type(s) is int or type(s) is long:
			if s <= -33 and s >= -0x8000: return '\x05' + struct.pack('H', 0x10000+s)
			if s < -0x8000 and s > 0x80000000 : return '\x06' + struct.pack('L', 0x100000000+s)
			if s < -0x80000000: return '\x07' + struct.pack('Q', 0x10000000000000000+s)
			if s < 0 and s >= -32: return '%c' % (s+ 0x80)
			if s < 30 and s >= 0: return '%c' % (s + 0x40)
			if s == 30: return '\x5e\x5f'
			if s > 30 and s <= 0x7fff: return '\x05' + struct.pack('h', s)
			if s > 0x7fff and s <=0x7fffffff: return '\x06' + struct.pack('l', s)
			if s > 0x7fffffff: return '\x07' + struct.pack('q', s)
		if type(s) is float:
			return '\x04' + struct.pack('d', s)
		if type(s) is str:
			return struct.pack('5s', s)
	
	@staticmethod
	def decode(s):
		pass

if __name__ == '__main__':
	b= FAN.encode('afdfd')
	assert(FAN.encode(-0x80000001)=='\x07\xff\xff\xff\x7f\xff\xff\xff\xff')
	print struct.unpack('h', '\x01\x01')
	for e in b: print '%02x' % ord(e)

