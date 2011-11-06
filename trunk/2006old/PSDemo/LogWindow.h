#pragma once

class CLogWindow : public CFrameWindowImpl<CLogWindow>,
		public CMessageFilter, public CIdleHandler, public CUpdateUI<CLogWindow>
{
public:
	//DECLARE_WND_CLASS(_T("Log Window Class"))
	DECLARE_FRAME_WND_CLASS(_T("Log Window Class"), IDR_MAINFRAME)
	
	BEGIN_UPDATE_UI_MAP(CLogWindow)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	
	BEGIN_MSG_MAP(CLogWindow)

		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(0x02A2, OnOther)
		//	MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)o
		
		MESSAGE_HANDLER(WM_NCCALCSIZE, OnNcCalcSize)
		MESSAGE_HANDLER(WM_NCPAINT, OnNcPaint)
		CHAIN_MSG_MAP(CFrameWindowImpl<CLogWindow>)
		CHAIN_MSG_MAP(CUpdateUI<CLogWindow>)
		
	END_MSG_MAP()


	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual BOOL OnIdle();



	//CLogEdit m_logedit;
	CLogEdit m_logedit;
	
	//CEdit m_logedit;
public:
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnNcCalcSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnNcPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOther(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

};

