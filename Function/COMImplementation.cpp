// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Function.h"

#include "Graphic_resource.h"

#include "VList.h"

#include "Function_i.c"
#include "Variable_i.c"

#include "Evaluator_i.c"
#include "Properties_i.c"

#include "Plot_i.c"
#include "DataSet_i.c"

   HMODULE hModule;
   char szModuleName[1024];
   BSTR wstrModuleName;
   ITypeInfo *pITypeInfo_IGSFunctioNater;
   ITypeInfo *pITypeInfo_IGSFunctioNaterEvents;
   ITypeInfo* pIEvaluatorEventsTypeInfo;

   unsigned short wsVersionMajor = 1;
   unsigned short wsVersionMinor = 0;
 
   STDAPI GSystemsVariablesDllGetClassObject(HMODULE hModule,REFCLSID clsid,REFIID riid,void** ppObject);

#ifdef FUNCTION_SAMPLE

   bool trialExpired = FALSE;

#endif

   BOOL WINAPI DllMain(HANDLE module,ULONG flag,void *) {

   switch(flag) {

   case DLL_PROCESS_ATTACH: {

      hModule = reinterpret_cast<HMODULE>(module);

      memset(szModuleName,0,1024);

      GetModuleFileName(hModule,szModuleName,1024);

      ITypeLib *ptLib;
      {
      char szLibraryName[1024];
      memset(szLibraryName,0,1024);
      sprintf(szLibraryName,"%s\\%d",szModuleName,FUNCTION_TYPELIB_ID);

      BSTR bstrLibraryName = SysAllocStringLen(NULL,strlen(szLibraryName) + 1);

      MultiByteToWideChar(CP_ACP,0,szLibraryName,-1,bstrLibraryName,strlen(szLibraryName) + 1);  
 
      LoadTypeLib(bstrLibraryName,&ptLib);
      SysFreeString(bstrLibraryName);
      }

      ptLib -> GetTypeInfoOfGuid(IID_IGSFunctioNater,&pITypeInfo_IGSFunctioNater);
      ptLib -> GetTypeInfoOfGuid(DIID_IGSFunctioNaterEvents,&pITypeInfo_IGSFunctioNaterEvents);

      {
      char szEvaluatorLib[1026];
      sprintf(szEvaluatorLib,"%s\\%d",szModuleName,EVALUATOR_TYPELIB_ID);
      BSTR bstrEvaluatorLib = SysAllocStringLen(NULL,strlen(szEvaluatorLib) + 1);
      memset(bstrEvaluatorLib,0,(strlen(szEvaluatorLib) + 1) * sizeof(OLECHAR));
      MultiByteToWideChar(CP_ACP,0,szEvaluatorLib,-1,bstrEvaluatorLib,strlen(szEvaluatorLib) + 1);

      LoadTypeLib(bstrEvaluatorLib,&ptLib);
      ptLib -> GetTypeInfoOfGuid(IID_IEvaluatorEvents,&pIEvaluatorEventsTypeInfo);
      SysFreeString(bstrEvaluatorLib);
      }

      wstrModuleName = SysAllocStringLen(NULL,strlen(szModuleName) + 1);
      memset(wstrModuleName,0,(strlen(szModuleName) + 1) * sizeof(OLECHAR));
      MultiByteToWideChar(CP_ACP, 0, szModuleName, -1, wstrModuleName, strlen(szModuleName) + 1);  

      }
      break;
 
   case DLL_PROCESS_DETACH:
 
      SysFreeString(wstrModuleName);

      break;
 
   default:
      break;
   }
   return TRUE;
   }
 
 
 
   class FunctionFactory : public IClassFactory {
   public:
 
      HRESULT STDMETHODCALLTYPE QueryInterface(const struct _GUID &,void **);
      unsigned long __stdcall AddRef();
      unsigned long __stdcall Release();
      HRESULT STDMETHODCALLTYPE CreateInstance(struct IUnknown *,const struct _GUID &,void **);
      HRESULT STDMETHODCALLTYPE LockServer(int);
 
      FunctionFactory() : refCount(0) {};
      ~FunctionFactory() {};
 
   private:
 
      int refCount;
 
   } functionFactory;
 
 

   STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppObject) {
   *ppObject = NULL;
   if ( rclsid !=  CLSID_GSystemFunctioNater)
      return CLASS_E_CLASSNOTAVAILABLE;
   return functionFactory.QueryInterface(riid,ppObject);
   }
 
 
 
   STDAPI DllRegisterServer() {

   HKEY hKey;                             
   DWORD dwDisposition;

   utilsDllRegisterTypeLib(wstrModuleName);

   GUID categoryIDs[6];
   categoryIDs[0] = CATID_PersistsToFile;
   categoryIDs[1] = CATID_PersistsToStorage;
   categoryIDs[2] = CATID_PersistsToStream;
   categoryIDs[3] = CATID_PersistsToStreamInit;
   categoryIDs[4] = CATID_PersistsToPropertyBag;
   categoryIDs[5] = CATID_NULL;

   return utilsDllRegisterObject(CLSID_GSystemFunctioNater,LIBID_FunctioNater,hModule,
                                  szModuleName,
                                  "InnoVisioNate FunctioNater Object",
                                  "InnoVisioNate.FunctioNaterControl",
                                  "InnoVisioNate.FunctioNaterControl.1",
                                  categoryIDs,
                                  IDOCXBITMAP_FUNCTION,oleMiscStatus,
                                  true,true,true);
   }
 
 
   STDAPI DllUnregisterServer() {
   utilsDllUnregisterObject(CLSID_GSystemFunctioNater,"InnoVisioNate.FunctioNaterControl","InnoVisioNate.FunctioNaterControl.1");
   return utilsDllUnregisterTypeLib(wstrModuleName,LIBID_FunctioNater,wsVersionMajor,wsVersionMinor);
   }
 
 
   HRESULT STDMETHODCALLTYPE FunctionFactory::QueryInterface(const struct _GUID &iid,void **ppv) {
   *ppv = NULL; 
   if ( iid != IID_IUnknown && iid != IID_IClassFactory && iid != IID_IGSFunctioNater ) 
      return E_NOINTERFACE; 
   *ppv = this; 
   AddRef(); 
   return S_OK; 
   }
   unsigned long __stdcall FunctionFactory::AddRef() {return ++refCount;}
   unsigned long __stdcall FunctionFactory::Release() {return --refCount;}
 
 
   HRESULT STDMETHODCALLTYPE FunctionFactory::CreateInstance(struct IUnknown *pUnkOuter,const struct _GUID &riid,void **ppv) {

   HRESULT hr = S_OK;
   *ppv = NULL;
 
   Function *f = new Function(pUnkOuter);
   hr = f -> QueryInterface(riid,ppv);
 
   if ( ! ppv ) 
      delete f;
 
   return hr;
   }
 
 
   HRESULT STDMETHODCALLTYPE FunctionFactory::LockServer(int) {
   return 0;
   }
