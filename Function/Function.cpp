/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <olectl.h>
#include <CommCtrl.h>

#include "Graphic_resource.h"
#include "gmessage.h"
#include "Properties_i.h"

#include "Function.h"

#include "List.cpp"

   LONG APIENTRY CalcDimensions(HWND hwndParent,SIZEL *pResult);

   STDAPI GSystemsVariablesDllGetClassObject(HMODULE hModule,REFCLSID clsid,REFIID riid,void** ppObject);

   long Function::functionCount = 0;
   long oleMiscStatus = OLEMISC_ACTIVATEWHENVISIBLE | OLEMISC_SETCLIENTSITEFIRST | OLEMISC_INSIDEOUT | 
                           OLEMISC_CANTLINKINSIDE | OLEMISC_RECOMPOSEONRESIZE | OLEMISC_ALWAYSRUN;

   Function::Function(IUnknown* pUnkOuter) :
 
       variableToDeleteList(),

       hwndSpecDialog(0),
       hwndProperties(0),
       hwndVariables(0),
       hwndVariablesTab(0),
       hwndPropertiesVisibility(0),
       hwndErrors(0),

       resultingWidth(0),
       resultingHeight(0),

       allowUserProperties(false),
       allowUserPropertiesControls(false),
       askedForInPlaceObject(false),
       isDesignMode(false),

       pVariableList(NULL),
       pManuallyAddedVariables(NULL),
       pPropertyDialogVariableList(NULL),
       pCurrentVariableList(NULL),

       currentShowingVariable(NULL),

       iterationCount(0),

       enumConnectionPoints(0),
       enumConnections(0),

       pStorage(NULL),

       pIOleClientSite(NULL),
       pIOleInPlaceSite(NULL),
       pIOleInPlaceActiveObject(NULL),
       pIPropertyNotifySink(NULL),
       pIPropertyPlots(NULL),

       pIPlot(NULL),

       pOleAdviseHolder(NULL),
       pDataAdviseHolder(NULL),
       pAdviseSink(NULL),
       pIUnknownProperties(NULL),
       iProperties(NULL),

       pIStream_Marshalling(NULL),
       hglMarshalling(NULL),

       oldResultLabelHandler(NULL),
       oldExpressionLabelHandler(NULL),
       oldExpressionHandler(NULL),

       evaluator(NULL),

       expectEvalUndefines(FALSE),

       pIPropertyExpressionLabel(NULL),
       pIPropertyResultsLabel(NULL),

       pIPropertyPropertiesVisible(NULL),
       pIPropertyPropertiesControlVisibility(NULL),

       pIPropertyExpressionVisible(NULL),
       pIPropertyResultsVisible(NULL),
       pIPropertyVariablesVisible(NULL),
       pIPropertyControlsVisible(NULL),
       pIPropertyStartVisible(NULL),
       pIPropertyPauseVisible(NULL),
       pIPropertyResumeVisible(NULL),
       pIPropertyStopVisible(NULL),
       pIPropertyVariables(NULL),

       pIPropertyManuallyAddedVariables(NULL),

       userMode(FALSE),
       expressionVisible(FALSE),
       resultsVisible(FALSE),
       variablesVisible(FALSE),
       controlsVisible(FALSE),
       startVisible(FALSE),
       pauseVisible(FALSE),
       resumeVisible(FALSE),
       stopVisible(FALSE),
       plotPropertiesVisible(FALSE),
       enteringData(FALSE),
       stopAllProcessing(FALSE),

       freezeEvents(0),
       backgroundColor(0),
       foregroundColor(0),

       pWhenChangedCallback(NULL),
       pWhenChangedCallbackArg(NULL),

       refCount(0)
 
   {

   pIOleInPlaceActiveObject = new _IOleInPlaceActiveObject(this);

   refCount = 100;

   memset(expression,0,sizeof(expression));
   memset(expressionLabel,0,sizeof(expressionLabel));
   memset(resultsLabel,0,sizeof(resultsLabel));

   memset(&rectDialog,0,sizeof(RECT));
   memset(&rectLabel1,0,sizeof(RECT));
   memset(&rectExpression,0,sizeof(RECT));
   memset(&rectLabel2,0,sizeof(RECT));
   memset(&rectResults,0,sizeof(RECT));
   memset(&rectVariables,0,sizeof(RECT));
   memset(&rectControls,0,sizeof(RECT));
   memset(&rectProperties,0,sizeof(RECT));

   bstrDefaultMinValue = SysAllocString(L"-1");
   bstrDefaultMaxValue = SysAllocString(L"1");
   defaultStepCount = 10;

   ++functionCount;
 
   if ( ! VList::pIClassFactory_variables )
      CoGetClassObject(CLSID_Variable,CLSCTX_INPROC_SERVER,NULL,IID_IClassFactory,reinterpret_cast<void **>(&VList::pIClassFactory_variables));
      
   IUnknown* pIUnknownThis;

   QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknownThis));

   CoCreateInstance(CLSID_InnoVisioNateProperties,pIUnknownThis,CLSCTX_INPROC_SERVER,IID_IUnknown,reinterpret_cast<void **>(&pIUnknownProperties));

   pIUnknownProperties -> QueryInterface(IID_IGProperties,reinterpret_cast<void**>(&iProperties));

   pIUnknownThis -> Release();

   evaluator = new ConnectedEvaluator(this);

   pIConnectionPoint = new _IConnectionPoint(this,DIID_IGSFunctioNaterEvents);

   iProperties -> Add(L"variables",&pIPropertyVariables);
   pIPropertyVariables -> put_type(TYPE_OBJECT_STORAGE_ARRAY);

   iProperties -> Add(L"manually added variables",&pIPropertyManuallyAddedVariables);
   pIPropertyManuallyAddedVariables -> put_type(TYPE_OBJECT_STORAGE_ARRAY);

   iProperties -> Add(L"expression",&pIPropertyExpression);
   pIPropertyExpression -> directAccess(TYPE_SZSTRING,expression,sizeof(expression));

   iProperties -> Add(L"expression label",&pIPropertyExpressionLabel);
   iProperties -> Add(L"results label",&pIPropertyResultsLabel);

   iProperties -> Add(L"allow user properties",&pIPropertyPropertiesVisible);
   iProperties -> Add(L"allow user properties control",&pIPropertyPropertiesControlVisibility);

   iProperties -> Add(L"entry field visible",&pIPropertyExpressionVisible);
   iProperties -> Add(L"results field visible",&pIPropertyResultsVisible);
   iProperties -> Add(L"variables visible",&pIPropertyVariablesVisible);
   iProperties -> Add(L"controls visible",&pIPropertyControlsVisible);
   iProperties -> Add(L"start visible",&pIPropertyStartVisible);
   iProperties -> Add(L"pause visible",&pIPropertyPauseVisible);
   iProperties -> Add(L"resume visible",&pIPropertyResumeVisible);
   iProperties -> Add(L"stop visible",&pIPropertyStopVisible);

   iProperties -> Add(L"rect",NULL);
   iProperties -> DirectAccess(L"rect",TYPE_BINARY,&rectDialog,sizeof(RECT));

   iProperties -> Add(L"plot properties visible",&pIPropertyPlotPropertiesVisible);

   pIPropertyPropertiesVisible -> directAccess(TYPE_BOOL,&allowUserProperties,sizeof(allowUserProperties));
   pIPropertyPropertiesControlVisibility -> directAccess(TYPE_BOOL,&allowUserPropertiesControls,sizeof(allowUserPropertiesControls));

   pIPropertyExpressionVisible -> directAccess(TYPE_BOOL,&expressionVisible,sizeof(expressionVisible));
   pIPropertyResultsVisible -> directAccess(TYPE_BOOL,&resultsVisible,sizeof(resultsVisible));
   pIPropertyVariablesVisible -> directAccess(TYPE_BOOL,&variablesVisible,sizeof(variablesVisible));
   pIPropertyControlsVisible -> directAccess(TYPE_BOOL,&controlsVisible,sizeof(controlsVisible));
   pIPropertyStartVisible -> directAccess(TYPE_BOOL,&startVisible,sizeof(startVisible));
   pIPropertyPauseVisible -> directAccess(TYPE_BOOL,&pauseVisible,sizeof(pauseVisible));
   pIPropertyResumeVisible -> directAccess(TYPE_BOOL,&resumeVisible,sizeof(resumeVisible));
   pIPropertyStopVisible -> directAccess(TYPE_BOOL,&stopVisible,sizeof(stopVisible));
   pIPropertyPlotPropertiesVisible -> directAccess(TYPE_BOOL,&plotPropertiesVisible,sizeof(plotPropertiesVisible));

   pIPropertyExpressionLabel -> directAccess(TYPE_SZSTRING,expressionLabel,sizeof(expressionLabel));
   pIPropertyResultsLabel -> directAccess(TYPE_SZSTRING,resultsLabel,sizeof(resultsLabel));

   pVariableList = new VList(evaluator);
   pManuallyAddedVariables = new VList(evaluator);
   pCurrentVariableList = pVariableList;

   refCount = 1;

   IGPropertiesClient *pIPropertiesClient;

   QueryInterface(IID_IGPropertiesClient,reinterpret_cast<void **>(&pIPropertiesClient));
   if ( pIPropertiesClient ) {
      iProperties -> Advise(pIPropertiesClient);
      pIPropertiesClient -> Release();
   }

   IGPropertyPageClient* pIPropertyPageClient;

   QueryInterface(IID_IGPropertyPageClient,reinterpret_cast<void**>(&pIPropertyPageClient));
   iProperties -> AdvisePropertyPageClient(pIPropertyPageClient);
   pIPropertyPageClient -> Release();

   CreateStreamOnHGlobal(NULL,TRUE,&pIStream_Marshalling);
   GetHGlobalFromStream(pIStream_Marshalling,&hglMarshalling);

   HRESULT rc = CoCreateInstance(CLSID_Plot,NULL,CLSCTX_INPROC_SERVER,IID_IPlot,reinterpret_cast<void **>(&pIPlot));

   iProperties -> Add(L"plots",&pIPropertyPlots);

   //NTC: 12-14-2017: I am not sure why this InitNew was not in here prior to today. I have addeded it, though commented out.
   InitNew();

   refCount = 0;

   return;
   };
 

   Function::~Function() {

   if ( pIUnknownProperties )
      pIUnknownProperties -> Release();

   if ( pVariableList )
      delete pVariableList;

   if ( pManuallyAddedVariables )
      delete pManuallyAddedVariables;

   if ( pPropertyDialogVariableList )
      delete pPropertyDialogVariableList;

   if ( pIOleInPlaceSite ) 
      pIOleInPlaceSite -> Release();

   if ( pIOleClientSite ) 
      pIOleClientSite -> Release();

   if ( pIPropertyNotifySink ) 
      pIPropertyNotifySink -> Release();

   if ( pAdviseSink ) 
      pAdviseSink -> Release();

   if ( pIPlot )
      pIPlot -> Release();

#if 0
   for ( IVariable **ppv : variableToDeleteList )
      (*ppv) -> Release();
#endif

   variableToDeleteList.clear();

   if ( hwndSpecDialog ) {
      DestroyWindow(hwndSpecDialog);
      SetWindowLong(hwndSpecDialog,GWL_USERDATA,0);  
   }

   if ( hwndVariables ) 
      DestroyWindow(hwndVariables);
   if ( hwndProperties ) 
      DestroyWindow(hwndProperties);
   if ( hwndPropertiesVisibility) 
      DestroyWindow(hwndPropertiesVisibility);

   if ( iProperties ) 
      iProperties -> Release();

   if ( enumConnectionPoints ) 
      delete enumConnectionPoints;

   delete evaluator;

   if ( bstrDefaultMinValue ) 
      SysFreeString(bstrDefaultMinValue);

   if ( bstrDefaultMaxValue ) 
      SysFreeString(bstrDefaultMaxValue);

   delete pIConnectionPoint;

   if ( pIStream_Marshalling ) 
      pIStream_Marshalling -> Release();

   functionCount--;

   }
 
 
   int Function::initWindows() {

   HWND hwndOwner = NULL;
   RECT rect;
   char szTemp[MAX_PROPERTY_SIZE];
 
   HRESULT hr = pIOleInPlaceSite -> GetWindow(&hwndOwner);

   if ( ! hwndOwner )
      return 0;

   DLGTEMPLATE *dt = (DLGTEMPLATE *)LoadResource(hModule,FindResource(hModule,MAKEINTRESOURCE(IDDIALOG_FUNCTION_SPEC),RT_DIALOG));

   hwndSpecDialog = CreateDialogIndirectParam(hModule,dt,hwndOwner,(DLGPROC)functionDialogHandler,(LPARAM)this);

   dt = (DLGTEMPLATE *)LoadResource(hModule,FindResource(hModule,MAKEINTRESOURCE(IDDIALOG_FUNCTION_VARIABLES),RT_DIALOG));
   hwndVariables = CreateDialogIndirectParam(hModule,dt,hwndSpecDialog,(DLGPROC)functionVariablesHandler,(LPARAM)this);
   SetWindowLongPtr(hwndVariables,GWLP_ID,(ULONG_PTR)IDDIALOG_FUNCTION_VARIABLES);
   hwndVariablesTab = GetDlgItem(hwndVariables,IDDI_FUNCTION_VARIABLES_TABS);

   TC_ITEM tie;
   tie.mask = TCIF_TEXT;
   tie.pszText = "Help";
   tie.cchTextMax = 8;
   TabCtrl_InsertItem(hwndVariablesTab,1,&tie);

   tie.pszText = "Errors";
   tie.cchTextMax = 8;
   TabCtrl_InsertItem(hwndVariablesTab,2,&tie);

   SendMessage(hwndVariablesTab,TCM_GETITEMRECT,(WPARAM)0,(LPARAM)&rect);
   SetParent(GetDlgItem(hwndVariables,IDDI_FUNCTION_VARIABLES_DOMAIN_BOX),hwndVariablesTab);
   SetWindowPos(GetDlgItem(hwndVariablesTab,IDDI_FUNCTION_VARIABLES_DOMAIN_BOX),HWND_TOP,(long)(0.5*(double)rect.bottom),(long)(1.5*(double)rect.bottom),0,0,SWP_NOSIZE);
   LoadString((HINSTANCE)hModule,IDSTRING_FUNCTION_INSTRUCTIONS,szTemp,MAX_PROPERTY_SIZE);
   SetDlgItemText(hwndVariablesTab,IDDI_FUNCTION_VARIABLES_DOMAIN_BOX,szTemp);

   dt = (DLGTEMPLATE *)LoadResource(hModule,FindResource(hModule,MAKEINTRESOURCE(IDDIALOG_FUNCTION_ERRORS),RT_DIALOG));
   hwndErrors = CreateDialogIndirectParam(hModule,dt,hwndVariablesTab,(DLGPROC)functionVariablesHandler,(LPARAM)this);
   SetWindowLong(hwndErrors,GWL_ID,IDDIALOG_FUNCTION_ERRORS);
   SendMessage(hwndVariablesTab,TCM_GETITEMRECT,(WPARAM)0,(LPARAM)&rect);
   SetWindowPos(hwndErrors,HWND_TOP,(long)(0.5*(double)rect.bottom),(long)(1.5*(double)rect.bottom),0,0,SWP_NOSIZE);

   GetWindowRect(hwndSpecDialog,&rectDialog);
   GetWindowRect(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_EXPRESSION_LABEL),&rectLabel1);
   subtractRect(&rectLabel1,&rectDialog,&rectLabel1);

   GetWindowRect(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_EQUATION_ENTRY),&rectExpression);
   subtractRect(&rectExpression,&rectDialog,&rectExpression);

   GetWindowRect(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_RESULT_LABEL),&rectLabel2);
   subtractRect(&rectLabel2,&rectDialog,&rectLabel2);

   GetWindowRect(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_RESULT),&rectResults);
   subtractRect(&rectResults,&rectDialog,&rectResults);

   GetWindowRect(hwndVariables,&rectVariables);
   subtractRect(&rectVariables,&rectDialog,&rectVariables);

   GetWindowRect(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_START),&rectControls);
   subtractRect(&rectControls,&rectDialog,&rectControls);

   GetWindowRect(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_PROPERTIES),&rectProperties);
   subtractRect(&rectProperties,&rectDialog,&rectProperties);

   GetWindowRect(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_PAUSE),&rect);

   controlSpacing = rect.left - rectControls.right - rectDialog.left;

   //SetWindowPos(hwndSpecDialog,HWND_TOP,0,0,0,0,SWP_NOMOVE | SWP_SHOWWINDOW);
 
   pIPropertyExpression -> setWindowItemText(hwndSpecDialog,IDDI_FUNCTION_EQUATION_ENTRY);

   pVariableList -> SetHwnds(hwndVariablesTab,hwndVariablesTab);

   pManuallyAddedVariables -> SetHwnds(hwndVariablesTab,hwndVariablesTab);

   Loaded();

