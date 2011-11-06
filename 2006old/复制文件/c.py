import os, threading
import shutil  as sh
from Queue import Queue

count = 0
allcount = 0
maxthread =10
exclude=[".exe", ".dll", ".lib",".ilk", ".pdb", ".log"]


tasks= Queue(40960)
bStop = False

def dowork():
	global tasks, bStop
	while 1:
		try:
			task = tasks.get(0,2)
		except:
			return
			if bStop:
				
				break
			else:
				continue
		tasktype, src, dst = task
		if tasktype == 1:
			if doreplace(src,dst):
				print "Replace", dst
				sh.copyfile(src, dst)
		elif tasktype == 2:
			cpdir(src, dst)
	print "thread exit"



workthread = None

threads = []

def startthread():
	global workthread, threads
	for e in range(maxthread):
		workthread = threading.Thread(target=dowork)
		workthread.start()
		threads.append(workthread)



def doreplace(src, dst):
	if not os.path.exists(dst): return True
	ext = os.path.splitext(dst)
	if ext in exclude: return False
	src_mt = os.stat(src)[-2]
	dst_mt = os.stat(src)[-2]
	return src_mt > dst_mt
	

def handle(src, dst):
	global allcount, count
	allcount+=1
	print allcount,"\r",
	#print "put"
	tasks.put((1,src,dst))
	#print "put ok"
	#sh.copyfile(src, dst)
	count += 1

def cpdir(src, dst):
	dirs = os.listdir(src)
	for e in dirs:
		nsrc = src+ "\\" + e
		ndst = dst+ "\\" + e
		if os.path.isdir(nsrc):
			if not os.path.exists(ndst): os.makedirs(ndst)
			tasks.put((2,nsrc,ndst))
		elif os.path.isfile(nsrc):
			handle(nsrc, ndst)


if __name__ == "__main__":
	import time
	
	start = time.time()
	
	#cpdir(r"\\192.168.1.75\New Work\Warrior Epic\Output\we", r"E:\Work\0429\Warrior Epic\Output\we")
	cpdir(r"\\lechuck\Product\Warrior Epic\Build Resources",  r"E:\Work\0429\Warrior Epic\Output\we")
	startthread()
	bStop = True
	print "wait %d thread  finish" % len(threads)
	
	for e in threads:
		e.join()
		print e,"join"
	end = time.time()
	print "all:", allcount
	print "update:", count
	print "use:", end-start ,"second"

	os.system("pause")
