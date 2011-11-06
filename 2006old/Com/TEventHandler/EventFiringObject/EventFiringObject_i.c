

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Thu May 17 13:28:18 2007
 */
/* Compiler settings for .\EventFiringObject.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IEventFiringObject,0x8E396CC0,0xA266,0x481E,0xB6,0xB4,0x0C,0xB5,0x64,0xDA,0xA3,0xBC);


MIDL_DEFINE_GUID(IID, LIBID_EVENTFIRINGOBJECTLib,0x0B199966,0x76EA,0x479F,0x88,0x1D,0xED,0x7C,0xB5,0xF3,0x45,0x7B);


MIDL_DEFINE_GUID(IID, DIID__IEventFiringObjectEvents,0x32F2B52C,0x1C07,0x43BC,0x87,0x9B,0x04,0xC7,0x0A,0x7F,0xA1,0x48);


MIDL_DEFINE_GUID(CLSID, CLSID_EventFiringObject,0xA17BC235,0xA924,0x4FFE,0x8D,0x96,0x22,0x06,0x8C,0xEA,0x99,0x59);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



