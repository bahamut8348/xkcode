from twisted.web import http
from twisted.mail import smtp

from email.MIMEBase import MIMEBase
from email.MIMEMultipart import MIMEMultipart
from email.MIMEText import MIMEText
from email.message import Message

from email import Encoders
import sqlite3
import adodbapi
import time
from threading import Lock
import cfg


import sys, mimetypes, os, time, Queue

class DB:
	def __init__( self ):
		self.con = adodbapi.connect( cfg.connectionstring )
		self.cur = self.con.cursor()
		self.lock = Lock()

	def Execute(self, sql ):
		try:
			self.lock.acquire()
			ret =  self.cur.execute(sql)
			self.lock.release()
		except:
			self.close()
			self.con = adodbapi.connect( cfg.connectionstring )
			self.cur = self.con.cursor()
			ret = self.cur.execute(sql)
			self.lock.release()
		return ret
	
	def putResult( self, emailid, loginname, succ=-1, err = "" ):
		sql = "insert into GroupEmailResult (loginname, emailid, result, isSuccess)values('%s',%d,'%s',%d) " % (loginname, emailid, err, succ )
		self.Execute( sql )
		self.con.commit()
	
	def setResult( self, emailid, err, loginname, succ ):
		err = err.replace("'","''")
		sql = "update GroupEmailResult set result='%s' , isSuccess=%d where emailid=%d and  loginname='%s'" % (err, succ, emailid, loginname )
		self.Execute( sql )
		self.con.commit()
	
	def getContent( self, emailid ):
		sql = "select subject, sender, emailcontent, searchcondition, status from groupEmail where id=%d" % emailid
		self.Execute( sql )
		row = self.cur.fetchone()
		ret = {}
		if row :
			ret = dict( subject= row[0], sender=row[1], msg=row[2], sql=row[3], status = row[4] )
		return  ret
	
	def getAccountInfo( self, accid ):
		cols = "loginname","Email"
		sql = "select " + ",".join(cols) + " from account where id = %d" % accid
		self.Execute( sql )
		row = self.cur.fetchone()
		ret = {}
		if row:
			for c, e in enumerate( cols ):
				ret[e] = row[c]
		return ret
	
	def hasSend( self, loginname, emailid ):
		sql = "select 1 from GroupEmailResult where loginname='%s' and emailid=%d" % ( loginname, emailid )
		self.Execute( sql )
		if self.cur.fetchone():
			return True
		else:
			return False
	
	def emailList(self, sql ):
		self.Execute( sql )
		return self.cur
	
	def close(self):
		try:
			self.con.close()
		except:
			print "Warning Connect Close, Retry"
	
	


class Tempdb:
	def __init__( self ):
		self.conn = sqlite3.connect('tempdata')
		self.cur = self.conn.cursor()
	
	def clear(self):
		self.cur.exeute("delete from emails")
		self.conn.commit()
	
	def hasTask( self, emailid ):
		self.cur.execute("select 1 from emails where emailid = %d " % emailid)
		row = self.cur.fetchone()
		return row != None
		
	def createTable( self ):
		self.cur.execute("create table emails (id INTEGER PRIMARY KEY AUTOINCREMENT, email text, hasend INTEGER, loginname text, emailid INTEGER )")
	
	def setEmailid( self, emailid ):
		sql = "update emails set emailid = %d , hasend=0" % emailid 
		self.cur.execute( sql )
		self.conn.commit()
	
	def putEmail( self, cur, emailid ):
		count = 0
		for row in cur:
			self.cur.execute("insert into emails (email, hasend, loginname, emailid ) values ('%s', %d, '%s', %d )" % ( row[1], 0, row[0], emailid ) )
			count += 1
		self.conn.commit()
		print "Data Number:", count
		# remove repeat
		#self.cur.execute("delete from emails where id not in (select Max(id) from emails group by email) and emailid = %d" % emailid )
		#self.conn.commit()
		print "Read Data Done"
	
	def getInfo( self, emailid ):
		total = 0
		rest = 0
		self.cur.execute("select count(1) from emails where emailid = %d" % emailid)
		row = self.cur.fetchone()
		total = row[0]
		self.cur.execute("select count(1) from emails where emailid = %d and hasend = 0" % emailid)
		row = self.cur.fetchone()
		rest = row[0]
		return total , rest

	
	def getEmail( self ):
		self.cur.execute("select id, loginname, email, emailid from emails where hasend=0")
		row = self.cur.fetchone()
		id, loginname, email, emailid = '','','',''
		if row:
			id, loginname, email, emailid = row
			self.cur.execute("update emails set hasend=1 where id=%d" % id)
			self.conn.commit()
		return id, loginname, email, emailid

def recovermail( src, dst, emailid ):
	ret = 0
	sql = "select GroupEmailResult.accountid, email from GroupEmailResult, account where GroupEmailResult.issuccess=0 and GroupEmailResult.emailid=%d and account.id = GroupEmailResult.accountid" % emailid
	src.cur.execute( sql )
	for row in src.cur:
		ret += 1
		dst.cur.execute("insert into emails (email, hasend, accountid, emailid) values ('%s', %d, %d, %d )" % ( row[1], 0, row[0], emailid ) )
	
	dst.conn.commit()
	src.cur.execute(" update groupemailresult set issuccess=2 where issuccess = 0  and emailid = %d" % emailid )
	src.con.commit()
	return ret
		
	

def buildmsg( fromaddr, toaddr, subject, body ):
	message = MIMEMultipart() 
#	message = MIMEText("body")
	message['Date'] = time.strftime("%a, %d %b %Y %H:%M:%S +0000", time.gmtime())
	message['From'] = fromaddr
	message['To'] = toaddr
	message['Subject'] = subject
	message['Sender'] = fromaddr
	message['X-mailer'] = 'PS - Email Sender 1.0.0'
	
	textPart = MIMEBase('text', 'html')
	textPart.set_payload(body)
	message.attach(textPart)
	return message.as_string( False )
	
	
if __name__ == "__main__":
	import smtplib

	msg = buildmsg( 'tset <test@possibilityspace.com>', "cnxuke@hotmail.com", "sub_test", "sdf")
	server = smtplib.SMTP('65.54.244.168')
	server.set_debuglevel(1)
	server.sendmail("test@possibilityspace.com", "cnxuke@hotmail.com", msg)
	server.quit()




