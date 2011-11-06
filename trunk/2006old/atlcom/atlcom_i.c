

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Thu Aug 16 16:39:30 2007
 */
/* Compiler settings for .\atlcom.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
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

MIDL_DEFINE_GUID(IID, IID_ICTestAtl,0x7AF59724,0xDCE4,0x471B,0x87,0x50,0xF0,0x46,0x56,0xBE,0x02,0x64);


MIDL_DEFINE_GUID(IID, LIBID_atlcomLib,0x8CB8A7C2,0xF1DC,0x435C,0xA7,0xBC,0xC9,0x55,0x1D,0xFE,0x7B,0x7B);


MIDL_DEFINE_GUID(IID, DIID__ICTestAtlEvents,0x4386704E,0x5F42,0x464A,0x9D,0x29,0x51,0xAB,0x63,0x2F,0xB2,0x42);


MIDL_DEFINE_GUID(CLSID, CLSID_CTestAtl,0x3C375FC9,0x0F07,0x4AD4,0x96,0xF7,0x55,0x9A,0x6C,0x74,0xFD,0xAA);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



