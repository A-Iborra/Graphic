// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Function.h"

#include "Graphic_resource.h"

   long __stdcall _IEvaluatorEvents::QueryInterface(REFIID riid,void **ppv) {

   if ( IID_IUnknown == riid ) 
      *ppv = this;
   else

   if ( IID_IDispatch == riid ) 
      *ppv = static_cast<IDispatch*>(this);
   else

   if ( IID_IEvaluatorEvents == riid ) 
      *ppv = static_cast<IEvaluatorEvents*>(this);
   else

      return E_NOINTERFACE;

   reinterpret_cast<IUnknown*>(*ppv) -> AddRef();
   return S_OK;
   }
   unsigned long __stdcall _IEvaluatorEvents::AddRef() {
   return 1;
   }
   unsigned long __stdcall _IEvaluatorEvents::Release() {
   return 1;
   }


   HRESULT _IEvaluatorEvents::GetTypeInfoCount(UINT * pctinfo) { 
   *pctinfo = 1;
   return S_OK;
   } 


   long __stdcall _IEvaluatorEvents::GetTypeInfo(UINT itinfo,LCID lcid,ITypeInfo **pptinfo) { 
   *pptinfo = NULL; 
   if ( itinfo != 0 ) 
      return DISP_E_BADINDEX; 
   *pptinfo = pIEvaluatorEventsTypeInfo;
   pIEvaluatorEventsTypeInfo -> AddRef();
   return S_OK; 
   } 
 

   HRESULT _IEvaluatorEvents::GetIDsOfNames(REFIID riid,OLECHAR** rgszNames,UINT cNames,LCID lcid, DISPID* rgdispid) { 
   return DispGetIDsOfNames(pIEvaluatorEventsTypeInfo,rgszNames,cNames,rgdispid);
   }


   HRESULT _IEvaluatorEvents::Invoke(DISPID dispidMember, REFIID riid, LCID lcid, 
                                           WORD wFlags,DISPPARAMS FAR* pdispparams, VARIANT FAR* pvarResult,
                                           EXCEPINFO FAR* pexcepinfo, UINT FAR* puArgErr) { 
   return DispInvoke(this,pIEvaluatorEventsTypeInfo,dispidMember,wFlags,pdispparams,pvarResult,pexcepinfo,puArgErr); 
   }


   HRESULT _IEvaluatorEvents::Started(long expectedIterations) {
   HWND hwnd = pParent -> hWnd();
   hwnd = GetDlgItem(hwnd,IDDIALOG_FUNCTION_VARIABLES);
   hwnd = GetDlgItem(hwnd,IDDI_FUNCTION_VARIABLES_TABS);
   hwnd = GetDlgItem(hwnd,IDDIALOG_FUNCTION_ERRORS);
   HWND hwndButton = GetDlgItem(hwnd,IDDI_FUNCTION_ERRORS_CLEAR);
   hwnd = GetDlgItem(hwnd,IDDI_FUNCTION_ERRORS_BOX);
   SendMessage(hwnd,LB_RESETCONTENT,0L,0L);
   EnableWindow(hwndButton,FALSE);
   EnableWindow(GetDlgItem(pParent -> hWnd(),IDDI_FUNCTION_START),FALSE);
   EnableWindow(GetDlgItem(pParent -> hWnd(),IDDI_FUNCTION_EQUATION_ENTRY),FALSE);
   EnableWindow(GetDlgItem(pParent -> hWnd(),IDDI_FUNCTION_PAUSE),TRUE);
   EnableWindow(GetDlgItem(pParent -> hWnd(),IDDI_FUNCTION_RESUME),FALSE);
   EnableWindow(GetDlgItem(pParent -> hWnd(),IDDI_FUNCTION_STOP),TRUE);
   pParent -> fire_Started(expectedIterations);
   return S_OK;
   }


   HRESULT _IEvaluatorEvents::TakeResultString(long iterationCount,char* pszNames,char* pszResults) {
   char* pszCookedResults = NULL;
   VARIANT_BOOL bResultsVisible;
   pParent -> get_ShowResults(&bResultsVisible);
   pParent -> fire_TakeValues(iterationCount,pszNames,pszResults,bResultsVisible ? &pszCookedResults : NULL);
   if ( pszCookedResults ) {
      SetDlgItemText(pParent -> hWnd(),IDDI_FUNCTION_RESULT,pszCookedResults);
      delete [] pszCookedResults;
   }
   return S_OK;
   }


   HRESULT _IEvaluatorEvents::UnknownVariable(BSTR variableName) {
   long rc = pParent -> fire_UndefinedVariable(variableName);
   BOOL isVariable;
   pParent -> evaluator -> IsVariable(variableName,&isVariable);
   if ( ! isVariable ) {
      char* errorString = new char[(DWORD)wcslen(variableName) + 1];
      WideCharToMultiByte(CP_ACP,0,variableName,-1,errorString,(DWORD)wcslen(variableName) + 1,0,0);
      bool bDummy;
      pParent -> UnknownVariable(errorString,&bDummy);
      delete [] errorString;
   }
   return S_OK;
   }


   HRESULT _IEvaluatorEvents::UnknownFunction(BSTR functionName) {
   pParent -> fire_UndefinedFunction(functionName);
   return S_OK;
   }


   HRESULT _IEvaluatorEvents::MathError(BSTR functionName,double argument) {
   return S_OK;
   }

   HRESULT _IEvaluatorEvents::Finished() {
   EnableWindow(GetDlgItem(pParent -> hWnd(),IDDI_FUNCTION_START),TRUE);
   EnableWindow(GetDlgItem(pParent -> hWnd(),IDDI_FUNCTION_EQUATION_ENTRY),TRUE);
   EnableWindow(GetDlgItem(pParent -> hWnd(),IDDI_FUNCTION_PAUSE),FALSE);
   EnableWindow(GetDlgItem(pParent -> hWnd(),IDDI_FUNCTION_RESUME),FALSE);
   EnableWindow(GetDlgItem(pParent -> hWnd(),IDDI_FUNCTION_STOP),FALSE);
   pParent -> fire_Finished();
   return S_OK;
   }


   HRESULT _IEvaluatorEvents::Clear() {
   pParent -> fire_Clear();
   return S_OK;
   }


   HRESULT _IEvaluatorEvents::DivideByZero() {
   pParent -> fire_DivideByZero();
   HWND hwnd = pParent -> hWnd();
   hwnd = GetDlgItem(hwnd,IDDIALOG_FUNCTION_VARIABLES);
   hwnd = GetDlgItem(hwnd,IDDI_FUNCTION_VARIABLES_TABS);
   hwnd = GetDlgItem(hwnd,IDDIALOG_FUNCTION_ERRORS);
   HWND hwndButton = GetDlgItem(hwnd,IDDI_FUNCTION_ERRORS_CLEAR);
   hwnd = GetDlgItem(hwnd,IDDI_FUNCTION_ERRORS_BOX);
   SendMessage(hwnd,LB_ADDSTRING,0L,(LPARAM)"Divide by Zero");
   EnableWindow(hwndButton,TRUE);
   return S_OK;
   }


   HRESULT _IEvaluatorEvents::InvalidArgument(BSTR bstrFunctionName,double badArgument) {
   pParent -> fire_InvalidArgument(bstrFunctionName,badArgument);
   long n = (DWORD)wcslen(bstrFunctionName) + 1;
   char *pszName = new char[n];
   memset(pszName,0,n);
   WideCharToMultiByte(CP_ACP,0,bstrFunctionName,-1,pszName,n,0,0);
   char szError[MAX_PROPERTY_SIZE];
   memset(szError,0,sizeof(szError));
   sprintf(szError,"Invalid argument: %s(%lf)",pszName,badArgument);
   HWND hwnd = pParent -> hWnd();
   hwnd = GetDlgItem(hwnd,IDDIALOG_FUNCTION_VARIABLES);
   hwnd = GetDlgItem(hwnd,IDDI_FUNCTION_VARIABLES_TABS);
   hwnd = GetDlgItem(hwnd,IDDIALOG_FUNCTION_ERRORS);
   HWND hwndButton = GetDlgItem(hwnd,IDDI_FUNCTION_ERRORS_CLEAR);
   hwnd = GetDlgItem(hwnd,IDDI_FUNCTION_ERRORS_BOX);
   SendMessage(hwnd,LB_ADDSTRING,0L,(LPARAM)szError);
   EnableWindow(hwndButton,TRUE);
   delete [] pszName;
   return S_OK;
   }