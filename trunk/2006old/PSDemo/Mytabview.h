#pragma once

class CMytabview :public CTabViewImpl<CMytabview>
{
public:
	DECLARE_WND_CLASS(_T("T_CustomTabView"))
	
	


	bool CreateTabControl();
	void UpdateLayout();
	void ShowTabControl(bool bShow);
	void OnContextMenu(int nPage, POINT pt);


};
