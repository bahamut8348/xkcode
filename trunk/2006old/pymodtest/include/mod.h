
namespace xkmod
{
	class H1
	{
	public:
		H1();
		void print( )
		{
			wprintf(L"haha\n");
		}
	};


	class H2
	{
	public:
		void foo(   H1 &  h )
		{
			h.print();
		}

		void bar ( bp::object o )
		{
			bp::extract<H1> h1(o);
			
			if( h1.check() )
			{
				H1 h = h1();
				h.print( );
			}
			else
			{
				wprintf( L"not H1\n" );
			}
		}
	};
}
