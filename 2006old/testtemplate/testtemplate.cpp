#include "stdafx.h"


template<typename T>
struct CmpFun
{
	_bool operator() ( const T & t) const
	{
		return _true;
	}
};


struct DoFun
{
	_void operator()(const int & t) const
	{
		
	}
};

template<typename T, typename FUN>
_void Foreach(const T & c, FUN  fun)
{
	for (T::Iterator Iter = c.GetHeadIterator(); Iter.Valid(); Iter++)
	{
		fun(Iter);
	}
}

template <typename T1, typename Key , typename FUN,
	template<typename ELEM, typename Key = ELEM> class T2>
T2<T1, Key> Filter(T2<T1, Key>  c, FUN fun)
{
	for (T2<T1, Key>::Iterator Iter = c.GetHeadIterator(); Iter.Valid(); Iter++)
	{
		if(!fun( (T1 &)Iter))
		{
			c.Remove(Iter);
		}
	}
	return c;
}




template <typename T1, typename Key , typename FUN,
	template<typename ELEM, typename Key = ELEM> class T2>
T2<T1, Key> & Removeif(T2<T1, Key> & c, FUN fun)
{
	T2<T1, Key>::Iterator Iter = c.GetHeadIterator();

	while(Iter.Valid())
	{
		T1 & t1 = Iter;
		if( fun(t1) )
		{
			c.Remove(Iter);
			
		}
		Iter++;
	}
	return c;	
}

template <typename T1, typename Key , typename FUN,
	template<typename ELEM, typename Key = ELEM> class T2>
T1 *Findif(T2<T1, Key> & c, FUN fun)
{
	T1 * ret = _null;
	for(T2<T1, Key>::Iterator Iter  = c.GetHeadIterator(); Iter.Valid(); Iter++)
	{
		T1 & tmp = Iter;
		if(fun(tmp))
		{
			ret = &tmp;
			break;
		}
	}
	return ret;
}

template < typename T>
struct sttest
{
	sttest(T a)
	{
		this->a = a;
	}
	T a;
	_bool operator() (  T  t) const
	{
		if (t==a)
			return true;
		return false;
	}

};

template < typename T, typename FUN>
struct sttest2
{
	sttest2(T a, FUN f)
	{
		this->a = a;
		this->fun = f;
	}
	T a;
	FUN fun;
	template< typename TT>
	_bool operator() ( TT & t) const
	{
		if (t.*fun==a)
			return true;
		return false;
	}

};


template < typename T, typename FUN>
struct sttest2ptr
{
	sttest2ptr(T a, FUN f)
	{
		this->a = a;
		this->fun = f;
	}
	T a;
	FUN fun;
	template< typename TT >
	_bool operator() ( TT * t) const
	{
		if (t->*fun==a)
			return true;
		return false;
	}

};

template < typename T, typename FUN>
struct sttest0ptrfun
{
	sttest0ptrfun(T a, FUN f)
	{
		this->a = a;
		this->fun = f;
	}
	T a;
	FUN fun;
	template< typename TT >
	_bool operator() ( TT * t) const
	{
		if ((t->*fun)()==a)
			return true;
		return false;
	}

};

template < typename T, typename FUN, typename A1>
struct sttest1ptrfun
{
	sttest1ptrfun(T a, FUN f, A1 a1)
	{
		this->a = a;
		this->fun = f;
		this->a1 = a1;
	}
	T a;
	A1 a1;
	FUN fun;
	template< typename TT >
	_bool operator() ( TT * t) const
	{
		if ((t->*fun)(a1)==a)
			return true;
		return false;
	}

};

template < typename T, typename FUN>
struct sttest0fun
{
	sttest0fun(T a, FUN f)
	{
		this->a = a;
		this->fun = f;
	}
	T a;
	FUN fun;
	template< typename TT >
	_bool operator() ( TT & t) const
	{
		if ((t.*fun)()==a)
			return true;
		return false;
	}

};

