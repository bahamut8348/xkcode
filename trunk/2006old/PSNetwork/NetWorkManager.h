#pragma once


namespace PSNetwork
{

	enum BUFFTYPE
	{
		BUFFTYPE_CREATE,
		BUFFTYPE_DELETE,
		BUFFTYPE_JOINFAILD,
		BUFFTYPE_PACKET,
	};


	struct BuffBase
	{
		virtual BUFFTYPE gettype() = 0;
		virtual ~BuffBase(){};
	};

	struct BuffMsg : public BuffBase
	{
		virtual BUFFTYPE gettype()
		{
			return mType;
		}
		BUFFTYPE mType;

		_dword SessionID;
		_dword ConnectionID;
	};

	struct BuffData : public BuffMsg
	{
		Array<_byte> mBuff;
	};


	
	
	class CBufferArray
	{
	public:
		List<BuffBase*> mList;
		Lock  mLock;
		void Add(  BuffBase *pBase )
		{
			mLock.Enter();
			mList.InsertTail(pBase);
			mLock.Leave();
		}

		BuffBase *Get() 
		{
			BuffBase *pret = _null;
			mLock.Enter();
			BuffBase ** pptemp = mList.FirstElement();
			if(pptemp)
			{
				pret = *pptemp;
				mList.Remove(mList.GetHeadIterator());
			}
			mLock.Leave();
			return pret;
		}
		
	};
	
	
	class CNetWorkManager
	{
	public:
		CNetWorkManager();
		~CNetWorkManager(void);

		struct TimerInfo
		{
			object mFun;
			_large  mCalltime;
			boost::python::tuple mArgs;
			dict mArgskw;
		};

		static void OnCreateConnectionCb(_dword sessionid, _dword connectionid );
		static void OnDeleteConnectionCb( _dword sessionid, _dword connectionid );
		static void OnReceivePacketCb( _dword sessionid, _dword connectionid, _byte* buffer, _dword size );
		static void OnJoinFailedCb( _dword sessionid, _dword connectionid );
		static void OnUpdateSessionCb( _dword sessionid );
		static void OnNetworkTimerCb( _dword tickcount );

		static CNetworkSession CreateSession( _dword sessionid, _dword maxconnection, PyObject *callback);
		static CNetworkSession CreateSession2( _dword sessionid,  PyObject *callback);
		static void Update();
		static void Run();
		static object CallLater( boost::python::tuple args, dict kw );
		static int CallFunction();
		static void Release();
	private:
		INetworkManager *m_NwManager;

		static PyObject *m_Callback;

		static Array<TimerInfo> s_CallArray;

		CBufferArray m_Buffers;
	};


	void InitNetworkManager();

}

