// testpcap.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


struct NetInterface
{
	std::string mName;
	std::string mDesc;
	
	std::string mAddr;
	sockaddr    mAddr_data;
	
	std::string mNetmask;
	sockaddr    mNetmask_data;

	std::string mBroadaddr;
	sockaddr    mBroadaddr_data;

	std::string mDstaddr;
	sockaddr    mDstaddr_data;

	std::string get_str_() const
	{
		ATL::CAtlStringA s;
		s.Format("<NetIF %s,addr=%s,netmask=%s>", mDesc.c_str(), mAddr.c_str(), mNetmask.c_str() );
		return (LPCSTR)s;
	}

	std::string repr() const
	{
		std::string s;
		s.assign( mDesc.begin()+mDesc.find('\'')+1, mDesc.begin()+mDesc.find_last_of('\'') );
		s.insert( s.begin(),'<');
		s.insert( s.end(),'>');
		return s;
	}

};

struct NetPacketInfo
{
	pcap_pkthdr mHeader;

	double ToFloat() const
	{
		double usec = ((double)mHeader.ts.tv_usec)/1000000.0f + mHeader.ts.tv_sec;
		return usec;
	}

	std::string ToString( ) const
	{
		ATL::CAtlStringA s;
		s.Format("<PacketInfo %.02f, CapLength: %d, PackLength: %d >", ToFloat(), mHeader.caplen, mHeader.len );
		return (LPCSTR)s;
	}
};

struct NetDumpFile
{
	pcap_dumper_t *mFileHandle;
	std::string    mFileName;
	NetDumpFile( pcap_dumper_t* f, const char *filename ): mFileHandle(f),mFileName( filename )
	{

	}

	NetDumpFile():mFileHandle(0)
	{

	}

	object MakeFile()
	{
		FILE* fp = pcap_dump_file( mFileHandle );
		if( fp == NULL )
		{
			PyErr_SetString( PyExc_Exception, "Can't make file." );
			throw_error_already_set();
		}
		PyObject* o = PyFile_FromFile( fp, "savefile", "wb", fclose );
		return object(borrowed(o));
	}

	void Dump( const NetPacketInfo & info, const str & s )
	{
		pcap_dump( (u_char*)mFileHandle, &info.mHeader, (u_char*)PyString_AsString(s.ptr()) );
	}

	void Flush()
	{
		pcap_dump_flush( mFileHandle);
	}

	void Close( )
	{
		pcap_dump_close( mFileHandle );
	}
};



enum ChannelInfoFlag
{
	AIRPCAP_CIF_TX_ENABLED = 0x01,
};

struct ChannelInfo
{
	u_int mFrequency;
	char	  mExtChannel;
	char mFlag;
	UCHAR 	Reserved [2];
};

enum KeyType
{
	_KeyType_WEP = 0,
	_KeyType_TKIP = 1 ,
	_KeyType_CCMP = 2 ,
};
struct AirKey
{
	KeyType mtype;
	u_int mKeyLen;
	BYTE mKeyData[32];

	str get_data()
	{
		return str( (char*)mKeyData, mKeyLen);
	}
};

char gErrbuf[PCAP_ERRBUF_SIZE];

typedef int (*Func_AirpcapSetDeviceChannel) ( PAirpcapHandle, u_int );
typedef int (*Func_AirpcapGetDeviceChannel) ( PAirpcapHandle, u_int* );
typedef int (*Func_AirpcapSetLinkType ) ( PAirpcapHandle, u_int );
typedef int (*Func_AirpcapGetLinkType) ( PAirpcapHandle, u_int* );
typedef PAirpcapHandle (*Func_pcap_get_airpcap_handle)( pcap_t* );
typedef int (*Func_AirpcapSetFcsPresence )( PAirpcapHandle, u_int );
typedef int (*Func_AirpcapGetFcsPresence )( PAirpcapHandle, u_int* );
typedef int (*Func_AirpcapGetMacAddress)( PAirpcapHandle, BYTE* );
typedef int (*Func_AirpcapSetMacAddress)( PAirpcapHandle, BYTE* );
typedef int (*Func_GetDeviceChannelEx)( PAirpcapHandle, ChannelInfo*);
typedef int (*Func_AirpcapGetDeviceKeys)(PAirpcapHandle, BYTE* , u_int*);
typedef int (*Func_AirpcapGetDeviceList)(BYTE*, BYTE*);



