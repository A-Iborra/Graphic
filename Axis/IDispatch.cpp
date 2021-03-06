// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Axis.h"


   long __stdcall Axis::GetTypeInfoCount(UINT *i) { 
   *i = 1; 
   return S_OK; 
   } 
  
 
   long __stdcall Axis::GetTypeInfo(UINT itinfo,LCID lcid,ITypeInfo **pptinfo) { 
   *pptinfo = NULL; 
   if ( itinfo != 0 ) 
      return DISP_E_BADINDEX; 
   *pptinfo = pITypeInfo;
   pITypeInfo -> AddRef();
   return S_OK; 
   } 
  
 
   long __stdcall Axis::GetIDsOfNames(REFIID riid,OLECHAR **rgszNames,UINT cNames,LCID lcid,DISPID *rgdispid) { 
   return DispGetIDsOfNames(pITypeInfo, rgszNames, cNames, rgdispid); 
   } 
 
       
   long __stdcall Axis::Invoke(DISPID dispidMember,
                            REFIID riid,
                            LCID lcid,
                            WORD wFlags, 
                            DISPPARAMS *pdispparams,
                            VARIANT *pvarResult,
                            EXCEPINFO *pexcepinfo, 
                            UINT *puArgErr) { 
   void *ppv;
   QueryInterface(IID_IDispatch,&ppv);
   HRESULT hr = pITypeInfo -> Invoke(ppv,dispidMember,wFlags,pdispparams,pvarResult,pexcepinfo,puArgErr);
   Release();
   return hr;
   } 
