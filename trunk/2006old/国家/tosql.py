import codecs

import sql

db1 = u"192.168.1.13",u"psweb",u"psweb",u"webserver"
db1 = u"192.168.1.13",u"psplayer",u"psplayer",u"psplayer"

sql.conndb(*db1)

s = open("k.sql").read()
#sql.uexecsql(unicode(s))

F = codecs.open("h","r","utf_8")
buf= F.read()
F.close()

import re

ma = re.findall(u'<option value="([A-Z]{2})"> ([^<]+)</option>', buf)

i = 1
F = codecs.open("h2","w","utf_8")
#F.write("")
for e in ma:
	s = u'm_Locations.Insert( LocationNode( LOCATION_%s, L"%s") );\n' % (e[0],e[1].strip())
	
	#F.write(s)
	name = e[1].replace("'","''")
	s = u"insert into locations (id,name,shortname) values(%d, N'%s', N'%s')" % (i, name.strip(),e[0])
	ret = sql.uexecsql(s)
	if not ret:
		print `s`
	i+=1
	
	
F.close()