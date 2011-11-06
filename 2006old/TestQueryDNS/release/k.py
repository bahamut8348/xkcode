from ctypes import *

qmx = CDLL("qMX.dll")
buf = create_string_buffer('\000' * 32)
n = "163.com"
print qmx.QueryMX( n, buf)
print buf.value

