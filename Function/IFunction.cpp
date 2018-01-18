/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/
#include <windows.h>
#include <commctrl.h>
#include <process.h>

#include "gmessage.h"
#include "Graphic_resource.h"

#include "Function.h"
#include "List.cpp"

#include "IGSFunctioNaterEvents.h"

   long __stdcall Function::GetTypeInfoCount(UINT *i) { 
   *i = 1; 
   return S_OK; 
   } 
  
 		
   long __stdcall Function::GetTypeInfo(UINT itinfo,LCID lcid,ITypeInfo **pptinfo) { 
   *pptinfo = NULL; 
   if ( itinfo != 0 ) 
      return DISP_E_BADINDEX; 
   *pptinfo = pITypeInfo_IGSFunctioNater;
   pITypeInfo_IGSFunctioNater -> AddRef();
   return S_OK; 
   } 
  
 
   long __stdcall Function::GetIDsOfNames(REFIID riid,OLECHAR **rgszNames,UINT cNames,LCID lcid,DISPID *rgdispid) { 
   return pITypeInfo_IGSFunctioNater -> GetIDsOfNames(rgszNames,cNames,rgdispid);
   } 
 
       
   long __stdcall Function::Invoke(DISPID dispidMember,
                            REFIID riid,
                            LCID lcid,
                            WORD wFlags, 
                            DISPPARAMS *pdispparams,
                            VARIANT *pvarResult,
                            EXCEPINFO *pexcepinfo, 
                            UINT *puArgErr) { 
   IDispatch* ppv;
   QueryInterface(IID_IDispatch,reinterpret_cast<void**>(&ppv));
   HRESULT hr = pITypeInfo_IGSFunctioNater -> Invoke(ppv,dispidMember,wFlags,pdispparams,pvarResult,pexcepinfo,puArgErr);
   ppv -> Release();
   return hr;
   } 
 
 
   HRESULT Function::put_Expression(BSTR newExpression) {
   memset(expression,0,sizeof(expression));
   WideCharToMultiByte(CP_ACP,0,newExpression,-1,expression,sizeof(expression),0,0);
   SetDlgItemText(hwndSpecDialog,IDDI_FUNCTION_EQUATION_ENTRY,expression);
   if ( pIPropertyNotifySink )
      pIPropertyNotifySink -> OnChanged(DISPID_UNKNOWN);
   resize();
   //if ( pWhenChangedCallback )
   //   pWhenChangedCallback(pWhenChangedCallbackArg);
   InvalidateRect(hwndSpecDialog,NULL,TRUE);
   return S_OK;
   }
 
 
   HRESULT Function::get_Expression(BSTR *getExpression) {
   *getExpression = SysAllocStringLen(NULL,strlen(expression) + 1);
   memset(*getExpression,0,strlen(expression) + 1);
   MultiByteToWideChar(CP_ACP,0,expression,-1,*getExpression,strlen(expression) + 1);
   return S_OK;
   }
 
   
   HRESULT Function::put_ExpressionLabel(BSTR bstrExpressionLabel) {
   WideCharToMultiByte(CP_ACP,0,bstrExpressionLabel,-1,expressionLabel,sizeof(expressionLabel),0,0);
   if ( pIPropertyNotifySink )
      pIPropertyNotifySink -> OnChanged(DISPID_UNKNOWN);
   resize();
   //if ( pWhenChangedCallback )
   //   pWhenChangedCallback(pWhenChangedCallbackArg);
   return S_OK;
   }
   HRESULT Function::get_ExpressionLabel(BSTR *pbstrExpressionLabel) {
   *pbstrExpressionLabel = SysAllocStringLen(NULL,strlen(expressionLabel) + 1);
   memset(*pbstrExpressionLabel,0,(strlen(expressionLabel) + 1) * sizeof(OLECHAR));
   MultiByteToWideChar(CP_ACP,0,expressionLabel,-1,*pbstrExpressionLabel,strlen(expressionLabel) + 1);
   return S_OK;
   }
 
   
   HRESULT Function::put_ResultsLabel(BSTR bstrResultsLabel) {
   WideCharToMultiByte(CP_ACP,0,bstrResultsLabel,-1,resultsLabel,sizeof(resultsLabel),0,0);
   if ( pIPropertyNotifySink )
      pIPropertyNotifySink -> OnChanged(DISPID_UNKNOWN);
   resize();
   //if ( pWhenChangedCallback )
   //   pWhenChangedCallback(pWhenChangedCallbackArg);
   return S_OK;
   }
   HRESULT Function::get_ResultsLabel(BSTR *pbstrResultsLabel) {
   *pbstrResultsLabel = SysAllocStringLen(NULL,strlen(resultsLabel) + 1);
   memset(*pbstrResultsLabel,0,(strlen(resultsLabel) + 1) * sizeof(OLECHAR));
   MultiByteToWideChar(CP_ACP,0,resultsLabel,-1,*pbstrResultsLabel,strlen(resultsLabel) + 1);
   return S_OK;
   }
 
   
   HRESULT Function::put_AllowPropertySettings(VARIANT_BOOL bAllowPropertySettings) {
   allowUserProperties = bAllowPropertySettings;
   resize();
   if ( pIPropertyNotifySink )
      pIPropertyNotifySink -> OnChanged(DISPID_UNKNOWN);
   //if ( pWhenChangedCallback )
   //   pWhenChangedCallback(pWhenChangedCallbackArg);
   return S_OK;
   }
   HRESULT Function::get_AllowPropertySettings(VARIANT_BOOL* pbAllowPropertySettings) {
   if ( ! pbAllowPropertySettings ) return E_POINTER;
   *pbAllowPropertySettings = allowUserProperties;
   return S_OK;
   }
 
   
   HRESULT Function::put_AllowControlVisibilitySettings(VARIANT_BOOL bAllowControlVisibilitySettings) {
   allowUserPropertiesControls = bAllowControlVisibilitySettings;
   if ( pIPropertyNotifySink )
      pIPropertyNotifySink -> OnChanged(DISPID_UNKNOWN);
   resize();
   //if ( pWhenChangedCallback )
   //   pWhenChangedCallback(pWhenChangedCallbackArg);
   return S_OK;
   }
   HRESULT Function::get_AllowControlVisibilitySettings(VARIANT_BOOL* pbAllowControlVisibilitySettings) {
   if ( ! pbAllowControlVisibilitySettings ) 
      return E_POINTER;
   *pbAllowControlVisibilitySettings = allowUserPropertiesControls;
   return S_OK;
   }
 
   
   HRESULT Function::put_ShowVariables(VARIANT_BOOL bShowVariables) {
   variablesVisible = bShowVariables;
   if ( pIPropertyNotifySink )
      pIPropertyNotifySink -> OnChanged(DISPID_UNKNOWN);
   resize();
   //if ( pWhenChangedCallback )
   //   pWhenChangedCallback(pWhenChangedCallbackArg);
   return S_OK;
   }
   HRESULT Function::get_ShowVariables(VARIANT_BOOL* pbShowVariables) {
   if ( ! pbShowVariables ) return E_POINTER;
   *pbShowVariables = variablesVisible;
   return S_OK;
   }
 

   HRESULT Function::put_ShowControls(VARIANT_BOOL bShowControls) {
   controlsVisible = bShowControls;
   if ( pIPropertyNotifySink )
      pIPropertyNotifySink -> OnChanged(DISPID_UNKNOWN);
   if ( bShowControls ) {
      put_ShowStart(bShowControls);
      put_ShowPause(bShowControls);
      put_ShowResume(bShowControls);
      put_ShowStop(bShowControls);
   }
   resize();
   //if ( pWhenChangedCallback )
   //   pWhenChangedCallback(pWhenChangedCallbackArg);
   return S_OK;
   }
 
 
   HRESULT Function::get_ShowControls(VARIANT_BOOL* pbShowControls) {
   if ( ! pbShowControls ) return E_POINTER;
   *pbShowControls = controlsVisible;
   return S_OK;
   }
 
   
   HRESULT Function::put_ShowStart(VARIANT_BOOL bShowStart) {
   startVisible = bShowStart;
   if ( pIPropertyNotifySink )
      pIPropertyNotifySink -> OnChanged(DISPID_UNKNOWN);
   resize();
   //if ( pWhenChangedCallback )
   //   pWhenChangedCallback(pWhenChangedCallbackArg);
   return S_OK;
   }
 
 
   HRESULT Function::get_ShowStart(VARIANT_BOOL* pbShowStart) {
   if ( ! pbShowStart ) return E_POINTER;
   *pbShowStart = startVisible;
   return S_OK;
   }
 
   
   HRESULT Function::put_ShowPause(VARIANT_BOOL bShowPause) {
   pauseVisible = bShowPause;
   if ( pIPropertyNotifySink )
      pIPropertyNotifySink -> OnChanged(DISPID_UNKNOWN);
   resize();
   //if ( pWhenChangedCallback )
   //   pWhenChangedCallback(pWhenChangedCallbackArg);
   return S_OK;
   }
 
 
   HRESULT Function::get_ShowPause(VARIANT_BOOL* pbShowPause) {
   if ( ! pbShowPause ) return E_POINTER;
   *pbShowPause = pauseVisible;
   return S_OK;
   }
 
   
   HRESULT Function::put_ShowResume(VARIANT_BOOL bShowResume) {
   resumeVisible = bShowResume;
   if ( pIPropertyNotifySink )
      pIPropertyNotifySink -> OnChanged(DISPID_UNKNOWN);
   resize();
   //if ( pWhenChangedCallback )
   //   pWhenChangedCallback(pWhenChangedCallbackArg);
   return S_OK;
   }
 
 
   HRESULT Function::get_ShowResume(VARIANT_BOOL* pbShowResume) {
   if ( ! pbShowResume ) return E_POINTER;
   *pbShowResume = resumeVisible;
   return S_OK;
   }
 
   
   HRESULT Function::put_ShowStop(VARIANT_BOOL bShowStop) {
   stopVisible = bShowStop;
   if ( pIPropertyNotifySink )
      pIPropertyNotifySink -> OnChanged(DISPID_UNKNOWN);
   resize();
   //if ( pWhenChangedCallback )
   //   pWhenChangedCallback(pWhenChangedCallbackArg);
   return S_OK;
   }
 
 
   HRESULT Function::get_ShowStop(VARIANT_BOOL* pbShowStop) {
   if ( ! pbShowStop ) return E_POINTER;
   *pbShowStop = stopVisible;
   return S_OK;
   }

 
   HRESULT Function::put_ShowResults(VARIANT_BOOL bShowResults) {
   resultsVisible = bShowResults;
   if ( pIPropertyNotifySink )
      pIPropertyNotifySink -> OnChanged(DISPID_UNKNOWN);
   resize();
   //if ( pWhenChangedCallback )
   //   pWhenChangedCallback(pWhenChangedCallbackArg);
   return S_OK;
   }
 
 
   HRESULT Function::get_ShowResults(VARIANT_BOOL* pbShowResults) {
   if ( ! pbShowResults ) return E_POINTER;
   *pbShowResults = resultsVisible;
   return S_OK;
   }

 
   HRESULT Function::put_ShowExpression(VARIANT_BOOL bShowExpression) {
   expressionVisible = bShowExpression;
   if ( pIPropertyNotifySink )
      pIPropertyNotifySink -> OnChanged(DISPID_UNKNOWN);
   resize();
   //if ( pWhenChangedCallback )
   //   pWhenChangedCallback(pWhenChangedCallbackArg);
   return S_OK;
   }
 
 
   HRESULT Function::get_ShowExpression(VARIANT_BOOL* pbShowExpression) {
   if ( ! pbShowExpression ) return E_POINTER;
   *pbShowExpression = expressionVisible;
   return S_OK;
   }

 
   HRESULT Function::put_DefaultMinValue(BSTR defMin) {
   if ( bstrDefaultMinValue ) 
      SysFreeString(bstrDefaultMinValue);
   bstrDefaultMinValue = SysAllocString(defMin);
   return S_OK;
   }
   HRESULT Function::get_DefaultMinValue(BSTR* pdefMin) {
   if ( ! pdefMin ) return E_POINTER;
   *pdefMin = SysAllocString(bstrDefaultMinValue);
   return S_OK;
   }

   HRESULT Function::put_DefaultMaxValue(BSTR defMax) {
   if ( bstrDefaultMaxValue ) 
      SysFreeString(bstrDefaultMaxValue);
   bstrDefaultMaxValue = SysAllocString(defMax);
   return S_OK;
   }
   HRESULT Function::get_DefaultMaxValue(BSTR* pdefMax) {
   if ( ! pdefMax ) return E_POINTER;
   *pdefMax = SysAllocString(bstrDefaultMaxValue);
   return S_OK;
   }

   HRESULT Function::put_DefaultStepCount(long defStepCount) {
   defaultStepCount = defStepCount;
   return S_OK;
   }
   HRESULT Function::get_DefaultStepCount(long* pdefStepCount) {
   if ( ! pdefStepCount ) return E_POINTER;
   *pdefStepCount = defaultStepCount;
   return S_OK;
   }

   
   HRESULT Function::DefineIndependentVariable(BSTR variableName,BSTR minValue,BSTR maxValue,long stepCount) {

   long n = wcslen(variableName) + 1;
   char* szVariableName = new char[n];
   memset(szVariableName,0,n);
   WideCharToMultiByte(CP_ACP,0,variableName,-1,szVariableName,n,0,0);

   IVariable* v;

   if ( expectEvalUndefines ) {

      if ( ! (v = pVariableList -> Get(szVariableName)) ) 
         v = pVariableList -> Add(szVariableName);
   
      v -> put_IEvaluator(evaluator);
      v -> put_IsIndependent(TRUE);
      v -> put_MinValueExpression(minValue);
      v -> put_MaxValueExpression(maxValue);
      v -> put_StepCount(stepCount);

      hideAllTabs();

      showVariableTab(v,szVariableName);

      delete [] szVariableName;

      return S_OK;

   }

   if ( ! (v = pManuallyAddedVariables -> Get(szVariableName)) ) 
      if ( ! (v = pVariableList -> Get(szVariableName)) ) 
         v = pManuallyAddedVariables -> Add(szVariableName);

   v -> put_IEvaluator(evaluator);
   v -> put_IsIndependent(TRUE);
   v -> put_MinValueExpression(minValue);
   v -> put_MaxValueExpression(maxValue);
   v -> put_StepCount(stepCount);

   hideAllTabs();

   showVariableTab(v,szVariableName);

   delete [] szVariableName;

   return S_OK;
   }


   HRESULT Function::DefineSimpleVariable(BSTR variableName,VARIANT variableValue) {

   long n = wcslen(variableName) + 1;
   char* szVariableName = new char[n];
   memset(szVariableName,0,n);
   WideCharToMultiByte(CP_ACP,0,variableName,-1,szVariableName,n,0,0);

   IVariable* vNew = NULL;
   IVariable* vExisted = pManuallyAddedVariables -> Get(szVariableName);
   if ( ! vExisted ) {
      if ( ! (vExisted = pVariableList -> Get(szVariableName)) ) {
         vNew = pManuallyAddedVariables -> Add(szVariableName);
         vNew -> put_IEvaluator(evaluator);
         vNew -> put_IsIndependent(FALSE);
         if ( variableValue.vt != VT_BSTR ) 
            VariantChangeType(&variableValue,&variableValue,0,VT_BSTR);
         vNew -> put_ValueExpression(variableValue.bstrVal);
      }
   }

   hideAllTabs();

   if ( vExisted ) 
      showVariableTab(vExisted,szVariableName);
   else
      showVariableTab(vNew,szVariableName);

   delete [] szVariableName;

   return S_OK;
   }


   HRESULT Function::UndefineVariable(BSTR bstrVariableName) {

   long n = wcslen(bstrVariableName) + 1;
   char* szVariableName = new char[n];
   memset(szVariableName,0,n);
   WideCharToMultiByte(CP_ACP,0,bstrVariableName,-1,szVariableName,n,0,0);

   IVariable* vOld;
   if ( ! (vOld = pManuallyAddedVariables -> Get(szVariableName)) ) {
      if ( ! (vOld = pVariableList -> Get(szVariableName)) )
         return S_OK;
      pVariableList -> Remove(vOld,&currentShowingVariable);
   } else
      pManuallyAddedVariables -> Remove(vOld,&currentShowingVariable);

   currentShowingVariable = pManuallyAddedVariables -> GetFirst();
   if ( ! currentShowingVariable ) 
      currentShowingVariable = pVariableList -> GetFirst();

   if ( currentShowingVariable )
      SendMessage(hwndVariablesTab,TCM_SETCURSEL,2,0L);
   else
      SendMessage(hwndVariablesTab,TCM_SETCURSEL,0,0L);

   HWND hwndV;
   IVariable* v = NULL;
   while ( v = pManuallyAddedVariables -> GetNext(v) ) {
      v -> get_HWND(&hwndV);
      ShowWindow(hwndV,SW_HIDE);
   }
   while ( v = pVariableList -> GetNext(v) ) {
      v -> get_HWND(&hwndV);
      ShowWindow(hwndV,SW_HIDE);
   }
   v = pVariableList -> GetLast();
   if ( ! v )
      v = pManuallyAddedVariables -> GetLast();
   if ( v ) {
      n = SendMessage(hwndVariablesTab,TCM_GETITEMCOUNT,0L,0L);
      SendMessage(hwndVariablesTab,TCM_SETCURSEL,n - 1,0L);
      v -> get_HWND(&hwndV);
      ShowWindow(hwndV,SW_SHOW);
   } else {
      SendMessage(hwndVariablesTab,TCM_SETCURSEL,0L,0L);
      ShowWindow(GetDlgItem(hwndVariablesTab,IDDI_FUNCTION_VARIABLES_DOMAIN_BOX),SW_SHOW);
   }

   return S_OK;
   }


   HRESULT Function::Start() {
#ifdef MULTITHREADED_EVALUATOR
   long k = 0;
   while ( pIConnectionPoint -> connections[k].pUnk ) {
      CoMarshalInterface(pIStream_Marshalling,IID_IDispatch,pIConnectionPoint -> connections[k].pUnk,MSHCTX_LOCAL,0,MSHLFLAGS_TABLESTRONG);
      k++;
   }
   DWORD threadID;
   CreateThread(NULL,0,evaluate,(void *)this,0,&threadID);
#else
   evaluate((void *)this);
#endif
   return S_OK;
   }
 
 
   HRESULT Function::Pause() {
   fire_Paused();
   evaluator -> Suspend();
   EnableWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_PAUSE),FALSE);
   EnableWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_RESUME),TRUE);
   return S_OK;
   }
 
   
   HRESULT Function::Resume() {
   fire_Resumed();
   evaluator -> Resume();
   EnableWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_RESUME),FALSE);
   EnableWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_PAUSE),TRUE);
   return S_OK;
   }
 
   
   HRESULT Function::Stop() {
   evaluator -> Terminate();
   EnableWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_EQUATION_ENTRY),TRUE);
   EnableWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_START),TRUE);
   EnableWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_PAUSE),FALSE);
   EnableWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_RESUME),FALSE);
   EnableWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_STOP),FALSE);
   fire_Finished();
   return S_OK;
   }


   HRESULT Function::put_Enabled(VARIANT_BOOL setUserMode) {
   if ( pIPropertyNotifySink )
      pIPropertyNotifySink -> OnChanged(DISPID_UNKNOWN);
   return S_OK;
   }
   HRESULT Function::get_Enabled(VARIANT_BOOL* pEnabled) {
   if ( ! pEnabled ) return E_POINTER;
   *pEnabled = true;
   return S_OK;
   }


   HRESULT Function::PersistFrom(BSTR bstrFileName) {
   iProperties -> OpenFile(bstrFileName);
   return S_OK;
   }


   HRESULT Function::PersistTo(BSTR bstrFileName) {
   iProperties -> SaveTo(bstrFileName);
   return S_OK;
   }


   HRESULT Function::parseExpression() {

   IUnknown* pIUnknown;
   DWORD dwConnectionCookie;
   IGSFunctioNaterEvents *pIGSFunctioNaterEvents = new _IGSFunctioNaterEvents(this);
   pIGSFunctioNaterEvents -> QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknown));
   pIConnectionPoint -> Advise(pIUnknown,&dwConnectionCookie);
   defineFunction(expression);
   pIConnectionPoint -> Unadvise(dwConnectionCookie);
   pIUnknown -> Release();

   return S_OK;
   }

   STDMETHODIMP Function::EditProperties() {
   IUnknown *pUnknown;
   QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pUnknown));
   iProperties -> ShowProperties(NULL,pUnknown);
   pUnknown -> Release();
   if ( pWhenChangedCallback )
      pWhenChangedCallback(pWhenChangedCallbackArg,whenChangedCallbackCookie);
   return S_OK;
   }

   STDMETHODIMP Function::put_IPlot(void *pvIPlot) {
   if ( pIPlot )
      pIPlot -> Release();
   pIPlot = (IPlot *)pvIPlot;
   if ( pIPlot )
      pIPlot -> AddRef();
   return S_OK;
   }

   STDMETHODIMP Function::get_IPlot(void **ppvIPlot) {
   if ( ! ppvIPlot )
      return E_POINTER;
   *ppvIPlot = (void *)pIPlot;
   return S_OK;
   }

   STDMETHODIMP Function::put_IDataSet(void *pvIDataSet) {
   if ( pIDataSet )
      pIDataSet -> Release();
   pIDataSet = (IDataSet *)pvIDataSet;
   if ( pIDataSet )
      pIDataSet -> AddRef();
   return S_OK;
   }

   STDMETHODIMP Function::get_IDataSet(void **ppvIDataSet) {
   if ( ! ppvIDataSet )
      return E_POINTER;
   *ppvIDataSet = (void *)pIDataSet;
   return S_OK;
   }

   STDMETHODIMP Function::Initialize(IDataSet* pIDataSet_Domain,IOpenGLImplementation *pIOpenGLImplementation,
                                          IGProperty* pIPropertyLineColor,IGProperty* pIPropertyLineWeight,
                                          IGProperty *parentPropertyPlotView,
                                          IGProperty *parentPropertyDefault2DPlotSubType,
                                          IGProperty *parentPropertyDefault3DPlotSubType,
                                          IGProperty *parentPropertyBackgroundColor,
                                          IGProperty *parentPropertyXFloor,IGProperty *parentPropertyXCeiling,
                                          IGProperty *parentPropertyYFloor,IGProperty *parentPropertyYCeiling,
                                          IGProperty *parentPropertyZFloor,IGProperty *parentPropertyZCeiling,
                                          void (__stdcall *pCallback)(void *,ULONG_PTR),void *pArg,ULONG_PTR callbackCookie) {

   pIPlot -> put_IDataSet(pIDataSet);

   pIDataSet -> put_IPlot(pIPlot);

   pWhenChangedCallback = pCallback;
   pWhenChangedCallbackArg = pArg;
   whenChangedCallbackCookie = callbackCookie;

   HRESULT rc = pIPlot -> Initialize(pIDataSet_Domain,pIOpenGLImplementation,evaluator,pIPropertyLineColor,pIPropertyLineWeight,parentPropertyPlotView,parentPropertyDefault2DPlotSubType,parentPropertyDefault3DPlotSubType,
                                       parentPropertyBackgroundColor,
                                       parentPropertyXFloor,parentPropertyXCeiling,
                                       parentPropertyYFloor,parentPropertyYCeiling,
                                       parentPropertyZFloor,parentPropertyZCeiling,
                                       pCallback,pArg,callbackCookie);

   pIDataSet -> put_IsFunctionSource(TRUE);
   BSTR bstrExpression;
   get_Expression(&bstrExpression);
   pIDataSet -> put_DataSource(bstrExpression);
   SysFreeString(bstrExpression);

   return rc;
   }

   STDMETHODIMP Function::put_OnChangeCallback(void (__stdcall *pCallback)(void *,ULONG_PTR),void *pArg,ULONG_PTR callbackCookie) {
   pWhenChangedCallback = pCallback;
   pWhenChangedCallbackArg = pArg;
   whenChangedCallbackCookie = callbackCookie;
   return S_OK;
   }

   HRESULT Function::get_AnyControlVisible(VARIANT_BOOL* pbAnyVisible) {
   if ( ! pbAnyVisible )
      return E_POINTER;
   *pbAnyVisible = anyVisible() ? TRUE : FALSE;
   return S_OK;
   }

   STDMETHODIMP Function::AdviseGSystemStatusBar(IGSystemStatusBar* p) {
   if ( pIDataSet )
      pIDataSet -> AdviseGSystemStatusBar(p);
   if ( pIPlot )
      pIPlot -> AdviseGSystemStatusBar(p);
   if ( ! p ) {
      if ( ! pIGSystemStatusBar ) 
         return E_UNEXPECTED;
      pIGSystemStatusBar = NULL;
   }
   pIGSystemStatusBar = p;
   return S_OK;
   }

   STDMETHODIMP Function::AdviseGSGraphicServices(void *pvGraphicServices) {
   if ( pIDataSet )
      pIDataSet -> AdviseGSGraphicServices(pvGraphicServices);
   if ( pIPlot )
      pIPlot -> AdviseGSGraphicServices(pvGraphicServices);
   if ( ! pvGraphicServices ) {
      if ( ! pIGSGraphicServices ) 
         return E_UNEXPECTED;
      pIGSGraphicServices = NULL;
   }
   pIGSGraphicServices = (IGSGraphicServices *)pvGraphicServices;
   return S_OK;
   }