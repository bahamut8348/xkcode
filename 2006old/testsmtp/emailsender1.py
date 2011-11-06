from twisted.web import http
from twisted.mail import smtp
from twisted.internet import reactor, defer

from email.MIMEBase import MIMEBase
from email.MIMEMultipart import MIMEMultipart
from email import Encoders
import sqlite3

import sys, mimetypes, os, time, Queue
from string import Template
import cfg

from ctypes import *


from maillib import *

DEBUG = False

MAX_NUMBER = 30
sendq = Queue.Queue( 10000 )
tempdb = Tempdb()

sqldb = DB()

qmx = CDLL("qMX.dll")


class SelfError:
	def __init__(self, msg ):
		self.msg = msg
	def getErrorMessage(self):
		return self.msg

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
			StartSend()
		else:
			print "no content"
	request.write( ret )
	request.finish( )	

class EmailSender:
	def __init__( self, fromaddr, toaddr, subject, body, emailid, id):
		self.fromaddr = fromaddr
		self.toaddr = toaddr
		self.subject = subject
		self.body = body
		self.emailid = emailid
		self.id = id
	
		if DEBUG:
			self.fromaddr = '"xu" <xk@warriorepic.com>'
	
	def getSMTPServer(self, emailAddress):
		domain = ""
		try:
			username, domain = emailAddress.split("@")
		except:
			domain = emailAddress
		domain = domain.strip(" >")
		return domain
	
	def sendComplete(self, result):
		print time.strftime("[%Y-%m-%d %H:%M:%S]"),"Success", result
		sendq.get()
		StartSend()
		sqldb.setResult( self.emailid,  str(result),  self.id , 1)
		
	def handleSendError(self, error):
		print time.strftime("[%Y-%m-%d %H:%M:%S]"), "Error", error.getErrorMessage( )
		sendq.get()
		StartSend()
		sqldb.setResult( self.emailid,  error.getErrorMessage( ),  self.id , 0)
	
  
	def send(self):
		msg = buildmsg( self.fromaddr, self.toaddr, self.subject, self.body )
		faddr = self.fromaddr
		if self.fromaddr.find("<") > 0:
			name, faddr = self.fromaddr.split("<")
			faddr = faddr.strip("  >")
			print faddr
		cbuf = create_string_buffer('\000' * 32)
		domain = self.getSMTPServer(self.toaddr)
		if qmx.QueryMX( c_char_p (domain), cbuf) == 1:
			sending = smtp.sendmail( cbuf.value, faddr, self.toaddr, msg)
			sending.addCallback( self.sendComplete).addErrback( self.handleSendError )
		else:
			self.handleSendError( SelfError("Query MX For %s Failed" % domain) )
		
		#self.getSMTPServer(self.toaddr).addCallback(self.getMX).addErrback(self.handleSendError)
		
		
		
def StartSend():
	a = MAX_NUMBER - sendq.qsize()
	print "Current Queue:", sendq.qsize()
	cfg = {}
	if a > 0:
		for e in range( a ):
			id, email, emailid = tempdb.getEmail()
			if( id == "") : return 
			sqldb.putResult( emailid,  id )
			if not cfg.has_key(emailid):
				cfg[emailid] = sqldb.getContent(emailid)
			allinfo = sqldb.getAccountInfo( id  )
			sender = EmailSender( cfg[emailid]['sender'], email, cfg[emailid]['subject'], Template(cfg[emailid]['msg']).safe_substitute( allinfo ), emailid, id )
			print email
			sendq.put(1)
			sender.send()
			
			
def handlePost(request):
	request.write("its a post")
	request.finish( )

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

if __name__ == "__main__":
	port = cfg.port
	os.system("title EmailSender - PS")
	print "Start OK", "port:", port
	try:
		tempdb.createTable()
	except:
		pass
	reactor.listenTCP(port, MyHttpFactory( ))
	reactor.run( )
