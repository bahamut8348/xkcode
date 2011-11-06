#include "StdAfx.h"
#include "UserInstance.h"

UserInstance::UserInstance(  LPCTSTR tapname, DWORD pid)
:mTapGUID(tapname),
mMsgPID(pid)
{

}

UserInstance::~UserInstance(void)
{

}
