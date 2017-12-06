/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <olectl.h>
#include <CommCtrl.h>

#include "Graphic_resource.h"
#include "gmessage.h"
//#include "GSystemsProperties.h"
#include "Properties_i.h"

#include "Function.h"

#include "List.cpp"

#ifdef FUNCTION_SAMPLE

   extern bool trialExpired;

#endif

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

       allowUserProperties(false),
       allowUserPropertiesControls(false),
       askedForInPlaceObject(false),

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
       enteringData(FALSE),
       stopAllProcessing(FALSE),

       resultingHeight(0),
       freezeEvents(0),
       backgroundColor(0),
       foregroundColor(0),

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

   containerSize.cx = -1;
   containerSize.cy = -1;

   bstrDefaultMinValue = SysAllocString(L"-1");
   bstrDefaultMaxValue = SysAllocString(L"1");
   defaultStepCount = 10;

   ++functionCount;
 
#ifdef EMBEDDED_VARIABLES
   if ( ! VList::pIClassFactory_variables )
      GSystemsVariablesDllGetClassObject(hModule,CLSID_Variable,IID_IClassFactory,reinterpret_cast<void **>(&VList::pIClassFactory_variables));
#else
   if ( ! VList::pIClassFactory_variables )
      CoGetClassObject(CLSID_Variable,CLSCTX_INPROC_SERVER,NULL,IID_IClassFactory,reinterpret_cast<void **>(&VList::pIClassFactory_variables));
#endif
      
   IUnknown* pIUnknownThis;

   QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknownThis));

#ifdef EMBEDDED_PROPERTIES
   IClassFactory* pIClassFactory;
   GSystemsPropertiesDllGetClassObject(hModule,CLSID_GSystemProperties,IID_IClassFactory,reinterpret_cast<void**>(&pIClassFactory));
   pIClassFactory -> CreateInstance(pIUnknownThis,IID_IUnknown,reinterpret_cast<void**>(&pIUnknownProperties));
   pIClassFactory -> Release();
#else
   CoCreateInstance(CLSID_InnoVisioNateProperties,
                         pIUnknownThis,
                         CLSCTX_INPROC_SERVER,
                         IID_IUnknown,
                         reinterpret_cast<void **>(&pIUnknownProperties));
#endif

   pIUnknownProperties -> QueryInterface(IID_IGProperties,reinterpret_cast<void**>(&iProperties));

   pIUnknownThis -> Release();

   evaluator = new ConnectedEvaluator(this);

   pIConnectionPoint = new _IConnectionPoint(this,DIID_IGSFunctioNaterEvents),

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

   iProperties -> Add(L"container size",NULL);
   iProperties -> DirectAccess(L"container size",TYPE_BINARY,&containerSize,sizeof(SIZEL));

   iProperties -> Add(L"rect",NULL);
   iProperties -> DirectAccess(L"rect",TYPE_BINARY,&rectDialog,sizeof(RECT));

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

#if 0
   for ( IVariable **ppv : variableToDeleteList )
      (*ppv) -> Release();
