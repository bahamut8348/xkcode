// ShortCut.cpp : Implementation of CShortCut

#include "stdafx.h"
#include "ShortCut.h"
#include <algorithm>
#include <functional>

// CShortCut
#define  SHORTCUTDIR  L"E:\\常用\\"
#define  FOLDERCFG	  L"d:\\f.txt"


#define ResultFromShort(i)  ResultFromScode(MAKE_SCODE(SEVERITY_SUCCESS, 0, (USHORT)(i)))



BOOL LineToFolderPath(const TCHAR * str, CShortCut::FolderPath  & fp)
{
	CString s = str;
	int pos = s.Find(L'=');
	if(pos >0)
	{
		fp.Name = s.Mid(0, pos);
		fp.Path = s.Mid(pos+1);
		fp.Name.TrimRight(L" ");
		fp.Path.TrimLeft(L" ");
		return TRUE;
	}
	return FALSE;
}


BOOL ReadLines( const TCHAR * FileName , CAtlArray<CString> & Lines)
{
	Lines.RemoveAll();
	USES_CONVERSION;
	HANDLE hF = ::CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
	CAtlFile file(hF);
	ULONGLONG filesize;
	file.GetSize(filesize);
	if (filesize > 1024*1024)
	{
		
		return  FALSE;
	}
	char *buffer = new char[(DWORD)filesize+1];

	file.Read(buffer, (DWORD)filesize);
	buffer[filesize] = 0;
	CString s = A2W(buffer);
	int start = 0;
//	AtlMessageBox(NULL, (LPCTSTR)s);
	int pos = 0;
	pos = s.Find(L"\n");

	while (pos > -1)
	{
		CString Line = s.Mid(start, pos-start);
		Line.TrimRight(L"\r\n\t ");
		Lines.Add(Line);
		start = pos+1;
		pos = s.Find(L"\n", start);
	};

	s.ReleaseBuffer();
	return TRUE;
}

void WriteLines( const TCHAR * FileName , CAtlArray<CString> & Lines)
{
	
	
	HANDLE hF = ::CreateFile(FileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, TRUNCATE_EXISTING, 0, 0);
	CAtlFile file(hF);
	for(DWORD a=0; a<Lines.GetCount(); a++)
	{
		
		CString & line = Lines[a];
		USES_CONVERSION;
		char * linea = W2A((LPCTSTR)line);
//		::MessageBoxW(NULL,line,L"转前",0);
//		::MessageBoxA(NULL,linea,"转后",0);
		file.Write(linea, (DWORD)::strlen(linea));
		file.Write("\r\n", 2);
	}
	
}

void ShowArray(CAtlArray<CString> & Lines )
{
	CString s;
	for(DWORD a=0; a<Lines.GetCount(); a++)
	{
		s+= Lines[a];
		s+= L"\r\n";
	}
	MessageBoxW(NULL, s, L"", 0);
}

void  ReadFolders(CAtlArray<CShortCut::FolderPath> & Folders)
{
	CAtlArray<CString> Lines;
	if(ReadLines(FOLDERCFG, Lines))
	{
		
//		ShowArray(Lines);
		for(DWORD a=0; a<Lines.GetCount(); a++)
		{
			CShortCut::FolderPath FP;
			if(LineToFolderPath(Lines[a], FP))
			{
				Folders.Add(FP);
			}
		}
	}
}

void WriteFolder(CAtlArray<CShortCut::FolderPath> & Folders)
{
	CAtlArray<CString>  Lines;
	for(DWORD a=0; a<Folders.GetCount(); a++)
	{
		CString s;
		s.Format(L"%s = %s",(LPCTSTR) Folders[a].Name, (LPCTSTR) Folders[a].Path);
		Lines.Add(s);
	}
	WriteLines(FOLDERCFG, Lines);
}

