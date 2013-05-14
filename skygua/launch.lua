
local server
_connect( "localhost:8007", function( net)
	print("!! net", net)
	_callout(net, function( self, name, args, data)
		self:send(string.from32b(#data))
		self:send(data)
	end)
	server = net
--	server.Call({"C"})
	for i=0,0xff do
		net:send(_encode(i))
		net:send("^^^^")
	end
end, function( net, timeout, notconn, err )
	print("!! err:", err)
end, 10)
