

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Wed Jul 27 01:14:31 2011
 */
/* Compiler settings for .\ShortCutExt.idl:
    Oicf, W1, Zp8, env=Win64 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "ShortCutExt.h"

#define TYPE_FORMAT_STRING_SIZE   3                                 
#define PROC_FORMAT_STRING_SIZE   1                                 
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _ShortCutExt_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } ShortCutExt_MIDL_TYPE_FORMAT_STRING;

typedef struct _ShortCutExt_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } ShortCutExt_MIDL_PROC_FORMAT_STRING;

typedef struct _ShortCutExt_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } ShortCutExt_MIDL_EXPR_FORMAT_STRING;


static RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const ShortCutExt_MIDL_TYPE_FORMAT_STRING ShortCutExt__MIDL_TypeFormatString;
extern const ShortCutExt_MIDL_PROC_FORMAT_STRING ShortCutExt__MIDL_ProcFormatString;
extern const ShortCutExt_MIDL_EXPR_FORMAT_STRING ShortCutExt__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IShortCut_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IShortCut_ProxyInfo;



#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif

static const ShortCutExt_MIDL_PROC_FORMAT_STRING ShortCutExt__MIDL_ProcFormatString =
    {
        0,
        {

			0x0
        }
    };

static const ShortCutExt_MIDL_TYPE_FORMAT_STRING ShortCutExt__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */

			0x0
        }
    };


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IShortCut, ver. 0.0,
   GUID={0x53355447,0x4EEA,0x458B,{0xBB,0x79,0x81,0x15,0x14,0x0A,0xFA,0x50}} */

#pragma code_seg(".orpc")
static const unsigned short IShortCut_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IShortCut_ProxyInfo =
    {
    &Object_StubDesc,
    ShortCutExt__MIDL_ProcFormatString.Format,
    &IShortCut_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IShortCut_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    ShortCutExt__MIDL_ProcFormatString.Format,
    &IShortCut_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(7) _IShortCutProxyVtbl = 
{
    0,
    &IID_IShortCut,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */
};


static const PRPC_STUB_FUNCTION IShortCut_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION
};

CInterfaceStubVtbl _IShortCutStubVtbl =
{
    &IID_IShortCut,
    &IShortCut_ServerInfo,
    7,
    &IShortCut_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    ShortCutExt__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x70001f4, /* MIDL Version 7.0.500 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };

const CInterfaceProxyVtbl * _ShortCutExt_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IShortCutProxyVtbl,
    0
};

const CInterfaceStubVtbl * _ShortCutExt_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IShortCutStubVtbl,
    0
};

PCInterfaceName const _ShortCutExt_InterfaceNamesList[] = 
{
    "IShortCut",
    0
};

const IID *  _ShortCutExt_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define _ShortCutExt_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _ShortCutExt, pIID, n)

int __stdcall _ShortCutExt_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_ShortCutExt_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo ShortCutExt_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _ShortCutExt_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _ShortCutExt_StubVtblList,
    (const PCInterfaceName * ) & _ShortCutExt_InterfaceNamesList,
    (const IID ** ) & _ShortCutExt_BaseIIDList,
    & _ShortCutExt_IID_Lookup, 
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* defined(_M_AMD64)*/

