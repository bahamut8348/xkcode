import PyV8 as V8
import logging

class Debugger(V8.JSDebug):
	logger = logging.getLogger("dbg")

	def __init__(self, ctxt):
		
		V8.JSDebug.__init__(self)
		self.logger.addHandler( logging.StreamHandler() )
		self.logger.setLevel( logging.INFO )
		self.logger.info("sdf")
		self.ctxt = ctxt

		

	def __enter__(self):
		self.enabled = True
		print "enter"
		return self

	def __exit__(self, exc_type, exc_value, traceback):
		V8.debugger.enabled = False

	def onMessage(self, msg):
		self.logger.info("Debug message: %s", msg)

		try:
			if msg['type'] == 'event' and msg['event'] == 'break':
				print self.stepNext()
		except:
			import traceback

			traceback.print_exc()

	def onBreak(self, evt):
		self.logger.info("Break event: %s", evt)

	def onException(self, evt):
		self.logger.info("Exception event: %s", evt)

	def onNewFunction(self, evt):
		self.logger.info("New function event: %s", evt)

	def onBeforeCompile(self, evt):
		self.logger.info("Before compile event: %s", evt)

	def onAfterCompile(self, evt):
		self.logger.info("After compile event: %s", evt)

ctxt = V8.JSContext()
dbg = Debugger( ctxt )
dbg.setEnabled( True )
ctxt.enter()


ctxt.locals.__filename = "k.js"
ctxt.eval( "dsfd" )
