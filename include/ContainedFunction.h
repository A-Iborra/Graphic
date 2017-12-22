
#pragma once

#include "ContainedObject.h"
#include "Properties_i.h"
#include "DataSet_i.h"
#include "OpenGLImplementation_i.h"
#include "Function_i.h"

  class ContainedFunction : 
      public ContainedObject, 
      public IGSFunctioNaterEvents {

  public:

     ContainedFunction(G* pParent,long plotID,HWND hwndTab,IGSFunctioNater *pIFunction,IUnknown* pIUnknownObject,REFIID riidEventsInterface);

     IGSFunctioNater *pFunction() { return pIGSFunctioNater; };

     STDMETHOD(QueryInterface)(REFIID riid,void **ppv);

     STDMETHOD_ (ULONG, AddRef)();
     STDMETHOD_ (ULONG, Release)();

  private:

     STDMETHOD(GetTypeInfoCount)(UINT *pctinfo);
     STDMETHOD(GetTypeInfo)(UINT itinfo, LCID lcid, ITypeInfo **pptinfo);
     STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid);
     STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pdispparams, VARIANT *pvarResult, EXCEPINFO *pexcepinfo, UINT *puArgErr);

     STDMETHOD(UndefinedVariable)(BSTR);
     STDMETHOD(UndefinedFunction)(BSTR);
     STDMETHOD(Clear)();
     STDMETHOD(Parsed)();
     STDMETHOD(Started)(long);
     STDMETHOD(Paused)();
     STDMETHOD(Resumed)();
     STDMETHOD(Stopped)();
     STDMETHOD(TakeValues)(long,long,SAFEARRAY**,SAFEARRAY**);
     STDMETHOD(TakeResults)(long iterationNumber,BSTR bstrResults);
     STDMETHOD(Finished)();

     long plotID;

     IGSFunctioNater *pIGSFunctioNater;

  };

