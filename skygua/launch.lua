
a = {}
c = {3.00000000001, ""}
local d = _encode(nil):tostr()

for i=1, #d do
	print(("\\x%02x"):format(d:byte(i)))
end

dump(_decode(d))