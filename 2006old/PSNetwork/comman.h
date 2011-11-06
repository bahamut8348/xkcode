#pragma once 


namespace PSNetwork
{
	#define Len( l ) extract<int>( l.attr("__len__")() )


#define PyAssert( v, msg ) \
	if(v == _null)\
	{\
		PyErr_SetString(PyExc_Exception, msg);\
		throw boost::python::error_already_set();\
	}

	class PyLock
	{
	public:
		PyGILState_STATE m_gil;
		PyLock()
		{

			//_tprintf(L"PyLock\n");
			m_gil = PyGILState_Ensure();
		}
		~PyLock()
		{
			//_tprintf(L"~PyLock\n");
			PyGILState_Release(m_gil);
		}
	};


	struct allow_threading_guard
	{
		allow_threading_guard()
			: save(PyEval_SaveThread())
		{}

		~allow_threading_guard()
		{
			PyEval_RestoreThread(save);
		}

		PyThreadState* save;
	};



	template <class F, class R>
	struct allow_threading
	{
		allow_threading(F fn)
			: fn(fn)
		{}

		template <class A0>
		R operator()(A0& a0)
		{
			allow_threading_guard guard;
			return (a0.*fn)();
		}

		template <class A0, class A1>
		R operator()(A0& a0, A1& a1)
		{
			allow_threading_guard guard;
			return (a0.*fn)(a1);
		}

		template <class A0, class A1, class A2>
		R operator()(A0& a0, A1& a1, A2& a2)
		{
			allow_threading_guard guard;
			return (a0.*fn)(a1,a2);
		}

		template <class A0, class A1, class A2, class A3>
		R operator()(A0& a0, A1& a1, A2& a2, A3& a3)
		{
			allow_threading_guard guard;
			return (a0.*fn)(a1,a2,a3);
		}

		template <class A0, class A1, class A2, class A3, class A4>
		R operator()(A0& a0, A1& a1, A2& a2, A3& a3, A4& a4)
		{
			allow_threading_guard guard;
			return (a0.*fn)(a1,a2,a3,a4);
		}

		template <class A0, class A1, class A2, class A3, class A4, class A5>
		R operator()(A0& a0, A1& a1, A2& a2, A3& a3, A4& a4, A5& a5)
		{
			allow_threading_guard guard;
			return (a0.*fn)(a1,a2,a3,a4,a5);
		}

		F fn;
	};

	template <class F>
	struct visitor : boost::python::def_visitor<visitor<F> >
	{
		visitor(F fn)
			: fn(fn)
		{}

		template <class Class, class Options, class Signature>
		void visit_aux(
			Class& cl, char const* name
			, Options const& options, Signature const& signature) const
		{
			typedef typename boost::mpl::at_c<Signature,0>::type return_type;

			cl.def(
				name
				, boost::python::make_function(
				allow_threading<F, return_type>(fn)
				, options.policies()
				, options.keywords()
				, signature
				)
				);
		}

		template <class Class, class Options>
		void visit(Class& cl, char const* name, Options const& options) const
		{
			this->visit_aux(
				cl, name, options
				, boost::python::detail::get_signature(fn, (typename Class::wrapped_type*)0)
				);
		}

		F fn;
	};

	// Member function adaptor that releases and aqcuires the GIL
	// around the function call.
	template <class F>
	visitor<F> allow_threads(F fn)
	{
		return visitor<F>(fn);
	}



	static Address str2addr(std::wstring s)
	{
		String str = s.c_str();
		str.Lowercase();

		_char ip1[4] = {0}, ip2[4] ={0}, ip3[4] ={0}, ip4[4] ={0}, proto[4]={0};
		_dword port;
		str.Parse(L"%[^:]://%[^.].%[^.].%[^.].%[^:]:%d", proto, ip4, ip3, ip2, ip1, &port);

		Address addr;
		if( StringPtr(proto)==L"tcp" )
			addr.mType = Address::_TYPE_TCP;
		else
			addr.mType = Address::_TYPE_UDP;

		addr.mIP1 = Long(ip1);
		addr.mIP2 = Long(ip2);
		addr.mIP3 = Long(ip3);
		addr.mIP4 = Long(ip4);

		addr.mPort = (_word)port;
		return addr;
	}

}