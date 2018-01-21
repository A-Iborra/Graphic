// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "DataSet.h"

#include <functional>

#include "Function_i.c"

   DataSet::_IGSFunctioNaterEvents::_IGSFunctioNaterEvents(DataSet * pp,IGSFunctioNater *pIF,std::function<void()>* pOnFinished) :
      pParent(pp),
      pIFunction(pIF),
      pExecuteOnFinish(pOnFinished) {

   IUnknown *pIUnknownObject = NULL;

   pIFunction -> QueryInterface(IID_IUnknown,reinterpret_cast<void **>(&pIUnknownObject));

   IConnectionPointContainer* pIConnectionPointContainer;

   if ( ! SUCCEEDED(pIUnknownObject -> QueryInterface(IID_IConnectionPointContainer,reinterpret_cast<void**>(&pIConnectionPointContainer))) ) 
      return;

   if ( ! SUCCEEDED(pIConnectionPointContainer -> FindConnectionPoint(DIID_IGSFunctioNaterEvents,&pIConnectionPoint)) ) {
      pIConnectionPointContainer -> Release();
      return;
   }

   IUnknown* pIUnknownThis;
   QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknownThis));
   pIConnectionPoint -> Advise(pIUnknownThis,&dwConnectionCookie);

   pIUnknownThis -> Release();

   pIConnectionPointContainer -> Release();

   pIUnknownObject -> Release();

   return;
   }

   DWORD WINAPI deleteLambda(void* pv) {
   Sleep(100);
   std::function<void()>* pLambda = (std::function<void()>*)pv;
   delete pLambda;
   return 0;
   }

   DataSet::_IGSFunctioNaterEvents::~_IGSFunctioNaterEvents() {

   pIConnectionPoint -> Unadvise(dwConnectionCookie);
   pIConnectionPoint -> Release();
   pIConnectionPoint = NULL;

   DWORD threadID;
   CreateThread(NULL,0,deleteLambda,(void *)pExecuteOnFinish,0,&threadID);

   return;
   }


   long __stdcall DataSet::_IGSFunctioNaterEvents::QueryInterface(REFIID riid,void **ppv) {

   if ( IID_IUnknown == riid )
      *ppv = static_cast<IUnknown *>(this);
   else

   if ( DIID_IGSFunctioNaterEvents == riid ) 
      *ppv = static_cast<IGSFunctioNaterEvents*>(this);
   else

   if ( IID_IDispatch == riid ) 
      *ppv = static_cast<IDispatch*>(this);
   else

      return pParent -> QueryInterface(riid,ppv);

   reinterpret_cast<IUnknown*>(*ppv) -> AddRef();
   return S_OK;
   }
   unsigned long __stdcall DataSet::_IGSFunctioNaterEvents::AddRef() { return pParent -> AddRef(); }
   unsigned long __stdcall DataSet::_IGSFunctioNaterEvents::Release() { return pParent -> Release(); }



   HRESULT DataSet::_IGSFunctioNaterEvents::GetTypeInfoCount(UINT * pctinfo) { 
   *pctinfo = 1;
   return S_OK;
   } 


   long __stdcall DataSet::_IGSFunctioNaterEvents::GetTypeInfo(UINT itinfo,LCID lcid,ITypeInfo **pptinfo) { 
   *pptinfo = NULL; 
   if ( itinfo != 0 ) 
      return DISP_E_BADINDEX; 
   *pptinfo = pITypeInfo_IGSFunctioNaterEvents;
   pITypeInfo_IGSFunctioNaterEvents -> AddRef();
   return S_OK; 
   } 
 

   HRESULT DataSet::_IGSFunctioNaterEvents::GetIDsOfNames(REFIID riid,OLECHAR** rgszNames,UINT cNames,LCID lcid, DISPID* rgdispid) { 
   return DispGetIDsOfNames(pITypeInfo_IGSFunctioNaterEvents,rgszNames,cNames,rgdispid);
   }

   HRESULT DataSet::_IGSFunctioNaterEvents::Invoke(DISPID dispidMember, REFIID riid, LCID lcid, 
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



   HRESULT DataSet::_IGSFunctioNaterEvents::UndefinedVariable(BSTR bstrName) {
   return S_OK;
   }


   HRESULT DataSet::_IGSFunctioNaterEvents::UndefinedFunction(BSTR bstrName) {
   return S_OK;
   }

   HRESULT DataSet::_IGSFunctioNaterEvents::Clear() {
   return S_OK;
   }

   HRESULT DataSet::_IGSFunctioNaterEvents::Parsed() {
   return S_OK;
   }

   HRESULT DataSet::_IGSFunctioNaterEvents::Started(long expectedValues) {
   return S_OK;
   }


   HRESULT DataSet::_IGSFunctioNaterEvents::Paused() {
   return S_OK;
   }

   HRESULT DataSet::_IGSFunctioNaterEvents::Resumed() {
   return S_OK;
   }

   HRESULT DataSet::_IGSFunctioNaterEvents::Stopped() {
   return S_OK;
   }

   HRESULT DataSet::_IGSFunctioNaterEvents::TakeValues(long iterationNumber,long valueCount,SAFEARRAY** bstrNames,SAFEARRAY** arrayValues) {
   //long index,lBound;
   //long n = 0;
   //static char szTest[128];
   //double v;

   //SafeArrayGetLBound(*arrayValues,1,&lBound);
   //index = lBound - 1;
   //for ( long k = 0; k < valueCount; k++ ) {
   //   index++;
   //   SafeArrayGetElement(*arrayValues,&index,&v);
   //   n += sprintf(szTest,"%g, ",v);
   //}

   //char* p,*pszValues = new char[n + 1];

   //p = pszValues;
   //memset(pszValues,0,n + 1);
   //memset(pszValues,' ',n);
   //index = lBound - 1;
   //for ( long k = 0; k < valueCount; k++ ) {
   //   index++;
   //   SafeArrayGetElement(*arrayValues,&index,&v);
   //   p += sprintf(p,"%g, ",v);

   //}      
   //BSTR bstrValues = SysAllocStringLen(NULL,n + 1);
   //MultiByteToWideChar(CP_ACP,0,pszValues,-1,bstrValues,n + 1);
   //pParent -> TakeDataString(bstrValues,plotID);
   //delete [] pszValues;
   //SysFreeString(bstrValues);
   return S_OK;
   }

   HRESULT DataSet::_IGSFunctioNaterEvents::TakeResults(long iterationNumber,BSTR bstrResults) {
   //pParent -> TakeDataString(bstrResults,plotID);
   return S_OK;
   }

   HRESULT DataSet::_IGSFunctioNaterEvents::Finished() {
   if ( pExecuteOnFinish )
      (*pExecuteOnFinish)();
   return S_OK;
   }
