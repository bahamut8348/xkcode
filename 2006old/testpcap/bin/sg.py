import winpcap as wincap
import stackless as SL
cap = wincap.open()
cap.setfilter('port 80 and dst 60.12.226.253')


def cap_thread():
	for t, data in cap:
		d = data[54:]
		pos = d.find("\n")
		if pos > 0:
			print d[:pos]
		SL.schedule( )

SL.tasklet( cap_thread )( )
SL.run(10000)
