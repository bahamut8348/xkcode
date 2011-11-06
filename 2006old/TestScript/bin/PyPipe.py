import win32event as _event, win32pipe as _pipe
import win32file as _file

class pypipe:
	
	hd = None
	def __init__(name):
		if name:
			self.hd = _pipe.Create