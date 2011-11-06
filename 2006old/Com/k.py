
class PythonUtilities:
	_public_methods_ = [ 'SplitString','GetDict', 'Makeerrmsg' ]
	_reg_progid_ = "PythonDemos.Utilities"
	# NEVER copy the following ID 
	# Use "print pythoncom.CreateGuid()" to make a new one.
	_reg_clsid_ = "{41E24E95-D45A-11D2-852C-204C4F4F5020}"

	def SplitString(self, val, item=None):
		import string
		if item != None: item = str(item)
		return string.split(str(val), item)

	def  GetDict(self):
		return "ll"


	def Makeerrmsg(self,s):
		ls = s.split("\n")
		newls = filter(lambda x: x.find("=")>0, ls)

		newls = map(lambda x: x.split("="), newls)
		ret = map(lambda x: '%s , "%s",' % (x[1].strip().strip(","), x[0].strip()), newls)
		all=""
		for e in ret:
			all+=e+"\r\n"
		return all


if __name__ == "__main__":
	
	print "Registering COM server..."
	import win32com.server.register
	
	win32com.server.register.UseCommandLine(PythonUtilities)
"""
	import sys
	fn = sys.argv[1]
	r = makeerrmsg(open(fn).read())
	for e in r: print e
"""