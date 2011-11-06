from sys import *
from os.path import *
from sha import *
from BitTorrent.bencode import *

class BT:
	
	def __init__(self, fn ):
		F = open(fn,"rb")
		self.info = bdecode( F.read( ) )
		F.close()
		announce =  self.info['announce']
		print announce


if __name__ == "__main__":
	
	c = BT("m")


