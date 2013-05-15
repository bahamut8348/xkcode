
local server
_connect( "localhost:8007", function( net)
	print("!! net", net)
	_callout(net, function( self, name, args, data)
		self:send(string.from32b(#data))
		self:send(data)
	end)
	server = net
	local s = ''
	for i=1, 63 do
		s = s .. '9'
	end
--	server.Call({"C"})
		i= s
		local d  = _encode(i)
		
		print(i)
		net:send(d)
		net:send("^^^^")
	
end, function( net, timeout, notconn, err )
	print("!! err:", err)
end, 10)
