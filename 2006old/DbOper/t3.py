#select from table2 where not in table1

import sql
from sql import geterror


db1 = u"192.168.1.13",u"psweb",u"psweb",u"webserver"

db2 = u"192.168.1.13",u"psweb",u"psweb",u"webserver"

# from db1 to db2


def check(ret,desp):
	if ret:
		print "[+]", desp, "OK"
		return True
	else:
		print "[-]", desp, "FAILED:", sql.geterror()
		return False

check(sql.conndb(*db1), "Conn db1")



