require("netext")
fun = package.loadlib("netext.so","l_version")

function var_dump(data, max_level, prefix)
 if type(prefix) ~= "string" then
  prefix = ""
 end
 if type(data) ~= "table" then
  print(prefix .. tostring(data))
 else
  print(data)
  if max_level ~= 0 then
   local prefix_next = prefix .. "    "
   print(prefix .. "{")
   for k,v in pairs(data) do
    io.stdout:write(prefix_next .. k .. " = ")
    if type(v) ~= "table" or (type(max_level) == "number" and max_level <= 1) then
     print(v)
    else
     if max_level == nil then
      var_dump(v, nil, prefix_next)
     else
      var_dump(v, max_level - 1, prefix_next)
     end
    end
   end
   print(prefix .. "}")
  end
 end
end

function vd(data, max_level)
 var_dump(data, max_level or 5)
end

print( netext.version() )

vd( netext.test( 1 ) )
print()
vd( netext.test( true ) )
print()
vd( netext.test( {} ) )
print()


local aa = {"a","b",10, { "t", 2 } , true }
vd( netext.test( aa ) )
print()
print()
print("--------")

function foo( a, b, c )
	print( a, b, c)
	vd ( call_table )
	c=c+1
	netext.callLater( 100, foo, a, b, c )
end

sp = { 
connectionMade = function( self, sock )
	vd( server_socks )
	print( "connectionMade", self, sock, sock:getpeername( )  )
--	sock:close()
end,

dataReceived = function( self, sock, data )
	print( "dataRecieved", sock, data )
end,

connectionLost = function( self, sock )
	print ("connectionLost", sock )
end
}
netext.server( 8000, sp )
print ( "metatable" )
vd ( socket_meta )
print ( "servers", vd( server_socks) )
--netext.callLater( 2000, foo, "--",1, 4 )

print (5/2)

netext.run()

