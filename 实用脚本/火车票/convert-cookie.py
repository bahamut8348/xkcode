
import sys
f = sys.argv[1]
domain = sys.argv[2]
data = open( f ).read()
cs = data.split(";")
print "#LWP-Cookies-2.0"
for cookie in cs:
	print 'Set-Cookie3: %s="%s"; path="/"; domain="%s"; path_spec; discard; version=0' % ( cookie.strip().split("=")[0], "=".join(cookie.strip().split("=")[1:]), domain )
