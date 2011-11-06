
#pragma once 

namespace PyPSClient2
{	

#define Len( l ) extract<int>( l.attr("__len__")() )


	class PyLock
	{
	public:
		PyGILState_STATE m_gil;
		PyLock():m_gil(PyGILState_Ensure()){}
		~PyLock()
		{
			PyGILState_Release(m_gil);
		}
	};


	template <typename T, typename KEY>
	_dword ListToArray( list & lst, Array<T, KEY> & ary)
	{
		for(int a=0; a<Len(lst); a++)
		{
			ary.Append( extract<T>(lst[a]));
		}
		return Len(lst);
	}

	template <typename T1, typename Key, 
		 template<typename ELEM, typename Key = ELEM > class T2 >
	list ArrayToList( const T2<T1, Key> & ary)
	{
		list lst;
		for(T2<T1, Key>::Iterator Iter  = ary.GetHeadIterator(); Iter.Valid(); Iter++)
		{
			const T1 & e = Iter;
			lst.append(e);
		}
		return lst;
	}

	template< typename T>
	list ArrayToList(const T * pT, _dword n)
	{
		list lst;
		for(_dword a=0; a<n; a++)
		{
			lst.append(pT[a]);
		}
		return lst;
	}

	template<>
	list ArrayToList(const _char *pch, _dword n)
	{
		list lst;
		for(_dword a=0; a<n; a++)
		{
			lst.append( std::wstring(pch) );
		}
		return lst;
	}

	template <typename Tsrc, typename Tdst>
	Tdst FConvert( const Tdst & dst)
	{
		return *( const Tdst *) &dst;
	}


}