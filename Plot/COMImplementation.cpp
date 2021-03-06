// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Plot.h"

#include "general.h"
#include "utils.h"

#include "GraphicSegment.h"

#include "GSystem_i.c"

#include "Plot_i.c"
#include "Properties_i.c"
#include "DataSet_i.c"
#include "OpenGLImplementation_i.c"
#include "Text_i.c"

#include "Variable_i.h"
#include "Evaluator_i.h"
#include "Evaluator_i.c"

  static SegmentFactory segmentFactory;

  STDAPI DllRegisterServer_Segment();

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
     wstrModuleName = SysAllocStringLen(NULL,(DWORD)strlen(szModuleName) + 1);
     memset(wstrModuleName,0,(strlen(szModuleName) + 1) * sizeof(OLECHAR));
     MultiByteToWideChar(CP_ACP, 0, szModuleName, -1, wstrModuleName, (DWORD)strlen(szModuleName));  
     ITypeLib *ptLib;
     LoadTypeLib(wstrModuleName,&ptLib);
     ptLib -> GetTypeInfoOfGuid(IID_IPlot,&pITypeInfo);
     break;

  case DLL_PROCESS_DETACH:
     SysFreeString(wstrModuleName);
     break;

  default:
     break;
  }

  return TRUE;
  }


   STDAPI DllRegisterServer() {	
   utilsDllRegisterObject(CLSID_Plot,LIBID_Plot,hModule,szModuleName,"InnoVisioNate Plot Object","InnoVisioNate.Plot","InnoVisioNate.Plot.1",(CATID *)NULL,0,0,false,false,true);
   utilsDllRegisterObject(CLSID_BasePlot,LIBID_Plot,hModule,szModuleName,"InnoVisioNate Base Plot Object","InnoVisioNate.BasePlot","InnoVisioNate.BasePlot.1",(CATID *)NULL,0,0,false,false,true);
   return DllRegisterServer_Segment();
   }


  STDAPI DllUnregisterServer() {
  utilsDllUnregisterObject(CLSID_Plot,"InnoVisioNate.Plot","InnoVisioNate.Plot.1");
  utilsDllUnregisterObject(CLSID_Plot,"InnoVisioNate.BasePlot","InnoVisioNate.BasePlot.1");
  return TRUE;
  }


  static class BasePlotFactory : public IClassFactory {
  public:

     STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
     STDMETHOD_ (ULONG, AddRef)();
     STDMETHOD_ (ULONG, Release)();
     STDMETHOD (CreateInstance)(IUnknown *punkOuter, REFIID riid, void **ppv);
     STDMETHOD (LockServer)(BOOL fLock);

     BasePlotFactory() : refCount(0) {};
     ~BasePlotFactory() {};

  private:
     int refCount;
  } basePlotFactory;

  static class PlotFactory : public BasePlotFactory {
  public:
     PlotFactory() : refCount(0) {};
     ~PlotFactory() {};
     STDMETHOD (CreateInstance)(IUnknown *punkOuter, REFIID riid, void **ppv);
  private:
     int refCount;
  } plotFactory;



  STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppObject) {
  *ppObject = NULL;
  if ( rclsid == CLSID_Plot )
     return plotFactory.QueryInterface(riid,ppObject);
  if ( rclsid == CLSID_BasePlot )
     return basePlotFactory.QueryInterface(riid,ppObject);
  if ( rclsid == CLSID_GraphicSegment )
     return segmentFactory.QueryInterface(riid,ppObject);
  return CLASS_E_CLASSNOTAVAILABLE;
  }



  long __stdcall BasePlotFactory::QueryInterface(REFIID iid, void **ppv) { 
  *ppv = NULL; 
  if ( iid == IID_IUnknown || iid == IID_IClassFactory ) 
     *ppv = this; 
  else 
     return E_NOINTERFACE; 
  AddRef(); 
  return S_OK; 
  } 
 
  unsigned long __stdcall BasePlotFactory::AddRef() { 
  return ++refCount; 
  } 

  unsigned long __stdcall BasePlotFactory::Release() { 
  return --refCount;
  } 

  HRESULT STDMETHODCALLTYPE BasePlotFactory::CreateInstance(IUnknown *punkOuter, REFIID riid, void **ppv) { 
  HRESULT hres;
  *ppv = NULL; 
  if ( punkOuter ) 
      return ResultFromScode(CLASS_E_NOAGGREGATION); 
  BasePlot *bp = new BasePlot();
  hres = bp -> QueryInterface(riid,ppv);
  if ( ! *ppv ) delete bp;
  return hres;
  } 

  long __stdcall BasePlotFactory::LockServer(int fLock) { 
  return S_OK; 
  } 


  HRESULT STDMETHODCALLTYPE PlotFactory::CreateInstance(IUnknown *punkOuter, REFIID riid, void **ppv) { 

  HRESULT hres;
  *ppv = NULL; 

  Plot *p = new Plot(punkOuter);

  hres = p -> QueryInterface(riid,ppv);

  if ( ! *ppv ) 
      delete p;

  return hres;
  } 
