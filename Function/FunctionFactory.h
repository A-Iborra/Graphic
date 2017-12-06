/*

                       Copyright (c) 1996,1997,1998 Nathan T. Clark

*/

#ifndef FUNCTIONFACTORY_DEFINED
#define FUNCTIONFACTORY_DEFINED

#include "FunctionObjectGUID.h"

  STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppObject);
  STDAPI DllRegisterServer();
  STDAPI DLlUnregisterServer();

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

  };



#endif