CString GetCurrentURL( HWND hwnd )
{
	CString ret;
	CComPtr<IShellWindows> psw;
	psw.CoCreateInstance(CLSID_ShellWindows);
	long lShellWindowCount=0;
	psw->get_Count(&lShellWindowCount);
	for(int a= 0; a< lShellWindowCount; a++)
	{	
		CComPtr<IDispatch> pdispShellWindow;
		psw->Item(CComVariant(a),&pdispShellWindow);
		CComQIPtr<IWebBrowser2> pIE(pdispShellWindow);
		if(pIE == NULL) continue;
		HWND iehwnd;
		pIE->get_HWND((SHANDLE_PTR *)&iehwnd);
		if(iehwnd == hwnd)
		{
			BSTR url;
			HRESULT r = pIE->get_LocationURL(&url);
			if( SUCCEEDED(r) )
			{
				ret = url;
			}
			break;
		}

	}

	return ret;
}

BOOL NavigateWindow(HWND hwnd, const TCHAR * url)
{
	BOOL bRet = FALSE;
	CComPtr<IShellWindows> psw;
	psw.CoCreateInstance(CLSID_ShellWindows);
	long lShellWindowCount=0;
	psw->get_Count(&lShellWindowCount);
	for(int a= 0; a< lShellWindowCount; a++)
	{	
		CComPtr<IDispatch> pdispShellWindow;
		psw->Item(CComVariant(a),&pdispShellWindow);
		CComQIPtr<IWebBrowser2> pIE(pdispShellWindow);
		if(pIE == NULL) continue;
		HWND iehwnd;
		pIE->get_HWND((SHANDLE_PTR *)&iehwnd);
		if(iehwnd == hwnd)
		{
			HRESULT ret = pIE->Navigate2(&CComVariant(url), &CComVariant(0), 
				&CComVariant(L"_self"), &CComVariant(0), &CComVariant(0));
			bRet = TRUE;
			break;
		}

	}

	return bRet;
}

CString GetShortCutPath(CString ShortCutFile)
{
	CComPtr<IShellLink> link;
	::CoCreateInstance(CLSID_ShellLink,NULL, CLSCTX_INPROC_SERVER,IID_IShellLink, (LPVOID *)&link);
	CComPtr<IPersistFile> pf;
	pf = link;
	pf->Load(ShortCutFile, STGM_READWRITE);
	CString LnkPath;
	WIN32_FIND_DATA data;
	link->GetPath(LnkPath.GetBuffer(MAX_PATH), MAX_PATH, &data, SLGP_RAWPATH);
	LnkPath.ReleaseBuffer();
	return LnkPath;
}

CString GetShortCutIndir(CString File, CString dir)
{
	
	CString ret;
	if(dir[dir.GetLength()-1] != L'\\')
		dir+= L"\\";
	WIN32_FIND_DATA data;
	HANDLE fH = ::FindFirstFile(dir+L"*.lnk", &data);
	if(fH != INVALID_HANDLE_VALUE)
	{
		while (1)
		{
			if( GetShortCutPath(dir + data.cFileName) == File)
			{
				ret = dir+data.cFileName;
				break;
			}

			if(FALSE == ::FindNextFile(fH, &data))
			{
				break;
			}

		}
		::FindClose(fH);
	}
	return ret;
}

BOOL CreateShortCutIndir(CString File, CString Dir)
{
	CComPtr<IShellLink> link;
	::CoCreateInstance(CLSID_ShellLink,NULL, CLSCTX_INPROC_SERVER,IID_IShellLink, (LPVOID *)&link);
	link->SetPath(File);
	int pos = File.ReverseFind(L'\\');
	CString FilePath = File.Mid(0, pos);
	CString FileName = File.Mid(pos+1);
	link->SetWorkingDirectory(FilePath);

	CComPtr<IPersistFile> pf;
	pf = link;
	pf->Save(Dir + L"\\" + FileName + L".lnk",  STGM_READWRITE);
	return TRUE;
}


