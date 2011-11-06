#pragma once

template<typename T>
class Singleton
{
public:
	static T* s_instance;
	static T* Instance()
	{
		if ( s_instance == NULL )
			s_instance = new T;
		return s_instance;
	}
};

template<typename T>
T *Singleton<T>::s_instance = NULL;

class Detours : public Singleton<Detours>
{
private:
	struct FunTable
	{
		PVOID* mOldfun;
		PVOID  mNewFun;
	};

	std::vector<FunTable> mFunTable;

public:
	template <typename T1, typename T2>
	bool Attach( T1 &  oldfun, T2 & newfun  )
	{
		FunTable tab;
		tab.mOldfun = &(PVOID&)oldfun;
		tab.mNewFun = newfun;
		DetourTransactionBegin();
		printf("Attach %d\n", (u_int)tab.mOldfun );
		DetourAttach( tab.mOldfun, tab.mNewFun );
		
		LONG ret = Commit();
		assert( ret == NO_ERROR);
		return ret == NO_ERROR;
	}
	
	template <typename T>
	bool Detach( T oldfun, T newfun )
	{
		DetourDetach( &(PVOID&)oldfun, (PVOID) newfun );
		bool ret = Commit() == NO_ERROR;
		return ret;
	}

	long Commit();
	void Release();

	Detours(void);
	~Detours(void);
};

