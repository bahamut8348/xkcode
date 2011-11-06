// SQlServer.h

#pragma once

using namespace System;
using namespace System::Data;
using namespace System::Data::Sql;
using namespace System::Data::SqlTypes;
using namespace Microsoft::SqlServer::Server;

namespace SQlServer {

	public ref class SQlServerClass
	{
	public:
		[Microsoft::SqlServer::Server::SqlProcedure] static void MyStoredProc();
	};
}
