// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMainFrame : public CFrameWindowImpl<CMainFrame>, public CUpdateUI<CMainFrame>,
		public CMessageFilter, public CIdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)


	


	CWtl3View m_view;

	CXKCommandBarCtrl m_CmdBar;

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
			return TRUE;

		return m_view.PreTranslateMessage(pMsg);
	}

	enum
	{
		l_leftpadding = 0,
		l_toppadding = 0,
		l_leftframewidth = 24,
		l_menuheight = 23,
		l_menuheight2 = 5,
		l_textpadding = 32,
		l_checkwidth = 19,
		l_exlength = 70,
		c_leftframeleft = 0x00F7F8F9,
		c_leftframeright = 0x00CCD4D7,
	};

	virtual BOOL OnIdle()
	{
		UIUpdateToolBar();
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_PAINT,  OnPaint)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
		MESSAGE_HANDLER(WM_MEASUREITEM, OnMeasureItem)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	
	void SetOwnerDraw(CMenuHandle  h)
	{
		for(int a =0; a<h.GetMenuItemCount(); a++)
		{
			CCommandBarCtrl::_MenuItemData  *pData = new CCommandBarCtrl::_MenuItemData;
			pData->lpstrText = new TCHAR[128];
			
			MENUITEMINFO Info;
			ZeroMemory(&Info, sizeof(Info));
			Info.cbSize = sizeof(Info);
			Info.fMask = MIIM_FTYPE | MIIM_DATA | MIIM_FTYPE | MIIM_STRING | MIIM_SUBMENU | MIIM_STATE;
			Info.cch = 128;
			Info.dwTypeData = pData->lpstrText;
			h.GetMenuItemInfo(a, TRUE, &Info);
			
			pData->fState = Info.fState;
			pData->fType = Info.fType;
			pData->iButton = -1;
			
			
			pData->dwMagic = 1;

			Info.fType |= MFT_OWNERDRAW;
			
			
			Info.dwItemData = (DWORD)pData;
			
			h.SetMenuItemInfo(a, TRUE,&Info);
			if(Info.hSubMenu != NULL)
			{
				SetOwnerDraw(h.GetSubMenu(a));
			}
		}
	}

	LRESULT OnPaint (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
	{
		return DefWindowProc(uMsg, wParam, lParam);
		HDC hdc = ::GetDC(m_hWndClient);
		Gdiplus::Graphics graphics(hdc);
		//  刷子
		Gdiplus::LinearGradientBrush linGrBrush(Gdiplus::Point(0, 0), Gdiplus::Point(800 , 800),  //  绘制区域
			Gdiplus::Color::Blue,  //  第一种颜色
			Gdiplus::Color::Green//,  //  第二种颜色 
			);  //  渐变色的角度

		graphics.FillRectangle(&linGrBrush, Gdiplus::Rect(0, 0, 800, 800));
		::DeleteDC(hdc);
		return 0;
	}
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		
		GdiplusStartupInput gdiplusStartupInput; 
		ULONG_PTR gdiplusToken; 
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL); 
		
		// create command bar window
		HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
		// attach menu
		m_CmdBar.AttachMenu(GetMenu());
		// load command bar images
		m_CmdBar.LoadImages(IDR_MAINFRAME);
		// remove old menu
		SetMenu(NULL);

		m_CmdBar.m_bFlatMenus = 0;

		//m_CmdBar.m_bImagesVisible = FALSE;
		
		//SetOwnerDraw(m_CmdBar.GetMenu());
		

		HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

		CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
		AddSimpleReBarBand(hWndCmdBar);
		AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

		CreateSimpleStatusBar();

		m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL | ES_SAVESEL, WS_EX_CLIENTEDGE);
		m_view.SetFont(AtlGetDefaultGuiFont());

		UIAddToolBar(hWndToolBar);
		UISetCheck(ID_VIEW_TOOLBAR, 1);
		UISetCheck(ID_VIEW_STATUS_BAR, 1);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);


		return 0;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

		bHandled = FALSE;

		ULONG_PTR gdiplusToken = NULL; 
		GdiplusShutdown(gdiplusToken);

		return 1;
	}

	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		PostMessage(WM_CLOSE);
		return 0;
	}

	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: add code to initialize document

		return 0;
	}

	LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		static BOOL bVisible = TRUE;	// initially visible
		bVisible = !bVisible;
		CReBarCtrl rebar = m_hWndToolBar;
		int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
		rebar.ShowBand(nBandIndex, bVisible);
		UISetCheck(ID_VIEW_TOOLBAR, bVisible);
		UpdateLayout();
		return 0;
	}

	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
		::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
		UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
		UpdateLayout();
		return 0;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}


	void DrawMenuText(CDC & dc, RECT& rc, LPCTSTR lpstrText, COLORREF color)
	{
		bool m_bShowKeyboardCues = 1;
		
		int nTab = -1;
		for(int i = 0; i < lstrlen(lpstrText); i++)
		{
			if(lpstrText[i] == _T('\t'))
			{
				nTab = i;
				break;
			}
		}
		dc.SetTextColor(color);
		dc.SetBkMode(TRANSPARENT);
		dc.DrawText(lpstrText, nTab, &rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER | (m_bShowKeyboardCues ? 0 : DT_HIDEPREFIX));
		if(nTab != -1)
			dc.DrawText(&lpstrText[nTab + 1], -1, &rc, DT_SINGLELINE | DT_RIGHT | DT_VCENTER | (m_bShowKeyboardCues ? 0 : DT_HIDEPREFIX));
	}

	LRESULT OnDrawItem( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled )
	{
		int nID = wParam;
		bHandled = FALSE;
		LPDRAWITEMSTRUCT	lpdi = (LPDRAWITEMSTRUCT) lParam;
		if(lpdi->CtlType == ODT_MENU)
		{
			
			CDCHandle dc = lpdi->hDC;
			CDC  memDC;
			memDC.CreateCompatibleDC(dc);
			memDC.SelectFont(AtlGetDefaultGuiFont());
			CBitmap bmp;
			bmp.CreateCompatibleBitmap(dc, 400,400);
			memDC.SelectBitmap(bmp);
			CRect rect;
			rect = lpdi->rcItem;

			CCommandBarCtrl::_MenuItemData *pData = (CCommandBarCtrl::_MenuItemData *)lpdi->itemData;
			
			// all white
			memDC.FillRect(&rect, RGB(255,255,255));

			Gdiplus::Font font(memDC);

			// draw left
			Gdiplus::Graphics graphics(memDC);
			Gdiplus::LinearGradientBrush linGrBrush(Gdiplus::Rect(0, 0, l_leftframewidth, rect.Height()),  //  绘制区域
				Gdiplus::Color(249,248,247),  //  第一种颜色
				Gdiplus::Color(215,212,204),  //  第二种颜色 
				0	);  //  渐变色的角度
			graphics.FillRectangle(&linGrBrush, Gdiplus::Rect(rect.left, rect.top, l_leftframewidth, rect.Height()));
			
			BOOL bEnable =!(lpdi->itemState & ODS_GRAYED);
			
			if( (lpdi->itemState & ODS_SELECTED ) && (bEnable))
			{
				CRect rcRect = rect;
				rcRect.InflateRect(-1,-1,-1,-1);
				Gdiplus::SolidBrush br(Gdiplus::Color(182,189,210));
				graphics.FillRectangle(&br, rcRect.left, rcRect.top, rcRect.Width(), rcRect.Height());

				Gdiplus::Pen pen(Gdiplus::Color(10,36,106));
				graphics.DrawRectangle(&pen, rcRect.left, rcRect.top, rcRect.Width(), rcRect.Height());
			}
			
			// draw text
			{
				COLORREF color = 0;
				if(bEnable)
				{
					color = RGB(0,0,0);
				}
				else
				{
					color = RGB(166,166,166);
				}
				CRect rcRect = rect;
				rcRect.left+= l_textpadding;
				rcRect.right -= 22;
				DrawMenuText(memDC, rcRect, pData->lpstrText, color);
			}

			// draw insp
			if (pData->fType & MFT_SEPARATOR)
			{
				CRect rcRect = rect;
				Gdiplus::Pen pen(Gdiplus::Color(166,166,166));
				graphics.DrawLine(&pen, l_textpadding, rcRect.top + rcRect.Height()/2, 
									rcRect.right, rcRect.top + rcRect.Height()/2);
			}

			if(lpdi->itemState & ODS_CHECKED)
			{
				CRect rcRect = rect;
				rcRect.left += 2;
				rcRect.top = rect.top + (rect.Height() - l_checkwidth)/2;
				rcRect.bottom = rcRect.top + l_checkwidth;
				rcRect.right = rcRect.left + l_checkwidth;

				Gdiplus::SolidBrush brush(Gdiplus::Color(212,213,216));
				graphics.FillRectangle(&brush, rcRect.left, rcRect.top, rcRect.Width(), rcRect.Height());

				Gdiplus::Pen pen(Gdiplus::Color(10,36,106));
				graphics.DrawRectangle(&pen, rcRect.left, rcRect.top, rcRect.Width(), rcRect.Height());
				
				Gdiplus::SolidBrush br(Gdiplus::Color(0,0,0));
				Gdiplus::Font font(L"Marlett",10);
				graphics.DrawString(L"b",1,&font, Gdiplus::PointF((Gdiplus::REAL)rcRect.left+2, (Gdiplus::REAL)rcRect.top+4),&br);
			}

			
			



			dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), memDC, rect.left, rect.top, SRCCOPY);

			bHandled = TRUE;
			
		}
		

		return 0;
	}

	LRESULT OnMeasureItem (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
	{
		int nID = wParam;
		LPMEASUREITEMSTRUCT	lpms = (LPMEASUREITEMSTRUCT) lParam;
		CDC dc = GetWindowDC();
		Gdiplus::Font font(dc);
		if(lpms->CtlType == ODT_MENU)
		{
			CCommandBarCtrl::_MenuItemData *pmd = ( CCommandBarCtrl::_MenuItemData *) lpms->itemData;
			if(pmd == NULL) return 0;
			if(pmd->fType & MFT_SEPARATOR)   // separator - use half system height and zero width
			{
				lpms->itemHeight = l_menuheight2;
				lpms->itemWidth  = 0;
			}
			else
			{
				
				lpms->itemHeight = l_menuheight;

				CRect rcText(0,0,0,0);
				dc.DrawText(pmd->lpstrText, -1, &rcText, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_CALCRECT);
				int cx = rcText.right - rcText.left;

				cx += l_exlength;

				lpms->itemWidth = cx;

			}
			
		


		}
		
		
		return 0;
	}
};
