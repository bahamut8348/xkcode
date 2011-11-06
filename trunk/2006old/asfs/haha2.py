#!/usr/bin/env python
#! coding: utf-8
from libsg import SG
from twisted.internet import reactor, defer
from twisted.internet import reactor, defer
from twisted.internet import threads
from libsg import SG, tostr
import  time
from traceback import print_exc
import functools
from smtplib import SMTP
from email.mime.text import MIMEText
from random import randint

call_func_error_no = 0

def sendemail( content ):
	f = '"三国" <hk888@163.com>'
	t = 'netmud@gmail.com'
	smtp = SMTP()
	smtp.connect('smtp.163.com')
	smtp.login("hk888","3281044")

	msg = MIMEText(content)
	msg['subject']='三国第二个城'
	msg.set_charset('gbk')
	msg['From'] = f

	smtp.sendmail( f,t, msg.as_string( ) )
	

def call_update_building2( rgids = [] ):
	gids = map( lambda x:abs(x), rgids )
	tasklist = sg.get_current_update()
	bs = sg.get_all_building()
	vv = bs
	bs = filter( lambda x:x[3]<33, bs )
	
	l = len( tasklist)
	ts = tasklist.values()
	print sg.cname
	for e in tasklist:print "\t", sg.get_building_name( e ),"级别:", filter( lambda x:x[1] == e , vv )[0][3], "剩余时间:", tasklist[e]
	if len(ts) == 3 : return min(ts)
	if gids: bs = filter( lambda x:x[0] in gids , bs )
	m = min(ts) if l else 0
	bdings = filter( lambda x:x[1] in tasklist.keys(), bs )
	bs = filter( lambda x:x[1] not in tasklist.keys(), bs )
	if len(bs) == 0 :
		print  sg.cname, "无法升级成功任何建筑",min(ts),"秒后重试"
		return min(ts)
	bs.sort( key = lambda x:x[3] )

	# need destroy
	dgids = filter( lambda x:x <0 , rgids )
	dbs = filter( lambda x:-x[0] in dgids , bs )
	ndbs = filter( lambda x:-x[0] not in dgids , bs )
	for obj in dbs:
		ret = sg.destroy_building( obj[0], obj[1] )
		print sg.cname, "拆除", tostr(obj[2]), obj[1], "级别:", obj[3], ret['ret'] == 0
		return 5

	for obj in ndbs:
		if -obj[0] in rgids : continue
		ret = sg.force_update_building( obj[1] )
		print sg.cname, "升级", tostr(obj[2]), obj[1], "级别:", obj[3], ret['ret'] == 0
		if ret['ret'] == 0:
			return 5
	else:
		t = m if m > 0 and m < 300 else randint( 280, 320 )
		print  sg.cname, "无法升级成功任何建筑",t,"秒后重试"
		return t

def call_update_smart( upgrade=4 ):
	tasklist = sg.get_current_update()
	bs = sg.get_all_building()
	h_gid = 3
	l = len(tasklist)
	ts = tasklist.values()
	if l:
		m = min(ts)
		print sg.cname
		for e in tasklist:print "\t", sg.get_building_name( e ),"级别:", filter( lambda x:x[1] == e , bs )[0][3], "剩余时间:", tasklist[e]
	if l>=3:
		print sg.cname, m, "秒后重试"
		return m
	
	for t in tasklist:
		if sg.get_building_gid( t ) != h_gid :
	
			bs = filter( lambda x: x[0] != 5, bs )
			bs = filter( lambda x: x[0] == 3, bs )
			bs = filter( lambda x: x[1] not in tasklist.keys(), bs )
			bs.sort( cmp = lambda x,y : cmp(x[3], y[3]) )
			
			for obj in bs:
				ret = sg.force_update_building( obj[1] )
				print sg.cname, "升级", tostr(obj[2]), obj[1], "级别:", obj[3], ret['ret'] == 0
				if ret['ret'] == 0:
					break
			return 5
	else:
		obj = sg.get_pid_by_gid( upgrade )[0]
		if obj[3] < 33:
			ret = sg.force_update_building( obj[1] )
			print sg.cname, "升级", tostr(obj[2]), obj[1], "级别:", obj[3], ret['ret'] == 0
		else:
			ret = sg.destroy_building( obj[0], obj[1] )
			print sg.cname, "拆除", tostr(obj[2]), obj[1], "级别:", obj[3], ret['ret'] == 0
		
		return 5


def call_func( func, cid, *args, **awk ):
	global call_func_error_no
	try:
		
                sg.change_city( cid )
		r = func( *args, **awk )
		if r == -1: return
		call_func_error_no = 0
	except Exception, err:
		print func, sg.cid, args
                
		print_exc( )
		call_func_error_no += 1
		if call_func_error_no == 100 : threads.deferToThread( sendemail, "连接出错.." )
		if call_func_error_no == 150 : threads.deferToThread( sendemail, "连接出错.." )
		if call_func_error_no == 150 : reactor.stop()
		reactor.callLater( 10, call_func, func, cid, *args, **awk )
		return
	reactor.callLater( r, call_func, func, cid, *args, **awk )

def show_money( ):
	m = sg.get_money_number()
	print  time.asctime(), sg.cname, ", 铜钱:", m
	return 15


call_update_all =  functools.partial( call_update_building2, [3] ) #8, 6,7
#call_update_all =  functools.partial( call_update_building2, [1,4,11] ) #8, 6,7
# 3 为房屋k

sg = SG(179973)

cid = 180312 

call_func( call_update_all, 180312)
call_func( show_money, 180312 )
reactor.run()