struct AirCapure
{
	PAirpcapHandle mHandle;
	HMODULE mDll;
	HMODULE mWinpcapDll;
	Func_AirpcapSetDeviceChannel mSetChannel;
	Func_AirpcapGetDeviceChannel mGetChannel;

	Func_AirpcapSetLinkType mSetLinkType;
	Func_AirpcapGetLinkType mGetLinkType;

	Func_pcap_get_airpcap_handle mGetAirHandle;

	Func_AirpcapSetFcsPresence mSetFcsPresence;
	Func_AirpcapGetFcsPresence mGetFcsPresence;
	
	Func_AirpcapGetMacAddress mGetMacAddress;
	Func_AirpcapSetMacAddress mSetMacAddress;

	Func_GetDeviceChannelEx mGetChannelInfo;

	Func_AirpcapGetDeviceKeys mGetKeys;

	
	

	void init( pcap_t * h )
	{
		mWinpcapDll = ::GetModuleHandleA("wpcap");
		if ( mWinpcapDll == NULL )
		{
			PyErr_SetString( PyExc_Exception, "Can't load wpcap.dll" );
			throw_error_already_set();
		}
		
		mGetAirHandle = (Func_pcap_get_airpcap_handle)::GetProcAddress( mWinpcapDll, "pcap_get_airpcap_handle" );
		if( mGetAirHandle == NULL )
		{
			PyErr_SetString( PyExc_Exception, "wpcap has no airpcap support" );
			throw_error_already_set();
		}

		mHandle = mGetAirHandle( h );
		if( mHandle == NULL )
		{
			PyErr_SetString( PyExc_Exception, "Can't get air handle." );
			throw_error_already_set();
		}

		mDll = ::LoadLibrary(L"airpcap");
		if ( mDll == NULL )
		{
			PyErr_SetString( PyExc_Exception, "Can't load airpcap.dll" );
			throw_error_already_set();
		}

		
#define DLLFUNC( s ) (Func_##s)::GetProcAddress(mDll, #s );

		mSetChannel = (Func_AirpcapSetDeviceChannel)::GetProcAddress( mDll, "AirpcapSetDeviceChannel" );
		mGetChannel = (Func_AirpcapGetDeviceChannel)::GetProcAddress( mDll, "AirpcapGetDeviceChannel" );
		mSetLinkType = (Func_AirpcapSetLinkType)::GetProcAddress( mDll, "AirpcapSetLinkType" );
		mGetLinkType = (Func_AirpcapGetLinkType)::GetProcAddress( mDll, "AirpcapGetLinkType" );
		mSetFcsPresence = (Func_AirpcapSetFcsPresence)::GetProcAddress( mDll, "AirpcapSetFcsPresence" );
		mGetFcsPresence = (Func_AirpcapGetFcsPresence)::GetProcAddress( mDll, "AirpcapGetFcsPresence" );
		mGetMacAddress = DLLFUNC(  AirpcapGetMacAddress )
		mSetMacAddress = DLLFUNC(  AirpcapSetMacAddress )
		mGetChannelInfo = DLLFUNC( GetDeviceChannelEx ) 
		mGetKeys = DLLFUNC( AirpcapGetDeviceKeys )

	}
	void set_channel( u_int ch )
	{
		if( 0 == mSetChannel( mHandle, ch ) )
		{
			PyErr_SetString( PyExc_Exception, "set_channel" );
			throw_error_already_set();
		}
	}

	u_int get_channel()
	{
		u_int ch;
		if( mGetChannel( mHandle, &ch ) == 0 )
		{
			PyErr_SetString( PyExc_Exception, "get_channel" );
			throw_error_already_set();
		}
		return ch;
	}

	void set_linktype( u_int linktype )
	{
		if( 0 == mSetLinkType( mHandle, linktype ) )
		{
			PyErr_SetString( PyExc_Exception, "set_linktype" );
			throw_error_already_set();
		}
	}

	u_int get_linktype() const
	{
		u_int lt;
		if( mGetLinkType( mHandle, &lt ) == 0 )
		{
			PyErr_SetString( PyExc_Exception, "get_linktype" );
			throw_error_already_set();
		}
		return lt;
	}

	bool get_fcs() const
	{
		u_int fcs;
		if( mGetFcsPresence(mHandle, &fcs)  == 0 )
		{
			PyErr_SetString( PyExc_Exception, "get_fcs" );
			throw_error_already_set();
		}
		return fcs != 0;

	}

	void set_fcs( bool fcs) const
	{
		if( 0 == mSetFcsPresence( mHandle, fcs ) )
		{
			PyErr_SetString( PyExc_Exception, "set_fcs" );
			throw_error_already_set();
		}
	}

