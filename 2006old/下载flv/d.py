import urllib2

import cookielib, sys, os


	
def ku6(url,fn):
	
	req = urllib2.Request(url)
	req.add_header("user-agent","IE5")
	r = urllib2.urlopen(req)
	totallen = r.info()['Content-Length']
	filelen=0
	try:
		F = open(fn,"rb")
		filelen = len(F.read())
		F.close()
	except:
		F=open(fn,"wb")
		F.close()
		
	recvlen = 0
	recvdata = ""
	while 1:
		data = r.read(1024)
		recvlen += len(data)
		print recvlen, totallen, float(recvlen) * 100 / float(totallen),"%","\r", 
		ret = recvlen - filelen+len(recvdata)
		if ret > 0:
			recvdata += data[-ret:]
			if len(recvdata)>=2*1024*1024:
				print "Write",  len(recvdata)
				F = open(fn,"ab")
				F.write(recvdata)
				F.close()
				recvdata = ""
				filelen = recvlen
		elif ret==0:
			F = open(fn,"ab")
			F.write(recvdata)
			F.close()
			break
		else:
			continue
			
	
				
	print 
	print "done", fn
	
if __name__ == "__main__":
	exename, url, fn = sys.argv
	os.system("title "+fn)
	ku6(url , fn)
	
	
	