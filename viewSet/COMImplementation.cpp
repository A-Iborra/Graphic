// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ViewSet.h"

#include "utils.h"

#include "list.cpp"

#include "ViewSet_i.c"
#include "Properties_i.c"
#include "DataSet_i.c"
#include "Plot_i.c"
#include "Text_i.c"

#include "Variable_i.h"
#include "Evaluator_i.h"
#include "Evaluator_i.c"

#include "GraphicControl_i.c"

  HMODULE hModule;
  char szModuleName[1024];
  OLECHAR wstrModuleName[1024];

  STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppObject);

  BOOL WINAPI DllMain(HANDLE module,ULONG flag,void *res) {

  switch(flag) {
  case DLL_PROCESS_ATTACH:
     hModule = reinterpret_cast<HMODULE>(module);
     GetModuleFileName(hModule,szModuleName,1024);
     MultiByteToWideChar(CP_ACP, 0, szModuleName, -1, wstrModuleName, (DWORD)strlen(szModuleName));  
     break;

  case DLL_PROCESS_DETACH:
//     OleUninitialize();
     break;

  default:
     break;
  }

  return TRUE;
  }


  class ViewSetFactory : public IClassFactory {
  public:

     STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
     STDMETHOD_ (ULONG, AddRef)();
     STDMETHOD_ (ULONG, Release)();
     STDMETHOD (CreateInstance)(IUnknown *punkOuter, REFIID riid, void **ppv);
     STDMETHOD (LockServer)(BOOL fLock);

     ViewSetFactory() : refCount(0) {};
     ~ViewSetFactory() {};

  private:
     int refCount;
  };


  static ViewSetFactory vsFactory;

  STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppObject) {
  *ppObject = NULL;
  if ( rclsid != CLSID_ViewSet ) 
     return CLASS_E_CLASSNOTAVAILABLE;
  return vsFactory.QueryInterface(riid,ppObject);
  }


  STDAPI DllRegisterServer() {	
  return utilsDllRegisterObject(CLSID_ViewSet,LIBID_ViewSet,hModule,szModuleName,"InnoVisioNate ViewSet Object","InnoVisioNate.ViewSet","InnoVisioNate.ViewSet.1",(CATID*)NULL,0,0,false,true,true);
  }


  STDAPI DllUnregisterServer() {
  return TRUE;
  }


  long __stdcall ViewSetFactory::QueryInterface(REFIID iid, void **ppv) { 
  *ppv = NULL; 
  if ( iid == IID_IUnknown || iid == IID_IClassFactory ) 
     *ppv = this; 
  else 
     return E_NOINTERFACE; 
  AddRef(); 
  return S_OK; 
  } 
  unsigned long __stdcall ViewSetFactory::AddRef() { 
  return ++refCount; 
  } 
  unsigned long __stdcall ViewSetFactory::Release() { 
  return --refCount;
  } 


  HRESULT STDMETHODCALLTYPE ViewSetFactory::CreateInstance(IUnknown *punkOuter, REFIID riid, void **ppv) { 
  HRESULT hres;
  *ppv = NULL; 
  ViewSet *pvs = new ViewSet(punkOuter);
  hres = pvs -> QueryInterface(riid,ppv);
  if ( ! ppv ) delete pvs;
  return hres;
  } 


  long __stdcall ViewSetFactory::LockServer(int fLock) { 
  return S_OK; 
  }