	void set_mac( const str & s )
	{
		u_char * ps = extract<u_char*>(s);
		if( len(s) != 6 )
		{
			PyErr_SetString( PyExc_Exception, "Mac address must length 6." );
			throw_error_already_set();
		}
		if( mSetMacAddress(mHandle,  ps ) ==0 )
		{
			PyErr_SetString( PyExc_Exception, "set_mac" );
			throw_error_already_set();
		}
	}

	str get_mac()
	{
		BYTE macaddr[6];
		if( mGetMacAddress(mHandle,  macaddr ) ==0 )
		{
			PyErr_SetString( PyExc_Exception, "get_mac" );
			throw_error_already_set();
		}
		return str((const char*)macaddr, 6);
	}

	ChannelInfo get_channelinfo()
	{
		ChannelInfo info;
		if( mGetChannelInfo(mHandle,  & info ) ==0 )
		{
			PyErr_SetString( PyExc_Exception, "get_channelinfo" );
			throw_error_already_set();
		}
		return info;
	}
	
	list get_keys()
	{
		BYTE *p = 0;
		UINT  length = 0;
		mGetKeys( mHandle, p, &length );
		int ret = mGetKeys( mHandle, p, &length );
		if( ret == 0 )
		{
			PyErr_SetString( PyExc_Exception, "get_keys" );
			throw_error_already_set();
		}
		list ls;
		UINT *keynum = (UINT*)p;
		for( u_int a =0 ; a <*keynum; a++ )
		{
			AirKey * pkey = ((AirKey*) (p+4)) + a;
			ls.append( *pkey );
		}
		return ls;
	}
};




struct Capture
{
	struct pcap_pkthdr *mHeader;
	const u_char *mPkt_data;
	char mErrbuf[PCAP_ERRBUF_SIZE];
	pcap_t * mHandle;
	PyObject* mRettuple;
	NetPacketInfo mPacketInfo;
	bool mStop;
	
	//NetInterface mInterface;

	AirCapure* air()
	{
		AirCapure * pair = new AirCapure();
		pair->init( mHandle );
		return pair;
	}

	Capture* get_iter()
	{
		mStop = false;
		return this;
	}

	Capture():mRettuple(NULL)
	{

	}


	~Capture()
	{
		pcap_close( mHandle );
	}

	void stop()
	{
		mStop = true;
	}

	void send( PyObject* s )
	{
		if( PyString_Check(s) == 0) s = PyObject_Str(s);
		int length = (int)PyString_GET_SIZE(s);
		if( 0!= pcap_sendpacket( mHandle, (u_char*)PyString_AS_STRING(s), length ) )
		{
			PyErr_SetString( PyExc_Exception,  pcap_geterr( mHandle ) );
			throw_error_already_set();
		}
	}

	object Makefile()
	{
		FILE* fp = pcap_file( mHandle );
		if( fp == NULL )
		{
			PyErr_SetString( PyExc_Exception, "Can't make file." );
			throw_error_already_set();
		}
		PyObject* o = PyFile_FromFile( fp, "savefile", "wb", fclose );
		return object(borrowed(o));
	}

	void setfilter( const std::string & s )
	{
		bpf_program fcode;
		u_int netmask = 0xffffff;
	//	if ( mInterface.mAddr.size() )
			/* 获取接口第一个地址的掩码 */
	//		netmask=((struct sockaddr_in *)&mInterface.mNetmask_data)->sin_addr.S_un.S_addr;
		
		
		if (pcap_compile( mHandle , &fcode, (char *)s.c_str(), 1, netmask) < 0)
		{
			PyErr_SetString( PyExc_Exception,  "pcap_compile, error");
			throw_error_already_set();
		}
		
		if (pcap_setfilter( mHandle, &fcode ) < 0)
		{
			PyErr_SetString( PyExc_Exception,  "pcap_setfilter, error");
			throw_error_already_set();
		}
	}

	NetDumpFile makedump( const std::string & filename )
	{
		pcap_dumper_t * dumper = pcap_dump_open( mHandle, filename.c_str( ) );
		if( dumper == NULL )
		{
			PyErr_SetString( PyExc_Exception,  pcap_geterr( mHandle) );
			throw_error_already_set();
		}
		return NetDumpFile( dumper, filename.c_str() );
	}

