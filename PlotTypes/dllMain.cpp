// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "PlotTypes.h"

#include "utils.h"

   BOOL WINAPI DllMain(HANDLE module,ULONG flag,void *res) {

   switch(flag) {

   case DLL_PROCESS_ATTACH:

      hModule = reinterpret_cast<HMODULE>(module);
      GetModuleFileName(hModule,szModuleName,1024);
      break;

   case DLL_PROCESS_DETACH:
      break;

   default:
      break;
   }

   return TRUE;
   }


   STDAPI DllRegisterServer() {	

   utilsDllRegisterObject(CLSID_PlotTypes,LIBID_PlotTypes,hModule,szModuleName,"InnoVisioNate Plot Types Implementation Object","InnoVisioNate.PlotTypes","InnoVisioNate.PlotTypes.1",(CATID*)NULL,0,0,false,false,true);

   ICatRegister *pICatRegister;

   HRESULT rc = CoCreateInstance(CLSID_StdComponentCategoriesMgr,NULL,CLSCTX_ALL,IID_ICatRegister,reinterpret_cast<void **>(&pICatRegister));

   CATID categoryId = IID_IGSystemPlotType;

   pICatRegister -> RegisterClassImplCategories(CLSID_PlotTypes,1,&categoryId);

   pICatRegister -> Release();

   return S_OK;
   }


   STDAPI DllUnregisterServer() {
   utilsDllUnregisterObject(CLSID_PlotTypes,"InnoVisioNate.PlotTypes","InnoVisioNate.PlotTypes.1");
   return TRUE;
   }

  static class PlotTypesFactory : public IClassFactory {
  public:

     STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
     STDMETHOD_ (ULONG, AddRef)();
     STDMETHOD_ (ULONG, Release)();
     STDMETHOD (CreateInstance)(IUnknown *punkOuter, REFIID riid, void **ppv);
     STDMETHOD (LockServer)(BOOL fLock);

     PlotTypesFactory() : refCount(0) {};
     ~PlotTypesFactory() {};

  private:
     int refCount;
  } factory;


  STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppObject) {
  *ppObject = NULL;
  if ( rclsid == CLSID_PlotTypes )
     return factory.QueryInterface(riid,ppObject);
  return CLASS_E_CLASSNOTAVAILABLE;
  }



  long __stdcall PlotTypesFactory::QueryInterface(REFIID iid, void **ppv) { 
  *ppv = NULL; 
  if ( iid == IID_IUnknown || iid == IID_IClassFactory ) 
     *ppv = this; 
  else 
     return E_NOINTERFACE; 
  AddRef(); 
  return S_OK; 
  } 
 

  unsigned long __stdcall PlotTypesFactory::AddRef() { 
  return ++refCount; 
  } 


  unsigned long __stdcall PlotTypesFactory::Release() { 
  return --refCount;
  } 


  long __stdcall PlotTypesFactory::LockServer(int fLock) { 
  return S_OK; 
  } 


  HRESULT STDMETHODCALLTYPE PlotTypesFactory::CreateInstance(IUnknown *punkOuter, REFIID riid, void **ppv) { 
  HRESULT hres = E_NOINTERFACE;
  *ppv = NULL; 
  if ( punkOuter ) 
      return ResultFromScode(CLASS_E_NOAGGREGATION); 
  PlotTypes *pPlotTypes = new PlotTypes();
  hres = pPlotTypes -> QueryInterface(riid,ppv);
  if ( ! *ppv ) 
      delete pPlotTypes;
  return hres;
  } 