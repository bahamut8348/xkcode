#include "StdAfx.h"
#include "DetoursWapper.h"



Detours::Detours(void)
{
	DetourRestoreAfterWith();
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
}

Detours::~Detours(void)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	for( u_int a =0 ; a < mFunTable.size(); a ++ )
	{
		FunTable & tab = mFunTable[a];
		DetourDetach( tab.mOldfun, tab.mNewFun );
	}
}


long Detours::Commit()
{
	return DetourTransactionCommit();
}

void Detours::Release()
{
	delete this;
}