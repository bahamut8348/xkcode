#! coding:utf-8
from kx import  *
from twisted.internet import reactor
import time
from traceback import print_exc
kx = KX()
kx.home()
kx.get_garden_code()

def chek_seek( uid ):
	pass

def call_fun( fun, *arg, **aw ):
	r = 1
	try:
		r = fun( *arg, **aw )
	except:
		print fun, arg
		print_exc()
		reactor.callLater( 10, call_fun, fun, *arg, **aw )
	reactor.callLater( r , call_fun, fun, *arg, **aw )

def check_garden(uid):
	tt = []
	print "开始检查花园"
	fs = kx.get_garden(uid)
	for e in fs:
		if e[0]==1:
			print '收获', ifok(kx.havest( uid, int(e[1]) ))
		if e[0] == 2:
			tt.append( int(e[1]) ) 
		if e[0] == 3:
			# 种植
			farm, cid = int(e[1]), int( e[2] )
			if cid != 0:
				# 梨地
				kx.plough( uid, farm )
			seed = kx.farm_seed( farm, 63 )
			print '种植', seed
			if seed == False:
				pass
	
	# 成熟时间
	retrytime = 5
	if tt:
		retrytime = min(tt)
	print retrytime, "秒后重试"
	return retrytime

if __name__ == "__main__":
	call_fun( check_garden, 0 )
	reactor.run()
