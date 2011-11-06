#pragma 


namespace  asyncAdo
{
	class Common
	{
	public:
		static void info(const TCHAR * Desc, Error *pError, DWORD Status)
		{
			TCHAR * msg = L"";
			if(pError)
				pError->get_Description(&msg);
			CString Info;
			Info.Format(L"%s: Status: %d, Message: %s\n",Desc, Status, (LPCTSTR)msg );
			HANDLE hCon = ::GetStdHandle(STD_OUTPUT_HANDLE);
			::WriteConsole(hCon, (LPCTSTR)Info, Info.GetLength(), NULL, NULL);
		}
	};
}