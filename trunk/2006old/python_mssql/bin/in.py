#! coding: utf-8
import mssql
import datetime
import time, os

s = """
INSERT INTO [psplayer].[dbo].[UserLog_2007-10-24]
           ([AccountId]
           ,[IP]
           ,[SessionId]
           ,[ISPSUId]
           ,[OperationType]
           ,[OperationDescription]
           ,[LogTime]
           ,[ProductId]
           ,[MajorVersion]
           ,[SubVersion]
           ,[ISAccountId]
           ,[ISIP])
     VALUES
           (44
           ,'192.168.1.13'
           ,'sessionid'
           ,'psuid'
           ,2
           ,'OperationDescription'
           ,'%s'
           ,19
           ,1
           ,1
           ,1
           ,'ISIP');
"""

sql = mssql.mssql()
sql.conn("192.168.1.13","psplayer","psplayer","psplayer")
print time.strftime("%Y/%m/%d %H:%M:%S")
start = time.time()

n= 1

while 1:
	loginname = "ps_008A%d" % n
	s1 = "insert into account( loginname, password, nickname, email, country, city, sex, birth, sendemail, rightid, type, RegProductID ) values ( N'%s', N'%s', N'%s', N'%s', %d, N'%s', %d, '%d-%d-%d', %d, %d, %d, %d )"  % (loginname, "09e31d1ab4de57a06dac4834a7cb1432", "","dsf@163.com",4,"d",1, 1983,12,1, 1,1,1,1)
	sql.exe(s1)
	n+=1
	if n % 1000 == 0:
		print n ,time.time()
	
	
os.exit(1)

a = 1
step = 1
while a<1000:
	s1 = "".join( map( lambda x: s % time.strftime("%Y/%m/%d %H:%M:%S"), range(step) ))
	sql.exe(s1)
	a+=step
print time.strftime("%Y/%m/%d %H:%M:%S")
print "total:",time.time()-start

	




