// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Evaluator.h"
#include <stdio.h>

#include "Graphic_resource.h"
#include "utils.h"

#include "VList.h"
#include "Evaluator.h"

   HMODULE gsEvaluator_hModule;
   char gsEvaluator_szModuleName[1024];
   BSTR gsEvaluator_wstrModuleName;
   ITypeInfo *pITypeInfo_IEvaluator;
   unsigned short gsEvaluator_wsVersionMajor = 1;
   unsigned short gsEvaluator_wsVersionMinor = 0;

   STDAPI gsEvaluator_DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppObject);

   class EvaluatorFactory : public IClassFactory {
   public:
 
      HRESULT STDMETHODCALLTYPE QueryInterface(const struct _GUID &,void **);
      unsigned long __stdcall AddRef();
      unsigned long __stdcall Release();
      HRESULT STDMETHODCALLTYPE CreateInstance(struct IUnknown *,const struct _GUID &,void **);
      HRESULT STDMETHODCALLTYPE LockServer(int);
 
      EvaluatorFactory() : refCount(0) {};
      ~EvaluatorFactory() {};
 
   private:
 
      int refCount;
 
   } evaluatorFactory;



   BOOL WINAPI gsEvaluator_dllAttach(HANDLE module) {

   gsEvaluator_hModule = reinterpret_cast<HMODULE>(module);

   GetModuleFileName(gsEvaluator_hModule,gsEvaluator_szModuleName,1024);

   long n;
   char* pszLibraryName = new char[n = strlen(gsEvaluator_szModuleName) + 16];
   memset(pszLibraryName,0,n);
   sprintf(pszLibraryName,"%s\\%d",gsEvaluator_szModuleName,EVALUATOR_TYPELIB_ID);

   gsEvaluator_wstrModuleName = SysAllocStringLen(NULL,n);     

   MultiByteToWideChar(CP_ACP, 0, pszLibraryName, -1, gsEvaluator_wstrModuleName,n);  

   delete [] pszLibraryName;

   ITypeLib *ptLib;

   LoadTypeLib(gsEvaluator_wstrModuleName,&ptLib);

   ptLib -> GetTypeInfoOfGuid(IID_IEvaluator,&pITypeInfo_IEvaluator);

   memset(gsEvaluator_wstrModuleName,0,n);
   MultiByteToWideChar(CP_ACP, 0, gsEvaluator_szModuleName, -1, gsEvaluator_wstrModuleName, strlen(gsEvaluator_szModuleName));  
   return TRUE;
   }


   BOOL WINAPI gsEvaluator_dllDetach() {
   SysFreeString(gsEvaluator_wstrModuleName);
   return 0;
   }


   STDAPI GSystemsEvaluatorDllGetClassObject(HMODULE hModule,REFCLSID clsid,REFIID riid,void** ppObject ) {
   gsEvaluator_dllAttach(hModule);
   return gsEvaluator_DllGetClassObject(clsid,riid,ppObject);
   }


   STDAPI gsEvaluator_DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppObject) {
   *ppObject = NULL;
   if ( rclsid != CLSID_Evaluator )
      return CLASS_E_CLASSNOTAVAILABLE;
   return evaluatorFactory.QueryInterface(riid,ppObject);
   }
 
 
   STDAPI gsEvaluator_DllRegisterServer() {
   utilsDllRegisterTypeLib(gsEvaluator_wstrModuleName);
   return utilsDllRegisterObject(CLSID_Evaluator,LIBID_Evaluator,gsEvaluator_hModule,
                                 gsEvaluator_szModuleName,
                                 "GSystems Evaluator Object",
                                 "GSystem.EvaluatorControl",
                                 "GSystem.EvaluatorControl.1",
                                 (CATID *)NULL,
                                 0,0,false,true,true);
   return S_OK;
   }
 
 
   STDAPI gsEvaluator_DllUnregisterServer() {
   utilsDllUnregisterObject(CLSID_Evaluator,"GSystem.EvaluatorControl","GSystem.EvaluatorControl.1");
   return utilsDllUnregisterTypeLib(gsEvaluator_wstrModuleName,LIBID_Evaluator,gsEvaluator_wsVersionMajor,gsEvaluator_wsVersionMinor);
   }
 
 
   HRESULT STDMETHODCALLTYPE EvaluatorFactory::QueryInterface(const struct _GUID &iid,void **ppv) {
   *ppv = NULL; 
   if ( iid != IID_IUnknown && iid != IID_IClassFactory && iid != IID_IEvaluator ) 
      return E_NOINTERFACE; 
   *ppv = this; 
   AddRef(); 
   return S_OK; 
   }
   unsigned long __stdcall EvaluatorFactory::AddRef() {return ++refCount;}
   unsigned long __stdcall EvaluatorFactory::Release() {return --refCount;}
 
 
   HRESULT STDMETHODCALLTYPE EvaluatorFactory::CreateInstance(struct IUnknown *pUnkOuter,const struct _GUID &riid,void **ppv) {
   HRESULT hr;
   *ppv = NULL;
   Evaluator *e = new Evaluator(pUnkOuter);
   hr = e -> QueryInterface(riid,ppv);
   if ( ! ppv ) delete e;
   return hr;
   }
 
 
   HRESULT STDMETHODCALLTYPE EvaluatorFactory::LockServer(int) {
   return 0;
   }
 