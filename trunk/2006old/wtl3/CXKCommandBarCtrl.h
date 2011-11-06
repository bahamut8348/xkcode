#pragma  once  

#include <GdiPlus.h>
using namespace Gdiplus;

template <class T, class TBase = CCommandBarCtrlBase, class TWinTraits = ATL::CControlWinTraits>
class ATL_NO_VTABLE CXKCommandBarCtrlImpl : public CCommandBarCtrlImpl<T, TBase, TWinTraits>
{
public:	
	DECLARE_WND_SUPERCLASS(_T("WTL_XKCommandBar"), GetWndClassName())
	
	typedef CCommandBarCtrlImpl< T, TBase, TWinTraits > baseCtrlClass;
	
	BEGIN_MSG_MAP(CXKCommandBarCtrlImpl)
		
		CHAIN_MSG_MAP( baseCtrlClass )
		ALT_MSG_MAP(1)      // Parent window messages
		CHAIN_MSG_MAP_ALT( baseCtrlClass, 1 )
		ALT_MSG_MAP(2)      // MDI client window messages
		CHAIN_MSG_MAP_ALT( baseCtrlClass, 2 )
		ALT_MSG_MAP(3)      // Message hook messages
		CHAIN_MSG_MAP_ALT( baseCtrlClass, 3 )
	END_MSG_MAP()

	enum
	{
		l_LeftPadding = 1,
		l_TopPadding = 1,
		l_BottomPadding = 1,
		l_RightPadding = 1,
		l_LeftFrameWidth = 25,
		l_MenuHeight = 23,
		l_SeparatorHeight = 5,
		l_TextLeftPadding = 30,
		l_TextRightPadding = 18,
		l_CheckWidth = 19,
		l_ExLength = 70,

		

		c_ItemBackgroud = RGB(255,255,255),
		c_LeftFrameColorLeft = RGB(249,248,247),
		c_LeftFrameColorRight = RGB(215,212,204),

		c_SelFrameColor = RGB(10,36,106),
		c_SelBkColor = RGB(182,189,210),

		c_TextNormalColor = RGB(0,0,0),
		c_TextDisableColor = RGB(166,166,166),

		c_SeparatorColor = RGB(166,166,166),

		c_CheckBkColor = RGB(212,213,216),
		c_CheckFrameColor = RGB(10,36,106),
	};


	// brush
	
	DWORD RgbToArgb(DWORD in)
	{
		DWORD n = in | 0xff000000;

		BYTE *p = (BYTE*)&n;
		p[0] = GetBValue(in);
		p[2] = GetRValue(in);
		return n;
	}


