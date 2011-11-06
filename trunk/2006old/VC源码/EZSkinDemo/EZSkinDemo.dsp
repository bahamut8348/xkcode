# Microsoft Developer Studio Project File - Name="EZSkinDemo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=EZSkinDemo - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EZSkinDemo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EZSkinDemo.mak" CFG="EZSkinDemo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EZSkinDemo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "EZSkinDemo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/EZSkinDemo", UOAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EZSkinDemo - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "EZSkinDemo - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "EZSkinDemo - Win32 Release"
# Name "EZSkinDemo - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\EZBorder.cpp
# End Source File
# Begin Source File

SOURCE=.\EZCaption.cpp
# End Source File
# Begin Source File

SOURCE=.\EZCaptionSkin.cpp
# End Source File
# Begin Source File

SOURCE=.\EZDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\EZDialogSkin.cpp
# End Source File
# Begin Source File

SOURCE=.\EZDib.cpp
# End Source File
# Begin Source File

SOURCE=.\EZGenericSkin.cpp
# End Source File
# Begin Source File

SOURCE=.\EZRects.cpp
# End Source File
# Begin Source File

SOURCE=.\EZRuntimeClass.cpp
# End Source File
# Begin Source File

SOURCE=.\EZSkin.cpp
# End Source File
# Begin Source File

SOURCE=.\EZSkin_I.cpp
# End Source File
# Begin Source File

SOURCE=.\EZSkinDemo.cpp
# End Source File
# Begin Source File

SOURCE=.\EZSkinDemo.rc
# End Source File
# Begin Source File

SOURCE=.\EZSkinDemoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EZSkinIni.cpp
# End Source File
# Begin Source File

SOURCE=.\EZSkinManager.cpp
# End Source File
# Begin Source File

SOURCE=.\EZWindowNC.cpp
# End Source File
# Begin Source File

SOURCE=.\EZWndStyle.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Subclass.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\EZBorder.h
# End Source File
# Begin Source File

SOURCE=.\EZCaption.h
# End Source File
# Begin Source File

SOURCE=.\EZCaptionSkin.h
# End Source File
# Begin Source File

SOURCE=.\EZDC.h
# End Source File
# Begin Source File

SOURCE=.\EZDialog.h
# End Source File
# Begin Source File

SOURCE=.\EZDialogSkin.h
# End Source File
# Begin Source File

SOURCE=.\EZDib.h
# End Source File
# Begin Source File

SOURCE=.\EZGenericSkin.h
# End Source File
# Begin Source File

SOURCE=.\EZRects.h
# End Source File
# Begin Source File

SOURCE=.\EZRuntimeClass.h
# End Source File
# Begin Source File

SOURCE=.\EZSelector.h
# End Source File
# Begin Source File

SOURCE=.\EZSkin.h
# End Source File
# Begin Source File

SOURCE=.\EZSkin_I.h
# End Source File
# Begin Source File

SOURCE=.\EZSkinDemo.h
# End Source File
# Begin Source File

SOURCE=.\EZSkinDemoDlg.h
# End Source File
# Begin Source File

SOURCE=.\EZSkinIni.h
# End Source File
# Begin Source File

SOURCE=.\EZSkinManager.h
# End Source File
# Begin Source File

SOURCE=.\EZWindowNC.h
# End Source File
# Begin Source File

SOURCE=.\EZWndStyle.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Subclass.h
# End Source File
# Begin Source File

SOURCE=.\SuppressStyle.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\back.bmp
# End Source File
# Begin Source File

SOURCE=.\res\btns256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\btnsh256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\caption.bmp
# End Source File
# Begin Source File

SOURCE=.\res\EZSkinDemo.ico
# End Source File
# Begin Source File

SOURCE=.\res\EZSkinDemo.rc2
# End Source File
# Begin Source File

SOURCE=.\res\folder.ico
# End Source File
# Begin Source File

SOURCE=.\res\folderop.ico
# End Source File
# Begin Source File

SOURCE=.\res\label.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tree.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
