#pragma once


namespace pycfg
{
	class CFG
	{
	public:
		CFG(void);
		~CFG(void);

		PyObject* mMod;

		PyObject* mGetUserAddr;
		PyObject* mGetUserGroup;

		PyObject* mServerPort;


		std::wstring getUserAddr( char * userkey, int keylen );
		int getServerPort( );


		static CFG & instance( )
		{
			static CFG cfg;
			return cfg;
		}
	};
}

