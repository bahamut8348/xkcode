#pragma once

namespace asyncAdo
{
	class CTestAdo
	{
	public:
		CTestAdo(void);
		~CTestAdo(void);

		void Start();

		
		_ConnectionPtr & GetConn() ;

		_ConnectionPtr m_pconn;
	};

}
