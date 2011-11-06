// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <iostream>
#include <tchar.h>
#include "windows.h"

#include <AtlBase.h>
#include <AtlConv.h>

#include <ATLComTime.h>

#pragma setlocale("chs")

// TODO: reference additional headers your program requires here
#import "c:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF", "adoEOF")
#pragma setlocale("chs")

#include "Ado.h"

