import struct
from struct import pack, unpack
import cStringIO

class tableiter(object):
	def __init__(self, o):
		self.o = o
		self.i = 1
	
	def next(self):
		if self.i not in self.o.__dict__: raise StopIteration()
		a = self.o[self.i]
		self.i += 1
		return a

class table(object):
	def __init__(self, d=None):
		if type(d) is dict:
			for k in d: self[k] = d[k]
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
	def encode(s, con=None):
		if not con: 
			con = {}
			con[1]=0
		if type(s) is dict:
			s = con.setdefault(id(s), table(s))
		elif type(s) is list:
			s = con.setdefault(s, table(s))
		if type(s) is int or type(s) is long:
			if s <= -33 and s >= -0x8000: return '\x05' + pack('H', 0x10000+s)
			if s < -0x8000 and s > 0x80000000 : return '\x06' + pack('L', 0x100000000+s)
			if s < -0x80000000: return '\x07' + pack('Q', 0x10000000000000000+s)
			if -32 <= s < 0: return pack('B', s + 0x80)
			if 0 <= s < 30: return pack('B', s + 0x40)
			if s == 30: return '\x5e\x5f'
			if s > 30 and s <= 0x7fff: return '\x05' + pack('H', s)
			if s > 0x7fff and s <=0x7fffffff: return '\x06' + pack('L', s)
			if s > 0x7fffffff: return '\x07' + pack('Q', s)
		if type(s) is float:
			return '\x04' + pack('d', s)
		if type(s) is str:
			if s not in con:
				con[1] += 1
				con[s] = con[1]
			else: return pack('B', 0xc0 + con[s]) if con[s]<0x40 else pack('H', cons[s])
			l = len(s)
			if l<64: return pack('B', l+0x80) + s
			else : return '\t' + pack('H', l) + s
		if type(s) is table:
			if s not in con:
				con[1] += 1
				con[s] = con[1]
			else: return pack('B', 0xc0 + con[s]) if con[s]<0x40 else pack('H', cons[s])
			r = cStringIO.StringIO()
			l = len(s)
			if l<=0xff:
				r.write('\x10')
				r.write(pack('B', l))
			else:
				r.write('\x11')
				r.write(pack('H', l))
			i = 0
			ld = set()
			for e in s:
				i += 1
				ld.add(i)
				r.write(FAN.encode(e, con))
			for k in s.keys():
				if k not in ld:
					r.write(FAN.encode(k, con))
					r.write(FAN.encode(s[k], con))
			r.write('\x00')
			v = r.getvalue()
			r.close()
			return v
	
	@staticmethod
	def decode(s, con=None):
		if not con:
			con = {}
			con[1]=[]
		print `s`, con[1].__len__()
		a = s[0]
		ai = ord(a)
		if 0x40 <= ai < 0x40 + 30: con[0]=1; return ai - 0x40
		if 0x80 > ai >= 0x80 - 32: con[0]=1; return ai - 0x80
		if a == '\x5e' and s[1] == '\x5f': con[0]=2; return 30
		if ai == 5:
			i = unpack('H', s[1:3])
			con[0] = 3
			return i if i < 0x8000 else i - 0x10000, 2
		if ai == 6:
			con[0] = 5
			i = unpack('L', s[1:5])
			return i if i < 0x80000000 else i - 0x100000000, 4
		if ai == 7:
			con[0] = 9
			i - unpack('Q', s[1:9])
			return i if i < 0x8000000000000000L else i - 0x10000000000000000, 8
		if ai == 4:
			con[0] = 9
			return unpack('d', s[1:9])
		if ai == 8:
			con[0] = 3 + unpack('H', s[1:3])
			r = s[3:con[0]]
			con[1].append(r)
			return r
		if 0x80 <= ai < 0x80+64:
			con[0] = ai-0x80+1
			r = s[1:con[0]]
			con[1].append(r)
			return r
		if 0xc0 <ai <= 0xff:
			con[0]=1
			return con[1][ai-0xc1]
		elif ai == 0x0d:
			con[0]=3
			return con[1][unpack('H', s[1:3])]
		if 0x10 <= ai <= 0x11:
			r = table()
			con[1].append(r)
			n = 1
			l = 0
			if ai == 0x10: l = unpack('b', s[1])[0]; n += 1
			elif ai == 0x11: l = unpack('H', s[1:3])[0]; n += 2
			for i in range(l):
				r[i+1] = FAN.decode(s[n:], con)
				n += con[0]
			while 1:
				if s[n] == '\0': n += 1; break
				k = FAN.decode(s[n:], con)
				n += con[0]
				v = FAN.decode(s[n:], con)
				n += con[0]
				r[k] = v
			con[0] = n
			return r


		

if __name__ == '__main__':
	a = table()
	a[1] = 1
	a.oo = dict(a=1, b='1')
	a.c=a.oo
	b= FAN.encode(a)
	assert(FAN.encode(-0x80000001)=='\x07\xff\xff\xff\x7f\xff\xff\xff\xff')
	print struct.unpack('h', '\x01\x01')
	for e in b: print '%02x' % ord(e)

	

	d = FAN.decode(b)
	for k in d.keys():
		print k, d[k]

		

