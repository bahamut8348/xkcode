
import types
import cStringIO


class Event:
	def __init__( self, obj, action, args ):
		self.o = obj
		self.a = action
		self.s = args

ens = {}
vsn = {}

class BaseObj( object ):
	def __repr__( self ):
		return self.__class__.__name__ + str(self.__dict__	)
	

def def_enc( c, k = None, fake = None ):
	global vsn
	global ens
	if not isinstance( c, str ):
		name = c.__name__
		vsn[ name ] = c
	else:
		name = c
	
	
	if k == None : return
	
	if name in ens : raise Exception, "def_enc error:" + `c`
	ens[name] = dict( s =k, f=fake )

def has_id( o, vs ):
	return id( o ) in vs

def obj_id( o, vs, ks ):
	if id(o) in vs:
		vv = vs[id(o)]
		if vv[2] == 0:
			vv[2] = vs[0]
			vs[0]+=1
			ks.insert( vv[0], str(vv[2]) )
			ks.insert( vv[0], ":" )
			for k in vs:
				if k==0:continue
				v = vs[k]
				if v[0] > vv[0]: v[0] += 2

		ks.append( "=" )
		ks.append( str(vv[2]) )
		return 1	
	else:
		vs[id(o)] = [len(ks), 0, 0 ]  # pos, count, id
		return 0


def enc_dic( o, vs, ks ):
	if has_id( o, vs):
		obj_id( o, vs, ks )
	
	ks.append( "{" )
	ks.append( "" )
	obj_id( o, vs, ks )
	for k in o:
		ks.append(  k  )
		enc( o[k], vs, ks )
	ks.append("")
	ks.append( "}" )


def enc_obj( o, vs, ks ):
	if id( o ) in vs:
		obj_id( o , vs, ks )
		return
	
	global ens
	
	cls = o.__class__
	while cls:
		cname = cls.__name__
		if cname in ens:
			break
		else:
			if cls.__bases__: cls = cls.__bases__[0]
			else:
				break

	if cname in ens:
		gs= ens[ cname ]
		ks.append("{") 
		ks.append( gs["f"] if gs["f"] else o.__class__.__name__ )
		
		obj_id( o, vs, ks )
				
		for k in gs['s']:
			if hasattr( o, k ):
				ks.append( k )
				enc( getattr( o, k ), vs, ks )
			
		ks.append("")
		ks.append( "}" )

		
	else:
		raise Exception ,( "enc_obj not define encode ",str( o ))

def enc_list( o, vs, ks ):
	if id( o ) in vs:
		obj_id( o , vs, ks )
		return
	
	ks.append( "[" )
	ks.append( str(len(o)) ) 
	obj_id( o, vs, ks )
	for x in o:
		enc(x, vs, ks)
	ks.append( "]" )
	
	
def  enc(o,  vs = None,  ks = [] ):
	if vs is None : vs = {}; vs[0] = 1
	t = type(o)
	if o == None:  ks.append( "," )
	elif o is True : ks.append( ">" )
	elif o is False: ks.append( "<" )
	elif t == types.LongType : ks.append( str(o) )
	elif t == types.IntType :  ks.append( str(o) )
	elif t == types.FloatType :  ks.append( str(o) )
	elif t == types.StringType :  ks.append(""), ks.append(o)
	elif t == types.DictType :  enc_dic( o, vs = vs, ks = ks )
	elif t == types.TupleType :  enc_list( o, vs = vs, ks = ks )
	elif t == types.ListType : s = enc_list( o, vs = vs, ks = ks )
	else:
		s = enc_obj( o, vs = vs, ks = ks )
	
	


def fin_enc( ss ):
	l = []
	def inter_fin_enc( x ):
		for e in x:
			if type(e) is types.StringType:
				l.append(e)
			else:
				inter_fin_enc(e)
	inter_fin_enc(ss)

	return "\x10".join(l)
	

def dec( s , ks = None ):

	if ks is None : ks = {}
	sep = ''
	r = None
#	print "----", `s[:10]`
	if s[0] == ">":  del s[0]; return True
	elif s[0] == "<" : del s[0]; return False
	elif s[0] == "," : del s[0]; return None
	elif s[0] == "=" : r = ks[s[1]] ; del s[:2]
	elif s[0] == sep : r = s[1] ; del s[:2] 
	elif s[0] == "{" and s[1] == sep:  # dic
		r = {}
		rp = 1
		del s[:2]
		while 1:
			k = s[0]
			if k == ":":
				ks[ s[1] ] = r
				del s[:2]
				continue
			elif k == "" and s[1] == "}":
				del s[:2]
				return r
			
			del s[0]
			v = dec( s, ks )
			r[k]=v
			
			
	elif s[0] == "[" :  # array
		r = []
		l = int( s[1] )
		del s[:2]
		if l == 0:
			assert( s[0] == "]" )
			del s[0]
			return r, 
		if s[0] == ":":
			ks[ s[1] ] = r
			del s[:2]
		while 1:
			if s[0] == "]":
				del s[0]
				return r
			v = dec( s, ks )
			r.append( v )
			
	elif s[0] == "{":
		name = s[1]
		del s[:2]
		vvv = vsn
		
		if name in vvv:
			
			X = vvv[name]
		else:
			X = type( name, (BaseObj,),{} )
		r = X()
		
		c = 2
		while 1:
			k = s[0]
			del s[0]
			if k =="" and s[0] == "}":
				del s[0]
				return r
			elif k == ":":
				ks[ s[0] ] = r
				del s[0]
				continue
			v = dec ( s, ks )
			setattr( r, k , v )
			
	elif s[0] == "NaN":
		r  =s[0]
		del s[0]
	else:
		r=  int(s[0]) if s[0].find(".")==-1 else float(s[0])
		del s[0]
	return r

def pre_dec( s ):
	return [ x for x in s.split('\x10') ]

def xdec(s):
	s = pre_dec( s )
	return dec(s)

def xenc( s ):
	ks = []
	enc ( s, None, ks)
	
	return "\x10".join(ks)

def_enc( Event, ["o", "a", "s","n" ], fake = "!" )

import cProfile

if __name__ == "__main__":
	
	e = Event("UserNames", "signUp" , [1,2] )
	print enc( e )

	class CC:
		pass

	cc = CC()
	cc.id = None
	cc.owner = None
	def_enc( CC, ["id", "owner"] )
	del CC
	v = xenc({"k":-1222, "c":"sfdsf", "d": [1,2,"sd", True]})
	print v

	print xdec(v )
	
	print
	v = xenc( [cc ] * 10)
	print v
	v =  xdec( v )
	print v
	
	print xenc( [ 2,  '22' ] * 30 )
	
	buf = xenc( [ 1, True,None, [1.9, 987847 ]*250 ] )
	print len( buf )
# 1.67 36


	oo = [ 1, True,None, [1.9, 987847 ]*250 ]
	print oo
	import timeit
	t = timeit.Timer(stmt=" xenc( oo  ) ", setup="from __main__ import xenc, buf, oo")
	print t.timeit( number = 1000 ) 


	t = timeit.Timer(stmt=" xdec( buf ) ", setup="from __main__ import xdec, buf")
	print t.timeit( number = 1000 ) 




	cProfile.run('xdec( buf )')
