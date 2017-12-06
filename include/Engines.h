/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 3.01.75 */
/* at Sat Nov 14 17:52:05 1998
 */
/* Compiler settings for C:\prj\Engines\Engines.odl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: none
*/
//@@MIDL_FILE_HEADING(  )
#include "rpc.h"
#include "rpcndr.h"

#ifndef __Engines_h__
#define __Engines_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IDataEngineFactory_FWD_DEFINED__
#define __IDataEngineFactory_FWD_DEFINED__
typedef interface IDataEngineFactory IDataEngineFactory;
#endif 	/* __IDataEngineFactory_FWD_DEFINED__ */


#ifndef __DataEngineFactory_FWD_DEFINED__
#define __DataEngineFactory_FWD_DEFINED__

#ifdef __cplusplus
typedef class DataEngineFactory DataEngineFactory;
#else
typedef struct DataEngineFactory DataEngineFactory;
#endif /* __cplusplus */

#endif 	/* __DataEngineFactory_FWD_DEFINED__ */


#ifndef __IDataEngine_FWD_DEFINED__
#define __IDataEngine_FWD_DEFINED__
typedef interface IDataEngine IDataEngine;
#endif 	/* __IDataEngine_FWD_DEFINED__ */


#ifndef __DataEngine_FWD_DEFINED__
#define __DataEngine_FWD_DEFINED__

#ifdef __cplusplus
typedef class DataEngine DataEngine;
#else
typedef struct DataEngine DataEngine;
#endif /* __cplusplus */

#endif 	/* __DataEngine_FWD_DEFINED__ */


#ifndef __IFunctionEngineFactory_FWD_DEFINED__
#define __IFunctionEngineFactory_FWD_DEFINED__
typedef interface IFunctionEngineFactory IFunctionEngineFactory;
#endif 	/* __IFunctionEngineFactory_FWD_DEFINED__ */


#ifndef __FunctionEngineFactory_FWD_DEFINED__
#define __FunctionEngineFactory_FWD_DEFINED__

#ifdef __cplusplus
typedef class FunctionEngineFactory FunctionEngineFactory;
#else
typedef struct FunctionEngineFactory FunctionEngineFactory;
#endif /* __cplusplus */

#endif 	/* __FunctionEngineFactory_FWD_DEFINED__ */


#ifndef __IFunctionEngine_FWD_DEFINED__
#define __IFunctionEngine_FWD_DEFINED__
typedef interface IFunctionEngine IFunctionEngine;
#endif 	/* __IFunctionEngine_FWD_DEFINED__ */


#ifndef __FunctionEngine_FWD_DEFINED__
#define __FunctionEngine_FWD_DEFINED__

#ifdef __cplusplus
typedef class FunctionEngine FunctionEngine;
#else
typedef struct FunctionEngine FunctionEngine;
#endif /* __cplusplus */

#endif 	/* __FunctionEngine_FWD_DEFINED__ */


void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 


#ifndef __Engines_LIBRARY_DEFINED__
#define __Engines_LIBRARY_DEFINED__

/****************************************
 * Generated header for library: Engines
 * at Sat Nov 14 17:52:05 1998
 * using MIDL 3.01.75
 ****************************************/
/* [version][lcid][helpstring][helpfile][uuid] */ 



DEFINE_GUID(LIBID_Engines,0xFE5F67A0,0xAAD1,0x11d1,0x82,0xF8,0x00,0x00,0x00,0x00,0x00,0x00);


#ifndef __Engines_MODULE_DEFINED__
#define __Engines_MODULE_DEFINED__


/****************************************
 * Generated header for module: Engines
 * at Sat Nov 14 17:52:05 1998
 * using MIDL 3.01.75
 ****************************************/
/* [dllname] */ 


#endif /* __Engines_MODULE_DEFINED__ */

#ifndef __IDataEngineFactory_INTERFACE_DEFINED__
#define __IDataEngineFactory_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IDataEngineFactory
 * at Sat Nov 14 17:52:05 1998
 * using MIDL 3.01.75
 ****************************************/
/* [object][uuid] */ 



