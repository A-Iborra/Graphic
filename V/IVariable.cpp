// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "V.h"

#include <CommCtrl.h>
#include <stdio.h>

#include "Graphic_resource.h"
#include "utils.h"

   long __stdcall V::QueryInterface(REFIID riid,void **ppv) {
   *ppv = NULL; 
  
   if ( riid == IID_IUnknown )
      *ppv = this; 
   else
  
   if ( riid == IID_IVariable ) 
      *ppv = static_cast<IVariable *>(this);
   else

   if ( riid == IID_IGPropertiesClient ) 
      *ppv = static_cast<IGPropertiesClient *>(this);
   else

   if ( riid == IID_IGPropertyPageClient ) 
      *ppv = static_cast<IGPropertyPageClient *>(this);
   else

   if ( riid == IID_IGPropertyClient )
      *ppv = static_cast<IGPropertyClient *>(this);
   else

   if ( riid == IID_ISpecifyPropertyPages )
      return iProperties -> QueryInterface(IID_ISpecifyPropertyPages,ppv);
   else
  
   if ( riid == IID_IPersistStorage ) 
      return iProperties -> QueryInterface(IID_IPersistStorage,ppv);
   else
 
   if ( riid == IID_IPersistStreamInit )
      return iProperties -> QueryInterface(IID_IPersistStreamInit,ppv);
   else
 
   if ( riid == IID_IPersistStream )
      return iProperties -> QueryInterface(IID_IPersistStream,ppv);
   else

   if ( riid == IID_IGProperties )
      return iProperties -> QueryInterface(IID_IGProperties,ppv);
   else
 
      return E_NOINTERFACE;
  
   static_cast<IUnknown*>(*ppv) -> AddRef(); 
   return S_OK; 
   }
  
   unsigned long __stdcall V::AddRef() {
   return ++refCount;
   }
  
   unsigned long __stdcall V::Release() {
   if ( --refCount == 0 ) {
      delete this;
      return 0;
   }
   return refCount;
   }
  
  
   long __stdcall V::GetTypeInfoCount(UINT *i) { 
   *i = 1; 
   return S_OK; 
   } 
  
 		
   long __stdcall V::GetTypeInfo(UINT itinfo,LCID lcid,ITypeInfo **pptinfo) { 
   *pptinfo = NULL; 
   if ( itinfo != 0 ) 
      return DISP_E_BADINDEX; 
   *pptinfo = pITypeInfo_IVariable;
   pITypeInfo_IVariable -> AddRef();
   return S_OK; 
   } 
  
 
   long __stdcall V::GetIDsOfNames(REFIID riid,OLECHAR **rgszNames,UINT cNames,LCID lcid,DISPID *rgdispid) { 
   return DispGetIDsOfNames(pITypeInfo_IVariable, rgszNames, cNames, rgdispid); 
   } 
 
       
   long __stdcall V::Invoke(DISPID dispidMember,
                            REFIID riid,
                            LCID lcid,
                            WORD wFlags, 
                            DISPPARAMS *pdispparams,
                            VARIANT *pvarResult,
                            EXCEPINFO *pexcepinfo, 
                            UINT *puArgErr) { 
   void *ppv;
   QueryInterface(IID_IDispatch,&ppv);
   return pITypeInfo_IVariable -> Invoke(ppv,dispidMember,wFlags,pdispparams,pvarResult,pexcepinfo,puArgErr);
   }


   HRESULT V::put_Value(double v) {
   value.doubleValue = v;
   return S_OK;
   }


   HRESULT V::get_Value(double* pv) {
   if ( ! pv ) return E_POINTER;
   if ( hwndDialog && ! isIndependentVariable ) {
      long n = (long)SendMessage(GetDlgItem(hwndDialog,IDDI_VARIABLE_VALUE),WM_GETTEXTLENGTH,0L,0L);
      char* pszTemp = new char[n + 1];
      SendMessage(GetDlgItem(hwndDialog,IDDI_VARIABLE_VALUE),WM_GETTEXT,n + 1,(LPARAM)pszTemp);
      EVALUATE_SZ(iEvaluator,pszTemp,value.doubleValue)
      delete [] pszTemp;
   }
   *pv = value.doubleValue;
   return S_OK;
   }


   HRESULT V::put_ValueExpression(BSTR strExpression) {
   memset(valueExpression,0,sizeof(valueExpression));
   WideCharToMultiByte(CP_ACP,0,strExpression,-1,valueExpression,sizeof(valueExpression),0,0);
   if ( hwndDialog ) 
      SetDlgItemText(hwndDialog,IDDI_VARIABLE_VALUE,valueExpression);
   EVALUATE_SZ(iEvaluator,valueExpression,value.doubleValue)
   getDomain(hwndDialog);
   return S_OK;
   }
   HRESULT V::get_ValueExpression(BSTR* pstrExpression) {
   if ( ! pstrExpression ) return E_POINTER;
   *pstrExpression = SysAllocStringLen(NULL,(DWORD)strlen(valueExpression) + 1);
   MultiByteToWideChar(CP_ACP,0,valueExpression,-1,*pstrExpression,(DWORD)strlen(valueExpression) + 1);
   return S_OK;
   }


   HRESULT V::put_Name(BSTR bstrName) {
   memset(name,0,sizeof(name));
   WideCharToMultiByte(CP_ACP,0,bstrName,-1,name,sizeof(name),0,0);
   char * pszTemp = new char[(DWORD)strlen(name) + 10];
   sprintf(pszTemp,"<= %s <= ",name);

   SetWindowText(GetDlgItem(hwndProperties,IDDI_VARIABLE_DOMAIN_NAME),pszTemp);

   delete [] pszTemp;

   long steps = atol(stepExpression);

   if ( 0 == steps )
      steps = 10;

   sprintf(stepExpression,"%s",stepCount);
   sprintf(stepExpression,"%s = %s + (%s - %s)/%ld",name,name,maxValue,minValue,steps);

   if ( hwndTabControl ) {
      TC_ITEM tie;
      memset(&tie,0,sizeof(TC_ITEM));
      tie.mask = TCIF_TEXT; 
      tie.pszText = name;
      tie.cchTextMax = (DWORD)strlen(name);
      SendMessage(hwndTabControl,TCM_SETITEM,(WPARAM)tabIndex,(LPARAM)&tie);
   }
   getDomain(hwndDialog);
   return S_OK;
   }


   HRESULT V::get_Name(BSTR* pbstrName) {
   *pbstrName = SysAllocStringLen(NULL,(DWORD)strlen(name) + 1);
   MultiByteToWideChar(CP_ACP,0,name,-1,*pbstrName,(DWORD)strlen(name) + 1);
   return S_OK;
   }

   HRESULT V::put_IsIndependent(BOOL ind) {
   isIndependentVariable = ind;
   if ( isIndependentVariable ) {
      memset(minValue,0,sizeof(minValue));
      memset(maxValue,0,sizeof(maxValue));
      memset(stepCount,0,sizeof(stepCount));
   }
   getDomain(hwndDialog);
   return S_OK;
   }
   HRESULT V::get_IsIndependent(BOOL* pis) {
   if ( ! pis ) return E_POINTER;
   *pis = isIndependentVariable;
   return S_OK;
   }


   HRESULT V::put_DomainExpression(BSTR bstrDomainExpression) {
   memset(stepCount,0,sizeof(stepCount));
   WideCharToMultiByte(CP_ACP,0,bstrDomainExpression,-1,stepCount,sizeof(stepCount),0,0);
   SetDlgItemText(hwndDialog,IDDI_VARIABLE_DOMAIN_EXPRESSION,stepCount);
   getDomain(hwndDialog);
   return S_OK;
   }
   HRESULT V::get_DomainExpression(BSTR* pbstrDomainExpression) {
   if ( ! pbstrDomainExpression ) return E_POINTER;
   *pbstrDomainExpression = SysAllocStringLen(NULL,(DWORD)strlen(stepCount) + 1);
   MultiByteToWideChar(CP_ACP,0,stepCount,-1,*pbstrDomainExpression,(DWORD)strlen(stepCount) + 1);
   return S_OK;
   }


   HRESULT V::put_MinValueExpression(BSTR bstrMin) {
   memset(minValue,0,sizeof(minValue));
   WideCharToMultiByte(CP_ACP,0,bstrMin,-1,minValue,sizeof(minValue),0,0);
   SetDlgItemText(hwndDialog,IDDI_VARIABLE_DOMAIN_MIN,minValue);
   iEvaluator -> get_Eval(bstrMin,&startVal);
   getDomain(hwndDialog);
   return S_OK;
   }
   HRESULT V::get_MinValueExpression(BSTR* pbstrDomainExpression) {
   *pbstrDomainExpression = SysAllocStringLen(NULL,(DWORD)strlen(minValue) + 1);
   MultiByteToWideChar(CP_ACP,0,minValue,-1,*pbstrDomainExpression,(DWORD)strlen(minValue) + 1);
   getDomain(hwndDialog);
   return S_OK;
   }


   HRESULT V::put_MaxValueExpression(BSTR bstrMax) {
   memset(maxValue,0,sizeof(maxValue));
   WideCharToMultiByte(CP_ACP,0,bstrMax,-1,maxValue,sizeof(maxValue),0,0);
   SetDlgItemText(hwndDialog,IDDI_VARIABLE_DOMAIN_MAX,maxValue);
   EVALUATE_SZ(iEvaluator,maxValue,endVal)
   getDomain(hwndDialog);
   return S_OK;
   }
   HRESULT V::get_MaxValueExpression(BSTR* pbstrMax) {
   *pbstrMax = SysAllocStringLen(NULL,(DWORD)strlen(maxValue) + 1);
   MultiByteToWideChar(CP_ACP,0,maxValue,-1,*pbstrMax,(DWORD)strlen(maxValue) + 1);
   return S_OK;
   }


   HRESULT V::put_StepCount(long count) {
   memset(stepCount,0,sizeof(stepCount));
   sprintf(stepCount,"%ld",count);
   sprintf(stepExpression,"%s = %s + (%s - %s)/%s",name,name,maxValue,minValue,stepCount);
   SetDlgItemText(hwndDialog,IDDI_VARIABLE_DOMAIN_EXPRESSION,stepCount);
   getDomain(hwndDialog);
   return S_OK;
   }
   HRESULT V::get_StepCount(long* count) {
   if ( ! count ) return E_POINTER;
   *count = atol(stepCount);
   return S_OK;
   }


   HRESULT V::put_IEvaluator(IDispatch* pNewEvaluator) {
   if ( ! pNewEvaluator ) return E_POINTER;
   if ( iEvaluator )
      iEvaluator -> Release();
   pNewEvaluator -> QueryInterface(IID_IEvaluator,reinterpret_cast<void**>(&iEvaluator));
   return S_OK;
   }
   HRESULT V::get_IEvaluator(IDispatch** ppEvaluator) {
   if ( ! ppEvaluator ) return E_POINTER;
   iEvaluator -> QueryInterface(IID_IEvaluator,reinterpret_cast<void**>(ppEvaluator));
   return S_OK;
   }


   HRESULT V::put_TabIndex(long theIndex) {
   tabIndex = theIndex;
   return S_OK;
   }
   HRESULT V::get_TabIndex(long* theIndex) {
   if ( ! theIndex ) return E_POINTER;
   *theIndex = tabIndex;
   return S_OK;
   }
 
   
   HRESULT V::get_MinValue(double* pValue) {
   if ( ! pValue ) return E_POINTER;
   *pValue = startVal;
   return S_OK;
   }
 
   
   HRESULT V::get_MaxValue(double* pValue) {
   if ( ! pValue ) return E_POINTER;
   *pValue = endVal;
   return S_OK;
   }
 
 
   HRESULT V::get_HWND(HWND* pHwnd) {
   if ( ! pHwnd ) return E_POINTER;
   *pHwnd = hwndDialog;
   return S_OK;
   }
 
 
   HRESULT V::get_IsLess(double firstValue,double secondValue,short* theAnswer) {
   if ( ! theAnswer ) return E_POINTER;
   if ( startVal < endVal ) 
      *theAnswer = (short)isLessConventional(firstValue,secondValue);
   else
      *theAnswer = (short)isLessBackward(firstValue,secondValue);
   return S_OK;
   }


   HRESULT V::Start() {
   value.doubleValue = startVal;
   return S_OK;
   }

   
   HRESULT V::Step() {
   iEvaluator -> Evaluate_szExpression(stepExpression,&value.doubleValue);
   return S_OK;
   }


   HRESULT V::CopyFrom(IVariable* vSourceInterface) {
   V *vSource = static_cast<V*>(vSourceInterface);

   if ( vSource == this ) return S_OK;

   propertiesHaveChanged = vSource -> propertiesHaveChanged;

   isIndependentVariable = vSource -> isIndependentVariable;

   strcpy(name,vSource -> name);
   startVal = vSource -> startVal;
   value.doubleValue = vSource -> value.doubleValue;
   stepVal = vSource -> stepVal;
   endVal = vSource -> endVal;

   strcpy(minValue,vSource -> minValue);
   strcpy(maxValue,vSource -> maxValue);
   strcpy(stepCount,vSource -> stepCount);
   strcpy(stepExpression,vSource -> stepExpression);
   strcpy(valueExpression,vSource -> valueExpression);

   getDomain(hwndDialog);

   return S_OK;
   }
 
 
   HRESULT V::SetHwnds(HWND hwndParent,HWND hwndTab) {

   hwndOwner = hwndParent;
   hwndTabControl = hwndTab;

   if ( ! hwndDialog ) 
      initWindows();

   if ( name[0] ) {
      char szTemp[MAX_PROPERTY_SIZE];
      sprintf(szTemp,"<= %s <= ",name);
      SetWindowText(GetDlgItem(hwndDialog,IDDI_VARIABLE_DOMAIN_NAME),szTemp);
   } else
      SetWindowText(GetDlgItem(hwndDialog,IDDI_VARIABLE_DOMAIN_NAME),"");

   pIProperty_minValue -> setWindowItemText(hwndDialog,IDDI_VARIABLE_DOMAIN_MIN);
   pIProperty_stepCount -> setWindowItemText(hwndDialog,IDDI_VARIABLE_STEPS);
   pIProperty_stepExpression -> setWindowItemText(hwndDialog,IDDI_VARIABLE_DOMAIN_EXPRESSION);
   pIProperty_maxValue -> setWindowItemText(hwndDialog,IDDI_VARIABLE_DOMAIN_MAX);
   pIProperty_valueExpression -> setWindowItemText(hwndDialog,IDDI_VARIABLE_VALUE);
   pIProperty_isIndependentVariable -> setWindowItemChecked(hwndDialog,IDDI_VARIABLE_INDEPENDENT_VARIABLE);
 
   getDomain(hwndDialog);

   return S_OK;
   }


   HRESULT V::InitializeDomain() {
   getDomain(hwndDialog);
   return S_OK;
   }


   HRESULT V::Edit() {
   DialogBoxParam(gsVariables_hModule,MAKEINTRESOURCE(IDDIALOG_VARIABLE_PROPERTIES),hwndOwner,(DLGPROC)propertiesHandler,(LPARAM)this);
   return S_OK;
   }