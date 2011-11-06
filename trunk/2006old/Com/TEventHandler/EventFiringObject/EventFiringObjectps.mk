
EventFiringObjectps.dll: dlldata.obj EventFiringObject_p.obj EventFiringObject_i.obj
	link /dll /out:EventFiringObjectps.dll /def:EventFiringObjectps.def /entry:DllMain dlldata.obj EventFiringObject_p.obj EventFiringObject_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del EventFiringObjectps.dll
	@del EventFiringObjectps.lib
	@del EventFiringObjectps.exp
	@del dlldata.obj
	@del EventFiringObject_p.obj
	@del EventFiringObject_i.obj
