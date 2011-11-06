import winpcap as pcap
from construct import *
from libarp import IpAddress,MacAddr
import sqlite3

cap = pcap.open()

class DB:
	def __init__(self):
		self.conn = sqlite3.connect("data.sqlite")
		self.cur = self.conn.cursor()
	def execute(self, sql):
		r= self.cur.execute(sql)
		self.conn.commit()
		return r
	def get(self, mac):
		print mac
		k = self.execute("select mac, ip, name from mac where mac = '%s'" % mac )
		return k.fetchone()
	
	def setvalue(self, mac, ip, name ):
		self.execute("update mac set ip='%s', name='%s' where mac='%s' " % (ip, name,mac) )
	
	def add(self, mac, ip, name ):
		self.execute("insert into mac (mac,name, ip) values('%s','%s','%s')" % (mac, name, ip) )

db = DB()
for t, data in cap:
	if len(data) < 217:
		continue
	if data[12] != '\x08':
		continue
	if data[23] != '\x11':
		continue
	if data[37] != '\x8a':
		continue
	if data[42] != '\x11':
		continue
	if data[0xd2] != '\x01':
		continue
	s = CString("s").parse(data[193:])
	
	if s != '\MAILSLOT\BROWSE' : continue
	name = CString("name").parse(data[216:])
	ip = IpAddress("IP").parse(data[26:])
	mac = MacAddr("Mac").parse(data[6:])
	row = db.get(mac)
	if row:
		db.setvalue(mac, ip, name )
	else:
		db.add(mac, ip, name )
	print mac, ip, name
	
	
	