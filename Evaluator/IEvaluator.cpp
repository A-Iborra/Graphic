// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Evaluator.h"
#include <process.h>

#include "Utils.h"
#include "list.cpp"


   long __stdcall Evaluator::QueryInterface(REFIID riid,void **ppv) {
   *ppv = NULL; 
 
   if ( riid == IID_IUnknown )
      *ppv = static_cast<void*>(this); 
   else
 
      if ( riid == IID_IEvaluator ) 
         *ppv = static_cast<IEvaluator *>(this);
      else
 
      if ( riid == IID_IDispatch ) 
         *ppv = static_cast<IDispatch *>(this);
      else
 
      if ( riid == IID_IConnectionPointContainer ) 
         *ppv = static_cast<IConnectionPointContainer *>(this);
      else

      if ( riid == IID_IConnectionPoint ) 
         *ppv = static_cast<IConnectionPoint *>(&connectionPoint);
      else

         return E_NOINTERFACE;
 
   static_cast<IUnknown*>(*ppv) -> AddRef(); 
   return S_OK; 
   }
 
   unsigned long __stdcall Evaluator::AddRef() {
   return ++refCount;
   }
 
   unsigned long __stdcall Evaluator::Release() {
   if ( --refCount == 0 ) {
      delete this;
      return 0;
   }
   return refCount;
   }

   long __stdcall Evaluator::GetTypeInfoCount(UINT *i) { 
   *i = 1; 
   return S_OK; 
   } 
  
 		
   long __stdcall Evaluator::GetTypeInfo(UINT itinfo,LCID lcid,ITypeInfo **pptinfo) { 
   *pptinfo = NULL; 
   if ( itinfo != 0 ) 
      return DISP_E_BADINDEX; 
   *pptinfo = pITypeInfo_IEvaluator;
   pITypeInfo_IEvaluator -> AddRef();
   return S_OK; 
   } 
  
 
   long __stdcall Evaluator::GetIDsOfNames(REFIID riid,OLECHAR **rgszNames,UINT cNames,LCID lcid,DISPID *rgdispid) { 
   return DispGetIDsOfNames(pITypeInfo_IEvaluator, rgszNames, cNames, rgdispid); 
   } 
 
       
   long __stdcall Evaluator::Invoke(DISPID dispidMember,
                            REFIID riid,
                            LCID lcid,
                            WORD wFlags, 
                            DISPPARAMS *pdispparams,
                            VARIANT *pvarResult,
                            EXCEPINFO *pexcepinfo, 
                            UINT *puArgErr) { 
   void *ppv;
   QueryInterface(IID_IDispatch,&ppv);
   HRESULT hr = pITypeInfo_IEvaluator -> Invoke(ppv,dispidMember,wFlags,pdispparams,pvarResult,pexcepinfo,puArgErr);
   return hr;
   }


   HRESULT Evaluator::get_Eval(BSTR bstrExpression,double* pTheResult) {
   long n = (DWORD)wcslen(bstrExpression) + 1;
   char* pszInput = new char[n];
   memset(pszInput,0,n);
   WideCharToMultiByte(CP_ACP,0,bstrExpression,-1,pszInput,n,0,0);
   IEvaluator *pIEvaluator;
   QueryInterface(IID_IEvaluator,reinterpret_cast<void **>(&pIEvaluator));
   if ( pTheResult )
      *pTheResult = evalConsume(pIEvaluator,pszInput);
   else
      evalConsume(pIEvaluator,pszInput);
   pIEvaluator -> Release();
   delete [] pszInput;
   return S_OK;
   }

 
   HRESULT Evaluator::Evaluate_szExpression(char* szExpression,double* pTheResult) {
   if ( ! pTheResult ) return E_POINTER;
   *pTheResult = eval(szExpression);
   return S_OK;
   }

   
   HRESULT Evaluator::Consume_szExpression(char* szExpression,double* pTheResult) {
   if ( ! pTheResult ) return E_POINTER;
   int n = (DWORD)strlen(szExpression);
   if ( 0 == n ) {
      *pTheResult = 0.0;
      return S_OK;
   }
   char* externalTrack = new char[strlen(szExpression) + 1];
   char *p = externalTrack;
   strcpy(externalTrack,szExpression);
   *pTheResult = eval(szExpression,&externalTrack);
   strcpy(szExpression,externalTrack);
   delete [] p;
   return S_OK;
   }


   HRESULT Evaluator::UndefineVariable(IVariable *v) {
   if ( ! v ) return E_POINTER;
   Remove(v);
   return S_OK;
   }
 
 
   HRESULT Evaluator::DefineVariable(IVariable *v) {
   if ( ! v ) return E_POINTER;
   BSTR bstrName = SysAllocStringLen(NULL,64);
   char szName[64];
   memset(bstrName,0,sizeof(bstrName));
   v -> get_Name(&bstrName);
   memset(szName,0,sizeof(szName));
   WideCharToMultiByte(CP_ACP,0,bstrName,-1,szName,64,0,0);
   lowercase(szName);
   if ( Get(szName) == NULL )
      Add(v,szName);
   SysFreeString(bstrName);
   return S_OK;
   }
 
 
   HRESULT Evaluator::UndefineAllVariables() {
   IVariable *v = (IVariable *)NULL;
   while ( v = GetFirst() )
      Remove(v);

   while ( v = variableList.GetFirst() ) {
      v -> Release();
      variableList.Remove(v);
   }

   return S_OK;
   }
 

   HRESULT Evaluator::IsVariable(BSTR bstrName,BOOL* isVariable) {
   if ( ! isVariable ) return E_POINTER;
   long n = (DWORD)wcslen(bstrName) + 1;
   char *pszName = new char[n];
   memset(pszName,0,n);
   WideCharToMultiByte(CP_ACP,0,bstrName,-1,pszName,n,0,0);
   IVariable* v = Get(pszName);
   delete [] pszName;
   *isVariable = (v != NULL);
   return S_OK;
   }

   
   HRESULT Evaluator::Terminate() {
   terminateNow = TRUE;
   ReleaseMutex(threadControl);
   return S_OK;
   }


   HRESULT Evaluator::Suspend() {
   WaitForSingleObject(threadControl,INFINITE);
   return S_OK;
   }


   HRESULT Evaluator::Resume() {
   ReleaseMutex(threadControl);
   return S_OK;
   }


   HRESULT Evaluator::IsBusy() {
   if ( WAIT_OBJECT_0 == WaitForSingleObject(functionIsBusy,0) ) {
      ReleaseMutex(functionIsBusy);
      return S_FALSE;
   }
   return S_OK;
   }


   HRESULT Evaluator::Evaluate_bstrExpression(BSTR bstrExpression,double* pTheResult) {
   if ( ! pTheResult ) return E_POINTER;
   long n = (DWORD)wcslen(bstrExpression) + 1;
   char *pszExpression = new char[n];
   memset(pszExpression,0,n);
   WideCharToMultiByte(CP_ACP,0,bstrExpression,-1,pszExpression,n,0,0);
   *pTheResult = eval(pszExpression);
   delete [] pszExpression;
   return S_OK;
   }

