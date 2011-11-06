#! coding:utf-8
import urllib2, time, random, urllib, re
import httplib as httplib
import json
import socket, math, sys

#import img_verify as imgv

# 铁胎弓 201
# 透甲枪 101
# 厚背刀 102
# 霸王驽 202
# 精钢甲	302 24
# 翻羽马 402
# 离秦弓 203
# 宝雕弓 204
# 明光战甲 303
# 奔宵马  403
# 野行马  404
# 


# 石料  3
# 粮食  1
# 木材  2
# 铁矿  4


"""
种植技巧 1
冶炼技巧 2
挖掘技巧 3
伐木技巧 4
谋略技巧 5
侦察技巧 6
统帅技巧 7
阵法技巧 8
拔营技巧 9
储藏技巧 10
城建技巧 11
战马驯服术 12
盔甲制造术 13
弓弩制造术 14
武器制造术 15
弓手行军术 16
弓手协作力 17
弓手防御术 18
弓手熟练度  19
骑兵行军术 20
骑兵协作力 21
骑兵防御术 22
骑兵熟练度 23
步兵行军术 24
步兵协作力 25
步兵防御术 26
步兵熟练度 27
抚民技巧 28
城防技巧 29
"""


def tostr(s):
	return s.decode("utf8").encode("gbk")

