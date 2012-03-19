#! coding:utf8
import pickle
import socket
socket.setdefaulttimeout( 5 )
import requests, PIL, urllib2, urllib
import cookielib
from PIL import Image
from urllib2 import urlopen
import time
#! encoding:utf8

import win32ui
import win32gui
import os
os.system("title aaa")

jar = cookielib.LWPCookieJar()
jar.load( "cookie", ignore_discard=True, ignore_expires=True )
urllib2.install_opener( urllib2.build_opener ( urllib2.HTTPCookieProcessor( jar ) ) )

F = urllib2.urlopen( "https://dynamic.12306.cn/otsweb/loginAction.do?method=init" )
print jar
F.close()

def getcode():
	F = urlopen( "https://dynamic.12306.cn/otsweb/passCodeAction.do?rand=lrand" )
	with open("code.gif", "wb") as f:
		f.write( F.read() )

	img = Image.open("code.gif")
	img.save( "code.bmp", "BMP" )

	bmp = win32ui.CreateBitmap()
	bmp.LoadBitmapFile( open("code.bmp" ) )
	dc = win32ui.CreateDCFromHandle( win32gui.GetDC( win32gui.FindWindow(None,"aa")) )
	bmp.Paint( dc )
	while 1:
		code = raw_input("code:") 
		if code == "r" : return getcode()
		return code

def login():
	code = getcode()
	F = urlopen( "https://dynamic.12306.cn/otsweb/loginAction.do?method=login", urllib.urlencode( { "loginUser.user_name":"lovecpp@gmail.com", "user.password":"70ddAW","randCode": code } ) )
	with open("login.txt","wb") as f:
		d = F.read()
		if d.find("用户过多") > 0:
			print("too many user")
			return 1
		elif d.find("我的订单") > 0:
			print("login ok")
			jar.save( "cookie", ignore_discard=True, ignore_expires=True )
			return 2
		f.write( d )


def buy():
	win32gui.FlashWindow( win32gui.GetForegroundWindow(), 0 )
	print("! found")


buy()
import re
ls = set( ["D365", "D315", "D317","D319", "D331", "D333", "D335", "D337"] )
def query():
	req = urllib2.Request( "https://dynamic.12306.cn/otsweb/order/querySingleAction.do?method=queryLeftTicket&orderRequest.train_date=2012-01-21&orderRequest.from_station_telecode=VNP&orderRequest.to_station_telecode=JNK&orderRequest.train_no=&trainPassType=QB&trainClass=QB%23D%23Z%23T%23K%23QT%23&includeStudent=00&seatTypeAndNum=&orderRequest.start_time_str=00%3A00--24%3A00" ) 
	req.add_header( "Referer", 	"https://dynamic.12306.cn/otsweb/order/querySingleAction.do?method=init" )
	F = urlopen( req )
	with open("query.html", "wb") as f:
		d = F.read()
		f.write( d )
		if len(d) > 100:
			ma = re.findall( "(\w+)</span>.*?:\d\d,\d\d:\d\d(.*?)<input", d )
			for m in ma:
				if m[0] not in ls : continue
				s = map( lambda x: x if x.find("<") == -1 else re.findall(">([^<]+)<", x )[0], m[1].split(",") )[0:5]
		#		s = "\t".join( s ).decode("utf8").encode("gbk")
				print m[0], s[4].decode("utf8").encode("gbk")
				if s[4] != "无":
					buy()
					return 1
			return True

while 1 :
	try:
		q = query()
		if not q:
			while 1 :
				r = login()
				if r == 1 : continue
				if r == 2 : break
			
			print "login"
		print(" -------- wait -------- ", time.time())
		if q is 1 : break
		time.sleep( 1 )
	except:
		pass
"""
pic = "https://dynamic.12306.cn/otsweb/passCodeAction.do?rand=lrand"
req = requests.get( pic )
with open("code.gif", "wb") as F:
	F.write( req.content )
"""
