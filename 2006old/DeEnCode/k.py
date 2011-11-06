
ss = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
for e in range(128):
	char = chr(e)
	if ss.find(char) ==-1:
		print 0,",",
	else:
		print ss.find(char),",",
