// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <ocidl.h>
#include <math.h>

#include "list.h"
#include "Properties_i.h"
#include "Variable_i.h"
#include "Evaluator_i.h"

  extern HMODULE gsEvaluator_hModule;
  extern char gsEvaluator_szModuleName[1024];
  extern BSTR gsEvaluator_wstrModuleName;
  extern ITypeInfo *pITypeInfo_IEvaluator;

#define MAX_CONNECTION_POINT  8
#define ALLOC_CONNECTIONS     8

  class Evaluator : public List<IVariable>, public IConnectionPointContainer, public IEvaluator {

  public:

     Evaluator();
     Evaluator(IUnknown* pUnkOuter);
	  ~Evaluator();

     double eval(char*,char** externalTrack = (char **)NULL);

   // IUnknown

     HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid,void **ppv);
     ULONG STDMETHODCALLTYPE AddRef();
     ULONG STDMETHODCALLTYPE Release();

   // IDispatch

     STDMETHOD (GetTypeInfoCount)(UINT *pctinfo);
     STDMETHOD (GetTypeInfo)(UINT itinfo, LCID lcid, ITypeInfo **pptinfo);
     STDMETHOD (GetIDsOfNames)(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid);
     STDMETHOD (Invoke)(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pdispparams, VARIANT *pvarResult, EXCEPINFO *pexcepinfo, UINT *puArgErr);

   // IEvaluator

     HRESULT STDMETHODCALLTYPE get_Eval(BSTR theExpression,double* theResult);

     HRESULT STDMETHODCALLTYPE Evaluate_szExpression(char* szExpression,double*);
     HRESULT STDMETHODCALLTYPE Consume_szExpression(char* szExpression,double*);
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

//      IConnectionPointContainer

     STDMETHOD (FindConnectionPoint)(REFIID riid,IConnectionPoint **);
     STDMETHOD (EnumConnectionPoints)(IEnumConnectionPoints **);

     void Fire_Started(long expectedIterations);
     void Fire_TakeResultString(long iterationCount,char* names,char* results);
     void Fire_UnknownVariable(char* variableName);
     void Fire_UnknownFunction(char* functionName);
     void Fire_MathError(char* functionName,double argument);
     void Fire_Finished();
     void Fire_DivideByZero();
     void Fire_InvalidArgument(char* functionName,double invalidValue);

  private:

     struct _IConnectionPoint : IConnectionPoint {
	  public:

        STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
        STDMETHOD_ (ULONG, AddRef)();
        STDMETHOD_ (ULONG, Release)();

        STDMETHOD (GetConnectionInterface)(IID *);
        STDMETHOD (GetConnectionPointContainer)(IConnectionPointContainer **ppCPC);
        STDMETHOD (Advise)(IUnknown *pUnk,DWORD *pdwCookie);
   	  STDMETHOD (Unadvise)(DWORD);
        STDMETHOD (EnumConnections)(IEnumConnections **ppEnum);

        _IConnectionPoint(Evaluator *pp);
		  ~_IConnectionPoint();
		  IUnknown *AdviseSink() { return adviseSink; };

     private:

        int getSlot();
        int findSlot(DWORD dwCookie);

		  IUnknown *adviseSink;
		  Evaluator *pParent;
        DWORD nextCookie;
		  int countConnections,countLiveConnections;

        CONNECTDATA *connections;

     } connectionPoint;

	  struct _IEnumConnectionPoints : IEnumConnectionPoints {
	  public:

        STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
        STDMETHOD_ (ULONG, AddRef)();
        STDMETHOD_ (ULONG, Release)();

 	     STDMETHOD (Next)(ULONG cConnections,IConnectionPoint **rgpcn,ULONG *pcFetched);
        STDMETHOD (Skip)(ULONG cConnections);
        STDMETHOD (Reset)();
        STDMETHOD (Clone)(IEnumConnectionPoints **);

	     _IEnumConnectionPoints(Evaluator *pp,_IConnectionPoint **cp,int connectionPointCount);
       ~_IEnumConnectionPoints();

     private:

        int cpCount,enumeratorIndex;
		  Evaluator *pParent;
		  _IConnectionPoint **connectionPoints;

     } *enumConnectionPoints;

     struct _IEnumerateConnections : public IEnumConnections {
     public:

        _IEnumerateConnections(IUnknown* pParentUnknown,ULONG cConnections,CONNECTDATA* paConnections,ULONG initialIndex);
        ~_IEnumerateConnections();

         STDMETHODIMP         QueryInterface(REFIID, void **);
         STDMETHODIMP_(ULONG) AddRef();
         STDMETHODIMP_(ULONG) Release();
         STDMETHODIMP         Next(ULONG, CONNECTDATA*, ULONG*);
         STDMETHODIMP         Skip(ULONG);
         STDMETHODIMP         Reset();
         STDMETHODIMP         Clone(IEnumConnections**);

     private:

        ULONG       refCount;
        IUnknown    *pParentUnknown;
        ULONG       enumeratorIndex;
        ULONG       countConnections;
        CONNECTDATA *connections;

     } *enumConnections;

     DOUBLE starSequence(char **);
     DOUBLE plusSequence(char **);
     DOUBLE handleFunction(char **);
     int doVar(char* pszNames,IVariable *first,IVariable *last);

     List<IVariable> variableList;
     static IClassFactory* pIClassFactory_variables;

     static _exception exception;
     unsigned long refCount;
     BOOL terminateNow;
     HANDLE threadControl,functionIsBusy;

     char *pszFunctionExpression;
     char *pszResultName;
     long functionIterationCount;

     friend int _matherr(_exception *exception);

  };
