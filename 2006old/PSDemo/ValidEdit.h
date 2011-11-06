#pragma once


typedef DWORD (*CHECKFUN)(const TCHAR* str);


template <class T, COLORREF t_crBrushColor>
class CEditValid : public CMessageMap
{
public:
	CEditValid() { 
		m_brush = ::CreateSolidBrush(t_crBrushColor); 
		m_err[IDC_USERNAME] = PSFC::InfoCheck::CheckUserName;
		m_err[IDC_PASSWORD] = PSFC::InfoCheck::CheckPassword;
		m_err[IDC_EMAIL]	= PSFC::InfoCheck::CheckEmail;
		m_err[IDC_CITY]		= PSFC::InfoCheck::CheckCity;
		m_err[IDC_YEAR]		= PSFC::InfoCheck::CheckYear;
		m_err[IDC_MONTH]	= PSFC::InfoCheck::CheckMonth;
		m_err[IDC_NICKNAME] = PSFC::InfoCheck::CheckCity;
		m_err[IDC_PASSWORD1]= PSFC::InfoCheck::CheckPassword;
		m_err[IDC_OLDPASSWORD]= PSFC::InfoCheck::CheckPassword;

	}
	~CEditValid() { ::DeleteObject ( m_brush ); }

	BEGIN_MSG_MAP(CEditValid)
		MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnEditColor)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnEditColor)
		MESSAGE_HANDLER(WM_CTLCOLORSCROLLBAR, OnEditColor)
		MESSAGE_HANDLER(WM_CTLCOLORBTN, OnEditColor)
		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnEditColor)
		MESSAGE_HANDLER(WM_CTLCOLORLISTBOX, OnEditColor)
	END_MSG_MAP()

	LRESULT OnEditColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		T*   pT = static_cast<T*>(this);
		LRESULT ret = ::DefWindowProc(pT->m_hWnd, uMsg, wParam, lParam);
		CWindow win = (HWND)lParam;
		CDCHandle dc = (HDC)wParam;
		int nID = win.GetDlgCtrlID();
		CAtlMap<int,CHECKFUN>::CPair *pair = m_err.Lookup(nID);
		if(pair != NULL && pT->GetDlgItem(nID) == win)
		{
			BSTR text = 0;
			pT->GetDlgItemText(nID, text);
			if (text==0) text = L"";
			if( pair->m_value(text)!=AP_E_Info_Succ )
			{
				dc.SetTextColor(t_crBrushColor); 
			}
		}
		return (LRESULT)ret;
		
	}

protected:
	HBRUSH m_brush;
	CAtlMap<int,CHECKFUN> m_err;
};