class SG:
	
	_stone = 3
	_wood = 2
	_food = 1
	_iron = 4

	_w = 13  # 武器
	_p = 14  # 防具
	_h = 15  # 车马



	_tex = 0.072

	def __init__(self, cid=116399, baseurl = "http://sg2.dipan.com" ):
		socket.setdefaulttimeout(20)
		self.conn = None
		self.get_new_http()
		self.headers = {}
		self.cid = 0
		self.tid = 0
		self.cookie=open("cookie-" + str(cid),"rb").read()
		self.headers['Content-Type']="application/x-www-form-urlencoded; charset=UTF-8"
		self.headers['Referer'] = baseurl + "/city"
		self.headers['X-Requested-With'] = "XMLHttpRequest"
		self.headers['X-Prototype-Version'] = "1.5.0"
		self.headers['User-Agent']="Mozilla/5.0 (Windows; U; Windows NT 5.2; zh-CN; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3"
		self.headers['Cookie'] = self.cookie
		self.buildings = None
		self.building_data = {}
		self.city_names={}
		self.baseurl = baseurl
		citiesinfo = self.get_city_list()['infos']
		for e in citiesinfo:
			print tostr(e[2]), e[1]
		self.cs = filter(lambda x:x[-1] == 0, citiesinfo )
		self.ts = filter(lambda x:x[-1] > 0, citiesinfo)
		

		self.posinfos = {}
		for e in citiesinfo:
			self.posinfos[ e[1] ] = self.calc_xy( e[3] )
		
		self.res = {}
	
	def pos(self, cid =0):
		if cid == 0:
			cid = self.cid
		return self.posinfos[ cid ] 
		 
		
	
	def get_new_http(self):
		if self.conn:
			self.conn.close()
			del self.conn
		self.conn = httplib.HTTPConnection("sg2.dipan.com:80")
		
	
	def update_self_res(self):
		i = self.cid if self.tid == 0 else self.tid
		self.res[i] = self.get_resouce_number()
	
	def get_city_list( self ):
		r = self.post("/GateWay/City.ashx?id=7")
		return r
	
	def get_res_number( self, cid = 0 ):
		r = self.post("/GateWay/Common.ashx?id=31")
		cid = cid if cid else self.cid
		l = filter( lambda x: x[0] == cid , r['list'] )[0]
		k = {}
		k['food'] = l[5]
		k['wood'] = l[6]
		k['stone'] = l[7]
		k['iron'] =l[8]
		k['money'] = l[4]
		return k

	def init_building(self, cid ):
		#self.building_data[cid] = json.read(open("buildings.data.%d" % cid,"rb").read())
		self.building_data[cid] = self.get_all_building()
	
	def fix_cookie(self, cityid, tid=0 ):
		self.cid= cityid
		self.tid = tid
		s = re.findall('cid=-?\d+',self.cookie)[0]
		self.cookie = self.cookie.replace(s,"cid=%d" % cityid )
		s = re.findall('tid=-?\d+',self.cookie)[0]
		self.cookie = self.cookie.replace(s,"tid=%d" % tid )
		self.headers['Cookie'] = self.cookie
	
	def geturl( self, p ):
		return p
		return p + "&0." + str(  random.randint(1639979069258746, 9639979069258746) )
	
	def get_building_gid( self, pid ):
		return filter( lambda x: x[1] == pid, self.buildings)[0][0]
	
	def get_building_info( self, pid ):
		return filter( lambda x: x[1] == pid, self.buildings)[0]
	
	def get_building_name( self, pid ):
		return tostr(self.get_building_info(pid)[2])
			
	def send(self, url,  data ):
		if data.find("tid") == -1:
			data+="&tid=%d" % ( self.cid if self.tid==0 else self.tid )
		self.conn.request("POST", self.geturl(url), data, self.headers)
		ret = ""
		try:
			response = self.conn.getresponse()
			ret = response.read()
		except KeyboardInterrupt:
			print "<Ctrl-C> pressed, Exit."
 			sys.exit(0)
 			
		except:
			self.get_new_http()
		
		try:
			ret = json.read(ret)
			if 'ret' in ret:
				if ret['ret'] == 110 :
					print time.asctime(), "需要验证码"
		except:
			print "Json error:", ret
		return ret
		
	def make(self, num, gid, speed, tid=0 ):
		p={}
		m = gid/100
		p[1]=13
		p[2]=13
		p[3]=14
		p[4]=15
		p[5]=15
		pid = filter( lambda x: x[0] == p[m] , self.buildings )[0][1]
		#print num, gid, pid
		return self.post("/GateWay/OPT.ashx?id=38" ,"count=%d&gid=%d&pos=%d&lFaster=%d&tid=%d" % (num, gid, pid, speed, tid) )
	
	def show_weapon(self):
		#http://sg2.dipan.com/GateWay/Common.ashx?id=27&0.6599525046579348
		return self.send( self.geturl("/GateWay/Common.ashx?id=27") ,"" )
	
	def post( self, url, data=""):
		return self.send( self.geturl(url) , data )

	def get_seller( self, stype ):
		return self.post( "/GateWay/Common.ashx?id=38", "stab=1&tid=0&type=%d" % stype )
	
	def sell_resouce( rtype, num ):
		pass
	
	def sell( self, num, wtype=201, price=15):
		ps={}
		ps[202]=28
		ps[203]=50
		ps[205]=164
		ps[206]=297
		ps[302]=23
		ps[303]=40
		ps[304]=73
		ps[305] = 131
		ps[402]=22
		ps[403]=39
		ps[405] = 129
		ps[404]=71
		ps[204]=91
		if price ==0:
			price = ps[wtype]
		return self.send( self.geturl("/GateWay/OPT.ashx?id=48") ,"count=%d&price=%d&type=%d" % (num,price,wtype) )
	
	def buy(self, num, btype, restmoney=0 ):
		r = dict( ret = 1 )
		for seller in self.get_seller( btype )['infos']:
			if restmoney:
				all = self.get_money_number()
				if int(seller['price'])*num + restmoney > all:
					print num, seller['price']
					return dict(ret=1)
			seller['count'] = int( seller['count'] )
			buynum = num if seller['count']>=num else seller['count']
			r= self.internal_buy( buynum  , btype, seller['seqno'], seller['price'], seller['seller'], seller['count'] )
			if r['ret']==0:
				num=num-buynum
			if num <= 0: return dict(ret=0)
		
		return r
			

	def internal_buy( self, num, wtype, seqno, price, seller, buyincount ):
		data={}
		data['count']=num
		data['buyincount'] = buyincount
		data['buyinprice'] = price
		data['countprice'] = price
		data['res_hidden'] = 1
		data['res_jsondata']={}
		data['seller'] = seller
		data['seqno'] = seqno
		data['tid'] = 0
		data['type3'] = 2 
		return self.post("/GateWay/OPT.ashx?id=9", urllib.urlencode(data) )
	
	def get_build(self, gid=13 ):
		return self.post( "/GateWay/Build.ashx?id=2" ,"gid=%d&pid=%d&tab=%d" % (gid, -1, 1))['current']
	
	def get_build_detail( self, pid, tab = 1 ):
		return self.post( "/GateWay/Build.ashx?id=2" ,"gid=%d&pid=%d&tab=%d" % ( self.get_building_gid(pid), pid, tab))
	
	def get_resouce_number(self):
		r = self.post("/GateWay/City.ashx?id=20" )
		ret={}
		keys = ['stone', 'food',  'wood', 'iron']
		for e in keys:
			ret[e] = r[e][0]
			ret["-"+e] = r[e][1]
		return ret
	
	def get_yz_building(self, tid):
		return self.post("/GateWay/City.ashx?id=5","tid=%d" % tid )
			
	def get_weapon_number(self):
		r = self.show_weapon()['list'][0]
		#print r
		def get_info( a, b ): return int(r[a][b][0])
		return get_info(3,0),get_info(4,0), get_info(5,0)
	
	def update_tech(self, techid ):
		# 如果正在研究返回 48
		if self.get_money_number() - self.get_tech_update_money( techid )  >  10000:
			return self.post("/GateWay/OPT.ashx?id=44", "techid=%d&tid=0" % techid )
		return dict(ret=1)
	
	def get_current_update_tech( self ):
		ret = self.get_all_tech()
		if ret['current'][0]:
			return ret['current']
		return ""

	def get_need_resouce(self, pid=12):
		r= self.post("/GateWay/Build.ashx?id=2", "gid=%d&pid=%d&tab=%d" % ( self.get_building_gid(pid), pid, -1) )['update']
		ret={}
		ret['level'] = r[0]
		ret['stone'] = r[7]
		ret['money'] = r[9]
		ret['wood']  = r[6]
		ret['iron'] = r[8]
		ret['food']= r[5]
		return ret

	def get_current(self):
		# http://sg2.dipan.com/GateWay/Common.ashx?id=41&0.6108996970752234
		ret = self.post("/GateWay/City.ashx?id=37","ttid=%d" % (0 if self.tid == 0 else self.tid) )
		return ret
	
	def get_current_update(self):
		krt = self.get_current()
		r= map( lambda x:[x[0],x[-1]] ,krt['list'][0][4] )
		ret={}
		for e in r:
			ret[e[0]] = e[1]
		return ret

	def calc_update_money(self, pid):
		res = self.get_resouce_number()
		need = self.get_need_resouce(pid)
		allmoney = [0]
		def checkres( name ):
			if res[name] < need[name]:
				needbuy = (need[name] - res[name])/1000 + 2
				btype = eval("SG._" + name)
				#print "need buy",name, needbuy
				allmoney[0] += needbuy* int(self.get_seller( btype )['infos'][1]['price'])*(1 + 0 )
		checkres("stone")
		checkres("iron")
		checkres("wood")
		checkres("food")
		allmoney[0] += need['money']
		selfmoney = self.get_money_number()
		#print "needmoney:", allmoney[0]
		#print "current money:", selfmoney
		return selfmoney - allmoney[0]
	
	def get_tech_update_money( self, techid ):
		return self.get_all_tech()['list'][techid-1][7]


	def force_update_building(self, pid):
		current_update = self.get_current_update()
		if pid in current_update:
			return dict(ret=1)
		restmoney = self.calc_update_money( pid)
		if restmoney<10000:
			print "建造", self.get_building_name(pid),pid,"失败", ",rest:", restmoney
			return dict(ret=1)

		res = self.get_resouce_number()
		need = self.get_need_resouce(pid)
		def checkres( name ):
			if res[name] < need[name]:
				needbuy = (need[name] - res[name])/1000 + 2
				btype = eval("SG._" + name)
				print self.cname, "买入",name, needbuy
				self.buy(needbuy , btype, 2000 )
		checkres("stone")
		checkres("iron")
		checkres("wood")
		checkres("food")
		return self.update_building(pid)
	
	def update_building(self, pid, gid = 0 ):
		if gid ==0:
			gid = self.get_building_gid(pid)
		ret= self.post("/GateWay/OPT.ashx?id=65", "gid=%d&pid=%d&type=1" % (gid, pid))
		return ret
	
	def destroy_building( self, gid , pid = -1 ):
		if gid==-1:
			gid = self.get_building_gid(pid)
		return self.post("/GateWay/OPT.ashx?id=65", "gid=%d&pid=%d&type=2" % (gid, pid) )
	
	def get_money_number(self, cid = 0):
		ret= self.post("/GateWay/Common.ashx?id=29")['city']
		cid = cid if cid else self.cid
		ret = filter( lambda x:x[0] == cid , ret )
		return ret[0][2]
	
	def get_money_info(self, cid = 0 ):
		ret= self.post("/GateWay/Common.ashx?id=29")['city']
		cid = cid if cid else self.cid
		ret = filter( lambda x:x[0] == cid , ret )
		return ret[0]
	
	def get_money_all_info(self):
		ret = self.post("/GateWay/Common.ashx?id=29")['city']
		return ret
	
	def get_tech_level(self, techid):
		return filter(lambda x: x[0]==techid, self.get_all_tech()['list'])[0][2]
	
	def get_building_level(self, pid ):
		return filter( lambda x: x[1]==pid, self.get_all_building() )[0][3]
	
	def test_res_to_money( self ):
		rs = "stone", "iron", "wood", "food"
		rnum = self.get_resouce_number()
		ret = 0
		for r in rs:
			rtype = eval("SG._" + r)
			sellnum = (rnum[r]-2000)/1000
			inmoney = sellnum * int(self.get_seller( rtype )['infos'][1]['price']) * ( 1 - self._tex )
			ret += inmoney
		return ret
	
	def sell_resource(self, wtype, num, price ):
		return self.post("/GateWay/OPT.ashx?id=49", "countprice1=190&res1_hidden=0&res1_jsondata={}&sellcount1=%d&sellprice1=%d&type1=%d" % (num, price, wtype) )
	
	def sell_all_resource( self ):
		rs = "stone", "iron", "wood", "food"
		rnum = self.get_resouce_number()
		for r in rs:
			rtype = eval("SG._" + r)
			sellnum = (rnum[r]-2000)/1000
			self.sell_resource( rtype, sellnum, int(self.get_seller( rtype )['infos'][1]['price']) )

	def get_all_tech(self):
		ret= self.post("/GateWay/Build.ashx?id=2", "gid=6&pid=-1&tab=1")
		#print ret['current'][1].decode("utf8")
		# list 说明
		# 0: techid
		# 1: 名称
		# 2: level
		for e in ret['list']:
			#print tostr(e[1]), e[0]
			pass
		
		return ret
	
	def change_city( self,citynum = 116399 ):
		cid = 0
		tid = 0
		cinfo = tinfo = None
		if citynum > 0:
			cid = citynum
		else:
			tid = citynum
			tinfo = filter( lambda x: x[1]==tid, self.ts )[0]
			cid = tinfo[-1]
		
		cinfo = filter( lambda x: x[1]==cid, self.cs )[0]
			
		if self.cid == cid and self.tid==tid: return dict(ret=0)
		self.cid = cid
		self.tid = tid
		self.cname = tostr(cinfo[2])
		if tinfo:
			self.tname = tostr(tinfo[2])
		
		self.fix_cookie( cid, tid )
		ret = self.post("/GateWay/City.ashx?id=50","tid=%d" % tid )
		h = cid if tid==0 else tid
		if not self.building_data.has_key( h ):
			self.init_building( h )
		self.buildings = self.building_data[ h ]
		
		
		return dict(ret=0)
	
	def get_all_building(self):
		ret = self.post("/GateWay/City.ashx?id=5")
		self.building_data[self.cid] = map( lambda x: (x[1], x[0], x[2],x[3]), ret["infos"] )
		self.buildings = self.building_data[self.cid]
		return self.building_data[self.cid]

	def show_all_building(self):
		ret = self.post("/GateWay/City.ashx?id=5")
		for info in ret["infos"]:
			if info[1]:
				print tostr(info[2]),info[3], "gid=", info[1], "pid=", info[0]
				#print info[0],",",
	
	def make_buildings_data(self):
		
		open("buildings.data","wb" ).write(json.write(self.get_all_building()))
	
	def get_pid_by_gid(self, gid ):
		bs = filter( lambda x:x[0] == gid, self.buildings )
		return bs
	def get_market_info(self):
		ret= {}
		pid =self.get_pid_by_gid(11)[0][1]
		for e in  self.post("/GateWay/Build.ashx?id=2", "gid=11&pid=%d&tab=2" % pid)['equips']:
			ret[e[0]] = e
		return ret
	
	def get_work_people( self ):
		ret = {}
		keys='wood','food','iron','stone'
		r = self.post("/GateWay/Build.ashx?id=2", "gid=1&pid=8&tab=1")
		for e in r :
			if e in keys:
				ret[e] = r[e][0][4]
		return ret
	
	def get_people_info( self ):
		return self.get_city_man_info()['population']
	
	def get_city_man_info(self):
		return self.post("/GateWay/Common.ashx?id=30")
	
	def set_work_people(self, workid , num):
		workdic={}
		workdic[self._stone] = 1622456
		workdic[self._food] = 1625071
		workdic[self._wood]= 1621139
		workdic[self._iron] = 1626392
		return self.post("/GateWay/OPT.ashx?id=20","oid=%d&workman=%d" % (workdic[workid], num  ))
	
	def get_soldier_info(self):
		pid=17
		return self.post("/GateWay/Build.ashx?id=2", "gid=16&pid=-1&tab=6")['list']
	
	def update_shiqi(self, id, addpt = 10):
		if addpt > 10: addpt = 10
		return self.post("/GateWay/OPT.ashx?id=47","lAddPoint=%d&lGeneralID=%d" % (addpt,id) )

	def sell_auto( self, rtype, num ):
		price  = self.get_seller( rtype )['infos'][1]['price']
		return self.sell_resource( rtype, num, int(price) )
	
	def task_info(self):
		return self.post("/GateWay/Common.ashx?id=3")
	
	def do_task(self, taskid, gens ):
		p = [taskid]+gens+[0,0,0,0,0,0,0]
		return self.post("/GateWay/OPT.ashx?id=62","taskid=%d&general1=%d&general2=%d&general3=%d&general4=%d&general5=%d" % tuple( p[0:6] ) )
	
	def query_general(self ):
		return self.post("/GateWay/Build.ashx?id=2", "pid=25&gid=8&tab=2&tid=2")
	
	def get_report_list(self, page = 0 ):
		return self.post("/Interfaces/report_list.aspx?t=0&page=2", "t=0&page=%d" % page)
	
	def get_wall_info( self,  tid =0):
		return self.post("/GateWay/Build.ashx?id=2", "pid=0&gid=2&tab=1&tid=%d" % tid )
	
	def get_wall_value(self):
		key = self.cid
		if self.tid : key = self.tid
		return filter( lambda x:x[0] == key, self.post("/GateWay/Common.ashx?id=28")['list'] )[0]

	def make_wall( self, num, btype, speed=1 ):
		return self.post("/GateWay/OPT.ashx?id=38", "pos=0&tid=%d&gid=%d&count=%d&lFaster=%d" % (self.tid, btype, num, speed) )
	
	def get_wu_infos( self ):
		r = self.post("/GateWay/Build.ashx?id=2","pid=-1&gid=10&tab=1&tid=%d" % ( self.tid ) )
		if r['ret'] == 0:
			return r['incity']
		else:
			return []
	
	def find_gen(self, id ):
		return self.post('/GateWay/Build.ashx?id=2','pid=-1&gid=%d&tab=2&tid=0' % id )
	
	def find_wu(self):
		ret = self.post('/GateWay/Build.ashx?id=2','pid=-1&gid=8&tab=2&tid=0')
		return ret
	

	def find_wen(self):
		ret = self.post('/GateWay/Build.ashx?id=2','pid=-1&gid=7&tab=2&tid=0')
		return ret

	def get_wen_infos( self ):
		r= self.post("/GateWay/Build.ashx?id=2","pid=%d&gid=9&tab=1&tid=0" % -1 )
		if r['ret'] == 0:
			return r['incity']
		else:
			return []
	
	def give_money(self, genid, money ):
		return self.post("/GateWay/OPT.ashx?id=46","lCityID=%d&lGeneralID=%d&lMoney=%d" % (self.cid, genid, money ) )
	
	def get_minyuan(self):
		return self.post("/GateWay/Common.ashx?id=30")['morale'][2]
	
	def pre_anfu(self):
		return self.post("/GateWay/Build.ashx?id=2", "pid=-1&gid=1&tab=4&tid=0")
	
	def anfu(self):
		return self.post("/GateWay/OPT.ashx?id=4","lType=1&tid=0")
	
	def add_people(self):
		return self.post("/GateWay/OPT.ashx?id=4","lType=4&tid=0")
	
	def get_idle_wen(self):
		ls = self.get_wen_infos()
		ls = filter( lambda x:x[3]==x[4]==0, ls )
		return ls
	
	def getfind_info(self, wenid, dest):
		return self.post("/GateWay/OPT.ashx?id=36","lGeneralID=%d&lDestID=%d&tid=0" % (wenid, dest) )
	
	def _dofind(self, wenid, dest, money):
		return self.post("/GateWay/OPT.ashx?id=37","lGeneralID=%d&lDestID=%d&lMoney=%d&lHorseID=-1&lGiftID=-1&tid=0" % (wenid, dest, money) )
	
	def dofind(self, dest ):
		ls = self.get_wen_infos()
		ls = filter( lambda x: x[4]==0 , ls )
		ls.sort( cmp = lambda x,y: cmp(y[9], x[9]) )
		g = ls[0]
		self.give_job( g[0],0 )
		r = self.getfind_info(g[0], dest )
		print tostr(r['offical_name']) , "去寻访", tostr( r['hero_name'] ), "用时:", r['duration']
		return self._dofind( r['offical_id'],dest, r['money_percent'] )
	
	def train_new(self, num ):
		r = self.post("/GateWay/OPT.ashx?id=41","lAmount=%d&tid=0" % num )
		return r
	
	def army_info(self, num ):
		return self.post("/GateWay/Build.ashx?id=2","pid=-1&gid=16&tab=2&tid=0")
	
	def add_soldier(self, genid, stype, num ):
		return self.post("/GateWay/OPT.ashx?id=6","lGeneralID=%d&lSoldierType=%d&lAmount=%d&tid=0" % ( genid, stype, num) )

	def buy_wen( self, key = 4 ):
		r = self.post("/GateWay/Build.ashx?id=2", "pid=-1&gid=7&tab=1&tid=0")
		r['list'].sort( cmp = lambda x,y : cmp(y[key],x[key]) )
		weninfo = r['list'][0]
		r = self.post("/GateWay/OPT.ashx?id=33", "lGeneralID=%d&tid=0" % weninfo[0])
		return r
	
	def give_job( self, wenid, job ):
		return self.post("/GateWay/OPT.ashx?id=5","lCityID=%d&lGeneralID=%d&lType=%d" % (self.cid, wenid, job) )
	
	def get_gen_detail( self, genid ):
		return self.post("/GateWay/Common.ashx?id=14","lGeneralID=%d&lCityID=0" % genid )

	def get_all_gen( self ):
		return self.post("/GateWay/Common.ashx?id=15","lType=4&tid=undefined")
	
	def add_point( self, genid, prop, pt ):
		attrs = {}
		# "strength":74,"agility":116,"captain":43
		# charm":47,"brain":115,"manage":68
		attrs['strength'] = 1
		attrs['agility']  = 2
		attrs['captain']  = 3

		attrs['charm'] = 1
		attrs['brain']  = 2
		attrs['manage']  = 3

		if pt > 10:
			self.add_point( genid, prop, pt-10 )
			pt = 10
		if pt > 3:
			self.add_point( genid, prop, pt-3 )
			pt = 3
		if pt > 1:
			self.add_point( genid, prop, pt-1 )
			pt = 1

		if pt==1 or pt == 3 or pt == 10:
			pass
		return self.post("/GateWay/OPT.ashx?id=67", "lCityID=%d&lGeneralID=%d&lAttribType=%d&lAdd=%d"  % (self.cid, genid, attrs[prop], pt ) )
	
	def set_giving( self, dest , count, supid ):
		return self.post("/GateWay/OPT.ashx?id=64","lDestID=%d&lFood=%d&lIron=%d&lStone=%d&lSupplyID=%d&lWood=%d&tid=%d" % (dest, count, count, count, supid, count, self.tid) )

	def get_trader_info( self ):
		# {"ret":0,"trader":12,"plus_left":0,"come":[],"back":[],"goto":[[539885,"1周杰伦1",96389,-34034,"测试",1419863,5000,10000,10000,10000,0,4,938,1,1,0,"",0],[539888,"1周杰伦1",96389,-34034,"测试",1419863,5000,10000,10000,10000,0,4,943,1,1,0,"",0]]}
		return self.post("/GateWay/Build.ashx?id=2","pid=-1&gid=11&tab=1")

	def do_trans( self, dest, wood = 0, iron= 0, stone =0, food = 0, money = 0 ):
		return self.post("/GateWay/OPT.ashx?id=52", "tid=0&gid=0&gamount=0&Wood=%d&Iron=%d&Stone=%d&Food=%d&Money=%d&times=1&dest=%d" % (wood, iron, stone, food, money, dest))
	
	def get_best_tax(self):
		r = self.post("/GateWay/Common.ashx?id=30")
		peo = r['population']
		# 合理税率＝50-46*劳动人口/人口上限-50*军事人口/人口上限	
		return 50-46.0*peo[3]/peo[1]-50.0*peo[4]/peo[1]
	
	def get_fin_info( self, cid = 0):
		if cid == 0: cid = self.cid
		r= self.post("/GateWay/Common.ashx?id=29")['city']
		return filter( lambda x:x[0] == cid, r )[0]
	
	def get_useable_gens(self):
		return self.post("/GateWay/Build.ashx?id=2","pid=-1&gid=19&tab=4")["generals"]
	
	def do_beat_city(self, genids, dest , target = 0):
		return self.post("/GateWay/OPT.ashx?id=53","lDestID=%d&lType=1&lGeneralID1=364214&lGeneralID2=326572&lGeneralID3=363930&lBout=-1&lTarget1GID=%d&lTarget2GID=%d&lTarget3GID=%d&lPlusFuncID=0&lPlusDestID=0&tid=-50278" % (target, target, target) )
	
	def do_beat_city(self, genids, dest , target = 0):
		genids += [0,0,0,0]
		return self.post("/GateWay/OPT.ashx?id=53","lDestID=%d&lType=1&lGeneralID1=%d&lGeneralID2=%d&lGeneralID3=%d&lBout=-1&lTarget1GID=%d&lTarget2GID=%d&lTarget3GID=%d&lPlusFuncID=0&lPlusDestID=0&tid=%d" % ( dest, genids[0], genids[1], genids[2],target, target, target, self.tid ) )
	
	def get_current_wars( self, tid = 0 ):
		ret = []
		infos = self.get_mili_info( tid )
		for gt in infos['goto']:
			ret.append( self.calc_mid( * self.tomyxy(gt[6], gt[7]) ) )
		infos = self.get_battle_info( tid )
		for bt in infos['battle']:
			ret.append( self.calc_mid( * self.tomyxy(bt[3], bt[4]) ) )
		
		return ret
	
	def get_max_time(self):
		return self.post("/GateWay/Build.ashx?id=2","pid=4&gid=19&tab=1&tid=%d" % self.tid )['move']
	
	def lookup_map(self, mid ):
		return self.post("/GateWay/Map.ashx?id=6", "mid=%d" % mid )
	
	def get_generals_info(self):
		# [442487,"沈般瀛",16670,9842,[0,305,"千户甲",105,"大砍刀",405,"越影马"],[0,405,"越影马",105,"大砍刀",0,""],[9842,205,"玉角弓",305,"千户甲",405,"越影马"],[501,"轻型投石车",0],100,19684,19684,9842]
		return self.post("/GateWay/Build.ashx?id=2", "pid=-1&gid=16&tab=2&tid=0" )
	
	def add_soldier(self, n ):
		return self.post("/GateWay/OPT.ashx?id=41","lAmount=%d&tid=0" % n )
	
	def add_to_gen(self, genid, t, n ):
		
		return self.post("/GateWay/OPT.ashx?id=6","lGeneralID=%d&lSoldierType=%d&lAmount=%d&tid=0" % (genid, t ,n) )

	def lookup_xy( self, x , y, mid = 1623763 ):
		nmid = self.calc_mid(x,y, mid)
		return self.lookup_map( nmid )
	
	
	def get_mili_info( self, tid =0 ):
		return  self.post("/GateWay/Common.ashx?id=40", "tid=%d" % tid )
	
	def calc_mid( self, x,y , mid = 1623763):
		nmid = mid - x*1313 - y
		return nmid
	
	def stand_mid( self, x, y):
		# 2, 0   861987
		# 0, 2   859359
		# -2,0	861983
		# 0,-2   864611
		return 861985 + x - y * 1313
	
	def stand_xy(self, mid):
		t = 861985 - mid 
		y = (abs(t)/1313) * (1 if t > 0 else -1)
		x = abs(t) - y*1313
		return x, y
	

	def calc_xy( self, mid , abmid = 1623763):
		t = abmid - mid
		basenum = 0
		if abs(t)%1313 > 656: basenum += 1
		s_x = 1 if t > 0 else -1
		s_y = 1
		x = abs(t)/1313 + basenum
		y = abs(t) - x*1313
		return x*s_x, y*s_x
	
	def tomyxy( self, x, y ):
		# 1588368 27 -56
		#         294 -553
		return self.calc_xy(self.stand_mid(x,y))
	
	def get_battle_info(self, tid=0 ):
		return self.post("/GateWay/Build.ashx?id=2","pid=-1&gid=16&tab=1&tid=%d" % tid )
	
	def dig_bao(self):
		return self.post("/GateWay/OPT.ashx?id=100","DestMapID=1632930&HeroID=460057&tid=116399" )
	
	def get_user_info(self, uid):
		return self.post("/GateWay/Common.ashx?id=45","uid=%d" % uid )

    

	def get_img_code(self):