#ifdef FUNCTION_SAMPLE
   if ( trialExpired ) {
      EnableWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_START),FALSE);
      EnableWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_PAUSE),FALSE);
      EnableWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_RESUME),FALSE);
      EnableWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_STOP),FALSE);
   }
#endif

   SetWindowLongPtr(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_RESULT_LABEL),GWLP_USERDATA,(ULONG_PTR)this);
   oldResultLabelHandler = (WNDPROC)SetWindowLongPtr(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_RESULT_LABEL),GWLP_WNDPROC,(ULONG_PTR)functionDialogHandler);

   SetWindowLongPtr(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_EXPRESSION_LABEL),GWLP_USERDATA,(ULONG_PTR)this);
   oldExpressionLabelHandler = (WNDPROC)SetWindowLongPtr(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_EXPRESSION_LABEL),GWLP_WNDPROC,(ULONG_PTR)functionDialogHandler);

   SetWindowLongPtr(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_EQUATION_ENTRY),GWLP_USERDATA,(ULONG_PTR)this);
   oldExpressionHandler = (WNDPROC)SetWindowLongPtr(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_EQUATION_ENTRY),GWLP_WNDPROC,(ULONG_PTR)expressionHandler);

   return 0;
   }


   static int okayToResize = true;

   long widthOf(HWND hwnd) {
   RECT rc;
   GetWindowRect(hwnd,&rc);
   return rc.right - rc.left;
   }

   long rightEdge(HWND hwndParent,HWND hwnd) {
   RECT rc,rcParent;
   GetWindowRect(hwndParent,&rcParent);
   GetWindowRect(hwnd,&rc);
   return rc.right - rcParent.left;
   }

   long bottomEdge(HWND hwndParent,HWND hwnd) {
   RECT rc,rcParent;
   GetWindowRect(hwndParent,&rcParent);
   GetWindowRect(hwnd,&rc);
   return rc.bottom - rcParent.top;
   }

   long rightEdge(HWND hwndParent,long id) {
   RECT rc,rcParent;
   GetWindowRect(hwndParent,&rcParent);
   GetWindowRect(GetDlgItem(hwndParent,id),&rc);
   return rc.right - rcParent.left;
   }

   long bottomEdge(HWND hwndParent,long id) {
   RECT rc,rcParent;
   GetWindowRect(hwndParent,&rcParent);
   GetWindowRect(GetDlgItem(hwndParent,id),&rc);
   return rc.bottom - rcParent.top;
   }

   int Function::resize() {

   if ( ! okayToResize )
      return 0;

   int gapRight,deltay = 0,deltax = 0;
   int expressionLabelDeltay = 0;
   int resultsLabelDeltay = 0;
   bool anyControlVisible = false;

   resultingHeight = 0;
   resultingWidth = 0;

   gapRight = 8;

   RECT rc;
   GetWindowRect(hwndVariables,&rc);

   long cxMax = rc.right - rc.left + 2 * gapRight;

   long currentBottom = 0;
   
   if ( allowUserProperties ) {

      if ( ! expressionLabel[0] || ! expressionVisible )
         expressionLabelDeltay = 14;
      else 
         expressionLabelDeltay = 0;

      SetWindowPos(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_PROPERTIES),HWND_TOP,rectProperties.left,rectProperties.top - expressionLabelDeltay,0,0,SWP_NOSIZE | SWP_SHOWWINDOW);

      if ( ! expressionLabel[0] || ! expressionVisible )
         ShowWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_EXPRESSION_LABEL),SW_HIDE);
      else {
         long x = rightEdge(hwndSpecDialog,IDDI_FUNCTION_PROPERTIES) + 4;
         SetWindowPos(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_EXPRESSION_LABEL),HWND_TOP,x,rectLabel1.top,cxMax - x - 2 * gapRight,rectLabel1.bottom - rectLabel1.top,SWP_SHOWWINDOW);
         resultingWidth = max(resultingWidth,rightEdge(hwndSpecDialog,IDDI_FUNCTION_EXPRESSION_LABEL));
      }

      if ( ! expressionVisible ) 
         ShowWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_EQUATION_ENTRY),SW_HIDE);
      else {

         long x = rightEdge(hwndSpecDialog,IDDI_FUNCTION_PROPERTIES) + 4;

         SetWindowPos(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_EQUATION_ENTRY),HWND_TOP,x,rectExpression.top - expressionLabelDeltay,cxMax - x - 2 * gapRight,rectExpression.bottom - rectExpression.top,SWP_SHOWWINDOW);

         resultingWidth = max(resultingWidth,rightEdge(hwndSpecDialog,IDDI_FUNCTION_PROPERTIES));
         resultingWidth = max(resultingWidth,rightEdge(hwndSpecDialog,IDDI_FUNCTION_EQUATION_ENTRY));
      }

      currentBottom = bottomEdge(hwndSpecDialog,IDDI_FUNCTION_PROPERTIES);

   } else {

      ShowWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_PROPERTIES),SW_HIDE);

      if ( ! expressionLabel[0] ) {
         expressionLabelDeltay = 14;
         ShowWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_EXPRESSION_LABEL),SW_HIDE);
      } else {
         expressionLabelDeltay = 0;
         SetWindowPos(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_EXPRESSION_LABEL),HWND_TOP,gapRight,rectLabel1.top,cxMax - 2 * gapRight,rectExpression.bottom - rectExpression.top,SWP_SHOWWINDOW);
         resultingWidth = max(resultingWidth,rightEdge(hwndSpecDialog,IDDI_FUNCTION_EXPRESSION_LABEL));
      }

      SetWindowPos(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_EQUATION_ENTRY),HWND_TOP,gapRight,rectExpression.top - expressionLabelDeltay,cxMax - 8 - 2 * gapRight,rectExpression.bottom - rectExpression.top,0L);
      resultingWidth = max(resultingWidth,rightEdge(hwndSpecDialog,IDDI_FUNCTION_EQUATION_ENTRY));

      currentBottom = bottomEdge(hwndSpecDialog,IDDI_FUNCTION_EQUATION_ENTRY);

   }

   if ( ! resultsLabel[0] || ! resultsVisible ) 
      ShowWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_RESULT_LABEL),SW_HIDE);

   if ( ! resultsVisible ) 
      ShowWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_RESULT),SW_HIDE);
   else {

      if ( resultsLabel[0] ) {
         SetWindowPos(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_RESULT_LABEL),HWND_TOP,gapRight,currentBottom + 4,cxMax - 2 * gapRight,rectExpression.bottom - rectExpression.top,SWP_SHOWWINDOW);
         currentBottom = bottomEdge(hwndSpecDialog,IDDI_FUNCTION_RESULT_LABEL);
         resultingWidth = max(resultingWidth,rightEdge(hwndSpecDialog,IDDI_FUNCTION_RESULT_LABEL));
      }

      SetWindowPos(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_RESULT),HWND_TOP,gapRight,currentBottom + 2,cxMax - 2 * gapRight,rectExpression.bottom - rectExpression.top,SWP_SHOWWINDOW);
      resultingWidth = max(resultingWidth,rightEdge(hwndSpecDialog,IDDI_FUNCTION_RESULT));
      currentBottom = bottomEdge(hwndSpecDialog,IDDI_FUNCTION_RESULT);
   }

   if ( ! variablesVisible ) {
      ShowWindow(hwndVariables,SW_HIDE);
      ShowWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_VARIABLES_DOMAIN_BOX),SW_HIDE);
   } else {
      SetWindowPos(hwndVariables,HWND_TOP,gapRight,currentBottom + 8,cxMax - 2 * gapRight,rectVariables.bottom - rectVariables.top,SWP_SHOWWINDOW);
      SetWindowPos(hwndVariablesTab,HWND_TOP,gapRight,currentBottom + 8,cxMax - 2 * gapRight,rectVariables.bottom - rectVariables.top,SWP_NOMOVE | SWP_SHOWWINDOW);
      ShowWindow(GetDlgItem(hwndVariablesTab,IDDI_FUNCTION_VARIABLES_DOMAIN_BOX),SW_SHOW);
      resultingWidth = max(resultingWidth,rightEdge(hwndSpecDialog,hwndVariables));
      resultingWidth = max(resultingWidth,rightEdge(hwndVariablesTab,IDDI_FUNCTION_VARIABLES_DOMAIN_BOX));
      currentBottom = bottomEdge(hwndSpecDialog,hwndVariables);
   }

   long currentRight = 0L;
   long controlsTop = currentBottom + 4;

   if ( ! controlsVisible || ! startVisible )
      ShowWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_START),SW_HIDE);
   else {
      SetWindowPos(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_START),HWND_TOP,gapRight,controlsTop,0,0,SWP_NOSIZE | SWP_SHOWWINDOW);
      currentRight = rightEdge(hwndSpecDialog,IDDI_FUNCTION_START);
      currentBottom = bottomEdge(hwndSpecDialog,IDDI_FUNCTION_START);
      resultingWidth = max(resultingWidth,rightEdge(hwndSpecDialog,IDDI_FUNCTION_START));
   }
      
   if ( ! controlsVisible || ! pauseVisible ) 
      ShowWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_PAUSE),SW_HIDE);
   else {
      SetWindowPos(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_PAUSE),HWND_TOP,currentRight + 8,controlsTop,0,0,SWP_NOSIZE | SWP_SHOWWINDOW);
      currentRight = rightEdge(hwndSpecDialog,IDDI_FUNCTION_PAUSE);
      currentBottom = bottomEdge(hwndSpecDialog,IDDI_FUNCTION_PAUSE);
      resultingWidth = max(resultingWidth,rightEdge(hwndSpecDialog,IDDI_FUNCTION_PAUSE));
   }

   if ( ! controlsVisible || ! resumeVisible ) 
      ShowWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_RESUME),SW_HIDE);
   else {
      SetWindowPos(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_RESUME),HWND_TOP,currentRight + 8,controlsTop,0,0,SWP_NOSIZE | SWP_SHOWWINDOW);
      currentRight = rightEdge(hwndSpecDialog,IDDI_FUNCTION_RESUME);
      currentBottom = bottomEdge(hwndSpecDialog,IDDI_FUNCTION_RESUME);
      resultingWidth = max(resultingWidth,rightEdge(hwndSpecDialog,IDDI_FUNCTION_RESUME));
   }

   if ( ! controlsVisible || ! stopVisible ) 
      ShowWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_STOP),SW_HIDE);
   else {
      SetWindowPos(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_STOP),HWND_TOP,currentRight + 8,controlsTop,0,0,SWP_NOSIZE | SWP_SHOWWINDOW);
      currentRight = rightEdge(hwndSpecDialog,IDDI_FUNCTION_STOP);
      currentBottom = bottomEdge(hwndSpecDialog,IDDI_FUNCTION_STOP);
      resultingWidth = max(resultingWidth,rightEdge(hwndSpecDialog,IDDI_FUNCTION_STOP));
   }

   if ( ! controlsVisible || ! plotPropertiesVisible ) 
      ShowWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_PLOT_PROPERTIES),SW_HIDE);
   else {
      SetWindowPos(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_PLOT_PROPERTIES),HWND_TOP,gapRight,currentBottom + 4,0,0,SWP_NOSIZE | SWP_SHOWWINDOW);
      currentBottom = bottomEdge(hwndSpecDialog,IDDI_FUNCTION_PLOT_PROPERTIES);
      resultingWidth = max(resultingWidth,rightEdge(hwndSpecDialog,IDDI_FUNCTION_PLOT_PROPERTIES));
   }

   resultingHeight = currentBottom;

   rectDialog.right = rectDialog.left + resultingWidth;
   rectDialog.bottom = rectDialog.top + resultingHeight;

   if ( ! okayToResize ) return 0;

   okayToResize = false;

   SetWindowPos(hwndSpecDialog,HWND_TOP,0,0,resultingWidth,resultingHeight,SWP_NOMOVE);

   if ( askedForInPlaceObject && pIOleInPlaceSite ) {
      pIOleInPlaceSite -> OnPosRectChange(&rectDialog);
   }

   okayToResize = true;

   return 0;
   }


   short Function::anyVisible() {
   for ( int k = 0; k < 3; k++ ) 
      if ( visibleItems[k] )
         return true;
   for ( int k = 4; k < 9; k++ )
      if ( controlsVisible && visibleItems[k] )
         return true; 
   return false;
   }


   int Function::hideAllTabs() {
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
   ShowWindow(GetDlgItem(hwndVariablesTab,IDDI_FUNCTION_VARIABLES_DOMAIN_BOX),SW_HIDE);
   ShowWindow(hwndErrors,SW_HIDE);
   return 0;
   }


   int Function::showVariableTab(IVariable* pV,char* pszVariableName) {
   if ( ! pV ) return 0;
   HWND hwndV;
   long n = SendMessage(hwndVariablesTab,TCM_GETITEMCOUNT,0L,0L);
   for ( long k = 0; k < n; k++) {
      TCITEM tcItem;
      char szName[MAX_PROPERTY_SIZE];
      memset(&tcItem,0,sizeof(TCITEM));
      tcItem.mask = TCIF_TEXT;
      tcItem.pszText = szName;
      tcItem.cchTextMax = MAX_PROPERTY_SIZE;
      SendMessage(hwndVariablesTab,TCM_GETITEM,k,(LPARAM)&tcItem);
      if ( strcmp(szName,pszVariableName) == 0 ) {
         SendMessage(hwndVariablesTab,TCM_SETCURSEL,k,0L);
         pV -> get_HWND(&hwndV);
         ShowWindow(hwndV,SW_SHOW);
         currentShowingVariable = pV;
         break;
      }
   }
   return 0;
   }