STDMETHODIMP CShortCut::Initialize( LPCITEMIDLIST, LPDATAOBJECT pDataObj, HKEY )
{
	FORMATETC fmt = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	STGMEDIUM stg = { TYMED_HGLOBAL };
	HDROP     hDrop;

	mDosCmd = -1;
	
	ReadFolders(m_FolderPath);

//	OutputDebugStringA("short cut initalize");

	if(pDataObj == NULL)
	{
		m_szFile=L"";
		return S_OK;
	}
	// Look for CF_HDROP data in the data object.
	if ( FAILED( pDataObj->GetData ( &fmt, &stg ) ))
	{
		// Nope! Return an "invalid argument" error back to Explorer.
		return E_INVALIDARG;
	}

	WIN32_FIND_DATA FindFileData;


	HANDLE fhandle =::FindFirstFile(SHORTCUTDIR L"*", &FindFileData);
	if(fhandle != INVALID_HANDLE_VALUE)
	{
		while(1)
		{
			BOOL ret = ::FindNextFile(fhandle, &FindFileData);
			if(ret == FALSE)
			{
				break;
			}

			if(FILE_ATTRIBUTE_DIRECTORY & FindFileData.dwFileAttributes )
			{
				ShortCutPath path;
				path.Name = FindFileData.cFileName;
				path.Path = SHORTCUTDIR + path.Name;
				m_ShortCutPath.Add(path);
			}
		}
		FindClose(fhandle);
	}

	// Get a pointer to the actual data.
	hDrop = (HDROP) GlobalLock ( stg.hGlobal );

	// Make sure it worked.
	if ( NULL == hDrop )
		return E_INVALIDARG;

	// Sanity check - make sure there is at least one filename.
	UINT uNumFiles = DragQueryFile ( hDrop, 0xFFFFFFFF, NULL, 0 );
	HRESULT hr = S_OK;

	if ( 0 == uNumFiles )
	{
		GlobalUnlock ( stg.hGlobal );
		ReleaseStgMedium ( &stg );
		return E_INVALIDARG;
	}

	// Get the name of the first file and store it in our member variable m_szFile.
	if ( 0 == DragQueryFile ( hDrop, 0, m_szFile.GetBuffer(MAX_PATH), MAX_PATH ) )
		hr = E_INVALIDARG;
	m_szFile.ReleaseBuffer();

	GlobalUnlock ( stg.hGlobal );
	ReleaseStgMedium ( &stg );
	
	CString s;
	s.Format(L"short cut initalize :file: %s ", m_szFile);
//	OutputDebugString( s );

	return hr;
}

STDMETHODIMP CShortCut::GetCommandString(UINT_PTR idCmd, UINT uFlags, UINT* pwReserved, LPSTR pszName, UINT cchMax)
{
	USES_CONVERSION;
	
	// Check idCmd, it must be 0 since we have only one menu item.

	// If Explorer is asking for a help string, copy our string into the
	// supplied buffer.
	if ( uFlags & GCS_HELPTEXT )
	{
		LPCTSTR szText = m_szFile;

		if ( uFlags & GCS_UNICODE )
		{
			// We need to cast pszName to a Unicode string, and then use the
			// Unicode string copy API.
			lstrcpynW ( (LPWSTR) pszName, T2CW(szText), cchMax );
		}
		else
		{
			// Use the ANSI string copy API to return the help string.
			lstrcpynA ( pszName, T2CA(szText), cchMax );
		}

		return S_OK;
	}

	return E_INVALIDARG;
}

STDMETHODIMP CShortCut::QueryContextMenu(  HMENU hmenu, UINT uMenuIndex, UINT uidFirstCmd,
										 UINT uidLastCmd, UINT uFlags )
{
	// If the flags include CMF_DEFAULTONLY then we shouldn't do anything.
	if ( uFlags & CMF_DEFAULTONLY )
		return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 0 );
	
	int MenuCount = 0;

	if( uFlags & CMF_CANRENAME  )
	{
		if( m_szFile == _T("") )
			return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 0 );
	}

	CStringA debugstr;
	debugstr.Format("QueryContextMenu Flag: %d, File: %s, %d", uFlags , CStringA(m_szFile),PathIsDirectory( m_szFile ) );
