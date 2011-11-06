require( "netext" )

local proto = { 
	connectionMade = function( self, sock )
		print(" connectMade:", sock )
	end
}
print ( netext.connect( "1.1.1.10", 999, proto ) )

netext.run()