	tuple next( )
	{
		if( mRettuple == NULL )
		{
			mRettuple = PyTuple_New( 2 );
		}
		while( mStop == false )
		{
			int res = pcap_next_ex( mHandle, &mHeader, &mPkt_data );
			if (res > 0)
			{			
				mPacketInfo.mHeader = *mHeader;
				return make_tuple( mPacketInfo, str((char*) mPkt_data, mHeader->len ) );
			}
			else if( res == 0 )
			{
				continue;
			}
			else
			{
				PyErr_SetString( PyExc_Exception,  pcap_geterr( mHandle));
				throw_error_already_set();
			}
		}
		PyErr_SetString( PyExc_StopIteration,  "pcap_next_ex, error");
		throw_error_already_set();
		return make_tuple();
	}	
	


};



static list findalldevs()
{
	list ls;
	pcap_if_t *alldevs;
	
	if( pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL , &alldevs, gErrbuf) == -1 )
	{
		PyErr_SetString( 0,  gErrbuf);
		throw_error_already_set();
	}

	for(pcap_if_t* d= alldevs; d != NULL; d= d->next)
	{
		NetInterface nif;
		nif.mDesc = d->description;
		nif.mName = d->name ;
		if( d->addresses )
		{
			in_addr* pin;
			SOCKADDR_IN *paddr =(SOCKADDR_IN *) d->addresses->addr;
			
			pin = ( in_addr* ) (d->addresses->addr->sa_data + 2) ;
			nif.mAddr = inet_ntoa( *pin );
			nif.mAddr_data = *d->addresses->addr;

			pin = ( in_addr* ) (d->addresses->broadaddr->sa_data + 2) ;
			nif.mBroadaddr = inet_ntoa( *pin );
			nif.mBroadaddr_data = *d->addresses->broadaddr;
	
			pin = ( in_addr* ) (d->addresses->netmask->sa_data + 2) ;
			nif.mNetmask = inet_ntoa( *pin );
			nif.mNetmask_data = *d->addresses->netmask;

		}
		

		ls.append( nif );
	}
	pcap_freealldevs( alldevs );
	return ls;
}

static Capture* opendev_by_str( const std::string & src, u_int mode  = PCAP_OPENFLAG_PROMISCUOUS, u_int timeout = 200, const std::string & user = "", const std::string & pass = "" , int authtype = 0 )
{
	pcap_rmtauth auth;
	pcap_rmtauth* pauth = &auth;
	if( user.size() == 0 )
	{
		pauth = 0;
	}
	else
	{
		auth.type = authtype;
		auth.username = const_cast<char*>( user.c_str() );
		auth.password = const_cast<char*>( pass.c_str() );
	}

	pcap_t* hd = pcap_open( src.c_str( ) , 65536,  mode, timeout, pauth, gErrbuf );
	if (hd == NULL)
	{
		PyErr_SetString( PyExc_Exception,  gErrbuf);
		throw_error_already_set();
	}

	Capture*  cap = new Capture;
	cap->mHandle = hd;
//	cap->mInterface = nif;
	cap->mStop = false;
	return cap;
}

static Capture* opendev_by_name( const NetInterface & nif, u_int mode  = PCAP_OPENFLAG_PROMISCUOUS, u_int timeout = 200, const std::string & user = "", const std::string & pass = "" , int authtype = 0  )
{
	return opendev_by_str( nif.mName, mode, timeout, user, pass, authtype );
}

static Capture* openfile( const std::string & filename )
{
	std::string s = std::string("file://") + filename;
	return opendev_by_str( s );
}

static NetInterface get_default_nif()
{
	DWORD idx;
	DWORD r= ::GetBestInterface( inet_addr("202.202.202.202"), &idx  );
	if( r != NO_ERROR)
	{
		PyErr_SetString( PyExc_Exception,  "GetBestInterface");
		throw_error_already_set();
	}

	DWORD tablesize = 0;
	GetInterfaceInfo( 0, &tablesize);
	
	BYTE* buf = new BYTE[tablesize];
	GetInterfaceInfo(( PIP_INTERFACE_INFO) buf, &tablesize );
	PIP_INTERFACE_INFO tab = ( PIP_INTERFACE_INFO)buf;
	BOOL bFound = 0;
	for( int a = 0; a < tab->NumAdapters; a++ )
	{
		IP_ADAPTER_INDEX_MAP * row = &tab->Adapter[a];
		if( row->Index == idx )
		{
			USES_CONVERSION;
			list devs = findalldevs( );
			for( int a =0 ; a < len(devs); a++ )
			{
				NetInterface  nif = extract<NetInterface>(devs[a]);
				if( nif.mName.find(W2A((row->Name+13))) != -1)
				{
					return nif;
				}
			}
			
		}
	}
	PyErr_SetString( PyExc_Exception,  "Can't find default net interface.");
	throw_error_already_set();

	return NetInterface();
}


