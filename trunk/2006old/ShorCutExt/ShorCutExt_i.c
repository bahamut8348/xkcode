

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Wed Aug 29 18:22:37 2007
 */
/* Compiler settings for .\ShorCutExt.idl:
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

MIDL_DEFINE_GUID(IID, IID_IMyFactory,0xBA359EC9,0xC3B3,0x490C,0xA0,0x06,0x2B,0x25,0xD7,0x37,0x27,0x34);


MIDL_DEFINE_GUID(IID, IID_IShellExt,0x5997762E,0x4E74,0x49A2,0xAD,0x57,0x21,0x0C,0xEF,0xA8,0xC5,0xE1);


MIDL_DEFINE_GUID(IID, LIBID_ShorCutExtLib,0x12D02135,0x0648,0x4493,0x8D,0xDE,0x49,0x63,0xAF,0xF5,0x55,0x3A);


MIDL_DEFINE_GUID(CLSID, CLSID_MyFactory,0xDD02782A,0xFF67,0x40C6,0x87,0x36,0x59,0x2B,0x5C,0xF6,0x6F,0x7B);


MIDL_DEFINE_GUID(CLSID, CLSID_ShellExt,0x95A21815,0x1753,0x4195,0xAF,0x6A,0xED,0xD2,0x6D,0x36,0x27,0x91);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