#		self.post("/GateWay/Common.ashx?id=73" )
		
		self.conn.request("GET", "/VerifyCode.gif?t=8599&xf=0.%d" % (random.randint(16724855855977736,96724855855977736) ), None, headers=self.headers )
		res = self.conn.getresponse()
		buf = res.read()
		open("img_verify\\v.gif","wb").write(buf)

		self.conn.request("POST", "/GateWay/Common.ashx?id=73&%d" % (random.randint(16724855855977736,96724855855977736) ), "1", headers=self.headers )
		res = self.conn.getresponse()
		buf = res.read()
		imgs = re.findall("(\d+)", buf)[1:]
		res.close()
		print imgs
#		imgv.get_code(imgs, "img_verify\\" )


class SG2(SG):
	def __init__(self, cid ):
		SG.__init__(self, cid, 'http://s1.bl.txwy.com' )
		



if __name__ == "__main__":
	sg = SG()
#	print sg.change_city( 57747 )
#	print sg.query_general()
	print sg.change_city( 116399 )
	print sg.get_img_code()
#	print sg.pos()
	#print sg.cname #, sg.tname
	#print sg.change_city( 145742 )
	#print sg.get_report_list(1)
	#print sg.get_build_detail(19,1)
	#print sg.buildings
	#print sg.get_building_name(28)
	#print sg.show_all_building()
	#print sg.foo()
	#print sg.geturl("/GateWay/OPT.ashx?id=38")
	#print sg.make(1,207,1)
	#print sg.get_weapon_number()
	#print sg.get_build(1)
	#print sg.get_build(15,19)
	#print sg.get_build(15,19,1)
	#print sg.get_current_wars( 116399 )
	#print sg.get_mili_info()
	#print sg.get_resouce_number()
	#print sg.change_city( 116399 )
	#print sg.get_resouce_number()
	#print "买入:", sg.buy( 20, sg._stone )
	#print "买入:", sg.buy( 8, sg._wood )
	#print sg.get_resouce_number()
	#sg.get_all_tech()
	#print sg.update_tech(12)
	#print sg.get_current_update_tech()
	#print sg.get_need_resouce(14)  # 兵器
	#print sg.get_need_resouce(14,24,-1)  # 防具
	#print sg.get_need_resouce(15,19,-1)  # 车马
	#print sg.get_need_resouce(6,14,-1)  # 大学升级
	#print sg.get_need_resouce(12)
	#print sg.get_current_update()
	#print sg.update_building(15,19)
	#print sg.force_update_building( 14)
	#print sg.get_money_number()
	#sg.buy( 1, sg._iron )
	#print sg.get_money_number()
	#sg.force_update_building(12)
	#sg.show_all_building()
#	print sg.get_img_code( )
#	print sg.get_build( 13 )
	#print sg.get_money_info(  )
	#print sg.get_building_level(14)
	#print sg.make_buildings_data()
	#print "空闲:",sg.get_people_info()[5]
	#print sg.get_work_people()
	#print sg.get_tech_update_money(14)
	#print sg.test_res_to_money(), sg.get_money_number()
	#print sg.test_res_to_money() + sg.get_money_number()
	#print sg.sell_resource(sg._wood, 1, 340 )
	#print sg.sell_all_resource()
	#print sg.get_soldier_info()
	#print sg.get_resouce_number()
	#print sg.sell_auto( SG._stone, 300 )
	#print sg.get_resouce_number()
	#print sg.task_info()
	#print sg.query_general()
	#print sg.show_weapon()
	#print sg.get_wall_info( 57747 )
	#print sg.make_wall(1,901)
	#print sg.make_wall(1,901)
	#print sg.dofind(1)  # use generals
	#print sg.get_trader_info( )
	#print sg.get_money_all_info( )[0]
