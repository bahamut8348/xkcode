; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDlg4
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "dhtml2.h"

ClassCount=5
Class1=CDhtml2App
Class2=CDhtml2Dlg

ResourceCount=4
Resource2=IDD_DHTML2_DIALOG (English (U.S.))
Resource3=IDD_DIALOG1
Resource1=IDR_MAINFRAME
Class3=CDlg2
Class4=CDlg3
Class5=CDlg4
Resource4=IDD_DIALOG2

[CLS:CDhtml2App]
Type=0
HeaderFile=dhtml2.h
ImplementationFile=dhtml2.cpp
Filter=N

[CLS:CDhtml2Dlg]
Type=0
HeaderFile=dhtml2Dlg.h
ImplementationFile=dhtml2Dlg.cpp
Filter=D
LastObject=CDhtml2Dlg



[DLG:IDD_DHTML2_DIALOG (English (U.S.))]
Type=1
Class=CDhtml2Dlg
ControlCount=1
Control1=IDC_STATIC1,static,1342308352

[DLG:IDD_DIALOG1]
Type=1
Class=CDlg2
ControlCount=1
Control1=IDC_STATIC1,static,1342308352

[CLS:CDlg2]
Type=0
HeaderFile=Dlg2.h
ImplementationFile=Dlg2.cpp
BaseClass=CDialog
Filter=D

[DLG:IDD_DIALOG2]
Type=1
Class=CDlg3
ControlCount=5
Control1=IDCANCEL,button,1342242816
Control2=IDC_BUTTON2,button,1342242816
Control3=IDC_BUTTON1,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342177296

[CLS:CDlg3]
Type=0
HeaderFile=Dlg3.h
ImplementationFile=Dlg3.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

[CLS:CDlg4]
Type=0
HeaderFile=Dlg4.h
ImplementationFile=Dlg4.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

