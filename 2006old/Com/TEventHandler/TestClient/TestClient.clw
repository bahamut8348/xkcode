; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTestClientApp
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "TestClient.h"

ClassCount=3
Class1=CTestClientApp
Class2=CTestClientDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_TESTCLIENT_DIALOG

[CLS:CTestClientApp]
Type=0
HeaderFile=TestClient.h
ImplementationFile=TestClient.cpp
Filter=N
LastObject=CTestClientApp
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CTestClientDlg]
Type=0
HeaderFile=TestClientDlg.h
ImplementationFile=TestClientDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CTestClientDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=TestClientDlg.h
ImplementationFile=TestClientDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_TESTCLIENT_DIALOG]
Type=1
Class=CTestClientDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_BUTTON_CALL_TEST_FUNCTION,button,1342242816

