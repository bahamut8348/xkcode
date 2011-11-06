// ShortCut.h : Declaration of the CShortCut

#pragma once
#include "resource.h"       // main symbols

#include "ShortCutExt.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CShortCut

class ATL_NO_VTABLE CShortCut :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CShortCut, &CLSID_ShortCut>,
	public IDispatchImpl<IShortCut, &IID_IShortCut, &LIBID_ShortCutExtLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IShellExtInit,
	public IContextMenu
{
public:
	CShortCut()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SHORTCUT)


BEGIN_COM_MAP(CShortCut)
	COM_INTERFACE_ENTRY(IShortCut)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IContextMenu)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

	// IShellExtInit
	STDMETHODIMP Initialize(LPCITEMIDLIST, LPDATAOBJECT, HKEY);

	// IContextMenu
	STDMETHODIMP GetCommandString(UINT, UINT, UINT*, LPSTR, UINT);
	STDMETHODIMP InvokeCommand(LPCMINVOKECOMMANDINFO);
	STDMETHODIMP QueryContextMenu(HMENU, UINT, UINT, UINT, UINT);

	
	enum
	{
		SHORTCUTCMD = 0,
		FOLDERCMD = 255,
	};
public:
	struct ShortCutPath
	{
		CString Path;
		CString Name;
	};


	struct FolderPath : public ShortCutPath
	{
		
	};
	
public:
	int IsShortCutCmd(DWORD iCmd);
	int IsFolderCmd(DWORD iCmd);
	int IsAddFolderCmd(DWORD iCmd);
	int IsRemoveFolderCmd(DWORD iCmd);
	int IsSettingFolderCmd(DWORD iCmd);


	int GetPathIndex();
protected:
	CString m_szFile;
	CAtlArray<ShortCutPath> m_ShortCutPath;
	CAtlArray<FolderPath> m_FolderPath;

public:

};

OBJECT_ENTRY_AUTO(__uuidof(ShortCut), CShortCut)
