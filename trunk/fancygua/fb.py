import t3, fansy

class Move_OK( t3.MOVE ):
	pass

if __name__ == "__main__":
	fansy.callLater( 0, t3.work, t3.FB_OK )
#	fansy.callLater( 0, t3.work, t3.FB_LMS )
#	fansy.callLater( 0, t3.work, Move_OK )
#	fansy.callLater( 0, t3.work, t3.FB_FATHER )
	fansy.run()