static Capture* opendev_by_default( u_int mode = PCAP_OPENFLAG_PROMISCUOUS, u_int timeout = 200, const std::string & user = "", const std::string & pass = "" , int authtype = 0 )
{
	NetInterface nif = get_default_nif();
	return opendev_by_name( nif, mode, timeout , user, pass, authtype );
}

static std::string createsrcstr( const std::string & host, const std::string & port, const std::string & name, u_int type  )
{
	char buf[2048];
	char errbuf[1024];
	int ret = pcap_createsrcstr( buf, type, host.c_str( ), port.c_str( ), name.c_str( ), errbuf );
	if(ret == 0 )
	{
		return buf;
	}
	else
	{
		PyErr_SetString( PyExc_Exception,  errbuf );
		throw_error_already_set();
	}
	return "";
}




BOOST_PYTHON_MODULE( winpcap )

{
	class_<NetInterface>("NetInterface")
		.def_readonly( "name", &NetInterface::mName )
		.def_readonly( "desc", &NetInterface::mDesc )
		.def_readonly( "addr", &NetInterface::mAddr )
		.def_readonly( "netmask", &NetInterface::mNetmask )
		.def_readonly( "broadaddr" , &NetInterface::mBroadaddr )
		.def( "__str__", &NetInterface::get_str_)
		.def( "__repr__", &NetInterface::repr )

		;
	
	class_<NetDumpFile>("NetDumpFile" )
		.def("dump", &NetDumpFile::Dump )
		.def("flush", &NetDumpFile::Flush )
		.def("close",	&NetDumpFile::Close )
		;

	class_<NetPacketInfo>("NetPacketInfo")
		.def("__float__", &NetPacketInfo::ToFloat )
		.def("__repr__",   &NetPacketInfo::ToString )
		.def("__str__",   &NetPacketInfo::ToString )
		;
	
	class_<Capture>("NetCapture")
		.def("__iter__", &Capture::get_iter, return_value_policy< reference_existing_object >( ) )
		.def("next",		&Capture::next )
		.def("setfilter",	&Capture::setfilter )
		.def("send",		&Capture::send )
		.def("stop",		&Capture::stop )
		.def("air",			&Capture::air , return_value_policy< manage_new_object >( ) )
		.def("makedump",		&Capture::makedump)
		;

	class_<AirCapure>("NetAirCapture")
		.add_property("channel", &AirCapure::get_channel, &AirCapure::set_channel )
		.add_property("linktype", &AirCapure::get_linktype, &AirCapure::set_linktype )
		.add_property("fcs", &AirCapure::get_fcs, &AirCapure::set_fcs )
		.add_property("mac", &AirCapure::get_mac, &AirCapure::set_mac)
		.add_property("channelinfo", &AirCapure::get_channelinfo )
		.add_property("keys", &AirCapure::get_keys )
		;
	
	enum_<KeyType>("KeyType")
		.value("WEP", _KeyType_WEP )
		.value("TKIP", _KeyType_TKIP )
		.value("CCMP", _KeyType_CCMP)
		.export_values()
		;
	
	class_<AirKey>("AirKey")
		.def_readwrite("type", &AirKey::mtype )
		.add_property("data", &AirKey::get_data )
		;
		

	class_<ChannelInfo>("NetAirChannelInfo")
		.def_readwrite("frequency", &ChannelInfo::mFrequency )
		.def_readwrite("ext_channel", &ChannelInfo::mExtChannel	)
		.def_readwrite("flag",		&ChannelInfo::mFlag )
		;
	
	def("createsrcstr", createsrcstr );
	def("findalldevs" , findalldevs );
	def("open", opendev_by_name, return_value_policy< manage_new_object > ( ), (arg("mode")=1, arg("timeout")=200, arg("username")="" , arg("password")="", arg("authtype")=0) );
	def("open", opendev_by_default, return_value_policy< manage_new_object > ( ),(arg("mode")=1, arg("timeout")=200,arg("username")="" , arg("password")="", arg("authtype")=0) );
	def("open", openfile ,  return_value_policy< manage_new_object > ( ));
	//def("open", opendev_by_str, return_value_policy< manage_new_object > ( ),(arg("mode")=1, arg("timeout")=200,arg("username")="" , arg("password")="", arg("authtype")=0) );
}


