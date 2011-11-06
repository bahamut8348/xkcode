#include "StdAfx.h"
#include "DetoursWapper.h"
#include "DataManager.h"
#include "assert.h"

DataManager::DataManager(void):
mMemSize(102400)
{
	mMemName = L"Global\\ShareMemName_1";
	mMutexName = L"Global\\MemMutexName_1";

	mhMutex = ::CreateMutex( NULL, FALSE, mMutexName.c_str( ) );
	BOOL has_exists = FALSE;
	if( GetLastError() == ERROR_ALREADY_EXISTS )
	{
		has_exists = TRUE;
	}
	
	mhMem = ::CreateFileMapping(
		INVALID_HANDLE_VALUE,	// 文件句柄
		NULL,					// 安全属性(WIN95忽略)
		PAGE_READWRITE,			// 文件的保护属性
		0,						// 使用页面文件
		mMemSize,				// 共享内存大小
		mMemName.c_str());		// 共享内存名称

	mMemPtr = (BYTE*)::MapViewOfFile(
		mhMem,						// 核心对象的文件HANDLE
		FILE_MAP_ALL_ACCESS,        // Read/Write
		0,                          // 使用页面文件
		0,                          // 映射整个共享内存区域
		0);                         // 映射整个空间
	if(mMemPtr == NULL)
	{
		assert(FALSE);
	}
	
}


DataManager::~DataManager( void )
{
	UnmapViewOfFile( mMemPtr );
	CloseHandle(mhMem);
	CloseHandle(mhMutex);
}

void DataManager::SetHWND( HWND hwnd )
{
	mhWnd = hwnd;
	MemHeader *pheader = ( MemHeader* )this;
	pheader->mhWnd = hwnd;
}

HWND DataManager::GetHWND() const
{
	return FindWindow(L"SOCK_MON_FRAME",L"sockmon");;
}

void DataManager::Lock()
{
	WaitForSingleObject( mhMutex,( DWORD ) -1 );
}

void DataManager::UnLock()
{
	ReleaseMutex( mhMutex );
}

void DataManager::SetData( DWORD threadID, DWORD msgtype, NetInfo::emType subtype, SOCKET sock, BYTE* data, DWORD datalen, SOCKADDR_IN addr )
{
	HWND hwnd = GetHWND();
	if( hwnd )
	{

		DWORD dstpid = 0;
		DWORD srcpid = ::GetCurrentProcessId();
		GetWindowThreadProcessId( hwnd, &dstpid );
		HANDLE dstHandle, srcHandle;
		dstHandle = ::OpenProcess( PROCESS_DUP_HANDLE, FALSE, dstpid);
		srcHandle = ::OpenProcess( PROCESS_DUP_HANDLE, FALSE, srcpid);
		SOCKET dstsock = 0;
		BOOL ret = DuplicateHandle( srcHandle, (HANDLE)sock, dstHandle, (LPHANDLE)&dstsock, 0, FALSE, DUPLICATE_SAME_ACCESS );
		assert(ret);
		assert(dstHandle);
		MemHeader *header = ( MemHeader* )mMemPtr;
		NetInfo *pinfo = ( NetInfo*)(header+1);
		Lock();
		header->mMsgType = 1;
		header->mThreadID = threadID;
		pinfo->mType = subtype;
		pinfo->mLength = datalen;
		pinfo->mSock = dstsock;
		memcpy( pinfo->mData, data, datalen);
		::SendMessage( hwnd, DataManager::_CALL_MSG, 0, 0 );
		UnLock();

		::CloseHandle( dstHandle );
		::CloseHandle( srcHandle );
	}
}

void DataManager::GetData( DWORD& threadID, DWORD& msgtype, NetInfo::emType& subtype, SOCKET& sock, BYTE* data, DWORD& datalen, SOCKADDR_IN& addr )
{
	MemHeader *header = ( MemHeader* )mMemPtr;
	NetInfo *pinfo = ( NetInfo*)(header+1);
	
	threadID = header->mThreadID;
	subtype = pinfo->mType;
	datalen = pinfo->mLength ;
	sock = pinfo->mSock;
	memcpy( data, pinfo->mData, datalen);
}