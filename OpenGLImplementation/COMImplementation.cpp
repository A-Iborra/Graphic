// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "OpenGLImplementor.h"

#include "general.h"
#include "Graphic_resource.h"
#include "utils.h"

#include "OpenGLImplementation_i.c"
#include "Properties_i.c"

#include "Variable_i.h"
#include "Evaluator_i.h"
#include "Evaluator_i.c"

//#include "List.cpp"

   HMODULE hModule;
   char szModuleName[1024];
   BSTR wstrModuleName;
   ITypeInfo *pITypeInfo;
 
   STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppObject);
 
   unsigned short wsVersionMajor = 1;
   unsigned short wsVersionMinor = 0;

   class OpenGLImplementorFactory : public IClassFactory {
   public:
 
      STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
      STDMETHOD_ (ULONG, AddRef)();
      STDMETHOD_ (ULONG, Release)();
      STDMETHOD (CreateInstance)(IUnknown *punkOuter, REFIID riid, void **ppv);
      STDMETHOD (LockServer)(BOOL fLock);
 
      OpenGLImplementorFactory() {};
      ~OpenGLImplementorFactory() {};
 
   };
 
   static OpenGLImplementorFactory factory;
 
   BOOL WINAPI DllMain(HANDLE module,ULONG flag,void *res) {
 
   switch(flag) {
   case DLL_PROCESS_ATTACH:

      hModule = reinterpret_cast<HMODULE>(module);

      GetModuleFileName(hModule,szModuleName,1024);
      wstrModuleName = SysAllocStringLen(NULL,strlen(szModuleName) + 1);
      memset(wstrModuleName,0,sizeof(OLECHAR) * (strlen(szModuleName) + 1));
      MultiByteToWideChar(CP_ACP, 0, szModuleName, -1, wstrModuleName, strlen(szModuleName));  
 
      ITypeLib *ptLib;
      LoadTypeLib(wstrModuleName,&ptLib);
      ptLib -> GetTypeInfoOfGuid(CLSID_OpenGLImplementor,&pITypeInfo);
 
      break;
 
   case DLL_PROCESS_DETACH:
      break;
 
   default:
      break;
   }
 
   return TRUE;
   }
 
 

   STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppObject) {
   *ppObject = NULL;
   if ( rclsid != CLSID_OpenGLImplementor ) 
      return CLASS_E_CLASSNOTAVAILABLE;
   return factory.QueryInterface(riid,ppObject);
   }
 
 
   STDAPI DllRegisterServer() {
   utilsDllRegisterTypeLib(wstrModuleName);
   return utilsDllRegisterObject(CLSID_OpenGLImplementor,LIBID_OpenGLImplementor,
                                  hModule,szModuleName,
                                  "InnoVisioNate OpenGLImplementor Object",
                                  "InnoVisioNate.OpenGLImplementor",
                                  "InnoVisioNate.OpenGLImplementor.1",
                                  (CATID*)NULL,0,0,false,true,false);
   }
 
 
   STDAPI DllUnregisterServer() {
   utilsDllUnregisterObject(CLSID_OpenGLImplementor,"InnoVisioNate.OpenGLImplementor","InnoVisioNate.OpenGLImplementor.1");
   return utilsDllUnregisterTypeLib(wstrModuleName,LIBID_OpenGLImplementor,wsVersionMajor,wsVersionMinor);
   }
 
 
   long __stdcall OpenGLImplementorFactory::QueryInterface(REFIID iid, void **ppv) { 
   *ppv = NULL; 
   if ( iid == IID_IUnknown || iid == IID_IClassFactory ) 
      *ppv = this; 
   else 
      return E_NOINTERFACE; 
   AddRef(); 
   return S_OK; 
   } 
   unsigned long __stdcall OpenGLImplementorFactory::AddRef() { return 1; } 
   unsigned long __stdcall OpenGLImplementorFactory::Release() { return 1; } 
 
 
   HRESULT STDMETHODCALLTYPE OpenGLImplementorFactory::CreateInstance(IUnknown *punkOuter, REFIID riid, void **ppv) { 
   HRESULT hres;
   *ppv = NULL; 
   OpenGLImplementor *p = new OpenGLImplementor(punkOuter);
   hres = p -> QueryInterface(riid,ppv);
   if ( ! *ppv ) delete p;
   return hres;
   } 
 
 
   long __stdcall OpenGLImplementorFactory::LockServer(int fLock) { 
   return S_OK; 
   }