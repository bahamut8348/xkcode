// adoview.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "ComViewer.h"
int _tmain(int argc, _TCHAR* argv[])
{
	::CoInitialize(NULL);
	adoview::CComViewer view(L"Adodb.RecordSet");
	view.GetInfo();
	return 0;
}

