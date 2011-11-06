
from libsg import SG, tostr

import sqlite3 
#from trackback import print_exec

def tostr2( s ):
	return s
togbk = tostr
tostr = tostr2

sg =SG()

class DB:
	
	def __init__(self):
		self.conn = sqlite3.connect("map.sqlite")
		self.cursor = self.conn.cursor()
	
	def insert( self, username, populariry, userid, login_name, citynum, union_, civilizational, pos ):
		self.cursor.execute("insert into user (username, popularity, user_id, login_name, citynum, union_name, civilizational, pos) values('%s', %d,%d,'%s', %d, '%s', %d, %d) " % (username, populariry, userid, login_name, citynum, union_, civilizational, pos) )
		self.conn.commit()
	
	def has( self, userid ):
		self.cursor.execute("select 1 from user where user_id=%d" % userid )
		return self.cursor.fetchone( )


def test():
	r = sg.lookup_xy( 0, 0 )

	users = r['users']
	db = DB()
	for user in users:
		print user[0], tostr(user[1]), tostr(user[3])
		info = sg.get_user_info(user[0])
		print info
		cities = info['citylist']
		info = info['userinfo'][0]
		print cities
		print info
		#db.insert( tostr(info['login_name']), int(info['popularity']), int(info['user_id']), info['login_name'], len(cities), tostr(info['union_name']), int(info['civilizational']), int(cities[0]['map_id']) )

	for city in r['citys']:
		print sg.stand_xy(city[0]), tostr( city[1] )


def doit():
	db = DB()
	n = 120
	for x in range(-n, n, 8 ):
		for y in range(-n, n, 8 ):
			r = sg.lookup_xy(x,y)
			users = r['users']
			for user in users:
				print user[0] #, user[1].decode("utf8"), user[3].decode("utf8")
				if db.has(user[0]) : continue
				info = sg.get_user_info(user[0])
				cities = info['citylist']
				info = info['userinfo'][0]
				try:
					db.insert( tostr(info['login_name']), int(info['popularity']), int(info['user_id']), info['login_name'], len(cities), tostr(info['union_name']), int(info['civilizational']), int(cities[0]['map_id']) )
				except:
					pass

if __name__ == "__main__":
	doit()
