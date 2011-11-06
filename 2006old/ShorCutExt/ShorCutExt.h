

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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

#ifndef __ShorCutExt_h__
#define __ShorCutExt_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IMyFactory_FWD_DEFINED__
#define __IMyFactory_FWD_DEFINED__
typedef interface IMyFactory IMyFactory;
#endif 	/* __IMyFactory_FWD_DEFINED__ */


#ifndef __IShellExt_FWD_DEFINED__
#define __IShellExt_FWD_DEFINED__
typedef interface IShellExt IShellExt;
#endif 	/* __IShellExt_FWD_DEFINED__ */


#ifndef __MyFactory_FWD_DEFINED__
#define __MyFactory_FWD_DEFINED__

#ifdef __cplusplus
typedef class MyFactory MyFactory;
#else
typedef struct MyFactory MyFactory;
#endif /* __cplusplus */

#endif 	/* __MyFactory_FWD_DEFINED__ */


#ifndef __ShellExt_FWD_DEFINED__
#define __ShellExt_FWD_DEFINED__

#ifdef __cplusplus
typedef class ShellExt ShellExt;
#else
typedef struct ShellExt ShellExt;
#endif /* __cplusplus */

#endif 	/* __ShellExt_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IMyFactory_INTERFACE_DEFINED__
#define __IMyFactory_INTERFACE_DEFINED__

/* interface IMyFactory */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IMyFactory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BA359EC9-C3B3-490C-A006-2B25D7372734")
    IMyFactory : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IMyFactoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMyFactory * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMyFactory * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMyFactory * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMyFactory * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMyFactory * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMyFactory * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMyFactory * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IMyFactoryVtbl;

    interface IMyFactory
    {
        CONST_VTBL struct IMyFactoryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMyFactory_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMyFactory_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMyFactory_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMyFactory_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMyFactory_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMyFactory_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMyFactory_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMyFactory_INTERFACE_DEFINED__ */


#ifndef __IShellExt_INTERFACE_DEFINED__
#define __IShellExt_INTERFACE_DEFINED__

/* interface IShellExt */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IShellExt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5997762E-4E74-49A2-AD57-210CEFA8C5E1")
    IShellExt : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IShellExtVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IShellExt * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IShellExt * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IShellExt * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IShellExt * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IShellExt * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IShellExt * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IShellExt * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IShellExtVtbl;

    interface IShellExt
    {
        CONST_VTBL struct IShellExtVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IShellExt_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IShellExt_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IShellExt_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IShellExt_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IShellExt_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IShellExt_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IShellExt_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IShellExt_INTERFACE_DEFINED__ */



#ifndef __ShorCutExtLib_LIBRARY_DEFINED__
#define __ShorCutExtLib_LIBRARY_DEFINED__

/* library ShorCutExtLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_ShorCutExtLib;

EXTERN_C const CLSID CLSID_MyFactory;

#ifdef __cplusplus

class DECLSPEC_UUID("DD02782A-FF67-40C6-8736-592B5CF66F7B")
MyFactory;
#endif

EXTERN_C const CLSID CLSID_ShellExt;

#ifdef __cplusplus

class DECLSPEC_UUID("95A21815-1753-4195-AF6A-EDD26D362791")
ShellExt;
#endif
#endif /* __ShorCutExtLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


