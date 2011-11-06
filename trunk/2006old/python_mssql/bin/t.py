#! coding: utf-8
import mssql
import datetime
import time


sql = mssql.mssql()
sql.conn("192.168.1.65","psplayer","psplayer","psplayer")
print time.strftime("%Y/%m/%d %H:%M:%S")
n = 0
last = 0
while 1:
	n+=1
	data = sql.query("select loginname ,id, password, nickname, email, country, city, sex, birth, sendemail, rightid, [money] from account where loginname = N'ps_ma.rui'")
	if n % 1000 ==0:
		t = time.time()
		print n, t, t - last
		last = t
#data = sql.query("select  * from account where loginname ='ps_test'")

#r2 = data[0]
#print` r2['password']`
#s = r2['password']
#print `s`
#r = data[1]
#r['password'] = s
#r.update()

