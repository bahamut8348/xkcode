// PSNetwork.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "StructDef.h"
#include "PSNetwork.h"
#include "NetworkSession.h"
#include "NetWorkManager.h"
#include "NetworkConnection.h"



BOOST_PYTHON_MODULE(PSNetwork)
{

	Py_Initialize();
	PyEval_InitThreads();
	
	PSNetwork::InitNetworkSession();
	PSNetwork::InitNetworkManager();
	PSNetwork::InitNetworkConnection();
	PSNetwork::InitStruct();

	def("str2addr", PSNetwork::str2addr);
}








BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	   return TRUE;
}
