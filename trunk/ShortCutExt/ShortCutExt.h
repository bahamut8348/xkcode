

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __ShortCutExt_h__
#define __ShortCutExt_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IShortCut_FWD_DEFINED__
#define __IShortCut_FWD_DEFINED__
typedef interface IShortCut IShortCut;
#endif 	/* __IShortCut_FWD_DEFINED__ */


#ifndef __ShortCut_FWD_DEFINED__
#define __ShortCut_FWD_DEFINED__

#ifdef __cplusplus
typedef class ShortCut ShortCut;
#else
typedef struct ShortCut ShortCut;
#endif /* __cplusplus */

#endif 	/* __ShortCut_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IShortCut_INTERFACE_DEFINED__
#define __IShortCut_INTERFACE_DEFINED__

/* interface IShortCut */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IShortCut;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("53355447-4EEA-458B-BB79-8115140AFA50")
    IShortCut : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IShortCutVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IShortCut * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IShortCut * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IShortCut * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IShortCut * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IShortCut * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IShortCut * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IShortCut * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IShortCutVtbl;

    interface IShortCut
    {
        CONST_VTBL struct IShortCutVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IShortCut_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IShortCut_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IShortCut_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IShortCut_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IShortCut_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IShortCut_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IShortCut_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IShortCut_INTERFACE_DEFINED__ */



#ifndef __ShortCutExtLib_LIBRARY_DEFINED__
#define __ShortCutExtLib_LIBRARY_DEFINED__

/* library ShortCutExtLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_ShortCutExtLib;

EXTERN_C const CLSID CLSID_ShortCut;

#ifdef __cplusplus

class DECLSPEC_UUID("A81EBE8C-831C-4D06-AAC7-CA605822026E")
ShortCut;
#endif
#endif /* __ShortCutExtLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


