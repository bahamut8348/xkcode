################################################################################
# [Mar..5.2007.15:36:11]                                                       #
#                                  OleDBTest                                   #
#                                                                              #
#                                               Thursday 17 May 2007, 12:53:21 #
################################################################################

##### GROUPS ###################################################################

FG00=\
	ado.obj\
	OleDBTest.obj\
	stdafx.obj

FG01=\
	kernel32.lib


##### BUILD ####################################################################

ALL: $(FG00)
	LINK /nologo /SUBSYSTEM:CONSOLE /MACHINE:X86 /DEBUG /OUT:OleDBTest.EXE $(FG00) $(FG01)

OleDBTest.EXE: $(FG00)
	LINK /nologo /SUBSYSTEM:CONSOLE /MACHINE:X86 /DEBUG /OUT:OleDBTest.EXE $(FG00) $(FG01)


##### COMMANDS #################################################################

run:  OleDBTest.EXE
	OleDBTest.EXE

debug:  OleDBTest.EXE
	VCExpress.exe OleDBTest.EXE

peview:  OleDBTest.EXE
	uetools -pe OleDBTest.EXE

makef: 
	uestudio makefile


##### COMPILE ##################################################################

INC001=J:\Program\OLEDBT~1\stdafx.h\
	D:\PROGRA~1\MICROS~2\VC\include\stdio.h\
	D:\PROGRA~1\MICROS~2\VC\include\crtdefs.h\
	D:\PROGRA~1\MICROS~2\VC\include\sal.h\
	D:\PROGRA~1\MICROS~2\VC\include\crtassem.h\
	D:\PROGRA~1\MICROS~2\VC\include\vadefs.h\
	D:\PROGRA~1\MICROS~2\VC\include\swprintf.inl\
	D:\PROGRA~1\MICROS~2\VC\include\tchar.h\
	D:\PROGRA~1\MICROS~2\VC\include\wchar.h\
	D:\PROGRA~1\MICROS~2\VC\include\wtime.inl\
	D:\PROGRA~1\MICROS~2\VC\include\string.h\
	D:\PROGRA~1\MICROS~2\VC\include\mbstring.h\
	J:\Program\OLEDBT~1\Ado.h

ado.obj: ..\ado.cpp $(INC001)
	CL /FC /nologo /W3 /MD /fp:precise /GR- /Gd /TP /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /ZI /c ..\ado.cpp


INC002=J:\Program\OLEDBT~1\stdafx.h\
	D:\PROGRA~1\MICROS~2\VC\include\stdio.h\
	D:\PROGRA~1\MICROS~2\VC\include\crtdefs.h\
	D:\PROGRA~1\MICROS~2\VC\include\sal.h\
	D:\PROGRA~1\MICROS~2\VC\include\crtassem.h\
	D:\PROGRA~1\MICROS~2\VC\include\vadefs.h\
	D:\PROGRA~1\MICROS~2\VC\include\swprintf.inl\
	D:\PROGRA~1\MICROS~2\VC\include\tchar.h\
	D:\PROGRA~1\MICROS~2\VC\include\wchar.h\
	D:\PROGRA~1\MICROS~2\VC\include\wtime.inl\
	D:\PROGRA~1\MICROS~2\VC\include\string.h\
	D:\PROGRA~1\MICROS~2\VC\include\mbstring.h\
	J:\Program\OLEDBT~1\Ado.h

OleDBTest.obj: ..\OleDBTest.cpp $(INC002)
	CL /FC /nologo /W3 /MD /fp:precise /GR- /Gd /TP /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /ZI /c ..\OleDBTest.cpp


INC003=J:\Program\OLEDBT~1\stdafx.h\
	D:\PROGRA~1\MICROS~2\VC\include\stdio.h\
	D:\PROGRA~1\MICROS~2\VC\include\crtdefs.h\
	D:\PROGRA~1\MICROS~2\VC\include\sal.h\
	D:\PROGRA~1\MICROS~2\VC\include\crtassem.h\
	D:\PROGRA~1\MICROS~2\VC\include\vadefs.h\
	D:\PROGRA~1\MICROS~2\VC\include\swprintf.inl\
	D:\PROGRA~1\MICROS~2\VC\include\tchar.h\
	D:\PROGRA~1\MICROS~2\VC\include\wchar.h\
	D:\PROGRA~1\MICROS~2\VC\include\wtime.inl\
	D:\PROGRA~1\MICROS~2\VC\include\string.h\
	D:\PROGRA~1\MICROS~2\VC\include\mbstring.h\
	J:\Program\OLEDBT~1\Ado.h

stdafx.obj: ..\stdafx.cpp $(INC003)
	CL /FC /nologo /W3 /MD /fp:precise /GR- /Gd /TP /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /ZI /c ..\stdafx.cpp



##### CLEAN ####################################################################

CLEAN:
	-@erase "ado.obj"
	-@erase "OleDBTest.obj"
	-@erase "stdafx.obj"
	-@erase "OleDBTest.EXE"
	-@erase "*.pdb"
	-@erase "*.idb"
	-@erase "*.ilk"

