
#pragma  once 


namespace pyplus
{	
	
	
	struct pyinstance
	{
		pyinstance();
		void init();
		void release();
		~pyinstance();
	};

	
	struct pyobject
	{

		PyObject *obj;
		
		void decref();
		void incref();

		pyobject();
		explicit  pyobject(long i): obj(PyLong_FromLong(i)) { }
		explicit pyobject(unsigned long i): obj(PyLong_FromUnsignedLong(i)) {}
		explicit pyobject(double i): obj(PyFloat_FromDouble(i)){}
		explicit pyobject(float i): obj(PyFloat_FromDouble((double)i)){}
		explicit pyobject(const char *s): obj(PyString_FromString(s)){}
		explicit pyobject(const wchar_t *s): obj(PyUnicode_FromWideChar(s, wcslen(s))){}
		
		virtual ~pyobject();

		PyObject*  ptr() const;

		pyobject(PyObject *o);

		bool isNone() const;
		
		bool import(const char *name);

		pyobject callfunction(char *format, ...);

		pyobject attr(const char *name);

		pyobject dict() const;

		pyobject call(pyobject & args, pyobject  kw = NULL);

		pyobject callmethod(char *method) { return PyObject_CallMethodObjArgs(ptr(), pyobject(method).ptr(),NULL);}
		template<typename A1> pyobject callmethod(char *method, const A1 & a1)
		{
			return PyObject_CallMethodObjArgs(ptr(), pyobject(method).ptr(), pyobject(a1).ptr(), NULL);
		}
		template<typename A1,typename A2> pyobject callmethod(char *method, const A1 & a1,const A2 & a2) { return PyObject_CallMethodObjArgs(ptr(), pyobject(method).ptr(), pyobject(a1).ptr(),pyobject(a2).ptr(), NULL);} template<typename A1,typename A2,typename A3> pyobject callmethod(char *method, const A1 & a1,const A2 & a2,const A3 & a3) { return PyObject_CallMethodObjArgs(ptr(), pyobject(method).ptr(), pyobject(a1).ptr(),pyobject(a2).ptr(),pyobject(a3).ptr(), NULL);} template<typename A1,typename A2,typename A3,typename A4> pyobject callmethod(char *method, const A1 & a1,const A2 & a2,const A3 & a3,const A4 & a4) { return PyObject_CallMethodObjArgs(ptr(), pyobject(method).ptr(), pyobject(a1).ptr(),pyobject(a2).ptr(),pyobject(a3).ptr(),pyobject(a4).ptr(), NULL);} template<typename A1,typename A2,typename A3,typename A4,typename A5> pyobject callmethod(char *method, const A1 & a1,const A2 & a2,const A3 & a3,const A4 & a4,const A5 & a5) { return PyObject_CallMethodObjArgs(ptr(), pyobject(method).ptr(), pyobject(a1).ptr(),pyobject(a2).ptr(),pyobject(a3).ptr(),pyobject(a4).ptr(),pyobject(a5).ptr(), NULL);} template<typename A1,typename A2,typename A3,typename A4,typename A5,typename A6> pyobject callmethod(char *method, const A1 & a1,const A2 & a2,const A3 & a3,const A4 & a4,const A5 & a5,const A6 & a6) { return PyObject_CallMethodObjArgs(ptr(), pyobject(method).ptr(), pyobject(a1).ptr(),pyobject(a2).ptr(),pyobject(a3).ptr(),pyobject(a4).ptr(),pyobject(a5).ptr(),pyobject(a6).ptr(), NULL);} template<typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7> pyobject callmethod(char *method, const A1 & a1,const A2 & a2,const A3 & a3,const A4 & a4,const A5 & a5,const A6 & a6,const A7 & a7) { return PyObject_CallMethodObjArgs(ptr(), pyobject(method).ptr(), pyobject(a1).ptr(),pyobject(a2).ptr(),pyobject(a3).ptr(),pyobject(a4).ptr(),pyobject(a5).ptr(),pyobject(a6).ptr(),pyobject(a7).ptr(), NULL);} template<typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7,typename A8> pyobject callmethod(char *method, const A1 & a1,const A2 & a2,const A3 & a3,const A4 & a4,const A5 & a5,const A6 & a6,const A7 & a7,const A8 & a8) { return PyObject_CallMethodObjArgs(ptr(), pyobject(method).ptr(), pyobject(a1).ptr(),pyobject(a2).ptr(),pyobject(a3).ptr(),pyobject(a4).ptr(),pyobject(a5).ptr(),pyobject(a6).ptr(),pyobject(a7).ptr(),pyobject(a8).ptr(), NULL);}

		pyobject & operator = (const pyobject & o);

		void attach(PyObject * o);
		PyObject* dettach();


		// conversation
		long tolong() const;
		unsigned long toulong() const;
		long long toll() const;
		unsigned long long toull() const;
		float tofloat() const;
		double todouble() const;

		operator float () const;
		operator long () const;
	};

	struct pylong: public pyobject
	{
		explicit pylong(long i);
		explicit pylong(long long i);
		explicit pylong(unsigned long i);
		explicit pylong(unsigned long long i);
	};


	struct pytuple : public pyobject
	{
		explicit  pytuple( int size );
		pytuple();

		pytuple(const pyobject & o);
		
		bool check() const;
		int size() const;

		pyobject get(int pos) const;
		void set(int pos, pyobject & o);

		pyobject  operator [] ( const int idx ) const;

	};

	struct pydict : public pyobject
	{
		pydict();
		pydict( const pyobject & o);
		int size() const;
		pyobject get(const pyobject & key) const;
		int set(const pyobject & key, const pyobject & value);
		void clear();
		int has_key(const pyobject & key) const;
		pyobject  operator [] ( const pyobject & key ) const;
		
	};

	struct pyfloat : public pyobject
	{
		explicit pyfloat(double v);
		explicit pyfloat(float v);
	};
	
	static PyCFunctionObject *free_list = NULL;
	struct pymodule : public pyobject
	{
		pymodule(const char *modname);
		
		
		const char *name() const; 

		template < typename T >
		int reg( const char *fun_name,  T Fn  )
		{
			return 1;
		}
	};


	// function

	
	struct global
	{
		static pyobject import(const char *name);
		static void pyrelease();
	};

	
}

#pragma comment(lib, "pyplus")

