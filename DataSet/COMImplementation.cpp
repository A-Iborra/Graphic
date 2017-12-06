/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <olectl.h>
#include <stdio.h>

#include "Graphic_resource.h"
#include "utils.h"

#include "GSystem_i.h"
#include "Properties_i.h"

#include "DataSet_i.h"

#include "DataSet.h"

#include "DataSet_i.c"
#include "Properties_i.c"

#include "Variable_i.h"
#include "Evaluator_i.h"
#include "Evaluator_i.c"

  HMODULE hModule;
  char szModuleName[1024];
  OLECHAR wstrModuleName[1024];

  STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppObject);

  BOOL WINAPI DllMain(HANDLE module,ULONG flag,void *res) {

  switch(flag) {
  case DLL_PROCESS_ATTACH:

     hModule = reinterpret_cast<HMODULE>(module);
     GetModuleFileName(hModule,szModuleName,1024);
     memset(wstrModuleName,0,sizeof(wstrModuleName));
     MultiByteToWideChar(CP_ACP, 0, szModuleName, -1, wstrModuleName, strlen(szModuleName));  
     break;

  case DLL_PROCESS_DETACH:
//     OleUninitialize();
     break;

  default:
     break;
  }

  return TRUE;
  }


  class DataSetFactory : public IClassFactory {
  public:

     STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
     STDMETHOD_ (ULONG, AddRef)();
     STDMETHOD_ (ULONG, Release)();
     STDMETHOD (CreateInstance)(IUnknown *punkOuter, REFIID riid, void **ppv);
     STDMETHOD (LockServer)(BOOL fLock);

     DataSetFactory() : refCount(0) {};
     ~DataSetFactory() {};

  private:
     int refCount;
  };


  static DataSetFactory DataSetFactory;

  STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppObject) {
  *ppObject = NULL;
  if ( rclsid != CLSID_DataSet ) 
     return CLASS_E_CLASSNOTAVAILABLE;
  return DataSetFactory.QueryInterface(riid,ppObject);
  }



  STDAPI DllRegisterServer() {	
  return utilsDllRegisterObject(CLSID_DataSet,LIBID_DataSet,hModule,
                                 szModuleName,
                                 "GSystem DataSet Object",
                                 "GSystem.DataSet",
                                 "GSystem.DataSet.1",
                                 (CATID*)NULL,0,0,false,false,false);
  }


  STDAPI DllUnregisterServer() {
  return TRUE;
  }


  long __stdcall DataSetFactory::QueryInterface(REFIID iid, void **ppv) { 
  *ppv = NULL; 
  if ( iid == IID_IUnknown || iid == IID_IClassFactory ) 
     *ppv = this; 
  else 
     return E_NOINTERFACE; 
  AddRef(); 
  return S_OK; 
  } 
  unsigned long __stdcall DataSetFactory::AddRef() { 
  return ++refCount; 
  } 
  unsigned long __stdcall DataSetFactory::Release() { 
  return --refCount;
  } 


  HRESULT STDMETHODCALLTYPE DataSetFactory::CreateInstance(IUnknown *punkOuter, REFIID riid, void **ppv) { 
  HRESULT hres;
  *ppv = NULL; 
  DataSet *pa = new DataSet(punkOuter);
  hres = pa -> QueryInterface(riid,ppv);
  if ( !*ppv ) delete pa;
  return hres;
  } 


  long __stdcall DataSetFactory::LockServer(int fLock) { 
  return S_OK; 
  } 