#pragma once

//class CLogEdit : public CRichEditCtrl,
class CLogEdit : public CWindowImpl<CLogEdit, CRichEditCtrl>
	,public CMessageFilter
	,public ILog
{
public:
	
	DECLARE_WND_SUPERCLASS(NULL, CRichEditCtrl::GetWndClassName())

	CLogEdit();
	~CLogEdit();

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		pMsg;
		return FALSE;
	}

	BEGIN_MSG_MAP(CLogEdit)
		MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_CREATE, OnInitDlg)
		//MESSAGE_HANDLER(WM_NCCALCSIZE, OnNcCalcSize)
		//MESSAGE_HANDLER(WM_NCPAINT, OnNcPaint)
	END_MSG_MAP()


	struct ColorMap
	{
		_char *Key;
		_dword Color;
		_bool test(const _char *key)
		{
			int a= StringPtr(key).SearchL2R(Key);
			return a>-1;
		}
	};

	ColorMap *pcmap;
	int ColorNum;

	File m_F;

	enum
	{
		l_leftPadding = 10,
	};

	_dword GetColor(const _char *c);

	
	LRESULT OnNcCalcSize (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnNcPaint (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnPaint (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnInitDlg (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);

	virtual _void log(const _char* Format, ... );
	_void AddLog( _dword type, const _char* Format, ... );
	
public:
	LRESULT OnContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};
