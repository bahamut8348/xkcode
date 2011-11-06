import socket
import struct, time
def send(s):
	
	b = "{Walks[611000021602200{Ch:1id100002nametest1flv2sex0head0itemEs{ItemEss[17{ItemEid0idx31903}{ItemEid1idx31903},,{ItemEid4idx31001}{ItemEid5idx31003}{ItemEid6idx31006}{ItemEid7idx31101},,,,,,,,,]}angle0}]}"
	b = b.replace("","\020")
	l = len(b)

	buf_l = struct.pack(">L", l )
	return s.send( buf_l + b )

if __name__ == "__main__":
	s = socket.create_connection( ("1.1.1.11",4000) )
	while 1:
		if send(s) < 1:
			break
		
		time.sleep(0.01)