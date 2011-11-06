import codecs

F = codecs.open("h","r","utf_8")
buf= F.read()
F.close()

import re

ma = re.findall(u'"([A-Z]{2})"', buf)

i = 1
for e in ma:
	
	print "LOCATION_%s = %d," % (e,i)
	i+=1