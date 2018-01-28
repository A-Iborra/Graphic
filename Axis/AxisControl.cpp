// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Axis.h"

#include "utils.h"

#include "Axis_i.c"
#include "Text_i.c"
#include "Properties_i.c"

#include "Variable_i.h"
#include "Evaluator_i.h"
#include "Evaluator_i.c"

  HMODULE hModule;
  char szModuleName[1024];
  BSTR wstrModuleName;
  ITypeInfo *pITypeInfo;

  unsigned short wsVersionMajor = 1;
  unsigned short wsVersionMinor = 0;

  STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppObject);

  BOOL WINAPI DllMain(HANDLE module,ULONG flag,void *res) {

  switch(flag) {
  case DLL_PROCESS_ATTACH:

     hModule = reinterpret_cast<HMODULE>(module);

     GetModuleFileName(hModule,szModuleName,1024);
     wstrModuleName = SysAllocStringLen(NULL,(DWORD)strlen(szModuleName) + 1);
     memset(wstrModuleName,0,((DWORD)strlen(szModuleName) + 1) * sizeof(OLECHAR));
     MultiByteToWideChar(CP_ACP, 0, szModuleName, -1,wstrModuleName,(DWORD)strlen(szModuleName) + 1);  

     ITypeLib *ptLib;
     LoadTypeLib(wstrModuleName,&ptLib);
     ptLib -> GetTypeInfoOfGuid(IID_IAxis,&pITypeInfo);

     break;

  case DLL_PROCESS_DETACH:
//     OleUninitialize();
     break;

  default:
     break;
  }

  return TRUE;
  }


  class AxisFactory : public IClassFactory {
  public:

     STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
     STDMETHOD_ (ULONG, AddRef)();
     STDMETHOD_ (ULONG, Release)();
     STDMETHOD (CreateInstance)(IUnknown *punkOuter, REFIID riid, void **ppv);
     STDMETHOD (LockServer)(BOOL fLock);

     AxisFactory() : refCount(0) {};
     ~AxisFactory() {};

  private:
     int refCount;
  };


  static AxisFactory AxisFactory;

  STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppObject) {
  *ppObject = NULL;
  if ( rclsid != CLSID_GSystemAxis ) 
     return CLASS_E_CLASSNOTAVAILABLE;
  return AxisFactory.QueryInterface(riid,ppObject);
  }



  STDAPI DllRegisterServer() {	
  utilsDllRegisterTypeLib(wstrModuleName);
  return utilsDllRegisterObject(CLSID_GSystemAxis,LIBID_Axis,hModule,
                                 szModuleName,
                                 "InnoVisioNate Axis Object",
                                 "InnoVisioNate.Axis",
                                 "InnoVisioNate.Axis.1",
                                 (CATID*)NULL,0,0,false,true,false);
  }


  STDAPI DllUnregisterServer() {
  utilsDllUnregisterObject(CLSID_GSystemAxis,"InnoVisioNate.Axis","InnoVisioNate.Axis.1");
  return utilsDllUnregisterTypeLib(wstrModuleName,LIBID_Axis,wsVersionMajor,wsVersionMinor);
  }


  long __stdcall AxisFactory::QueryInterface(REFIID iid, void **ppv) { 
  *ppv = NULL; 
  if ( iid == IID_IUnknown || iid == IID_IClassFactory ) 
     *ppv = this; 
  else 
     return E_NOINTERFACE; 
  AddRef(); 
  return S_OK; 
  } 
  unsigned long __stdcall AxisFactory::AddRef() { 
  return ++refCount; 
  } 
  unsigned long __stdcall AxisFactory::Release() { 
  return --refCount;
  } 


  HRESULT STDMETHODCALLTYPE AxisFactory::CreateInstance(IUnknown *punkOuter, REFIID riid, void **ppv) { 
  HRESULT hres;
  *ppv = NULL; 
  Axis *pa = new Axis(punkOuter);
  hres = pa -> QueryInterface(riid,ppv);
  if ( !*ppv ) delete pa;
  return hres;
  } 


  long __stdcall AxisFactory::LockServer(int fLock) { 
  return S_OK; 
  } 
