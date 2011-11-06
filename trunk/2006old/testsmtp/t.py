from ctypes import *
cbuf = create_string_buffer('\000' * 32)
qmx = CDLL("qmx")
u, d = "xk135@163.com".split("@")
print qmx.QueryMX( c_char_p (d), cbuf)
print cbuf.value