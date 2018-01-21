// Copyright 2017 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"

#include "utils.h"

#include "ContainedFunction.h"
#include "Graphic.h"


   ContainedFunction::ContainedFunction(G* pp,long pID,HWND hwndT,IGSFunctioNater *pIFunction,IUnknown* pIUnknownObject,REFIID riidEventsInterface) : 
      plotID(pID),
      pIGSFunctioNater(pIFunction),
      ContainedObject(pp,hwndT,pIUnknownObject,riidEventsInterface) {};


   long __stdcall ContainedFunction::QueryInterface(REFIID riid,void **ppv) {

   if ( DIID_IGSFunctioNaterEvents == riid ) 
      *ppv = static_cast<IGSFunctioNaterEvents*>(this);
   else

   if ( IID_IDispatch == riid ) 
      *ppv = static_cast<IDispatch*>(this);
   else

      return ContainedObject::QueryInterface(riid,ppv);

   reinterpret_cast<IUnknown*>(*ppv) -> AddRef();
   return S_OK;
   }
   unsigned long __stdcall ContainedFunction::AddRef() { return ContainedObject::AddRef(); }
   unsigned long __stdcall ContainedFunction::Release() { return ContainedObject::Release(); }



   HRESULT ContainedFunction::GetTypeInfoCount(UINT * pctinfo) { 
   *pctinfo = 1;
   return S_OK;
   } 


   long __stdcall ContainedFunction::GetTypeInfo(UINT itinfo,LCID lcid,ITypeInfo **pptinfo) { 
   *pptinfo = NULL; 
   if ( itinfo != 0 ) 
      return DISP_E_BADINDEX; 
   *pptinfo = pITypeInfo_IGSFunctioNaterEvents;
   pITypeInfo_IGSFunctioNaterEvents -> AddRef();
   return S_OK; 
   } 
 

   HRESULT ContainedFunction::GetIDsOfNames(REFIID riid,OLECHAR** rgszNames,UINT cNames,LCID lcid, DISPID* rgdispid) { 
   return DispGetIDsOfNames(pITypeInfo_IGSFunctioNaterEvents,rgszNames,cNames,rgdispid);
   }


   HRESULT ContainedFunction::Invoke(DISPID dispidMember, REFIID riid, LCID lcid, 
                                           WORD wFlags,DISPPARAMS FAR* pDispParams, VARIANT FAR* pvarResult,
                                           EXCEPINFO FAR* pexcepinfo, UINT FAR* puArgErr) { 
   VARIANTARG *pArgs = pDispParams -> rgvarg;
   switch ( dispidMember ) 
   case functionEventID_UndefinedVariable: {
      UndefinedVariable(pArgs[0].bstrVal);
      break;
   case functionEventID_UndefinedFunction: 
      UndefinedFunction(pArgs[0].bstrVal);
      break;
   case functionEventID_Clear:
      Clear();
      break;
   case functionEventID_Started:
      Started(pArgs[0].lVal);
      break;
   case functionEventID_TakeValues:
      TakeValues(pArgs[3].lVal,pArgs[2].lVal,pArgs[1].pparray,pArgs[0].pparray);
      break;
   case functionEventID_TakeResults:
      break;
   case functionEventID_Finished:
      Finished();
      break;
   }
   return S_OK;
   }


   HRESULT ContainedFunction::UndefinedVariable(BSTR bstrName) {
   return S_OK;
   }


   HRESULT ContainedFunction::UndefinedFunction(BSTR bstrName) {
   return S_OK;
   }

   HRESULT ContainedFunction::Clear() {
   return S_OK;
   }

   HRESULT ContainedFunction::Parsed() {
   return S_OK;
   }

   HRESULT ContainedFunction::Started(long expectedValues) {
   pParent -> PrepareForData(plotID);
   return S_OK;
   }


   HRESULT ContainedFunction::Paused() {
   return S_OK;
   }

   HRESULT ContainedFunction::Resumed() {
   return S_OK;
   }

   HRESULT ContainedFunction::Stopped() {
   return S_OK;
   }

   HRESULT ContainedFunction::TakeValues(long iterationNumber,long valueCount,SAFEARRAY** bstrNames,SAFEARRAY** arrayValues) {
   long index,lBound;
   long n = 0;
   static char szTest[128];
   double v;

   SafeArrayGetLBound(*arrayValues,1,&lBound);
   index = lBound - 1;
   for ( long k = 0; k < valueCount; k++ ) {
      index++;
      SafeArrayGetElement(*arrayValues,&index,&v);
      n += sprintf(szTest,"%g, ",v);
   }

   char* p,*pszValues = new char[n + 1];

   p = pszValues;
   memset(pszValues,0,n + 1);
   memset(pszValues,' ',n);
   index = lBound - 1;
   for ( long k = 0; k < valueCount; k++ ) {
      index++;
      SafeArrayGetElement(*arrayValues,&index,&v);
      p += sprintf(p,"%g, ",v);

   }      
   BSTR bstrValues = SysAllocStringLen(NULL,n + 1);
   MultiByteToWideChar(CP_ACP,0,pszValues,-1,bstrValues,n + 1);
   pParent -> TakeDataString(bstrValues,plotID);
   delete [] pszValues;
   SysFreeString(bstrValues);
   return S_OK;
   }

   HRESULT ContainedFunction::TakeResults(long iterationNumber,BSTR bstrResults) {
   pParent -> TakeDataString(bstrResults,plotID);
   return S_OK;
   }

   HRESULT ContainedFunction::Finished() {
   pParent -> FinishedWithData(plotID);
   pParent -> Draw(plotID);
   return S_OK;
   }
