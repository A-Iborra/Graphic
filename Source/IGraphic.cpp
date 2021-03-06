// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"
#include <olectl.h>

#include "utils.h"
#include "Graphic_resource.h"

#include "GraphicControl_i.h"
#include "List.cpp"


   long __stdcall G::GetTypeInfoCount(UINT *i) { 
   *i = 1; 
   return S_OK; 
   } 
  
 
   long __stdcall G::GetTypeInfo(UINT itinfo,LCID lcid,ITypeInfo **pptinfo) { 
   *pptinfo = NULL; 
   if ( itinfo != 0 ) 
      return DISP_E_BADINDEX; 
   *pptinfo = pITypeInfo_IG;
   pITypeInfo_IG -> AddRef();
   return S_OK; 
   } 
  
 
   long __stdcall G::GetIDsOfNames(REFIID riid,OLECHAR **rgszNames,UINT cNames,LCID lcid,DISPID *rgdispid) { 
   return DispGetIDsOfNames(pITypeInfo_IG, rgszNames, cNames, rgdispid); 
   } 
 
       
   long __stdcall G::Invoke(DISPID dispidMember,REFIID riid,LCID lcid,WORD wFlags, DISPPARAMS *pdispparams,VARIANT *pvarResult,EXCEPINFO *pexcepinfo, UINT *puArgErr) { 

   if ( DISPATCH_PROPERTYGET == wFlags ) {

      switch ( dispidMember ) {

      case DISPID_AMBIENT_USERMODE:

         pvarResult -> vt = VT_BOOL;
         pvarResult -> boolVal = isRunning;

         return S_OK;

      default:
         break;
      }

   }

   IDispatch *ppv;
   QueryInterface(IID_IDispatch,reinterpret_cast<void**>(&ppv));
   HRESULT hr = pITypeInfo_IG -> Invoke(ppv,dispidMember,wFlags,pdispparams,pvarResult,pexcepinfo,puArgErr);
   ppv -> Release();
   return hr;
   } 
 
   STDMETHODIMP G::Draw(ULONG_PTR sourceID) {
   render(sourceID);
   return S_OK;
   }
 
 
   STDMETHODIMP G::AddAxis(char type,IDispatch *pAxis) {
   IAxis *pIAxis;
   pAxis -> QueryInterface(IID_IAxis,reinterpret_cast<void**>(&pIAxis));
   axisList.Add(pIAxis);
   pIAxis -> Initialize(type,xaxis,yaxis,zaxis,propertyPlotView,
                           propertyXFloor,propertyXCeiling,
                           propertyYFloor,propertyYCeiling,
                           propertyZFloor,propertyZCeiling,
                           pIDataSetMaster,pIOpenGLImplementation,pIEvaluator,someObjectChanged,(void *)this,(ULONG_PTR)pIAxis);
   return S_OK;
   }


   STDMETHODIMP G::AddText(BSTR theText,IDispatch **pText) {

   IText *t = newText();

   t -> put_Text(theText);

   t -> PrepData();
   t -> Draw();
   if ( pText ) 
      t -> QueryInterface(IID_IDispatch,reinterpret_cast<void**>(pText));

   return S_OK;
   }


   STDMETHODIMP G::AddTextInteractive(IDispatch **pText) {

   IText *t = newText();

   t -> PrepData();
   t -> Draw();
   IUnknown* pIUnknownText;
   t -> QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknownText));
   pIGProperties -> ShowProperties(Canvas(),pIUnknownText);
   pIUnknownText -> Release();

   t -> Draw();
   if ( pText ) 
      t -> QueryInterface(IID_IDispatch,reinterpret_cast<void**>(pText));

   return S_OK;
   }
 
 
   STDMETHODIMP G::RemoveText(IDispatch *pText) {
   IText *t = (IText *)NULL;
   while ( t = textList.GetNext(t) ) {
      IDispatch *pTest;
      t -> QueryInterface(IID_IDispatch,reinterpret_cast<void**>(&pTest));
      pTest -> Release();
      if ( pText == pTest ) {
         textList.Remove(t);
         t -> Release();
         return S_OK;
      }
   }
   return E_INVALIDARG;
   }
 
 

   STDMETHODIMP G::RemoveAllText() {
   IText *t;
   while ( t = textList.GetFirst() ) {
      t -> Release();
      textList.Remove(t);
   }
   return S_OK;
   }


   STDMETHODIMP G::AddFunction(BSTR expression,IDispatch **ppFunction) {

   IGSFunctioNater *f = newFunction(true);

   f -> put_Expression(expression);

   SendMessage(hwndDataSourcesFunctions,TCM_SETCURSEL,(WPARAM)(SendMessage(hwndDataSourcesFunctions,TCM_GETITEMCOUNT,0L,0L) - 1),0L);
   ContainedFunction *pf = (ContainedFunction *)NULL;
   while ( pf = containedFunctionList.GetNext(pf) )
      ShowWindow(pf -> HWNDSite(),SW_HIDE);
   pf = containedFunctionList.Get((ULONG_PTR)f);
   ShowWindow(pf -> HWNDSite(),SW_SHOW);
   if ( ppFunction )
      f -> QueryInterface(IID_IDispatch,reinterpret_cast<void**>(ppFunction));

   return S_OK;
   }


   STDMETHODIMP G::AddFunctionInteractive(IDispatch **ppFunction) {

   IGSFunctioNater *f = newFunction(true);

   IUnknown* pIUnknownFunction;
   f -> QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknownFunction));
   pIGProperties -> ShowProperties(hwndFrame,pIUnknownFunction);
   pIUnknownFunction -> Release();

   char szTemp[MAX_PROPERTY_SIZE];
   BSTR bstrExpression;
   f -> get_Expression(&bstrExpression);
   memset(szTemp,0,sizeof(szTemp));
   WideCharToMultiByte(CP_ACP,0,bstrExpression,-1,szTemp,sizeof(szTemp),0,0);
   SysFreeString(bstrExpression);

   if ( szTemp[0] ) {

      SendMessage(hwndDataSourcesFunctions,TCM_SETCURSEL,(WPARAM)(SendMessage(hwndDataSourcesFunctions,TCM_GETITEMCOUNT,0L,0L) - 1),0L);
      ContainedFunction *pf = (ContainedFunction *)NULL;
      while ( pf = containedFunctionList.GetNext(pf) )
         ShowWindow(pf -> HWNDSite(),SW_HIDE);
      pf = containedFunctionList.Get((ULONG_PTR)f);
      ShowWindow(pf -> HWNDSite(),SW_SHOW);
      if ( ppFunction )
         f -> QueryInterface(IID_IDispatch,reinterpret_cast<void**>(ppFunction));

   } else {
      deleteFunction(f);
   }

   return S_OK;
   }


   STDMETHODIMP G::RemoveFunction(IDispatch *pFunction) {
   IGSFunctioNater *f = (IGSFunctioNater *)NULL;
   while ( f = functionList.GetNext(f) ) {
      IDispatch *pTest;
      f -> QueryInterface(IID_IDispatch,reinterpret_cast<void**>(&pTest));
      pTest -> Release();
      if ( pFunction == pTest ) {
         deleteFunction(f);
         return S_OK;
      }
   }
   return E_INVALIDARG;
   }


   STDMETHODIMP G::RemoveAllFunctions() {
   IGSFunctioNater *f;
   while ( f = functionList.GetFirst() )
      deleteFunction(f);
   return S_OK;
   }


   STDMETHODIMP G::AddPlot(long id,IDispatch **ppIDispatch) {
   IPlot *p = newPlot(id);
   if ( ppIDispatch ) 
      p -> QueryInterface(IID_IDispatch,reinterpret_cast<void **>(ppIDispatch));
   return S_OK;
   }


   STDMETHODIMP G::RemovePlot(long id) {
   IPlot *p = (IPlot *)NULL;
   if ( p = plotList.Get(id) ) {
      p -> Release();
      plotList.Remove(p);
      return S_OK;
   }
   return E_INVALIDARG;
   }


   STDMETHODIMP G::RemoveAllPlots() {
   IPlot *p;
   while ( p = plotList.GetFirst() ) {
      p -> Release();
      plotList.Remove(p);
   }
   return S_OK;
   }

   STDMETHODIMP G::Synchronise() {
   wait();
   return S_OK;
   }