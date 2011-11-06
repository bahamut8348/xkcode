#! coding: utf-8

from libsg import SG, tostr, math


sg = SG()

r = sg.lookup_xy(0,0)


all = []
def showinfo( r ):
	for e in r['tents']:
		u = filter( lambda x:x[0] == e[2], r['users'] )[0]
		if tostr( u[3] ).find("ÐÌÌì") != -1:
			v = sg.calc_xy( e[3] )
			
			print  tostr( u[1] ), tostr( u[3] ), tostr( e[1] ), math.sqrt(v[0]**2+v[1]**2),v, (238-v[1], v[0]-580)
			all.append( (tostr( u[1] ), tostr( u[3] ), tostr( e[1] ), math.sqrt(v[0]**2+v[1]**2),v, (238-v[1], v[0]-580)) )


o = 60

for x in range( -o, o, 9 ):
	for y in range( -o, o, 9 ):
		showinfo( sg.lookup_xy(x,y) )


all.sort( key = lambda x:x[3] )

print "*" *240
for e in all:
	print e[0], e[1], e[2], e[-1], e[3]