#endif

   variableToDeleteList.clear();

   if ( hwndSpecDialog ) {
      DestroyWindow(hwndSpecDialog);
      SetWindowLong(hwndSpecDialog,GWL_USERDATA,0);  
   }

   if ( hwndVariables ) DestroyWindow(hwndVariables);
   if ( hwndProperties ) DestroyWindow(hwndProperties);
   if ( hwndPropertiesVisibility) DestroyWindow(hwndPropertiesVisibility);

   if ( iProperties ) iProperties -> Release();

   if ( enumConnectionPoints ) delete enumConnectionPoints;

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
   SetWindowLong(hwndVariables,GWL_ID,IDDIALOG_FUNCTION_VARIABLES);
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

   SetWindowPos(hwndSpecDialog,HWND_TOP,0,0,containerSize.cx,containerSize.cy,SWP_NOMOVE | SWP_SHOWWINDOW);
 
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

   int Function::resize(long cx) {

   int gapright,deltay = 0,deltax = 0;
   int expressionLabelDeltay,resultsLabelDeltay;
   bool anyControlVisible = false;

   resultingHeight = 0;

   gapright = 8;

   if ( allowUserProperties ) {
      ShowWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_PROPERTIES),expressionVisible ? SW_SHOW : SW_HIDE);
      int cxProperties = rectProperties.right - rectProperties.left + 8;
      if ( ! expressionLabel[0] ) {
         expressionLabelDeltay = 14;
         ShowWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_EXPRESSION_LABEL),SW_HIDE);
      } else {
         expressionLabelDeltay = 0;
         SetWindowPos(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_EXPRESSION_LABEL),HWND_TOP,cxProperties + 4,rectLabel1.top,cx - 2 * gapright - cxProperties + 4,rectLabel1.bottom - rectLabel1.top,SWP_SHOWWINDOW);
      }
      SetWindowPos(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_PROPERTIES),HWND_TOP,rectProperties.left,rectProperties.top - expressionLabelDeltay,0,0,SWP_NOSIZE);
      SetWindowPos(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_EQUATION_ENTRY),HWND_TOP,cxProperties + 4,rectExpression.top - expressionLabelDeltay,cx - 2 * gapright - cxProperties + 4,rectExpression.bottom - rectExpression.top ,0L);
   } else {
      ShowWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_PROPERTIES),SW_HIDE);
      if ( ! expressionLabel[0] ) {
         expressionLabelDeltay = 14;
         ShowWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_EXPRESSION_LABEL),SW_HIDE);
      } else {
         expressionLabelDeltay = 0;
         SetWindowPos(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_EXPRESSION_LABEL),HWND_TOP,8,rectLabel1.top,cx - 2 * gapright,rectExpression.bottom - rectExpression.top,SWP_SHOWWINDOW);
      }
      SetWindowPos(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_EQUATION_ENTRY),HWND_TOP,8,rectExpression.top - expressionLabelDeltay,cx - 2 * gapright,rectExpression.bottom - rectExpression.top,0L);
   }

   if ( ! resultsLabel[0] ) {
      resultsLabelDeltay = 14;
      ShowWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_RESULT_LABEL),SW_HIDE);
   } else {
      resultsLabelDeltay = 0;
      SetWindowPos(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_RESULT_LABEL),HWND_TOP,rectLabel1.left,rectLabel1.top - expressionLabelDeltay,cx - 2 * gapright,rectExpression.bottom - rectExpression.top,SWP_NOMOVE);
   }

   SetWindowPos(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_RESULT),HWND_TOP,0,0,cx - 2 * gapright,rectExpression.bottom - rectExpression.top,SWP_NOMOVE);

   SetWindowPos(hwndVariables,HWND_TOP,0,0,cx - 2 * gapright,rectVariables.bottom - rectVariables.top,SWP_NOMOVE);

   SetWindowPos(hwndVariablesTab,HWND_TOP,0,0,cx - 2 * gapright - 2,rectVariables.bottom - rectVariables.top - 2,SWP_NOMOVE);

   if ( ! expressionVisible )
      deltay = rectExpression.bottom - rectLabel1.top - expressionLabelDeltay;
   else 
      resultingHeight = rectExpression.bottom - expressionLabelDeltay;
 
   deltay += expressionLabelDeltay;
   deltay += resultsLabelDeltay;

   SetWindowPos(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_RESULT),HWND_TOP,rectResults.left,rectResults.top - deltay,0,0,SWP_NOSIZE);

   SetWindowPos(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_RESULT_LABEL),HWND_TOP,rectLabel2.left,rectLabel2.top - deltay,0,0,SWP_NOSIZE);

   if ( ! resultsVisible )
      deltay += rectResults.bottom - rectLabel2.top - resultsLabelDeltay / 2;
   else
      resultingHeight += rectResults.bottom - rectLabel2.top - resultsLabelDeltay / 2;

   SetWindowPos(hwndVariables,HWND_TOP,rectVariables.left,rectVariables.top - deltay,0,0,SWP_NOSIZE);

   if ( ! variablesVisible )
      deltay += rectVariables.bottom - rectVariables.top;
   else
      resultingHeight += rectVariables.bottom - rectVariables.top + 12;

   if ( ! controlsVisible || ! startVisible ) 
      deltax += rectControls.right - rectControls.left;
   else
      anyControlVisible = true;

   SetWindowPos(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_START),HWND_TOP,rectControls.left - deltax,rectControls.top - deltay,0,0,SWP_NOSIZE);

   if ( ! controlsVisible || ! pauseVisible ) 
      deltax += rectControls.right - rectControls.left;
   else {
      deltax -= controlSpacing;
      anyControlVisible = true;
   }

   SetWindowPos(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_PAUSE),HWND_TOP,rectControls.left + (rectControls.right - rectControls.left) - deltax,rectControls.top - deltay,0,0,SWP_NOSIZE);

   if ( ! controlsVisible || ! resumeVisible ) 
      deltax += rectControls.right - rectControls.left;
   else {
      deltax -= controlSpacing;
      anyControlVisible = true;
   }

   SetWindowPos(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_RESUME),HWND_TOP,rectControls.left + 2 * (rectControls.right - rectControls.left) - deltax,rectControls.top - deltay,0,0,SWP_NOSIZE);

   if ( ! controlsVisible || ! stopVisible ) 
      deltax += rectControls.right - rectControls.left;
   else {
      deltax -= controlSpacing;
      anyControlVisible = true;
   }

   SetWindowPos(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_STOP),HWND_TOP,rectControls.left + 3 * (rectControls.right - rectControls.left) - deltax,rectControls.top - deltay,0,0,SWP_NOSIZE);

   ShowWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_EXPRESSION_LABEL),expressionVisible && expressionLabel[0] ? SW_SHOW : SW_HIDE);
   ShowWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_EQUATION_ENTRY),expressionVisible ? SW_SHOW : SW_HIDE);
   ShowWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_RESULT),resultsVisible ? SW_SHOW : SW_HIDE);
   ShowWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_RESULT_LABEL),resultsVisible && resultsLabel[0] ? SW_SHOW : SW_HIDE);
   ShowWindow(hwndVariables,variablesVisible ? SW_SHOW : SW_HIDE);
   ShowWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_VARIABLES_DOMAIN_BOX),variablesVisible ? SW_SHOW : SW_HIDE);
   ShowWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_START),controlsVisible && startVisible ? SW_SHOW : SW_HIDE);
   ShowWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_PAUSE),controlsVisible && pauseVisible ? SW_SHOW : SW_HIDE);
   ShowWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_RESUME),controlsVisible && resumeVisible ? SW_SHOW : SW_HIDE);
   ShowWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_STOP),controlsVisible && stopVisible ? SW_SHOW : SW_HIDE);

   if ( anyControlVisible )
      resultingHeight += 2 * (rectControls.bottom - rectControls.top);
   else
      resultingHeight += 12;

   if ( 12 == resultingHeight ) resultingHeight = 64;

   rectDialog.right = rectDialog.left + cx;
   rectDialog.bottom = rectDialog.top + resultingHeight;

   if ( ! okayToResize ) return 0;

   okayToResize = false;

   SetWindowPos(hwndSpecDialog,HWND_TOP,0,0,rectDialog.right - rectDialog.left,rectDialog.bottom - rectDialog.top,SWP_NOMOVE);

   containerSize.cy = resultingHeight;

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
   for ( int k = 4; k < 8; k++ )
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

