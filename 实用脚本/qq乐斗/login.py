
import urllib2, urllib, cookielib
import random, re, hashlib
import Cookie

jar = cookielib.LWPCookieJar()
urllib2.install_opener(urllib2.build_opener(urllib2.HTTPCookieProcessor(jar)))

def urlopen(url, data = None):
	req = urllib2.Request(url, data)
	req.add_header("User-Agent","Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/534.30 (KHTML, like Gecko) Chrome/12.0.742.9 Safari/534.30")
	req.add_header("Referer","http://fight.pet.qq.com/")
	F = urllib2.urlopen(url)
	return F.read()


def md5_3(s): return hashlib.md5(hashlib.md5(hashlib.md5(s).digest()).digest()).hexdigest().upper()
def ptui_checkVC(a,b,c): return b,c
def md5hash(str): return hashlib.md5(str).digest()
def hex_md5hash(str): return hashlib.md5(str).hexdigest().upper()


class QQ:
	def __init__(self, qq, passwd):
		self.qq = qq
		self.passwd = passwd
	
	def getVerifycode(self):
		url = "http://qzone.qq.com/"
		urlopen(url)
		url = 'http://ptlogin2.qq.com/check?uin='+ ('%s' % self.qq)+'&appid=15000101&r='+ ('%s' % random.Random().random())
		url = 'http://check.ptlogin2.qq.com/check?uin=3222630&appid=37000201&r=0.4299010103090598'
		url = 'http://check.ptlogin2.qq.com/check?uin=%s&appid=37000201&r=%s' % (self.qq, random.Random().random())
		print "-> VerifyCode"
		print "   Url:", url
		data = urlopen(url)
		print "   ", data
		return eval(data.strip(";"))
	
	def genp(self, verifycode):
		vcode, uin = verifycode
		P1 = md5hash(self.passwd)
		P2 = hex_md5hash(P1+uin)
		C = hex_md5hash(P2+vcode)
		return C

	def login(self):
		jar.clear()
		code = self.getVerifycode()
		p = self.genp(code)
		verifycode = code[0]
		url = 'http://ptlogin2.qq.com/login?u=%s&p=%s&verifycode=%s&aid=37000201&u1=http%%3A%%2F%%2Ffight.pet.qq.com%%2Flogin.html&h=1&ptredirect=0&ptlang=2052&from_ui=1&dumy=&fp=loginerroralert&action=4-7-90728&mibao_css=&t=1&g=1' \
			% (self.qq, p, verifycode)
		
		d = urlopen(url)
		s = d.split(",")
		print s[4].decode("utf-8")
		jar.save( "cookie-%s"% self.qq, ignore_discard=True, ignore_expires=True )
		
def test():
	qq = QQ()
	s = r"ptui_checkVC('0','!U1E','\x00\x00\x00\x00\x00\x31\x2c\x66')"
	code = eval(s)
	qq.genp(code)
	print qq.genp(code)
	print "0BDDA7F5B5A9864367513062F9E3EA48"

def main():
	qq = QQ( "3222630", "3281044p")
	qq.login()
	qq = QQ( "8389527", "3281044p")
	qq.login()
	qq = QQ( "18020500", "3281044p")
	qq.login()
	qq = QQ( "4115213", "18020500")
	qq.login()

if __name__ == "__main__":
	main()

