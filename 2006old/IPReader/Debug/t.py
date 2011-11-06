from IPReader import Reader
import time

r = Reader()
r.load("qqwry.dat")
print len(r)
print r[327259]
print time.time()
F = open("test1","wb")
all = []
for e in range(len(r)):
	all.append( str( r[e] ) )
print time.time()
F.write("\r\n".join(all))
F.close()