/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include "Plot.h"


  long __stdcall Plot::GetTypeInfoCount(UINT *i) { 
  *i = 1; 
  return S_OK; 
  } 
 

  long __stdcall Plot::GetTypeInfo(UINT itinfo,LCID lcid,ITypeInfo **pptinfo) { 
  *pptinfo = NULL; 
  if ( itinfo != 0 ) 
     return DISP_E_BADINDEX; 
  Plot *p = static_cast<Plot *>(this);
  *pptinfo = pITypeInfo;
  pITypeInfo -> AddRef();
  return S_OK; 
  } 
 

  long __stdcall Plot::GetIDsOfNames(REFIID riid,OLECHAR **rgszNames,UINT cNames,LCID lcid,DISPID *rgdispid) { 
  return DispGetIDsOfNames(pITypeInfo, rgszNames, cNames, rgdispid); 
  } 

      
  long __stdcall Plot::Invoke(DISPID dispidMember,
                           REFIID riid,
                           LCID lcid,
                           WORD wFlags, 
                           DISPPARAMS *pdispparams,
                           VARIANT *pvarResult,
                           EXCEPINFO *pexcepinfo, 
                           UINT *puArgErr) { 
  void *ppv;
  QueryInterface(IID_IDispatch,&ppv);
  return pITypeInfo -> Invoke(ppv,dispidMember,wFlags,pdispparams,pvarResult,pexcepinfo,puArgErr);
  } 
