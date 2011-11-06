#pragma once 


class CMenuButton;
typedef CWindowImpl < CMenuButton, CWindow, CWinTraits < WS_CHILD | WS_VISIBLE | BS_OWNERDRAW> > CCustButton;


class CMenuButton : public CCustButton
{
public:
	DECLARE_WND_SUPERCLASS(NULL, _T("BUTTON"))

	BEGIN_MSG_MAP(CMenuButton)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	END_MSG_MAP()

	enum
	{
		l_leftPadding = 4,
		l_topPadding  = 3,
		l_width		= 15,
		l_height    = 15,
		
	};

	_dword m_MenuID;
	CMenu m_menu;


	void SpecailMenu(_dword nID)
	{
		m_MenuID = nID;
		m_menu.LoadMenu(m_MenuID);
		CMenuHandle hm = m_menu.GetSubMenu(0);
		return;
		for(int a=0;a<hm.GetMenuItemCount(); a++)
		{
			int nID = hm.GetMenuItemID(a);
			HBITMAP hBmp = AtlLoadBitmap(nID);
			if(hBmp != NULL)
			{
				CBitmap h;
				h.Attach(hBmp);
				h.SetBitmapDimension(16,16);
				hm.SetMenuItemBitmaps(nID, MF_BYCOMMAND, hBmp, NULL);
				h.Detach();
			}
		}
	}

	LRESULT OnLButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
	{
		//bHandled = FALSE;
		this->DefWindowProc(uMsg, wParam, lParam);
		CMenuHandle hm = m_menu.GetSubMenu(0);
		CRect rcWin;
		GetWindowRect(&rcWin);
		hm.TrackPopupMenu(0, rcWin.left, rcWin.bottom, GetParent());
		return 0;
	}

	LRESULT  OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
	{
		this->DefWindowProc(uMsg, wParam, lParam);
		//bHandled = FALSE;
		CDC dc = GetWindowDC();
		CRect rc;
		CFont font;
		dc.SetBkMode(TRANSPARENT);
		font.CreatePointFont(100,L"VisualUI", dc);
		HFONT OldFont = dc.SelectFont(font);
		GetClientRect(&rc);
		rc.left += l_leftPadding;
		rc.right = rc.left + l_width;
		rc.top += 2;
		rc.bottom = rc.top + l_height;
		dc.DrawText(L"",1, &rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		return 0;
	}
};