	void DrawItem3D(LPDRAWITEMSTRUCT lpDrawItemStruct)
	{
		LPDRAWITEMSTRUCT	lpdi = lpDrawItemStruct;
		CDCHandle dc = lpdi->hDC;
		CDC  memDC;
		memDC.CreateCompatibleDC(dc);
		memDC.SelectFont(AtlGetDefaultGuiFont());
		CBitmap bmp;
		bmp.CreateCompatibleBitmap(dc, 400,400);
		memDC.SelectBitmap(bmp);
		CRect rect;
		rect = lpdi->rcItem;

		Gdiplus::SolidBrush brush(Gdiplus::Color(0));
		Gdiplus::Pen		pen(Gdiplus::Color(0));

		CCommandBarCtrl::_MenuItemData *pData = (CCommandBarCtrl::_MenuItemData *)lpdi->itemData;

		Gdiplus::Font font(dc);

		// all white
		memDC.FillRect(&rect, c_ItemBackgroud);
		// left
		Gdiplus::Graphics graphics(memDC);
		Gdiplus::LinearGradientBrush linGrBrush(Gdiplus::Rect(0, 0, l_LeftFrameWidth, l_MenuHeight),  //  绘制区域
			Gdiplus::Color(RgbToArgb(c_LeftFrameColorLeft)),  //  第一种颜色
			Gdiplus::Color(RgbToArgb(c_LeftFrameColorRight)),  //  第二种颜色 
			0	);  //  渐变色的角度
		graphics.FillRectangle(&linGrBrush, Gdiplus::Rect(rect.left, rect.top, l_LeftFrameWidth, l_MenuHeight));



		//  select 
		BOOL bEnable =!(lpdi->itemState & ODS_GRAYED);
		if( (lpdi->itemState & ODS_SELECTED ) && (bEnable))
		{
			CRect rcRect = rect;
			rcRect.DeflateRect(l_LeftPadding,l_TopPadding,l_RightPadding,l_BottomPadding);
			brush.SetColor(Gdiplus::Color(RgbToArgb(c_SelBkColor)));
			graphics.FillRectangle(&brush, rcRect.left, rcRect.top, rcRect.Width(), rcRect.Height());

			pen.SetColor(Gdiplus::Color(RgbToArgb(c_SelFrameColor)));
			graphics.DrawRectangle(&pen, rcRect.left, rcRect.top, rcRect.Width(), rcRect.Height());
		}

		// draw text
		TCHAR *pos =_tcschr(pData->lpstrText, L'\t');
		Gdiplus::StringFormat fomater;
		fomater.SetHotkeyPrefix(Gdiplus::HotkeyPrefixShow );
		fomater.SetAlignment(Gdiplus::StringAlignmentFar);
		fomater.SetLineAlignment(Gdiplus::StringAlignmentCenter);
		DWORD TextColor = 0;
		if( bEnable) TextColor = c_TextNormalColor;
		else TextColor = c_TextDisableColor;
		CRect rcRect = rect;
		rcRect.left += l_TextLeftPadding;
		rcRect.right -= l_TextRightPadding;
		Gdiplus::RectF rcText( (Gdiplus::REAL)rcRect.left , (Gdiplus::REAL)rcRect.top,
			(Gdiplus::REAL)rcRect.Width(), (Gdiplus::REAL)rcRect.Height());
		brush.SetColor(Gdiplus::Color(RgbToArgb(TextColor)));
		int len = 0;
		if(pos)
		{
			len = pos - pData->lpstrText;
			graphics.DrawString(pos+1, _tcslen(pos+1), &font,
				rcText,&fomater,	&brush);
		}
		else
		{
			len = _tcslen(pData->lpstrText);
		}
		fomater.SetAlignment(Gdiplus::StringAlignmentNear);
		if(len>0)
		graphics.DrawString(pData->lpstrText, len, &font,
			rcText,&fomater,	&brush);


		// draw SEPARATOR
		if (pData->fType & MFT_SEPARATOR)
		{
			
			CRect rcRect = rect;
			rcRect.left += l_TextLeftPadding;
			memDC.FillRect(&rcRect, c_ItemBackgroud);
			pen.SetColor(Gdiplus::Color(RgbToArgb(c_SeparatorColor)));
			pen.SetWidth(1.0f);
			graphics.DrawLine(&pen, l_TextLeftPadding, rcRect.top + rcRect.Height()/2, 
				rcRect.right, rcRect.top + rcRect.Height()/2);
		}

		// draw check flag
		if(lpdi->itemState & ODS_CHECKED)
		{
			CRect rcRect = rect;
			rcRect.left += 2;
			rcRect.top = rect.top + (rect.Height() - l_CheckWidth)/2;
			rcRect.bottom = rcRect.top + l_CheckWidth;
			rcRect.right = rcRect.left + l_CheckWidth;

			brush.SetColor(Gdiplus::Color(RgbToArgb(c_CheckBkColor)));
			graphics.FillRectangle(&brush, rcRect.left, rcRect.top, rcRect.Width(), rcRect.Height());

			pen.SetColor(Gdiplus::Color(RgbToArgb(c_CheckFrameColor)));
			graphics.DrawRectangle(&pen, rcRect.left, rcRect.top, rcRect.Width(), rcRect.Height());

			brush.SetColor(Gdiplus::Color(RgbToArgb(c_TextNormalColor)));
			Gdiplus::Font font(L"Marlett",10);
			fomater.SetAlignment(StringAlignmentCenter );
			fomater.SetLineAlignment(StringAlignmentCenter );
			Gdiplus::RectF rcF((Gdiplus::REAL)rcRect.left, (Gdiplus::REAL)rcRect.top, 
				(Gdiplus::REAL)rcRect.Width(), (Gdiplus::REAL)rcRect.Height());
			graphics.DrawString(L"b", 1, &font, rcF, &fomater, &brush);
		}

		// draw bitmap
		int iButton = pData->iButton;
		if(iButton >= 0)
		{
			
			HBRUSH hBrushBackground = ::GetSysColorBrush(COLOR_BTNHILIGHT);
			HBRUSH hBrushDisabledImage = ::GetSysColorBrush(COLOR_3DSHADOW);
			
			int cx, cy;
			::ImageList_GetIconSize(m_hImageList, &cx, &cy);

			CPoint pt((l_LeftFrameWidth - cx)/2, rect.top + (l_MenuHeight - cy)/2);
			
			//::ImageList_Draw(m_hImageList, iButton, memDC, pt.x, pt.y, ILD_TRANSPARENT);
			IMAGELISTDRAWPARAMS ildp = { 0 };
			ildp.cbSize = sizeof(IMAGELISTDRAWPARAMS);
			ildp.himl = m_hImageList;
			ildp.i = iButton;
			ildp.hdcDst = memDC;
			ildp.x = pt.x;
			ildp.y = pt.y;
			ildp.cx = 0;
			ildp.cy = 0;
			ildp.rgbBk = CLR_NONE;
			ildp.xBitmap = 0;
			ildp.yBitmap = 0;
			ildp.fStyle = ILD_TRANSPARENT;
			ildp.fState = 0 ;
			ildp.Frame = 200;
			CImageList il;
			
			//il.DrawEx(iButton, memDC, pt.x, pt.y, 10,10,0,0,ILD_TRANSPARENT );
			//::ImageList_DrawIndirect(&ildp);
			
		}
	



		

		dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), memDC, rect.left, rect.top, SRCCOPY);

		return;
	}

	void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
	{
		LPMEASUREITEMSTRUCT	lpms = lpMeasureItemStruct;
		CDC dc = GetWindowDC();
		Gdiplus::Font font(dc);
		if(lpms->CtlType == ODT_MENU)
		{
			CCommandBarCtrl::_MenuItemData *pmd = ( CCommandBarCtrl::_MenuItemData *) lpms->itemData;
			if(pmd == NULL) return ;
			if(pmd->fType & MFT_SEPARATOR)   // separator - use half system height and zero width
			{
				lpms->itemHeight = l_SeparatorHeight;
				lpms->itemWidth  = 0;
			}
			else
			{

				lpms->itemHeight = l_MenuHeight;

				CRect rcText(0,0,0,0);
				dc.DrawText(pmd->lpstrText, -1, &rcText, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_CALCRECT);
				int cx = rcText.right - rcText.left;

				cx += l_ExLength;

				lpms->itemWidth = cx;

			}




		}
	}


};


class CXKCommandBarCtrl : public CXKCommandBarCtrlImpl<CXKCommandBarCtrl>
{
public:
	DECLARE_WND_SUPERCLASS(_T("WTL_XKCommandBar"), GetWndClassName())
};

