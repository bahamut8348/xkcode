#! coding:utf-8
import urllib2, time, random, urllib, re
import httplib as httplib
import json
import socket, math

import  time
from traceback import print_exc
import functools

class YX:
	
	def __init__(self, name = "yx" ):
		socket.setdefaulttimeout(20)
		self.conn = None
		self.get_new_http()
		self.headers = {}
		self.cookie=open("cookie-" + name,"rb").read()
		self.headers['Content-Type']="application/x-www-form-urlencoded; charset=UTF-8"
		self.headers['Referer'] = "http://yingxiongsvr25.webgame.xunlei.com"
		self.headers['X-Requested-With'] = "XMLHttpRequest"
		self.headers['X-Prototype-Version'] = "1.5.0"
		self.headers['User-Agent']="Mozilla/5.0 (Windows; U; Windows NT 5.2; zh-CN; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3"
		self.headers['Cookie'] = self.cookie

	def get_new_http(self):
		if self.conn:
			self.conn.close()
			del self.conn
		self.conn = httplib.HTTPConnection("yingxiongsvr25.webgame.xunlei.com:80")
	
	def geturl(self, url ):
		return "http://yingxiongsvr25.webgame.xunlei.com" + url + "&timeStamp=1236233710689&callback_func_name=callbackFnChkDetail"
	
	def send(self, url, m ="GET", data ="" ):
		self.conn.request(m, self.geturl(url), headers= self.headers, body = data)
		try:
			response = self.conn.getresponse()
		except:
			self.get_new_http()
		
		ret = response.read()
		try:
			ret = json.read(ret)
		except:
			#print "Json error:", ret
			pass
		return ret
	
	def post(self, url, data ):
		return send( url, "POST", data )
	
	def get_global_data(self):
		return self.send("/modules/refresh_scene_data.php?action=refresh&time=24")
	
	def refresh_npc(self):
		return self.send("/modules/refresh_npc_barrier.php?instance=0&time=10")
	
	def monster_fight(self, mid ):
		return self.send("/modules/monster_fight.php?mid=%d" % mid)
	
	def people_fight( self, i ):
		return self.send("/modules/duel_fight.php?action=fight&rid=%d" % i )
	
	def get_today_task(self):
		r=  self.send("/modules/task.php?timestamp=%d" % time.time())
		ma = re.findall("'day', (\d+),", r )
		return set(ma)
	
	def auto_task( self, id ):
		return self.send("/modules/role_mission.php?act=detail&op=auto_complete&function=day&id=%d" % id)
	
	def get_pk_user(self):
		
		r = self.send("/modules/duel.php?act=hall"  )
		ma0 = re.findall('<td align="left" width="15%" class="small_font">Lv.(\d+)</td>', r)
		
		ma1 = re.findall("Fight(\([^\)]+\))", r )
		ma2= map( lambda x: eval(x), ma1)
	
		ma3 = map ( lambda x,y : [int(x), y[0], y[1]] ,  ma0[:-1], ma2 )
		ma3.sort( key = lambda x:x[0] )
		return ma3
	
	def buy( self, url , t = 1, num = 100000 ):
		return self.post("mirror_money_type=%d&select_life_pool=%d&select_mana_pool=%d" % ( t, num, num ) )

if __name__ == "__main__":
	yx = YX()
	r = yx.get_today_task()

	print r