template < typename T, typename FUN, typename A1>
struct sttest1fun
{
	sttest1fun(T a, FUN f, A1 a1)
	{
		this->a = a;
		this->fun = f;
		this->a1 = a1;
	}
	T a;
	A1 a1;
	FUN fun;
	template< typename TT >
	_bool operator() ( TT * t) const
	{
		if ((t.*fun)(a1)==a)
			return true;
		return false;
	}

};






template <typename T >
sttest<T> test(T a)
{
	return sttest<T>(a);
}

template < typename T, typename FUN>
sttest2<T,FUN> bind1(FUN f, T  v)
{
	return sttest2<T,FUN>(v, f);
}

template < typename T, typename FUN>
sttest2ptr<T,FUN> bind1ptr(FUN f, T  v)
{
	return sttest2ptr<T,FUN>(v, f);
}

template < typename T, typename FUN>
sttest0fun<T,FUN> bindfun(FUN f, T  v)
{
	return sttest0fun<T,FUN>(v, f);
}

template < typename T, typename FUN, typename A1>
sttest1fun<T,FUN,A1> bindfun(FUN f, T  v, A1 a1)
{
	return sttest1fun<T,FUN,A1>(v, f, a1);
}


template < typename T, typename FUN>
sttest0ptrfun<T,FUN> bindptrfun(FUN f, T  v)
{
	return sttest0ptrfun<T,FUN>(v, f);
}

template < typename T, typename FUN, typename A1>
sttest1ptrfun<T,FUN,A1> bindptrfun(FUN f, T  v, A1 a1)
{
	return sttest1ptrfun<T,FUN>(v, f, a1);
}







_void print(const int & a)
{
	_tprintf(L"%d\n", a);
}




struct ST
{
	int a;
	int b;

	int geta() 
	{
		return a;
	}

	int getb()
	{
		return b;
	}
};


#define  N(n) #n



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

	int pos = 0;
	pos = s.Find(L"\n");

	while (pos > -1)
	{
		CString Line = s.Mid(start, pos-1);
		Lines.Add(Line);
		start = pos+1;
		pos = s.Find(L"\n", start);
	};



	s.ReleaseBuffer();
	return TRUE;
}


int _tmain(int argc, _TCHAR* argv[])
{
	

	
	Array<int> m;
	m.Append(1);
	m.Append(2);
	m.Append(3);

	_tprintf(L"Test : %s\n", _T(N(Array<int>)));

	
	//Foreach(Removeif(m, test), print);

	int * pint = Findif(m, test(1));
	if(pint == NULL)
	{
		_tprintf(L"Cant Find\n");
	}
	else
	{
		_tprintf(L"Got it: %d\n", *pint);
	}

	ST st;
	Array<ST> aa;
	st.a=1;st.b=2;
	aa.Append(st);
	st.a=3; st.b=2;
	aa.Append(st);
	st.a=5; st.b=6;
	aa.Append(st);

	Array<ST> bb = Filter(aa, bind1(&ST::b, 6));
	
	ST * pst  = Findif(aa, bindfun(&ST::geta, 3));
	if(pst == NULL)
	{
		_tprintf(L"Cant Find\n");
	}
	else
	{
		_tprintf(L"Got it: %d\n", pst->a);
	}


	
	Array<ST *> cc;
	ST *st1 = new ST;
	st1->a=1;st1->b=2;
	cc.Append(st1);
	ST *st2 = new ST;
	st2->a=3; st2->b=2;
	cc.Append(st2);
	ST *st3 = new ST;
	st3->a=5; st3->b=6;
	cc.Append(st3);

	Array<ST *> dd = Filter(cc, bind1ptr(&ST::a, 5));
	ST ** ppst  = Findif(cc, bindptrfun(&ST::geta, 5));
	if(ppst == NULL)
	{
		_tprintf(L"Cant Find\n");
	}
	else
	{
		_tprintf(L"Got it: %d\n", (*ppst)->a);
	}



	system("Pause");

	
	return 0;

}

