from kx import *
from twisted.internet import reactor
import time

kx = KX()
kx.login('lovecpp@gmail.com','3281044')
kx.home()
kx.get_garden_code()

def havest( uid, name ):
	print u'检测 %s 花园' % tou8(name)
	crops = kx.get_garden( uid )
	for crop in crops:
		if crop[0]==1:
			print "\t", u'偷窃', ifok(kx.havest( uid, crop[1] ))

fs =  kx.get_friends()
for e in fs:
	uid, name = e
	uid = int(uid)
	havest( uid, name )
	time.sleep(5)
