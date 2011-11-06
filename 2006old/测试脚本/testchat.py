
from common.common import *



p_username = "PSpace_" + day() + hour() + minute() + second() + "_" + random()
p_password = "123456"

Register(globals())

Login(globals())

areas = GetArea()

p_roomname = now() + random()

result = CreateRoom(globals())
if result:
	
	p_chattouser = ""
	p_chatmsg = "test"
	
	ExpectResult = 0
	
	for e in range(10):
		test( Chat, globals(), ExpectResult,  "Test Chat To " + p_chattouser + str(e) )
		
		
		
def OnRecvUser(roomname, users):
	for user in users:
		p_chattouser = user
		p_chattype = public
		test( Chat, globals(), ExpectResult,  "Test Chat To " + p_chattouser + str(e) )
	

def OnRecvChatMsg(roomname, chatfromuser, chatmsg):
	p_chattouser = ""
	p_chatmsg = chatmsg
	ExpectResult = 1
	test( Chat, globals(), ExpectResult,  "Test Recv Chat To " + p_chattouser )




