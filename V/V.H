// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#pragma once

#include "Properties_i.h"
#include "Variable_i.h"
#include "Evaluator_i.h"
#include "GSystem_i.h"
#include "List.h"

   class V : public IVariable,IGPropertiesClient,IGPropertyPageClient,IGPropertyClient {
   public:
 
      V();
      ~V();
 
//    IUnknown

      STDMETHOD(QueryInterface)(REFIID riid,void **ppv);
      STDMETHOD_(ULONG, AddRef)();
      STDMETHOD_(ULONG, Release)();

//    IDispatch

      STDMETHOD(GetTypeInfoCount)(UINT *pctinfo);
      STDMETHOD(GetTypeInfo)(UINT itinfo, LCID lcid, ITypeInfo **pptinfo);
      STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid);
      STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pdispparams, VARIANT *pvarResult, EXCEPINFO *pexcepinfo, UINT *puArgErr);

//    IVariable

      HRESULT STDMETHODCALLTYPE put_Value(double value);
      HRESULT STDMETHODCALLTYPE get_Value(double* value);
      HRESULT STDMETHODCALLTYPE put_ValueExpression(BSTR);
      HRESULT STDMETHODCALLTYPE get_ValueExpression(BSTR*);
      HRESULT STDMETHODCALLTYPE put_Name(BSTR);
      HRESULT STDMETHODCALLTYPE get_Name(BSTR*);
      HRESULT STDMETHODCALLTYPE put_IsIndependent(BOOL);
      HRESULT STDMETHODCALLTYPE get_IsIndependent(BOOL*);
      HRESULT STDMETHODCALLTYPE put_DomainExpression(BSTR);
      HRESULT STDMETHODCALLTYPE get_DomainExpression(BSTR*);
      HRESULT STDMETHODCALLTYPE put_MinValueExpression(BSTR);
      HRESULT STDMETHODCALLTYPE get_MinValueExpression(BSTR*);
      HRESULT STDMETHODCALLTYPE put_MaxValueExpression(BSTR);
      HRESULT STDMETHODCALLTYPE get_MaxValueExpression(BSTR*);
      HRESULT STDMETHODCALLTYPE put_StepCount(long);
      HRESULT STDMETHODCALLTYPE get_StepCount(long*);

      HRESULT STDMETHODCALLTYPE put_IEvaluator(IDispatch*);
      HRESULT STDMETHODCALLTYPE get_IEvaluator(IDispatch**);

      HRESULT STDMETHODCALLTYPE put_TabIndex(long tabIndex);
      HRESULT STDMETHODCALLTYPE get_TabIndex(long* tabIndex);

      HRESULT STDMETHODCALLTYPE get_MinValue(double*);
      HRESULT STDMETHODCALLTYPE get_MaxValue(double*);

      HRESULT STDMETHODCALLTYPE get_HWND(HWND*);
 
      HRESULT STDMETHODCALLTYPE get_IsLess(double firstValue,double secondValue,short* theAnswer);

      HRESULT STDMETHODCALLTYPE Start();
      HRESULT STDMETHODCALLTYPE Step();

      HRESULT STDMETHODCALLTYPE CopyFrom(IVariable*);

      HRESULT STDMETHODCALLTYPE SetHwnds(HWND hwndOwner,HWND hwndTabControl);

      HRESULT STDMETHODCALLTYPE InitializeDomain();

      HRESULT STDMETHODCALLTYPE Edit();

//    IPropertiesClient
 
      STDMETHOD(SavePrep)();
      STDMETHOD(InitNew)();
      STDMETHOD(Loaded)();
      STDMETHOD(Saved)();
      STDMETHOD(IsDirty)();
      STDMETHOD(GetClassID)(BYTE *pCLSID);

//    IPropertyPageClient
 
      STDMETHOD(BeforeAllPropertyPages)();
      STDMETHOD(GetPropertyPagesInfo)(long* countPages,SAFEARRAY** stringDescriptions,SAFEARRAY** stringHelpDirs,SAFEARRAY** pSize);
      STDMETHOD(CreatePropertyPage)(long,HWND,RECT*,BOOL,HWND* hwndPropertyPage);
      STDMETHOD(Apply)();
      STDMETHOD(IsPageDirty)(long,BOOL*);
      STDMETHOD(Help)(BSTR);
      STDMETHOD(TranslateAccelerator)(long,long*);
      STDMETHOD(AfterAllPropertyPages)(BOOL);
      STDMETHOD(DestroyPropertyPage)(long);

      STDMETHOD(GetPropertySheetHeader)(void *pHeader);
      STDMETHOD(get_PropertyPageCount)(long *pCount);
      STDMETHOD(GetPropertySheets)(void *pSheets);

//    IPropertyClient

      STDMETHOD(Changed)(IGProperty *);

   private:
 
      int getDomain(HWND hwndCurrent);
      int setDomainExpression(HWND hwndCurrent);

      char name[NAME_SIZE];
      tValue value;

      DOUBLE startVal,endVal,stepVal;
      HWND hwndDialog,hwndTabControl,hwndOwner;
      HWND hwndProperties;
      long tabIndex;
      BOOL propertiesHaveChanged;
      BOOL isIndependentVariable;

      IUnknown* pIUnknownProperties;
      IGProperties *iProperties;
      IEvaluator* iEvaluator;
      IGPropertyPageClient *iPropertyPageClient;
      IGPropertiesClient *iPropertiesClient;
      IGProperty *pIProperty_name,*pIProperty_valueExpression,*pIProperty_minValue,*pIProperty_maxValue;
      IGProperty *pIProperty_stepCount,*pIProperty_stepExpression,*pIProperty_isIndependentVariable;

      char minValue[MAX_PROPERTY_SIZE];
      char maxValue[MAX_PROPERTY_SIZE];
      char stepCount[MAX_PROPERTY_SIZE];
      char stepExpression[MAX_PROPERTY_SIZE];
      char valueExpression[MAX_PROPERTY_SIZE];

      unsigned int refCount;
 
      static LRESULT CALLBACK handler(HWND hwnd,UINT msg,WPARAM mp1,LPARAM mp2);
      static LRESULT CALLBACK propertiesHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

      int initWindows();
 
      friend class VList;

   };

   extern HMODULE gsVariables_hModule;
   extern char gsVariables_szModuleName[];
   extern BSTR gsVariables_wstrModuleName;
   extern ITypeInfo *pITypeInfo_IVariable;
