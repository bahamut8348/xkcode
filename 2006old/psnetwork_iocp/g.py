from wx import *
from PSNetwork import *

ID_ABOUT 	= 1
ID_EXIT 		= 2
ID_START 	= 3


g_man = None

class NetCallback:
	def __init__(self, box):
		self.ss = None
		self.box = box
		
	def OnCreateConnection(self, connid):
		print "OnCreateConnect", connid
		self.box.Append("OnCreateConnect %d" % connid)
		#print self.ss
		self.ss.SendPacket(connid, "*" * 5)
		#print "send lenght",e
		pass
	
	def OnDeleteConnection(self, connid):
		print "OnDeleteConnect", connid
		#self.box.Append("OnDeleteConnect %d" % connid)
		pass
		
	def OnReceivePacke(self, connid, buff):
		print "OnReceivePacke", connid, len(buff)
		#self.ss.SendPacket(connid, buff)
		#self.box.Append("OnReceivePacke %d, Len: %d" % (connid, len(buff)) )
		self.ss.CloseConnection(connid, False)
		#self.ss.JoinSession(  str2addr("tcp://127.0.0.1:7788") , 1 , 0 )
		pass
		
	def OnJoinFailed(self, connid):
		print "OnJoinFailed", connid
		pass


class MyFrame(Frame):
	def __init__(self, parent, ID, title):
		Frame.__init__(self, parent, ID, title, DefaultPosition, Size(200,150))		
		self.CreateStatusBar()
		self.SetStatusText('this is the statusbar')
		
		menu = Menu()
		menu.Append(ID_ABOUT, "&About", "More Info about this program")
		menu.Append(ID_START, "&Start", "Start test")
		menu.AppendSeparator()
		menu.Append(ID_EXIT, "E&xit", "Terminate the program")
		
		menuBar = MenuBar()
		menuBar.Append(menu, "&File")
		self.SetMenuBar(menuBar)
		
		self.box = ListBox(self, NewId(), DefaultPosition, (100,100), [], LB_SINGLE)


		EVT_MENU(self, ID_ABOUT, self.OnAbout)
		EVT_MENU(self, ID_START, self.OnStart)
		EVT_MENU(self, ID_EXIT,  self.TimeToQuit)

	def OnAbout(self, event):
		dlg = MessageDialog(self, "This sample program shows off\n"
			"frames, menus, statusbars, and this\n"
			"message dialog.",
			"About Me", OK | ICON_INFORMATION)
		dlg.ShowModal()
		dlg.Destroy()
		
	def OnStart(self, event):
		self.man = Manager()
		cb = NetCallback( self.box )
		
		self.net = self.man.CreateSession(2, 10, cb)
		cb.ss = self.net
		self.net.JoinSession( str2addr("tcp://127.0.0.1:7788"), 1,0 )
		print self.net, "joinSesson"


	def TimeToQuit(self, event):
		self.Close(True)




		
class MyApp(App):
	def OnInit(self):
		frame = MyFrame(None, -1, "hello from wxPython")
		frame.Center()
		frame.Show(True)
		self.SetTopWindow(frame)
		return True

app = MyApp(0)
app.MainLoop()

