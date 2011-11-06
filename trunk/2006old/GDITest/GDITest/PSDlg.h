#pragma once


// CPSDlg dialog

class CPSDlg : public CDialog
{
	DECLARE_DYNAMIC(CPSDlg)

public:
	CPSDlg(CWnd* pParent = NULL);   // standard constructor
	CPSDlg(DWORD DlgID, CWnd* pParent = NULL);
	virtual ~CPSDlg();

// Dialog Data
	enum { IDD = IDD_PSDLG };
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNcPaint();
public:
	int RenderFrame(unsigned int _rgn);

	static const	LONG	cTitleHeight		= 34;
	static const	LONG	cCornerWidth		= 34;
	static const	LONG	cCornerHeight		= 34;
	static const	LONG	cFrameWidth			= 2;
	static const	LONG	cBtnWidth			= 24;
	static const	LONG	cBtnHeight			= 24;
	static const	LONG	cBtnTopOffset		= 6;
	static const	LONG	cBtnRightOffset		= 2;
	static const	LONG	cBtnHDis			= 4;
	static const	LONG	cTitleTextLeftOffset= 7;
	static const	LONG	cTitleTextTopOffset	= 35;



public:  //
	CImage m_ImgLeftTop;
	CImage m_ImgRightTop;
	CImage m_ImgLBCorner;
	CImage m_ImgBtnClose;
	CImage m_ImgBtnMin;

	CDC m_LeftTopInGameMemDC;
	CDC m_RightTopInGameMemDC;
	CDC m_TitleBarMemDC;
	CDC m_LeftBottomMemDC;
	CDC m_BtnCloseMemDC;
	CDC m_BtnMinMemDC;
	
	CDC m_WinMemDC;
	CBitmap m_WinBmp;
	CImage m_WinImg;


	CRgn m_CornerRgnLT;
	CRgn m_CornerRgnRT;

	
	BOOL bNCLButtonDown;
	BOOL bMouseInClose;
	BOOL bMouseInMin;

	BOOL bBtnClosePressed;
	BOOL bBtnMinPressed;

	CRect m_rcCloseBtn;
	CRect m_rcMinBtn;

public:
	virtual BOOL OnInitDialog();
	void UpdateRegion();
	BOOL HaveMinBtn();
	void DrawCloseBtn(CDC *pDC);
	CRect GetBtnCloseRect();
	CRect GetBtnMinRect();
	BOOL IsMouseInClose(POINT pt);
	BOOL IsMouseInMin(POINT pt);
	void DrawMinBtn(CDC *pDC);
	void DrawTitle(CDC *pDC);


	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects,	NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnNcLButtonDown(UINT nHitTest,CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest,CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest,CPoint point);
	afx_msg void OnSizing(UINT nSide, LPRECT lpRect);
	afx_msg void OnMove( int x, int y );
	afx_msg void OnMoving(UINT nSide,LPRECT lpRect);
	afx_msg void OnLButtonUp(UINT nFlags,CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnInitMenu(CMenu* pMenu);
	

	virtual LRESULT WindowProc(	UINT message,WPARAM wParam,	LPARAM lParam );




	

};
