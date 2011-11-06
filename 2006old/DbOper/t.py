

import sql
from sql import geterror

db1 = u"192.168.1.13",u"psweb",u"psweb",u"webserver"
#db2 = u"192.168.1.13",u"psweb",u"psweb",u"webserver"
db2 = u"192.168.1.13",u"psweb",u"psweb",u"webserver"

# sync db1 to db2

otb = "account1"
ntb = "acc2"

def check(ret,desp):
	if ret:
		print "[+]", desp, "OK"
		return True
	else:
		print "[-]", desp, "FAILED:", sql.geterror()
		return False
		
def createtable():
	check(sql.conndb(*db2), "Connect to db2: %s" % db2[0])
	sql.execsql("drop table %s" % ntb)
	s = 'select * into %s from %s' % (ntb, otb)
	print "- Create table:", sql.execsql(s)
	
def cls():
	print "- Conndb2:", sql.conndb(*db2)
	print "- Truncate table:", sql.execsql("truncate table %s" % ntb)
	

def l2s(l):
	c=0
	ret=""
	for e in l:
		t = str(type(e))
		
		if t=="<type 'datetime.datetime'>":
			l[c] = u"'%s'" % unicode(e.isoformat(' '))
		elif t=="<type 'unicode'>":
			l[c] = u"N'%s'" % e
		elif t=="<type 'long'>":
			l[c] = u"%d" % e
		
		c+=1
		
	ret = ",".join(l)
	ret = "(%s)" % ret
	return ret

def formatdesp(l):
	ret = u"],[".join(l)
	ret = u"([%s])" % ret
	return ret


def syncdb1todb2():

	check(sql.conndb(*db2), "Connect to db2")
	desp,rs = sql.query("select [id] from %s order by [id] desc;" % ntb)
	
	maxid = 0
	if rs==False:
		print "[-]", "Error:",geterror()
	else:
		if len(rs) > 0:
			maxid = rs[0][0]

	print "[+]", "GetMaxID:", maxid
	check(sql.conndb(*db1), "Connect To db1")
	desp, rs  = sql.query("select id, loginname, password, lastlogintime, lastloginip, Email, nickname, city, sex, age, sendemail, status, birth, rightid, type from %s where id > %d" % (otb, maxid))
	keys = formatdesp(desp)
	check(sql.conndb(*db2), "Connect To db2")
	r = check(sql.execsql("set IDENTITY_INSERT %s on" % ntb), "set IDENTITY_INSERT on")
	if not r: return
	for e in rs:
		pswd = e[2]
		userid = e[0]
		v = l2s(e)
		s = u"insert into %s %s values %s\n" % (ntb,keys, v)
		s += u"update %s set password=N'%s' where id=%s" % ( ntb, sql.md5(pswd), e[0])
		check(sql.uexecsql(s), u"Insert and Update Password for Account(%d) %s" % (userid,e[1]))
	
	check(1,"Update %d record" % len(rs))
	
	
	
	
if __name__ == "__main__":
	cls()
	syncdb1todb2()
	
	#print sql.md5(u"samsam")
	#createtable()
	#sql.conndb(*db1)
	#desp,rs = sql.query("select * from account1 where id=1")
	#print rs[0]
	#print sql.md5(rs[0][2])
	#print `formatdesp(desp)`
	#for e in desp:
	#	print `l2s(e)`


"""
login(user, pass, 1, "test login")

uer="ll"
pass= "df"
register(email="lll", username="kk" , return=0x100045, desp( ) )


def   recvmsg(CHATMSG, from, content):
	if msgtype=="":
"""
	  
