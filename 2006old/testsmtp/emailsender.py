
from threadpool import ThreadPool
import time
from functools import partial

from twisted.web import http
from twisted.mail import smtp
from twisted.internet import reactor, defer
from twisted.internet import threads

from email.MIMEBase import MIMEBase
from email.MIMEMultipart import MIMEMultipart
from email import Encoders
import sqlite3

import sys, mimetypes, os, time, Queue, random
from string import Template
import cfg
import socket, smtplib, copy

from ctypes import *
from maillib import *

DEBUG = False

tempdb = Tempdb()

sqldb = DB()

qmx = CDLL("qMX.dll")

socket.setdefaulttimeout(30)

readnumber = 20
retry_count = 3

def sendmailfun( name, loginname, emailid, email, mailinfo, sendnum  ):

	print "Send:", email
	def error ( msg ):
		reactor.callLater(0, sqldb.setResult, emailid,  str(msg),  loginname , 0 )
	
	def finish( msg ):
		reactor.callLater(0, sqldb.setResult, emailid,  str(msg),  loginname , 1 )

	sender = mailinfo['sender']
	subject = mailinfo['subject']
	omsg = mailinfo['msg']
	body = Template(omsg).safe_substitute( mailinfo )
	cbuf = create_string_buffer('\000' * 100 )

	# domin
	domain = ""
	try:
		username, domain = email.split("@")
	except:
		domain = email
	domain = domain.strip(" >")
	if 0==qmx.QueryMX( c_char_p(domain) , cbuf ):
		return error( "Query MX for %s Failed" % domain )
	
	try:
		smtp = smtplib.SMTP(cbuf.value)
		if DEBUG:
			smtp.set_debuglevel(1)
		smtp.sendmail(sender, email, buildmsg(sender, email, subject, body))
		smtp.quit()
		return finish( email )
	except Exception, err:
		code = -1
		msg = ""
		try:
			code, msg = err
		except:
			msg = str(err)
		print email, code, msg
		if int(code) in [421,451,450]:
			sendnum += 1
			sendfun = partial( sendmailfun, name= str(id), loginname=loginname, emailid=emailid, email = email, mailinfo = mailinfo , sendnum = sendnum )
			if  sendnum <= retry_count :
				tp.addtask(sendfun, random.randint( 600, 30*60 ) )
			return error("Host: %s, Retry: %d, Error: %d,%s" % ( cbuf.value, sendnum, code, err) )
		else:
			return error("Host: %s, Error: %d,%s" % ( cbuf.value, code, err) )
	
def ontaskdone( task ):
	pass


tp = ThreadPool( readnumber )
tp.setcallback( ontaskdone )

def handlePost(request):
	request.write("its a post")
	request.finish( )

def renderHomePage(request):
	args = request.args
	ret = "ERROR " + str(args)
	action, emailid = 0,0
	try:
		action = args['action'][0]
		emailid = int(args['id'][0])
	except:
		request.write( ret )
		request.finish( )
	
	if action == "sendmail":
		cfg = sqldb.getContent( emailid )
		if cfg:
			#print cfg
			print "Start Send Mail"
			if tempdb.hasTask( emailid ):
				ret = "ERROR already send"
				if DEBUG:
					tempdb.setEmailid( emailid )
			else:
				# copy table
				print "SQL:", cfg['sql']
				cur = sqldb.emailList( cfg['sql'] )
				if DEBUG == False:
					pass
					tempdb.putEmail( cur, emailid )
				if DEBUG:
					tempdb.setEmailid( emailid )
				ret = "Send mail ok, id=" + str( emailid )
			
		else:
			print "no content"
	elif action == "retry":
		n = recovermail( sqldb, tempdb, emailid )
		ret += "number %d" % n
	elif action == "getinfo":
		t, r = tempdb.getInfo( emailid )
		ret += "total: %d, rest: %d" % (t,r)
	elif action == "clear":
		tempdb.clear()
		ret+= "clear datebase"
	request.write( ret )
	request.finish( )	

def StartSend():
	emailcfg = {}

	n = readnumber - tp.count()
	#print tp.count(), n
	while n > 0:
		id, loginname, email, emailid = tempdb.getEmail()
		n -= 1
		if( id == "") : 
			return
		sqldb.putResult( emailid,  loginname )
		if not emailcfg.has_key(emailid):
			emailcfg[emailid] = sqldb.getContent(emailid)
		if emailcfg[emailid]['status'] != 0: continue
		allinfo = dict( loginname = loginname,Email = email )
		info1 = copy.deepcopy(emailcfg[emailid])
		info1.update(allinfo)
		
		sendfun = partial( sendmailfun, name= str(id), loginname=loginname, emailid=emailid, email = email, mailinfo =info1, sendnum = 0 )
		tp.addtask( sendfun )


class FunctionHandledRequest(http.Request):

	pageHandlers = {
		'/': renderHomePage,
		'/posthandler': handlePost,
	}
	def process(self):
		self.setHeader('Content-Type', 'text/html')
		if self.pageHandlers.has_key(self.path):
			handler = self.pageHandlers[self.path]
			handler(self)
		else:
			self.setResponseCode(http.NOT_FOUND)
			self.write("<h1>Not Found</h1>Sorry, no such page.")
			self.finish( )

class MyHttp(http.HTTPChannel):
    requestFactory = FunctionHandledRequest

class MyHttpFactory(http.HTTPFactory):
    protocol = MyHttp


def loopquery():
	StartSend()
	reactor.callLater(1, loopquery)


if __name__ == "__main__":
	port = cfg.port
	os.system("title EmailSender - PS")
	print "Start OK", "port:", port
	try:
		tempdb.createTable()
	except:
		pass
	reactor.listenTCP(port, MyHttpFactory( ))
	reactor.callLater(1, loopquery)
	reactor.run( )
	print "Stoping"
	tp.stop()


