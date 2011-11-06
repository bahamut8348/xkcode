
import Queue, time, threading
import sys, traceback

import pythoncom


class ThreadPool:
	
	_bStop = 0
	_q = Queue.Queue(300000)
	_timer_q = Queue.Queue(300000)
	_threads = []
	_thread_number = 2
	_lock = threading.Lock()
	_tasklist = []
	_taskdonecb = lambda x : 1
	_runingtask = Queue.Queue(300000)

	def count(self):
		return self._runingtask.qsize()

	def workthread( self, name ):
		#print name,"thread start"
		pythoncom.CoInitialize()

		t = None
		while 1:
			if self._bStop: 
				#print name,"thread stop"
				return
			try:
				t = self._timer_q.get_nowait()
			except:
				try:
					t = self._q.get_nowait()
				except:
					time.sleep(1)
					continue
			self._runingtask.put(1)
			try:
				t()
				
			except:
				print '-'*60
				print '-'*60
				print "Exception in user code:"
				traceback.print_exc(file=sys.stdout)
				print '-'*60
			self.ontaskdone( t )
			self._runingtask.get()
	
	def ontaskdone( self, t ):
			self._taskdonecb( t )
	
	def timerthread( self ):
		while 1:
			if self._bStop:
				#print name,"thread stop"
				return
			self._lock.acquire()
			for e in self._tasklist:
				clock = e[0]
				fun = e[1]
				if time.time() > clock:
					self._tasklist.remove(e)
					self._timer_q.put(fun)
			self._lock.release()
			time.sleep(0.5)
	def setcallback( self, cb ):
		self._taskdonecb = cb
	
	def __init__(self, n = 10):
		self._thread_number = n
		threading.Thread( target = self.timerthread ).start()
		for  e in range( self._thread_number ):
			T = threading.Thread( target = self.workthread, args=("[%d]" % e,) )
			self._threads.append(T)
			T.start()
	
	def stop(self):
		self._bStop = 1
		pass
	
	def addtask(self, task, clock = 0 ):
		if clock == 0:
			self._q.put(task)
		else:
			self.addtimertask( task, time.time()+clock)
	
	def addtimertask(self, task, clock):
		self._lock.acquire()
		self._tasklist.append( (clock, task) )
		self._lock.release()


if __name__ == "__main__":
	
	def f():
		print 1
	def k():
		for e in range(100):
			print 2
			time.sleep(0.3)
	tp = ThreadPool()
	tp.addtask(k)
	tp.addtask(f,2)
	tp.addtask(f, 10)
	tp.addtask(f,5)

