// Page2.cpp : implementation file
//

#include "stdafx.h"
#include "TabCtrlTest.h"
#include "Page2.h"


// CPage2 dialog

IMPLEMENT_DYNAMIC(CPage2, CDialog)

CPage2::CPage2(CWnd* pParent /*=NULL*/)
	: CDialog(CPage2::IDD, pParent)
{

}

CPage2::~CPage2()
{
}

void CPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPage2, CDialog)
END_MESSAGE_MAP()


// CPage2 message handlers
