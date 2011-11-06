def getparams(all):
	r = {}
	for e in all.keys():
		if e.find("p_")==0:
			r[e[2:]] = all[e]
	
	return r

def login(**dic):
	return 1
	
def starttest(all):
	
	for e in all:
		pass
		#print e, all[e]
	print all["testcase"].__name__, all["expectResult"] == login(**getparams(all)), ",",all["desc"]