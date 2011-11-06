#! coding: utf8

import glob
import os, codecs, sys

exts = ['.h', '.cpp','.cxx', '.hpp']

def convert_file(fn):
	F = open(fn,"rb")
	buf = F.read()
	F.close()
	print ">", fn
	if buf[:3] != "\xef\xbb\xbf":
		F = open( fn, "wb")
		F.write( buf.decode("mbcs", 'ignore').encode("gb2312",'ignore') )
		F.close()

def convert_folder( folder ):
	folder = os.path.realpath(folder)
	if folder[-1]!="\\": folder+="\\"
	flist = glob.glob(folder + "*")
	for e in flist:
		fullname =  e
		if os.path.isdir(fullname):
			convert_folder (fullname)
		elif os.path.isfile(fullname):
			ext = os.path.splitext(fullname)[1].lower()
			if ext in exts:
				convert_file( fullname)
		else:
			print fullname, "Error"


if __name__ == "__main__":
	convert_folder(".")