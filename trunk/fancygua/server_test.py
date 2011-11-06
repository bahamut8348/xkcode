import jot, fansy


if __name__ == "__main__":
	
	F = open("user.txt")
	for line in F.readlines():
		line = line[line.find("{"):].strip()
		print line
		line = line.replace("|","\x10")
		jot.xdec( line )
	F.close()

