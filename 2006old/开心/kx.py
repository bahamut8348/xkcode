import urllib2, urllib
import cookielib , re
import os, cPickle, sys
import xml.etree.ElementTree as Tree
import time

def pr(s):
	print s.decode("utf8").encode("gbk")

def togbk(s):
	return s.decode("utf8").encode("gbk")

def tou8(s):
	return s.decode("utf8")

def parsetime(s, ts):
	r = re.findall( '(\d+)' + s , ts )
	if r: return int(r[0])
	return 0

def ifok(s):
	return Tree.XML(s).find(u"ret").text != u"fail" 

def gettime(strtime):
	return 	 parsetime(u'天', strtime )*24*3600 + parsetime(u'小时', strtime ) *3600 + parsetime(u'分', strtime )*60 + parsetime(u'秒', strtime )

def getinttime( pat, s ):
	#距离可收获还有(.*?)&lt
	return gettime( re.findall(pat, s ) [0] )
class KX:
	def __init__(self):
		
		self.username=""
		self.password = ""
		self.cj = cookielib.MozillaCookieJar('kx_cookie')
		if os.path.exists( 'kx_cookie' ):
			self.cj.revert()
		httpprocessor = urllib2.HTTPCookieProcessor( self.cj )
		self.opener = urllib2.build_opener( httpprocessor);
		urllib2.install_opener( self.opener )

	def post( self, path, data = {}):
		if isinstance( data, dict ):
			data = urllib.urlencode(data)
		hs = {}
		hs['Host']='www.kaixin001.com'
		hs['User-Agent']='Mozilla/5.0 (Windows; U; Windows NT 5.2; zh-CN; rv:1.9.1.1) Gecko/20090715 Firefox/3.5.1'
		hs['Accept'] = 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8'
		hs['Accept-Language']='zh-cn,zh;q=0.5'
		hs['Accept-Encoding']=''
		hs['Accept-Charset']='GB2312,utf-8;q=0.7,*;q=0.7'
		hs['Keep-Alive'] = 300
		hs['Connection']= 'keep-alive'
		hs['Referer']='http://www.kaixin001.com/!house/garden/index.php'
		hs['Content-Type'] = 'application/x-www-form-urlencoded'
		
		req = urllib2.Request( 'http://www.kaixin001.com/' + path , data , hs  )
		F = urllib2.urlopen( req, timeout=30 )
		return F.read()

	def dump( self, s ):
		open('dump.txt', 'wb').write(s)

	def login( self, u, p ):
		self.post( 'login/login.php', {'remember':1,'email':u, 'password':p,'url':'/home/'} )
		self.cj.save()
	
	def home(self):
		self.post('home')

	def get_friends(self):
		o = self.post( 'friend/' )
		return re.findall(r'uid=(\d+)"\s+title="([^"]+)"', o )
	
	def get_garden_code(self):
		o = self.post( '!house/index.php?_lgmode=pri&t=63' )
		self.garden_code = re.findall(r'g_verify\s*=\s*"(\w+)";', o)[0]
		return self.garden_code
	def havest(self, uid , farm ):
		# http://www.kaixin001.com/!house/!garden/havest.php?r=0%2E24889584118500352&verify=6336984%5F1062%5F6336984%5F1248363254%5F1962ffac23f947f94f9d439f50a3de2e&farmnum=8&fuid=33799976&seedid=0
		# http://www.kaixin001.com/!house/!garden/havest.php?r=0%2E7183420225046575&verify=6336984%5F1062%5F6336984%5F1248363452%5F86dd319748f9acff268fafb3937799db&farmnum=11&fuid=760251&seedid=0
		return self.post("!house/!garden/havest.php?verify=%s&farmnum=%s&fuid=%d&seedid=0" % ( self.garden_code, farm, uid ) )
	def get_garden( self, uid = 0 ):
		# http://www.kaixin001.com/!house/!garden//getconf.php?verify=6336984_1062_6336984_1248278836_031fee4758ec1b63c516a3401bbe791f&fuid=6323280&r=0.16012040246278048
		d = self.post('!house/!garden//getconf.php?verify=%s&fuid=%d' % ( self.garden_code, uid )  )
		if d.find("还没有添加本组件") > -1: return []
		try:
			conf = Tree.XML( d )
		except:
			self.dump(d)
			sys.exit(0)
		self.dump(d)
		t1 = conf.findall("garden/item")
		# 返回为一个列表， 每一个元素为植物的类型
		#  1 为可偷 , 参数为 id
		#  2 为正在生长,  参数为时间
		ret = []
		for e in t1:
			if e.find("shared").text=="1" : continue
			crops = e.find("crops")

			if crops == None:
				# 为空
				ret.append (( 3, e.find("farmnum").text, e.find("cropsid").text ))
				continue
			
			#是否为可偷
			if crops.text.find(u"剩") > -1 and crops.text.find(u"偷") == -1 and crops.text.find(u"已")==-1:
				ret.append( (1, e.find("farmnum").text ) )
			
			# 判断是不是在生长
			if crops.text.find(u"生长阶段") > -1:
				strtime = re.findall(u"距离收获：([^<]+)", crops.text )[0]
				fintime = parsetime(u'天', strtime )*24*3600 + parsetime(u'小时', strtime ) *3600 + parsetime(u'分', strtime )*60 + parsetime(u'秒', strtime )
				ret.append( (2, fintime) )
		return ret
	
	def get_ranch( self , uid):
		o = self.post("!house/!ranch//getconf.php?verify=%s&fuid=%d" % ( self.garden_code , uid ) )
		if o.find("还没有添加本组件") > -1: return []
		self.dump(o)
		conf = Tree.XML( o )
		animals = conf.findall("animals/item")
		ret = []
		for e in animals:
			status =  e.find("tips").text
			#print status
			if status.find(u"生长阶段") > -1:  # 生长中
				tips = e.find("tips").text
				strtime = re.findall(u"距[\u4E00-\u9FFF]+?：([^&]+)", tips )[0]
				fintime = parsetime(u'天', strtime )*24*3600 + parsetime(u'小时', strtime ) *3600 + parsetime(u'分', strtime )*60 + parsetime(u'秒', strtime )

				ret.append( (1,  fintime) )

			if status.find(u"待产") > 0: # 待产
				ret.append( (2, e.find("animalsid").text ) )
				continue
			
			if status.find(u"期") > 0 : # 挤奶期
				ret.append( (3,  getinttime( u'距离下次挤奶：([^<]+)',e.find("tips").text )) )
				continue
			
			if status.find(u"中") > 0: # 收获中
				ret.append( (4, getinttime( u'距离可收获还有(.*?)&lt',e.find("tips").text ) ) )

			# 可收获可能 有重的
		result={}
		result['ani']=ret
		ret=[]
		for e in conf.findall("product2/item"):
			ret.append(  )
		return result
		
	def havest_ani( self,	):
		pass
	
	# 耕地
	def plough( self, uid, farm ):
		# !house/!garden/plough.php?seedid=0&verify=6336984%5F1062%5F6336984%5F1248490655%5F3cac5f9e7e970f62181284063c034f28&fuid=0&farmnum=11&r=0%2E6449370081536472
		o = self.post("!house/!garden/plough.php?seedid=0&verify=%s&fuid=%d&farmnum=%d" % ( self.garden_code, uid, farm ) )
		return ifok(o)
	
	def farm_seed( self, farm, seedid, uid = 0 ):
		# !house/!garden/farmseed.php?seedid=63&verify=%s&fuid=%d&farmnum=%d
		o = self.post("!house/!garden/farmseed.php?seedid=%d&verify=%s&fuid=%d&farmnum=%d" % ( seedid, self.garden_code, uid, farm) )
		
		return ifok(o)
	
	def buy_seed( self, seedid, num ):
		# http://www.kaixin001.com/!house/!garden//buyseed.php
		query="seedid=%d&verify=%s&num=%d" % ( seedid, self.garden_code, num )
		o = self.post("!house/!garden//buyseed.php", query )
		return ifok(o)
	
	def mhavest(self, anid, uid ):
		#http://www.kaixin001.com/!house/!ranch//mhavest.php?verify=6336984%5F1062%5F6336984%5F1248619951%5F38e7d40edd389213a604f2f6ddebad42&animalsid=475447765&fuid=0&r=0%2E3741318038664758
		# 收获动物 
		pass
	
	# 填饮料 63 为草
	def food( self , seedid, num, uid=0 ):
		o = self.post("!house/!ranch//food.php", dict(id=0,foodnum=num,seedid=seedid,fuid=uid,verify=self.garden_code,type=0, skey='' ) )
		return ifok(o)
	
	# 生产动物
	def product( self,  anid, uid=0 ):
		o = self.post('!house/!ranch//product.php?verify=%s&animalsid=%d&fuid=%d' % ( self.garden_code, anid, uid ) )
		pr(o)
		return ifok( o )
if __name__ == "__main__":
	
	kx = KX()
	kx.home()
	kx.login('lovecpp@gmail.com','3281044')
#	print kx.get_friends()

	print kx.get_garden_code()
#	print kx.get_garden( 0 )
	
	#print kx.buy_seed( 63, 20 )

#	print ifok( kx.havest( 760251, 9 ) )
#print	kx.get_ranch( 0 )
	
#	print kx.food( 0, 63, 10)
#	print kx.product( 475447765 )