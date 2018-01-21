// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Text.h"

#include "utils.h"

#include "Properties_i.c"
#include "Text_i.c"
#include "Plot_i.c"
#include "DataSet_i.c"
#include "OpenGLImplementation_i.c"

#include "Variable_i.h"
#include "Evaluator_i.h"
#include "Evaluator_i.c"

  HMODULE hModule;
  char szModuleName[1024];
  BSTR wstrModuleName;
  ITypeInfo *pITypeInfo;

  STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppObject);

  BOOL WINAPI DllMain(HANDLE module,ULONG flag,void *res) {

  switch(flag) {
  case DLL_PROCESS_ATTACH:

     hModule = reinterpret_cast<HMODULE>(module);

     GetModuleFileName(hModule,szModuleName,1024);

     wstrModuleName = SysAllocStringLen(NULL,strlen(szModuleName) + 1);
     memset(wstrModuleName,0,(strlen(szModuleName) + 1) * sizeof(OLECHAR));
     MultiByteToWideChar(CP_ACP, 0, szModuleName, -1,wstrModuleName,strlen(szModuleName) + 1);  

     LoadLibraryA("RICHED32.DLL");

     ITypeLib *ptLib;
     LoadTypeLib(wstrModuleName,&ptLib);
     ptLib -> GetTypeInfoOfGuid(IID_IText,&pITypeInfo);

     break;

  case DLL_PROCESS_DETACH:
     SysFreeString(wstrModuleName);
//     OleUninitialize();
     break;

  default:
     break;
  }

  return TRUE;
  }

  STDAPI DllRegisterServer() {	
  return utilsDllRegisterObject(CLSID_Text,LIBID_Text,hModule,
                                 szModuleName,
                                 "GSystems Text Object",
                                 "GSystem.Text",
                                 "GSystem.Text.1",
                                 (CATID*)NULL,0,0,false,true,false);
  }


  STDAPI DllUnregisterServer() {
  return TRUE;
  }


  static class TextFactory : public IClassFactory {
  public:

     STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
     STDMETHOD_ (ULONG, AddRef)();
     STDMETHOD_ (ULONG, Release)();
     STDMETHOD (CreateInstance)(IUnknown *punkOuter, REFIID riid, void **ppv);
     STDMETHOD (LockServer)(BOOL fLock);

     TextFactory() : refCount(0) {};
     ~TextFactory() {};

  private:
     int refCount;
  } textFactory;


  STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppObject) {
  *ppObject = NULL;
  if ( rclsid == CLSID_Text )
     return textFactory.QueryInterface(riid,ppObject);
  return CLASS_E_CLASSNOTAVAILABLE;
  }



  long __stdcall TextFactory::QueryInterface(REFIID iid, void **ppv) { 
  *ppv = NULL; 
  if ( iid == IID_IUnknown || iid == IID_IClassFactory ) 
     *ppv = this; 
  else 
     return E_NOINTERFACE; 
  AddRef(); 
  return S_OK; 
  } 
 
  unsigned long __stdcall TextFactory::AddRef() { 
  return ++refCount; 
  } 

  unsigned long __stdcall TextFactory::Release() { 
  return --refCount;
  } 

  HRESULT STDMETHODCALLTYPE TextFactory::CreateInstance(IUnknown *punkOuter, REFIID riid, void **ppv) { 
  HRESULT hres;
  *ppv = NULL; 
  Text *t = new Text(punkOuter);
  hres = t -> QueryInterface(riid,ppv);
  if ( !*ppv ) delete t;
  return hres;
  } 

  long __stdcall TextFactory::LockServer(int fLock) { 
  return S_OK; 
  } 


