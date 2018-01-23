// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "IGSFunctioNaterEvents.h"


   long __stdcall _IGSFunctioNaterEvents::QueryInterface(REFIID riid,void **ppv) {
  *ppv = NULL; 
 
   if ( riid == IID_IUnknown ) {
      *ppv = static_cast<void*>(this); 
   } else {
      if ( riid == IID_IDispatch ) {
         *ppv = static_cast<IDispatch *>(this);
      } else {
         if ( riid == DIID_IGSFunctioNaterEvents ) {
            *ppv = static_cast<IGSFunctioNaterEvents *>(this);
         }
         else
            return E_NOINTERFACE;
      }
   }
   AddRef();
   return S_OK;
   }

   unsigned long __stdcall _IGSFunctioNaterEvents::AddRef() {
   return ++refCount;
   }
   unsigned long __stdcall _IGSFunctioNaterEvents::Release() {
   long r = --refCount;
   if ( 0 == r ) {
      delete this;
      return 0;
   }
   return r;  
   }


   HRESULT _IGSFunctioNaterEvents::GetTypeInfoCount(UINT * pctinfo) { 
   *pctinfo = 1; 
   return S_OK;
   } 


   long __stdcall _IGSFunctioNaterEvents::GetTypeInfo(UINT itinfo,LCID lcid,ITypeInfo **pptinfo) { 
   *pptinfo = NULL; 
   if ( itinfo != 0 ) 
      return DISP_E_BADINDEX; 
   *pptinfo = pITypeInfo_IGSFunctioNaterEvents;
   pITypeInfo_IGSFunctioNaterEvents -> AddRef();
   return S_OK;
   } 
 

   HRESULT _IGSFunctioNaterEvents::GetIDsOfNames(REFIID riid,OLECHAR** rgszNames,UINT cNames,LCID lcid,DISPID* rgdispid) { 
   return pITypeInfo_IGSFunctioNaterEvents -> GetIDsOfNames(rgszNames,cNames,rgdispid);
   }


   HRESULT _IGSFunctioNaterEvents::Invoke(DISPID dispidMember, REFIID riid, LCID lcid, 
                                           WORD wFlags,DISPPARAMS *pDispParams, VARIANT *pvarResult,
                                           EXCEPINFO * pexcepinfo, UINT * puArgErr) { 
   HRESULT hr = S_OK;
   VARIANTARG *pArgs = pDispParams -> rgvarg;
   switch ( dispidMember ) 
   case functionEventID_UndefinedVariable: {
      hr = UndefinedVariable(pArgs[0].bstrVal);
      break;
   case functionEventID_UndefinedFunction: 
      hr = UndefinedFunction(pArgs[0].bstrVal);
      break;
   case functionEventID_Clear:
      hr = Clear();
      break;
   case functionEventID_Started:
      hr = Started(pArgs[0].lVal);
      break;
   }
   return hr;
   }


   HRESULT _IGSFunctioNaterEvents::UndefinedVariable(BSTR bstrName) {
   BSTR bstrDefaultMinValue;
   BSTR bstrDefaultMaxValue;
   long defaultStepCount;
   IGSFunctioNater* pIGSFunctioNater;
   pParent -> QueryInterface(IID_IGSFunctioNater,reinterpret_cast<void**>(&pIGSFunctioNater));
   pIGSFunctioNater -> get_DefaultMinValue(&bstrDefaultMinValue);
   pIGSFunctioNater -> get_DefaultMaxValue(&bstrDefaultMaxValue);
   pIGSFunctioNater -> get_DefaultStepCount(&defaultStepCount);
   pIGSFunctioNater -> DefineIndependentVariable(bstrName,bstrDefaultMinValue,bstrDefaultMaxValue,defaultStepCount);
   SysFreeString(bstrDefaultMinValue);
   SysFreeString(bstrDefaultMaxValue);
   pIGSFunctioNater -> Release();
   return S_OK;
   }

   HRESULT _IGSFunctioNaterEvents::UndefinedFunction(BSTR bstrName) {
   return S_OK;
   }

   HRESULT _IGSFunctioNaterEvents::Clear() {
   return S_OK;
   }

   HRESULT _IGSFunctioNaterEvents::Parsed() {
   return S_OK;
   }

   HRESULT _IGSFunctioNaterEvents::Started(long iterationCount) {
   return S_OK;
   }

   HRESULT _IGSFunctioNaterEvents::Paused() {
   return S_OK;
   }

   HRESULT _IGSFunctioNaterEvents::Resumed() {
   return S_OK;
   }

   HRESULT _IGSFunctioNaterEvents::Stopped() {
   return S_OK;
   }

   HRESULT _IGSFunctioNaterEvents::TakeValues(long iterationNumber,long valueCount,SAFEARRAY** bstrNames,SAFEARRAY** arrayValues) {
   return S_OK;
   }

   HRESULT _IGSFunctioNaterEvents::TakeResults(long iterationNumber,BSTR bstrResults) {
   return S_OK;
   }

   HRESULT _IGSFunctioNaterEvents::Finished() {
   return S_OK;
   }
