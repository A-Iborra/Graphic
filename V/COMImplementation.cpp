// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "V.h"

#include "utils.h"

   HMODULE gsVariables_hModule = NULL;
   char gsVariables_szModuleName[1024];
   BSTR gsVariables_wstrModuleName;
   ITypeInfo *pITypeInfo_IVariable;

   long gsVariables_oleMisc = OLEMISC_SETCLIENTSITEFIRST | OLEMISC_INVISIBLEATRUNTIME | OLEMISC_INSIDEOUT | OLEMISC_RECOMPOSEONRESIZE | OLEMISC_CANTLINKINSIDE;
   unsigned short gsVariables_wsVersionMajor = 1;
   unsigned short gsVariables_wsVersionMinor = 0;

   STDAPI gsVariables_DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppObject);

   class VariableFactory : public IClassFactory {
   public:
 
      HRESULT STDMETHODCALLTYPE QueryInterface(const struct _GUID &,void **);
      unsigned long __stdcall AddRef();
      unsigned long __stdcall Release();
      HRESULT STDMETHODCALLTYPE CreateInstance(struct IUnknown *,const struct _GUID &,void **);
      HRESULT STDMETHODCALLTYPE LockServer(int);
 
      VariableFactory() : refCount(0) {};
      ~VariableFactory() {};
 
   private:
 
      int refCount;

   } variableFactory;


   STDAPI GSystemsVariablesDllGetClassObject(HMODULE hModule,REFCLSID clsid,REFIID riid,void** ppObject ) {
   gsVariables_hModule = hModule;
   return gsVariables_DllGetClassObject(clsid,riid,ppObject);
   }


   STDAPI gsVariables_DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppObject) {
   *ppObject = NULL;
   if ( rclsid != CLSID_Variable)
      return CLASS_E_CLASSNOTAVAILABLE;
   return variableFactory.QueryInterface(riid,ppObject);
   }
 
 
   STDAPI gsVariables_DllRegisterServer() {
   utilsDllRegisterTypeLib(gsVariables_wstrModuleName);
   return utilsDllRegisterObject(CLSID_Variable,LIBID_Variable,gsVariables_hModule,
                                  gsVariables_szModuleName,"GSystem Variable Object","GSystem.Variable","GSystem.Variable.1",
                                  (CATID*)NULL,0,gsVariables_oleMisc,false,false,false);
   }
 
 
   STDAPI gsVariables_DllUnregisterServer() {
   utilsDllUnregisterObject(CLSID_Variable,"GSystem Variable Object","GSystem.Variable");
   return utilsDllUnregisterTypeLib(gsVariables_wstrModuleName,LIBID_Variable,gsVariables_wsVersionMajor,gsVariables_wsVersionMinor);
   }
 
 
   HRESULT STDMETHODCALLTYPE VariableFactory::QueryInterface(const struct _GUID &iid,void **ppv) {
   *ppv = NULL; 
   if ( iid != IID_IUnknown && iid != IID_IClassFactory ) 
      return E_NOINTERFACE; 
   *ppv = this; 
   AddRef(); 
   return S_OK; 
   }
   unsigned long __stdcall VariableFactory::AddRef() {return ++refCount;}
   unsigned long __stdcall VariableFactory::Release() {return --refCount;}
 
 
   HRESULT STDMETHODCALLTYPE VariableFactory::CreateInstance(IUnknown *pUnkOuter,const struct _GUID &riid,void **ppv) {
   HRESULT hr;
   *ppv = NULL;
   if ( pUnkOuter ) return CLASS_E_NOAGGREGATION;
   V *p = new V();
   hr = p -> QueryInterface(riid,ppv);
   if ( ! ppv ) delete p;
   return hr;
   }
 
 
   HRESULT STDMETHODCALLTYPE VariableFactory::LockServer(int) {
   return 0;
   }