//	OutputDebugStringA( debugstr );

	CMenu menu;
	menu.CreatePopupMenu();
	for(DWORD a=0; a<m_ShortCutPath.GetCount(); a++)
	{
		menu.AppendMenu(MF_STRING, uidFirstCmd + MenuCount,  m_ShortCutPath[a].Name);
		++MenuCount;
	}

	CMenu FolderMenu;
	FolderMenu.CreatePopupMenu();
	for(DWORD a=0; a<m_FolderPath.GetCount(); a++)
	{
		CString showstr;
		showstr.Format(L"[%s] %s", m_FolderPath[a].Name, m_FolderPath[a].Path);
		FolderMenu.AppendMenu(MF_STRING, uidFirstCmd + MenuCount, 
			showstr);
		++MenuCount;
	}
	
	FolderMenu.InsertMenu(0, MF_BYPOSITION |MF_SEPARATOR );

	if(PathIsDirectory(m_szFile))
	{
		if(GetPathIndex() == -1)
		{
			FolderMenu.InsertMenu(0, MF_BYPOSITION|MF_STRING, 
				uidFirstCmd + MenuCount , L"添加此文件夹(&A)");
		}
		MenuCount ++;

		if(GetPathIndex() != -1)
		{
			FolderMenu.InsertMenu(0, MF_BYPOSITION|MF_STRING, 
				uidFirstCmd + MenuCount , L"去除本文件夹(&R)");
		}
		MenuCount ++;
	}
	else
	{
		MenuCount +=2;
	}
	FolderMenu.InsertMenu(0, MF_BYPOSITION|MF_STRING, 
		uidFirstCmd + MenuCount , L"设置(&S)");
	MenuCount++;


	//InsertMenu ( hmenu, 0, MF_BYPOSITION | MF_SEPARATOR, -1, 0 );

	if(!PathIsDirectory(m_szFile) && PathFileExists(m_szFile) )
	{
		InsertMenu ( hmenu, 0, MF_BYPOSITION | MF_POPUP, (UINT_PTR)(HMENU)menu, _T("发送快捷方式到(&S)") );
	}

	
	{
		mDosCmd =  MenuCount;
		int err = InsertMenu( hmenu, 0,  MF_BYPOSITION | MF_STRING, mDosCmd + uidFirstCmd, _T("打开 DOS") );
		MenuCount++;
	}

	InsertMenu ( hmenu, 1, MF_BYPOSITION | MF_POPUP, (UINT_PTR)(HMENU)FolderMenu, _T("转到文件夹(&M)") );
	
	InsertMenu ( hmenu, 2, MF_BYPOSITION | MF_SEPARATOR, -1, 0 );

	return ResultFromShort( MenuCount );
}

