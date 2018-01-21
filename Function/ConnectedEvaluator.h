// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "Properties_i.h"
#include "Variable_i.h"
#include "Evaluator_i.h"
#include "Function.h"

   class Function;

   class _IEvaluatorEvents : public IEvaluatorEvents {
   public:

      _IEvaluatorEvents(Function* pp) : pParent(pp) {};
      ~_IEvaluatorEvents() {};

      STDMETHOD(QueryInterface)(REFIID riid,void **ppv);
      STDMETHOD_ (ULONG, AddRef)();
      STDMETHOD_ (ULONG, Release)();

      STDMETHOD(GetTypeInfoCount)(UINT *pctinfo);
      STDMETHOD(GetTypeInfo)(UINT itinfo, LCID lcid, ITypeInfo **pptinfo);
      STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid);
      STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pdispparams, VARIANT *pvarResult, EXCEPINFO *pexcepinfo, UINT *puArgErr);

      STDMETHOD(Started)(long expectedIterations);
      STDMETHOD(TakeResultString)(long iterationCount,char *pszNames,char *pszResults);
      STDMETHOD(UnknownVariable)(BSTR variableName);
      STDMETHOD(UnknownFunction)(BSTR functionName);
      STDMETHOD(MathError)(BSTR functionName,double argument);
      STDMETHOD(Finished)();
      STDMETHOD(Clear)();
      STDMETHOD(DivideByZero)();
      STDMETHOD(InvalidArgument)(BSTR functionName,double badArgument);

   private:

      Function* pParent;

   };


   class ConnectedEvaluator : public IEvaluator { 
   public:
 
      ConnectedEvaluator(Function* pParent);
      ~ConnectedEvaluator();
 
      STDMETHOD(QueryInterface)(REFIID riid,void **ppv);
      STDMETHOD_ (ULONG, AddRef)();
      STDMETHOD_ (ULONG, Release)();
 
      STDMETHOD(GetTypeInfoCount)(UINT *pctinfo);
      STDMETHOD(GetTypeInfo)(UINT itinfo, LCID lcid, ITypeInfo **pptinfo);
      STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid);
      STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pdispparams, VARIANT *pvarResult, EXCEPINFO *pexcepinfo, UINT *puArgErr);
 
    // IEvaluator
 
      HRESULT STDMETHODCALLTYPE get_Eval(BSTR theExpression,double* theResult);
 
      HRESULT STDMETHODCALLTYPE Evaluate_szExpression(char* szExpression,double* ptheResult);
      HRESULT STDMETHODCALLTYPE Consume_szExpression(char *szExpression,double* pTheResult);
      HRESULT STDMETHODCALLTYPE Evaluate_szFunction(char* szFunction);
      HRESULT STDMETHODCALLTYPE UndefineVariable(IVariable *v);
      HRESULT STDMETHODCALLTYPE DefineVariable(IVariable *v);
      HRESULT STDMETHODCALLTYPE UndefineAllVariables();
      HRESULT STDMETHODCALLTYPE IsVariable(BSTR,BOOL*);
      HRESULT STDMETHODCALLTYPE Terminate();
      HRESULT STDMETHODCALLTYPE Suspend();
      HRESULT STDMETHODCALLTYPE Resume();
      HRESULT STDMETHODCALLTYPE IsBusy();
      HRESULT STDMETHODCALLTYPE Evaluate_bstrExpression(BSTR bstrExpression,double*);

   private:
 
      _IEvaluatorEvents evaluatorEvents;
 
      Function* pParent;
 
      IEvaluator* evaluator;
 
      static IClassFactory* pIClassFactory_Evaluator;

      IConnectionPoint* pIConnectionPoint;
      DWORD dwConnectionCookie;
 
      friend class _IEvaluatorEvents;
 
   };