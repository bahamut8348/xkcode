
# database ip
ip = "192.168.1.13"
# new table name
newtable = "acc2"

from sql import *

def perror():
	print "[-] Error:", geterror()

def conn():
	
	print "[+] Connecting:", ip
	if conndb(ip)==0:
		print "[-] Error:", geterror()
	else:
		print "[+] Connect success!"
		#execsql("drop table %s" % newtable )

def createtable():
	sql_data = "select * into %s from account1" % newtable
	ret = execsql(sql_data)
	if ret==0:
		print "[-] createtable error", geterror()
		return 0
	else:
		print "[+] create new table",newtable
		return 1



def updatepassword():

	all = query("select id, password from %s" % newtable)
	if all==0:
		perror()
	else:
		print "[+] Get all account:", len(all)
		for e in all:
			i = e[0]
			pwd = e[1]
			pwd = md5(pwd)
			
			rs = execsql("update %s set password = N'%s' where id=%d" % (newtable, pwd, i))
			if rs==0:
				perror()
			else:
				pass
				#print "[+] update password  ok id:", i
		print "[+] All Update"

if __name__ == "__main__":
	conn()
	print datetime
	query("select top 1 * from account1")

"""
	if createtable():
		updatepassword()
	else:
		print "[-] Failed"
		#execsql("drop table %s" % newtable)
"""


"""
insert into b(a, b, c) select d,e,f from b;

select a.col1, b.col2, c.col3
into table4
from tableA a, tableB b, tableC c
where 1 = 0


INSERT INTO archivetitles 
   (title_id, title, type, pub_id)
SELECT title_id, title, type, pub_id
FROM titles
WHERE (pub_id = '0766')


"""