STDMETHODIMP CShortCut::InvokeCommand( LPCMINVOKECOMMANDINFO pCmdInfo)
{
	
	HWND hwnd = pCmdInfo->hwnd;
	// If lpVerb really points to a string, ignore this function call and bail out.

	if ( 0 != HIWORD( pCmdInfo->lpVerb ) )
		return E_INVALIDARG;

	// Get the command index - the only valid one is 0.
	int iCmd = LOWORD( pCmdInfo->lpVerb);
	
	CString cdir = CString( pCmdInfo->lpDirectory?pCmdInfo->lpDirectory:"" );

	CStringA s;
	s.Format("Invote icmd: %d,verb: %d, doscmd: %d, dir: %s", iCmd, (int) pCmdInfo->lpVerb, mDosCmd, CStringA(cdir) );
//	OutputDebugStringA(s);
	if( IsGotoDOSCmd(iCmd) !=-1 )
	{
		CString openpath;
		if( PathIsDirectory( m_szFile ) )
		{
			openpath = m_szFile;
		}
		else
		{
			openpath = cdir;
		}
		
		CString para;
		para = _T(" /K chdir /D ") + openpath;

		ShellExecute( NULL, (GetAsyncKeyState(VK_LCONTROL) || GetAsyncKeyState(VK_RCONTROL) )?_T("runas"):0, _T("cmd.exe"), para, openpath, SW_SHOWNORMAL );
		return NOERROR;
	}

	if(IsShortCutCmd(iCmd) != -1)
	{
		ShortCutPath & sc = m_ShortCutPath[iCmd];

		CString OldShortCutPath = GetShortCutIndir(m_szFile, sc.Path);
		if(OldShortCutPath !=L"")
		{
			int answer = ::MessageBox(hwnd, L"快捷方式已经存在, 是否覆盖?", L"警告", MB_YESNO|MB_ICONQUESTION);
			if(answer == IDYES)
			{
				::DeleteFile(OldShortCutPath);
				CreateShortCutIndir(m_szFile, sc.Path);
				AtlMessageBox(hwnd, L"创建快捷方式成功.",L"成功",64);
			}
			else 
			{
				
			}
		}
		else
		{
			CreateShortCutIndir(m_szFile, sc.Path);
			AtlMessageBox(hwnd, L"创建快捷方式成功.",L"成功",64);
		}
	}
	else if (IsFolderCmd(iCmd) != -1)
	{
		int CmdIndex = IsFolderCmd(iCmd);
		//if(FALSE == NavigateWindow(pCmdInfo->hwnd,m_FolderPath[CmdIndex].Path ))
		{
			::ShellExecute( hwnd , L"open", m_FolderPath[CmdIndex].Path, NULL, NULL, SW_SHOWNORMAL);
		}
		//AtlMessageBox(hwnd,(LPCTSTR) m_FolderPath[CmdIndex].Path,	(LPCTSTR)m_FolderPath[CmdIndex].Name,64);
	}
	else if(IsAddFolderCmd(iCmd) != -1)
	{
	
		FolderPath FP;
		FP.Path = m_szFile;
		FP.Name = m_szFile.Mid(m_szFile.ReverseFind(L'\\')+1);
		
		m_FolderPath.Add(FP);
		WriteFolder(m_FolderPath);
		CString msg;
		msg.Format(L"添加文件夹 %s 成功.", FP.Path);
		MessageBox(hwnd, msg, L"添加文件夹成功.", 64);

	}
	else if(IsRemoveFolderCmd(iCmd) != -1)
	{
		int idx = GetPathIndex();
		m_FolderPath.RemoveAt(idx);
		WriteFolder(m_FolderPath);
		CString msg;
		msg.Format(L"去除文件夹 %s 成功.", m_szFile);
		MessageBox(hwnd, msg, L"去除文件夹成功.", 64);
	}
	else if (IsSettingFolderCmd(iCmd)!=-1)
	{
		::ShellExecute(NULL, L"open", FOLDERCFG, L"", L"", SW_SHOW);
	}

	return E_INVALIDARG;
}

int CShortCut::IsShortCutCmd( DWORD iCmd )
{
	if(iCmd < (int)m_ShortCutPath.GetCount())
	{
		return iCmd;
	}
	return -1;
}

int CShortCut::IsFolderCmd( DWORD iCmd )
{
	if ( (iCmd>=m_ShortCutPath.GetCount()) && 
		(iCmd < m_FolderPath.GetCount() + m_ShortCutPath.GetCount()) )
	{
		return int(iCmd - m_ShortCutPath.GetCount());
	}
	return -1;
}

int CShortCut::IsAddFolderCmd( DWORD iCmd )
{
	if( m_FolderPath.GetCount() + m_ShortCutPath.GetCount() == iCmd)
		return 1;
	return -1;
}

int CShortCut::GetPathIndex()
{
	int  ret= -1;
	for(DWORD a=0; a<m_FolderPath.GetCount(); a++)
	{
		if(m_FolderPath[a].Path == m_szFile)
		{
			ret = a;
			break;
		}
	}
	return ret;
}

int CShortCut::IsRemoveFolderCmd( DWORD iCmd )
{
	if( m_FolderPath.GetCount() + m_ShortCutPath.GetCount() +1 == iCmd)
		return 1;
	return -1;
}

int CShortCut::IsSettingFolderCmd( DWORD iCmd )
{
	if( m_FolderPath.GetCount() + m_ShortCutPath.GetCount() +2 == iCmd)
		return 1;
	return -1;
}

int CShortCut::IsGotoDOSCmd( DWORD iCmd )
{
	if( iCmd == mDosCmd ){ return 1;}
	return -1;
}