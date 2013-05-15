
local server
_connect( "localhost:8007", function( net)
	print("!! net", net)
	_callout(net, function( self, name, args, data)
		self:send(string.from32b(#data))
		self:send(data)
	end)
	server = net
	local s = {}
	local p = {}
	
	for i=1, 2 do
--		s[i] = s
	end
	local c = 1000000
	s[1] = c
	s[2] = c
	s[3] = p
	s[4] = c
	s[5] = c
	s[6] = p
--	server.Call({"C"})
		i= s
		local d  = _encode(i)
		
		print(i)
		net:send(d)
		net:send("^^^^")
	
end, function( net, timeout, notconn, err )
	print("!! err:", err)
end, 10)
