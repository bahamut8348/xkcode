

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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

#ifndef __EventFiringObject_h__
#define __EventFiringObject_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IEventFiringObject_FWD_DEFINED__
#define __IEventFiringObject_FWD_DEFINED__
typedef interface IEventFiringObject IEventFiringObject;
#endif 	/* __IEventFiringObject_FWD_DEFINED__ */


#ifndef ___IEventFiringObjectEvents_FWD_DEFINED__
#define ___IEventFiringObjectEvents_FWD_DEFINED__
typedef interface _IEventFiringObjectEvents _IEventFiringObjectEvents;
#endif 	/* ___IEventFiringObjectEvents_FWD_DEFINED__ */


#ifndef __EventFiringObject_FWD_DEFINED__
#define __EventFiringObject_FWD_DEFINED__

#ifdef __cplusplus
typedef class EventFiringObject EventFiringObject;
#else
typedef struct EventFiringObject EventFiringObject;
#endif /* __cplusplus */

#endif 	/* __EventFiringObject_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IEventFiringObject_INTERFACE_DEFINED__
#define __IEventFiringObject_INTERFACE_DEFINED__

/* interface IEventFiringObject */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IEventFiringObject;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8E396CC0-A266-481E-B6B4-0CB564DAA3BC")
    IEventFiringObject : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE TestFunction( 
            /* [in] */ long lValue) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEventFiringObjectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEventFiringObject * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEventFiringObject * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEventFiringObject * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IEventFiringObject * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IEventFiringObject * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IEventFiringObject * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IEventFiringObject * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *TestFunction )( 
            IEventFiringObject * This,
            /* [in] */ long lValue);
        
        END_INTERFACE
    } IEventFiringObjectVtbl;

    interface IEventFiringObject
    {
        CONST_VTBL struct IEventFiringObjectVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEventFiringObject_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEventFiringObject_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEventFiringObject_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEventFiringObject_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IEventFiringObject_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IEventFiringObject_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IEventFiringObject_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IEventFiringObject_TestFunction(This,lValue)	\
    (This)->lpVtbl -> TestFunction(This,lValue)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IEventFiringObject_TestFunction_Proxy( 
    IEventFiringObject * This,
    /* [in] */ long lValue);


void __RPC_STUB IEventFiringObject_TestFunction_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEventFiringObject_INTERFACE_DEFINED__ */



#ifndef __EVENTFIRINGOBJECTLib_LIBRARY_DEFINED__
#define __EVENTFIRINGOBJECTLib_LIBRARY_DEFINED__

/* library EVENTFIRINGOBJECTLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_EVENTFIRINGOBJECTLib;

#ifndef ___IEventFiringObjectEvents_DISPINTERFACE_DEFINED__
#define ___IEventFiringObjectEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IEventFiringObjectEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IEventFiringObjectEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("32F2B52C-1C07-43BC-879B-04C70A7FA148")
    _IEventFiringObjectEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IEventFiringObjectEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IEventFiringObjectEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IEventFiringObjectEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IEventFiringObjectEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IEventFiringObjectEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IEventFiringObjectEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IEventFiringObjectEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IEventFiringObjectEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IEventFiringObjectEventsVtbl;

    interface _IEventFiringObjectEvents
    {
        CONST_VTBL struct _IEventFiringObjectEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IEventFiringObjectEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IEventFiringObjectEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IEventFiringObjectEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IEventFiringObjectEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IEventFiringObjectEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IEventFiringObjectEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IEventFiringObjectEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IEventFiringObjectEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_EventFiringObject;

#ifdef __cplusplus

class DECLSPEC_UUID("A17BC235-A924-4FFE-8D96-22068CEA9959")
EventFiringObject;
#endif
#endif /* __EVENTFIRINGOBJECTLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


