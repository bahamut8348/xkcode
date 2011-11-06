

import urllib, re

def gettitle(num):
	F = urllib.urlopen("http://thns.tsinghua.edu.cn/thnsebooks/ebook"+str(num)+"/list.htm")
	buf = F.read()
	F.close()
	return re.findall('#990033">([^<]*)</font>', buf)


FF = open("l.txt","a")

for e in range(200,400):
	ret = gettitle(e)
	title = "".join(ret)
	all = "%d %s\r\n" %(e, title)
	FF.write(all)
	print all,