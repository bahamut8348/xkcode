// Page1.cpp : implementation file
//

#include "stdafx.h"
#include "TabCtrlTest.h"
#include "Page1.h"


// CPage1 dialog

IMPLEMENT_DYNAMIC(CPage1, CDialog)

CPage1::CPage1(CWnd* pParent /*=NULL*/)
	: CDialog(CPage1::IDD, pParent)
{

}

CPage1::~CPage1()
{
}

void CPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPage1, CDialog)
END_MESSAGE_MAP()


// CPage1 message handlers
