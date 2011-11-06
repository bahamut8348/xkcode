#! coding: utf8

import urllib2
import xml.etree.ElementTree as tree
import StringIO



F = urllib2.urlopen("http://tech.163.com/special/00091JPQ/techimportant.xml")


ele = tree.parse("t.xml").findall("//item")
print dir(ele[0])
for e in ele:
	print e.attrib['id']
	print e.find("title").text




#print ele

print 