DEFINE_GUID(IID_IDataEngineFactory,0xFE5F67A4,0xAAD1,0x11d1,0x82,0xF8,0x00,0x00,0x00,0x00,0x00,0x00);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("FE5F67A4-AAD1-11d1-82F8-000000000000")
    IDataEngineFactory : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CreateInstance( 
            /* [in] */ IUnknown __RPC_FAR *punk,
            /* [in] */ GUID riid,
            /* [out][in] */ void __RPC_FAR *__RPC_FAR *newDataEngine) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE LockServer( 
            /* [in] */ int doLock) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDataEngineFactoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDataEngineFactory __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDataEngineFactory __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDataEngineFactory __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateInstance )( 
            IDataEngineFactory __RPC_FAR * This,
            /* [in] */ IUnknown __RPC_FAR *punk,
            /* [in] */ GUID riid,
            /* [out][in] */ void __RPC_FAR *__RPC_FAR *newDataEngine);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LockServer )( 
            IDataEngineFactory __RPC_FAR * This,
            /* [in] */ int doLock);
        
        END_INTERFACE
    } IDataEngineFactoryVtbl;

    interface IDataEngineFactory
    {
        CONST_VTBL struct IDataEngineFactoryVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDataEngineFactory_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDataEngineFactory_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDataEngineFactory_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDataEngineFactory_CreateInstance(This,punk,riid,newDataEngine)	\
    (This)->lpVtbl -> CreateInstance(This,punk,riid,newDataEngine)

#define IDataEngineFactory_LockServer(This,doLock)	\
    (This)->lpVtbl -> LockServer(This,doLock)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDataEngineFactory_CreateInstance_Proxy( 
    IDataEngineFactory __RPC_FAR * This,
    /* [in] */ IUnknown __RPC_FAR *punk,
    /* [in] */ GUID riid,
    /* [out][in] */ void __RPC_FAR *__RPC_FAR *newDataEngine);


void __RPC_STUB IDataEngineFactory_CreateInstance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDataEngineFactory_LockServer_Proxy( 
    IDataEngineFactory __RPC_FAR * This,
    /* [in] */ int doLock);


void __RPC_STUB IDataEngineFactory_LockServer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDataEngineFactory_INTERFACE_DEFINED__ */


#ifdef __cplusplus
DEFINE_GUID(CLSID_DataEngineFactory,0xFE5F67A3,0xAAD1,0x11d1,0x82,0xF8,0x00,0x00,0x00,0x00,0x00,0x00);

class DECLSPEC_UUID("FE5F67A3-AAD1-11d1-82F8-000000000000")
DataEngineFactory;
#endif

#ifndef __IDataEngine_INTERFACE_DEFINED__
#define __IDataEngine_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IDataEngine
 * at Sat Nov 14 17:52:05 1998
 * using MIDL 3.01.75
 ****************************************/
/* [object][dual][oleautomation][helpstring][uuid] */ 



DEFINE_GUID(IID_IDataEngine,0xFE5F67A2,0xAAD1,0x11d1,0x82,0xF8,0x00,0x00,0x00,0x00,0x00,0x00);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("FE5F67A2-AAD1-11d1-82F8-000000000000")
    IDataEngine : public IDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ long hwndOwner,
            /* [in] */ long x,
            /* [in] */ long y) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDataEngineVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDataEngine __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDataEngine __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDataEngine __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IDataEngine __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IDataEngine __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IDataEngine __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IDataEngine __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            IDataEngine __RPC_FAR * This,
            /* [in] */ long hwndOwner,
            /* [in] */ long x,
            /* [in] */ long y);
        
        END_INTERFACE
    } IDataEngineVtbl;

    interface IDataEngine
    {
        CONST_VTBL struct IDataEngineVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDataEngine_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDataEngine_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDataEngine_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDataEngine_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDataEngine_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDataEngine_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDataEngine_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDataEngine_Initialize(This,hwndOwner,x,y)	\
    (This)->lpVtbl -> Initialize(This,hwndOwner,x,y)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDataEngine_Initialize_Proxy( 
    IDataEngine __RPC_FAR * This,
    /* [in] */ long hwndOwner,
    /* [in] */ long x,
    /* [in] */ long y);


void __RPC_STUB IDataEngine_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDataEngine_INTERFACE_DEFINED__ */


#ifdef __cplusplus
DEFINE_GUID(CLSID_DataEngine,0xFE5F67A1,0xAAD1,0x11d1,0x82,0xF8,0x00,0x00,0x00,0x00,0x00,0x00);

class DECLSPEC_UUID("FE5F67A1-AAD1-11d1-82F8-000000000000")
DataEngine;
#endif

#ifndef __IFunctionEngineFactory_INTERFACE_DEFINED__
#define __IFunctionEngineFactory_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IFunctionEngineFactory
 * at Sat Nov 14 17:52:05 1998
 * using MIDL 3.01.75
 ****************************************/
/* [object][uuid] */ 



DEFINE_GUID(IID_IFunctionEngineFactory,0xFE5F67A8,0xAAD1,0x11d1,0x82,0xF8,0x00,0x00,0x00,0x00,0x00,0x00);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("FE5F67A8-AAD1-11d1-82F8-000000000000")
    IFunctionEngineFactory : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CreateInstance( 
            /* [in] */ IUnknown __RPC_FAR *punk,
            /* [in] */ GUID riid,
            /* [out][in] */ void __RPC_FAR *__RPC_FAR *newFunctionEngine) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE LockServer( 
            /* [in] */ int doLock) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IFunctionEngineFactoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IFunctionEngineFactory __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IFunctionEngineFactory __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IFunctionEngineFactory __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateInstance )( 
            IFunctionEngineFactory __RPC_FAR * This,
            /* [in] */ IUnknown __RPC_FAR *punk,
            /* [in] */ GUID riid,
            /* [out][in] */ void __RPC_FAR *__RPC_FAR *newFunctionEngine);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LockServer )( 
            IFunctionEngineFactory __RPC_FAR * This,
            /* [in] */ int doLock);
        
        END_INTERFACE
    } IFunctionEngineFactoryVtbl;

    interface IFunctionEngineFactory
    {
        CONST_VTBL struct IFunctionEngineFactoryVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFunctionEngineFactory_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFunctionEngineFactory_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFunctionEngineFactory_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IFunctionEngineFactory_CreateInstance(This,punk,riid,newFunctionEngine)	\
    (This)->lpVtbl -> CreateInstance(This,punk,riid,newFunctionEngine)

#define IFunctionEngineFactory_LockServer(This,doLock)	\
    (This)->lpVtbl -> LockServer(This,doLock)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IFunctionEngineFactory_CreateInstance_Proxy( 
    IFunctionEngineFactory __RPC_FAR * This,
    /* [in] */ IUnknown __RPC_FAR *punk,
    /* [in] */ GUID riid,
    /* [out][in] */ void __RPC_FAR *__RPC_FAR *newFunctionEngine);


void __RPC_STUB IFunctionEngineFactory_CreateInstance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFunctionEngineFactory_LockServer_Proxy( 
    IFunctionEngineFactory __RPC_FAR * This,
    /* [in] */ int doLock);


void __RPC_STUB IFunctionEngineFactory_LockServer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IFunctionEngineFactory_INTERFACE_DEFINED__ */


#ifdef __cplusplus
DEFINE_GUID(CLSID_FunctionEngineFactory,0xFE5F67A7,0xAAD1,0x11d1,0x82,0xF8,0x00,0x00,0x00,0x00,0x00,0x00);

class DECLSPEC_UUID("FE5F67A7-AAD1-11d1-82F8-000000000000")
FunctionEngineFactory;
#endif

#ifndef __IFunctionEngine_INTERFACE_DEFINED__
#define __IFunctionEngine_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IFunctionEngine
 * at Sat Nov 14 17:52:05 1998
 * using MIDL 3.01.75
 ****************************************/
/* [object][dual][oleautomation][helpstring][uuid] */ 



DEFINE_GUID(IID_IFunctionEngine,0xFE5F67A6,0xAAD1,0x11d1,0x82,0xF8,0x00,0x00,0x00,0x00,0x00,0x00);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("FE5F67A6-AAD1-11d1-82F8-000000000000")
    IFunctionEngine : public IDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ long hwndOwner,
            /* [in] */ long x,
            /* [in] */ long y) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IFunctionEngineVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IFunctionEngine __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IFunctionEngine __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IFunctionEngine __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IFunctionEngine __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IFunctionEngine __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IFunctionEngine __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IFunctionEngine __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            IFunctionEngine __RPC_FAR * This,
            /* [in] */ long hwndOwner,
            /* [in] */ long x,
            /* [in] */ long y);
        
        END_INTERFACE
    } IFunctionEngineVtbl;

    interface IFunctionEngine
    {
        CONST_VTBL struct IFunctionEngineVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFunctionEngine_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFunctionEngine_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFunctionEngine_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IFunctionEngine_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IFunctionEngine_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IFunctionEngine_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IFunctionEngine_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IFunctionEngine_Initialize(This,hwndOwner,x,y)	\
    (This)->lpVtbl -> Initialize(This,hwndOwner,x,y)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IFunctionEngine_Initialize_Proxy( 
    IFunctionEngine __RPC_FAR * This,
    /* [in] */ long hwndOwner,
    /* [in] */ long x,
    /* [in] */ long y);


void __RPC_STUB IFunctionEngine_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IFunctionEngine_INTERFACE_DEFINED__ */


#ifdef __cplusplus
DEFINE_GUID(CLSID_FunctionEngine,0xFE5F67A5,0xAAD1,0x11d1,0x82,0xF8,0x00,0x00,0x00,0x00,0x00,0x00);

class DECLSPEC_UUID("FE5F67A5-AAD1-11d1-82F8-000000000000")
FunctionEngine;
#endif
#endif /* __Engines_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
