

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
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

#ifndef __atlcom_h__
#define __atlcom_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICTestAtl_FWD_DEFINED__
#define __ICTestAtl_FWD_DEFINED__
typedef interface ICTestAtl ICTestAtl;
#endif 	/* __ICTestAtl_FWD_DEFINED__ */


#ifndef ___ICTestAtlEvents_FWD_DEFINED__
#define ___ICTestAtlEvents_FWD_DEFINED__
typedef interface _ICTestAtlEvents _ICTestAtlEvents;
#endif 	/* ___ICTestAtlEvents_FWD_DEFINED__ */


#ifndef __CTestAtl_FWD_DEFINED__
#define __CTestAtl_FWD_DEFINED__

#ifdef __cplusplus
typedef class CTestAtl CTestAtl;
#else
typedef struct CTestAtl CTestAtl;
#endif /* __cplusplus */

#endif 	/* __CTestAtl_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __ICTestAtl_INTERFACE_DEFINED__
#define __ICTestAtl_INTERFACE_DEFINED__

/* interface ICTestAtl */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ICTestAtl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7AF59724-DCE4-471B-8750-F04656BE0264")
    ICTestAtl : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ICTestAtlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICTestAtl * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICTestAtl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICTestAtl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICTestAtl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICTestAtl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICTestAtl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICTestAtl * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ICTestAtlVtbl;

    interface ICTestAtl
    {
        CONST_VTBL struct ICTestAtlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICTestAtl_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICTestAtl_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICTestAtl_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICTestAtl_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICTestAtl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICTestAtl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICTestAtl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICTestAtl_INTERFACE_DEFINED__ */



#ifndef __atlcomLib_LIBRARY_DEFINED__
#define __atlcomLib_LIBRARY_DEFINED__

/* library atlcomLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_atlcomLib;

#ifndef ___ICTestAtlEvents_DISPINTERFACE_DEFINED__
#define ___ICTestAtlEvents_DISPINTERFACE_DEFINED__

/* dispinterface _ICTestAtlEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__ICTestAtlEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("4386704E-5F42-464A-9D29-51AB632FB242")
    _ICTestAtlEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _ICTestAtlEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _ICTestAtlEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _ICTestAtlEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _ICTestAtlEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _ICTestAtlEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _ICTestAtlEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _ICTestAtlEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _ICTestAtlEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _ICTestAtlEventsVtbl;

    interface _ICTestAtlEvents
    {
        CONST_VTBL struct _ICTestAtlEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ICTestAtlEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _ICTestAtlEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _ICTestAtlEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _ICTestAtlEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _ICTestAtlEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _ICTestAtlEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _ICTestAtlEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___ICTestAtlEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_CTestAtl;

#ifdef __cplusplus

class DECLSPEC_UUID("3C375FC9-0F07-4AD4-96F7-559A6C74FDAA")
CTestAtl;
#endif
#endif /* __atlcomLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


