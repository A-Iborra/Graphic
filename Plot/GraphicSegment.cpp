/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002,2008 Nathan T. Clark

*/

#include <windows.h>
#include <olectl.h>

#include <stdio.h>

#include "general.h"
#include "utils.h"

#include "GraphicSegment.h"

  SegmentFactory segmentFactory;

  extern HMODULE hModule;
  extern char szModuleName[1024];
  extern BSTR wstrModuleName;

  STDAPI DllRegisterServer_Segment() {   
  return utilsDllRegisterObject(CLSID_GraphicSegment,LIBID_Plot,hModule,
                                 szModuleName,
                                 "GSystem Segment Object",
                                 "GSystem.Segment",
                                 "GSystem.Segment.1",
                                 (CATID*)NULL,0,0,false,false,false);
  }


  long __stdcall SegmentFactory::QueryInterface(REFIID iid, void **ppv) { 
  *ppv = NULL; 
  if ( iid == IID_IUnknown || iid == IID_IClassFactory ) 
     *ppv = this; 
  else 
     return E_NOINTERFACE; 
  AddRef(); 
  return S_OK; 
  } 
  unsigned long __stdcall SegmentFactory::AddRef() { 
  return ++refCount; 
  } 
  unsigned long __stdcall SegmentFactory::Release() { 
  return --refCount;
  } 
  HRESULT STDMETHODCALLTYPE SegmentFactory::CreateInstance(IUnknown *punkOuter, REFIID riid, void **ppv) { 
  HRESULT hres;
  *ppv = NULL; 
  GraphicSegment *ps = new GraphicSegment(punkOuter);
  hres = ps -> QueryInterface(riid,ppv);
  if ( ! *ppv ) delete ps;
  return hres;
  } 
  long __stdcall SegmentFactory::LockServer(int fLock) { 
  return S_OK; 
  } 


  GraphicSegment::GraphicSegment(IUnknown *pUnkOuter) : 
     refCount(0),
     segmentID(0),
     pIUnknownOuter(pUnkOuter),
     pIOpenGLImplementation(0),
     pIGraphicSegmentAction(0),
     propertyLineColor(0),
     propertyLineWeight(0)
  {
  return;
  }


  GraphicSegment::~GraphicSegment() {

  if ( pIOpenGLImplementation ) {
     pIOpenGLImplementation -> DeleteSegment(segmentID);
     pIOpenGLImplementation -> Release();
  }

  if ( propertyLineColor )
     propertyLineColor -> Release();

  if ( propertyLineWeight )
     propertyLineWeight -> Release();

  return;
  }
