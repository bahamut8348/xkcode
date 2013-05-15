import struct
from struct import pack
import cStringIO

class tableiter(object):
	def __init__(self, o):
		self.o = o
		self.i = 1
	
	def next(self):
		if self.i > len(self.o): raise StopIteration()
		a = self.o[self.i]
		self.i += 1
		return a

class table(object):
	def __init__(self, d=None):
		if type(d) is dict:
			self.__dict__.update(d)
		elif type(d) is list:
			for i in range(len(d)):
				self[i+1] = d[i]

	def __len__(self):
		i = 0
		while 1:
			if i+1 in self.__dict__: i=i+1
			else: break
		return i
	def __setitem__(self, k, v):
		self.__dict__[k] = v
	
	def __getitem__(self, k):
		return self.__dict__[k]
		
	def __iter__(self):
		return tableiter(self)
	
	def keys(self):
		return iter(self.__dict__)

class FAN:
	
	@staticmethod
	def encode(s):
		if type(s) is dict:
			s = table(s)
		elif type(s) is list:
			s = table(s)
		if type(s) is int or type(s) is long:
			if s <= -33 and s >= -0x8000: return '\x05' + struct.pack('H', 0x10000+s)
			if s < -0x8000 and s > 0x80000000 : return '\x06' + struct.pack('L', 0x100000000+s)
			if s < -0x80000000: return '\x07' + struct.pack('Q', 0x10000000000000000+s)
			if s < 0 and s >= -32: return '%c' % (s + 0x80)
			if s < 30 and s >= 0: return '%c' % (s + 0x40)
			if s == 30: return '\x5e\x5f'
			if s > 30 and s <= 0x7fff: return '\x05' + struct.pack('h', s)
			if s > 0x7fff and s <=0x7fffffff: return '\x06' + struct.pack('l', s)
			if s > 0x7fffffff: return '\x07' + struct.pack('q', s)
		if type(s) is float:
			return '\x04' + struct.pack('d', s)
		if type(s) is str:
			l = len(s)
			if l<64: return '%c' %(l+0x80) + s
			else : return '\t' + pack('h', l) + s
		if type(s) is table:
			r = cStringIO.StringIO()
			l = len(s)
			if l<=0xff:
				r.write('\x10')
				r.write(pack('b', l))
			else:
				r.write('\x11')
				r.write(pack('H', l))
			i = 0
			ld = set()
			for e in s:
				i += 1
				ld.add(i)
				r.write(FAN.encode(e))
			for k in s.keys():
				if k not in ld:
					r.write(FAN.encode(k))
					r.write(FAN.encode(s[k]))
			r.write('\x00')
			v = r.getvalue()
			r.close()
			return v
	
	@staticmethod
	def decode(s):
		pass

if __name__ == '__main__':
	a = table()
	a[1] = 1
	a.oo = dict(a=1, b=3)
	b= FAN.encode(a)
	assert(FAN.encode(-0x80000001)=='\x07\xff\xff\xff\x7f\xff\xff\xff\xff')
	print struct.unpack('h', '\x01\x01')
	for e in b: print '%02x' % ord(e)

	a = table()
	a.p = 1
	a[1] = 2
	a[4]=1
	for i, k in a:
		print '!! k', i
		

