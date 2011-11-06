import codecs



F = codecs.open("h","r","utf_8")
buf= F.read()
F.close()

import re

ma = re.findall(u'<option value="([A-Z]{2})">(.*?)</option>', buf)

i = 1
F = codecs.open("h2","w","utf_8")
F.write("")
for e in ma:
	s = u'm_Locations.Insert( LocationNode( LOCATION_%s, L"%s") );\n' % (e[0],e[1].strip())
	F.write(s)
	print i
	i+=1
	
	
F.close()