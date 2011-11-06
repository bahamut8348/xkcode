// TestSystem.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"


class LogWrap : public ILog
{
public:
	LogWrap(PyObject *self):mSelf(self){}
	LogWrap(){}
	_void log(const _char* Format, ... )
	{

		_char buffer[4096] = {0};
		StringFormatter::FormatBufferV( buffer, 4096, Format, (_chara*)( &Format ) + sizeof( Format ) );
		//_tprintf(L"%s", buffer);
		Write(buffer);

	};

	virtual _void Write( const std::wstring s)
	{
		return call_method<_void>(mSelf, "Write", s);
	}

	PyObject *mSelf;

};


struct ClientUIWrap : public IClientUI
{
	ClientUIWrap(PyObject *self) : mSelf(self) {}
	_void OnConnectIS(_bool bConnect, const PSUID is_id, _dword Reason, const wchar_t *desp = _null)
	{
		//return onConnectIS(bConnect, is_id, Reason, desp);
	};

	PyObject* mSelf;
};



class UIProcessorWrap : public IUIProcessor
{
public:
	PyObject *mSelf;
	UIProcessorWrap(PyObject* self) : mSelf(self) {}
	_bool ConnectToIS(){ return call_method<_bool>(mSelf, "ConnectToIS"); }
	_bool QueryISList(_dword Callback){ return call_method<_bool>(mSelf, "QueryISList"); };
	_bool QueryFSList(_dword Callback){return call_method<_bool>(mSelf, "QueryFSList"); }
	


};


struct FrameworkWrap : public IClientFramework
{
	FrameworkWrap(PyObject* self) : mSelf(self) {}
	_bool Initialize(IClientUI *p)  { return call_method<_bool>(mSelf, "Initialize", p); }
	void SetUIClient(IClientUI *p)	{ return call_method<void>(mSelf, "SetUIClient", p); }
	_bool Uninitialize() { return call_method<_bool>(mSelf, "Uninitialize"); }
	_dword GetVersion() const { return call_method<_dword>(mSelf, "GetVersion"); }
	String GetError(_dword a) const { return call_method<String>(mSelf, "GetError", a); }
	_void SetLogHandle(ILog *pLog) { return call_method<_void>(mSelf, "SetLogHandle",pLog); }
	PSUID GetLocalPSUID() const {return call_method<PSUID>(mSelf, "GetLocalPSUID"); }
	IUIProcessor *GetProcessor() {return call_method<IUIProcessor *>(mSelf, "GetProcessor");}
	Address GetLocalIP()	const	{ return call_method<Address> (mSelf, "GetLocalIP") ; }
	Address GetRemoteIP()	const	{ return call_method<Address> (mSelf, "GetRemoteIP") ; }
	Address GetPublicIP()	const	{ return call_method<Address> (mSelf, "GetPublicIP") ; }
	_void Update()					{ return call_method<_void > (mSelf, "Update") ; }
	_void SetAppInfo(_word AppID, _word SubID, _word MajorVer, _word SubVer)
	{
		return call_method<_void> (mSelf, "SetAppInfo", AppID, SubID, MajorVer, SubVer) ;
	}
	_void SetISAddress(const Address & addr){ return call_method<_void > (mSelf, "SetISAddress") ;};
	_void SetInitIS(_dword num, const Address *IsAddrs) = 0;
	Array<Address> GetInitIS() const		= 0;
	_void SetGetISUrl(const _char *url)		= 0;
	String GetISUrl() const					= 0;
	PyObject *mSelf;
};


BOOST_PYTHON_MODULE(PSClient2)
{

	class_<PSUID>("PSUID",  init<_large>())
		.add_property( "value", &PSUID::Value )
		;

	class_<Address>("Address", init<_dword, _word, _word>())
		.add_property( "Type", &Address::mType)
		.add_property( "Port", &Address::mPort)
		;

	class_<ILog, noncopyable, LogWrap>("ILog")
		.def("Write", &LogWrap::Write)
		;

	class_<IClientUI, noncopyable, ClientUIWrap>("IClientUI")
		//.def("OnConnectIS", pure_virtual(&IClientUI::OnConnectIS))
		;
#define   MAKEUI(n) .def(#n, pure_virtual(&IUIProcessor::n))
	class_<IUIProcessor, noncopyable, UIProcessorWrap>("IUIProcessor")
		MAKEUI(ConnectToIS)
		MAKEUI(QueryISList)
		MAKEUI(QueryFSList)
		;

#define  MAKEFM(n) .def(#n, pure_virtual(&IClientFramework::n))
	class_<IClientFramework,noncopyable, FrameworkWrap>("IClientFramework")
		MAKEFM(Initialize)
		MAKEFM(SetUIClient)
		MAKEFM(Uninitialize)
		MAKEFM(GetVersion)
		MAKEFM(GetError)
		MAKEFM(SetLogHandle)
		MAKEFM(GetLocalPSUID)
		.def("GetProcessor", pure_virtual(&IClientFramework::GetProcessor), return_value_policy<manage_new_object>())
		
		;




}


#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

