/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/
                              
#include <windows.h>
#include <CommCtrl.h>

#include "Graphic_resource.h"
#include "GSystemsProperties.h"

#include "utils.h"

#include "V.h"

#include "list.cpp"

#include "Variable_i.h"
#include "Properties_i.c"

   extern HMODULE gsVariables_hModule;

   V::V()
   :
      pIUnknownProperties(NULL),
      iProperties(NULL),
      iEvaluator(NULL),

      hwndDialog(0),
      hwndOwner(0),
      hwndTabControl(0),
      hwndProperties(0),

      tabIndex(0),

      pIProperty_name(NULL),
      pIProperty_valueExpression(NULL),
      pIProperty_minValue(NULL),
      pIProperty_maxValue(NULL),
      pIProperty_stepCount(NULL),
      pIProperty_stepExpression(NULL),
      pIProperty_isIndependentVariable(NULL),

      isIndependentVariable(TRUE),

      propertiesHaveChanged(FALSE)

  {

   refCount = 100;

   value.doubleValue = 0.0;

   memset(name,0,sizeof(name));
   memset(valueExpression,0,sizeof(valueExpression));
   memset(minValue,0,sizeof(minValue));
   memset(maxValue,0,sizeof(maxValue));
   memset(stepCount,0,sizeof(stepCount));
   memset(stepExpression,0,sizeof(stepExpression));

   IUnknown* pIUnknownThis;
   QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknownThis));

#ifdef EMBEDDED_PROPERTIES
   IClassFactory* pIClassFactory;
   GSystemsPropertiesDllGetClassObject(gsVariables_hModule,CLSID_InnoVisioNateProperties,IID_IClassFactory,reinterpret_cast<void**>(&pIClassFactory));
   pIClassFactory -> CreateInstance(pIUnknownThis,IID_IUnknown,reinterpret_cast<void**>(&pIUnknownProperties));
   pIClassFactory -> Release();
#else
   HRESULT rc = CoCreateInstance(CLSID_InnoVisioNateProperties,pIUnknownThis,CLSCTX_INPROC_SERVER,IID_IUnknown,reinterpret_cast<void **>(&pIUnknownProperties));
#endif
   pIUnknownProperties -> QueryInterface(IID_IGProperties,reinterpret_cast<void**>(&iProperties));
   pIUnknownThis -> Release();

   QueryInterface(IID_IGPropertiesClient,reinterpret_cast<void**>(&iPropertiesClient));
   QueryInterface(IID_IGPropertyPageClient,reinterpret_cast<void**>(&iPropertyPageClient));

   iProperties -> AdvisePropertyPageClient(iPropertyPageClient,true);

   iProperties -> put_DebuggingEnabled(TRUE);

   iProperties -> Advise(iPropertiesClient);

   iProperties -> Add(L"name",&pIProperty_name);
   iProperties -> Add(L"value expression",&pIProperty_valueExpression);
   iProperties -> Add(L"step count",&pIProperty_stepCount);
   iProperties -> Add(L"step expression",&pIProperty_stepExpression);
   iProperties -> Add(L"min value expression",&pIProperty_minValue);
   iProperties -> Add(L"max value expression",&pIProperty_maxValue);
   iProperties -> Add(L"is independent",&pIProperty_isIndependentVariable);

   pIProperty_name -> directAccess(TYPE_SZSTRING,name,sizeof(name));
   pIProperty_minValue -> directAccess(TYPE_SZSTRING,minValue,sizeof(minValue));
   pIProperty_maxValue -> directAccess(TYPE_SZSTRING,maxValue,sizeof(maxValue));
   pIProperty_stepCount -> directAccess(TYPE_SZSTRING,stepCount,sizeof(stepCount));
   pIProperty_stepExpression -> directAccess(TYPE_SZSTRING,stepExpression,sizeof(stepExpression));
   pIProperty_isIndependentVariable -> directAccess(TYPE_BOOL,&isIndependentVariable,sizeof(BOOL));
   pIProperty_valueExpression -> directAccess(TYPE_SZSTRING,valueExpression,sizeof(valueExpression));
   
   InitNew();

   IGPropertyClient* pIPropertyClient;
   QueryInterface(IID_IGPropertyClient,reinterpret_cast<void**>(&pIPropertyClient));

   refCount = 0;

   }


   V::~V() {

   refCount = 101;

   if ( hwndTabControl )
      long k = SendMessage(hwndTabControl,TCM_DELETEITEM,(WPARAM)tabIndex,0);

   if ( hwndDialog)
      DestroyWindow(hwndDialog);

   if ( pIUnknownProperties )
      pIUnknownProperties -> Release();

   if ( iProperties )
      iProperties -> Release();

   if ( iEvaluator )
      iEvaluator -> Release();

   refCount = 0;
   }
 
 
   int V::initWindows() {
 
   if ( ! hwndOwner ) return FALSE;

   hwndDialog = CreateDialogParam(gsVariables_hModule,MAKEINTRESOURCE(IDDIALOG_VARIABLE),hwndOwner,(DLGPROC)handler,(LPARAM)this);

   char* pszTemp = new char[strlen(name) + 10];
   sprintf(pszTemp,"<= %s <= ",name);
   SetWindowText(GetDlgItem(hwndDialog,IDDI_VARIABLE_DOMAIN_NAME),pszTemp);
   delete [] pszTemp;

   if ( hwndTabControl ) {

      RECT rect = {0,0,0,0};
      TC_ITEM tie;
      memset(&tie,0,sizeof(TC_ITEM));
      tie.mask = TCIF_TEXT; 
      tie.pszText = name;
      tie.cchTextMax = strlen(name);

      tabIndex = SendMessage(hwndTabControl,TCM_GETITEMCOUNT,0,0);

      SendMessage(hwndTabControl,TCM_INSERTITEM,(WPARAM)tabIndex,(LPARAM)&tie);

      //SendMessage(hwndTabControl,TCM_SETCURSEL,(WPARAM)tabIndex,0L);
 
      SendMessage(hwndTabControl,TCM_GETITEMRECT,(WPARAM)tabIndex,(LPARAM)&rect);

      SetWindowPos(hwndDialog,HWND_TOP,(long)(0.5*(double)rect.bottom),(long)(1.5*(double)rect.bottom),0,0,SWP_NOSIZE | SWP_NOZORDER);
 
   }
   else {

      //ShowWindow(hwndDialog,SW_HIDE);
      //SetWindowPos(hwndDialog,HWND_TOP,-1000,-1000,0,0,SWP_NOSIZE);

   }

   return TRUE;
   }
 

   LRESULT CALLBACK V::propertiesHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   if ( WM_INITDIALOG == msg ) {
   
      V *p = (V *)lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      ShowWindow(GetDlgItem(hwnd,IDDI_VARIABLE_DOMAIN_INVALID),SW_HIDE);

      p -> pIProperty_stepCount -> setWindowItemText(hwnd,IDDI_VARIABLE_STEPS);
      p -> pIProperty_stepExpression -> setWindowItemText(hwnd,IDDI_VARIABLE_DOMAIN_EXPRESSION);
      p -> pIProperty_minValue -> setWindowItemText(hwnd,IDDI_VARIABLE_DOMAIN_MIN);
      p -> pIProperty_maxValue -> setWindowItemText(hwnd,IDDI_VARIABLE_DOMAIN_MAX);
      p -> pIProperty_valueExpression -> setWindowItemText(hwnd,IDDI_VARIABLE_VALUE);

      SendDlgItemMessage(hwnd,IDDI_VARIABLE_VALUE_VARIABLE,BM_SETCHECK,p -> isIndependentVariable ? FALSE : TRUE,0L);
      SendDlgItemMessage(hwnd,IDDI_VARIABLE_INDEPENDENT_VARIABLE,BM_SETCHECK,p -> isIndependentVariable ? TRUE : FALSE,0L);

      EnableWindow(GetDlgItem(hwnd,IDDI_VARIABLE_VALUE),p -> isIndependentVariable ? FALSE : TRUE);
      EnableWindow(GetDlgItem(hwnd,IDDI_VARIABLE_DOMAIN_MIN),p -> isIndependentVariable ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_VARIABLE_DOMAIN_MAX),p -> isIndependentVariable ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_VARIABLE_STEPS),p -> isIndependentVariable ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_VARIABLE_DOMAIN_EXPRESSION),p -> isIndependentVariable ? TRUE : FALSE);

      char szTemp[MAX_PROPERTY_SIZE];
      sprintf(szTemp,"<= %s <= ",p -> name);
      SetWindowText(GetDlgItem(hwnd,IDDI_VARIABLE_DOMAIN_NAME),szTemp);

      sprintf(szTemp,"Variable: %s",p -> name);
      SetWindowText(hwnd,szTemp);

      SetWindowPos(hwnd,HWND_TOP,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

      return (LRESULT)FALSE;

   }

   return handler(hwnd,msg,wParam,lParam);
   }

 
   LRESULT CALLBACK V::handler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   V *p = (V *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch ( msg ) {

   case WM_INITDIALOG: {

      p = (V *)lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      ShowWindow(GetDlgItem(hwnd,IDDI_VARIABLE_DOMAIN_INVALID),SW_HIDE);

      p -> hwndProperties = hwnd;

      p -> pIProperty_stepCount -> setWindowItemText(hwnd,IDDI_VARIABLE_STEPS);
      p -> pIProperty_stepExpression -> setWindowItemText(hwnd,IDDI_VARIABLE_DOMAIN_EXPRESSION);
      p -> pIProperty_minValue -> setWindowItemText(hwnd,IDDI_VARIABLE_DOMAIN_MIN);
      p -> pIProperty_maxValue -> setWindowItemText(hwnd,IDDI_VARIABLE_DOMAIN_MAX);
      p -> pIProperty_valueExpression -> setWindowItemText(hwnd,IDDI_VARIABLE_VALUE);

      SendDlgItemMessage(hwnd,IDDI_VARIABLE_VALUE_VARIABLE,BM_SETCHECK,p -> isIndependentVariable ? FALSE : TRUE,0L);
      SendDlgItemMessage(hwnd,IDDI_VARIABLE_INDEPENDENT_VARIABLE,BM_SETCHECK,p -> isIndependentVariable ? TRUE : FALSE,0L);

      EnableWindow(GetDlgItem(hwnd,IDDI_VARIABLE_VALUE),p -> isIndependentVariable ? FALSE : TRUE);
      EnableWindow(GetDlgItem(hwnd,IDDI_VARIABLE_DOMAIN_MIN),p -> isIndependentVariable ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_VARIABLE_DOMAIN_MAX),p -> isIndependentVariable ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_VARIABLE_STEPS),p -> isIndependentVariable ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_VARIABLE_DOMAIN_EXPRESSION),p -> isIndependentVariable ? TRUE : FALSE);

      char szTemp[MAX_PROPERTY_SIZE];
      sprintf(szTemp,"<= %s <= ",p -> name);
      SetWindowText(GetDlgItem(hwnd,IDDI_VARIABLE_DOMAIN_NAME),szTemp);

      sprintf(szTemp,"Variable: %s",p -> name);
      SetWindowText(hwnd,szTemp);

      RECT rect,rectParent;
      GetWindowRect(hwnd,&rectParent);
      GetWindowRect(GetDlgItem(hwnd,IDDI_VARIABLE_VALUE),&rect);
      SetWindowPos(GetDlgItem(hwnd,IDDI_VARIABLE_VALUE),HWND_TOP,rect.left - rectParent.left,rect.top - rectParent.top,0,0,SWP_NOSIZE);
      GetWindowRect(GetDlgItem(hwnd,IDDI_VARIABLE_DOMAIN_MIN),&rect);
      SetWindowPos(GetDlgItem(hwnd,IDDI_VARIABLE_DOMAIN_MIN),HWND_TOP,rect.left - rectParent.left,rect.top - rectParent.top,0,0,SWP_NOSIZE);
      GetWindowRect(GetDlgItem(hwnd,IDDI_VARIABLE_DOMAIN_MAX),&rect);
      SetWindowPos(GetDlgItem(hwnd,IDDI_VARIABLE_DOMAIN_MAX),HWND_TOP,rect.left - rectParent.left,rect.top - rectParent.top,0,0,SWP_NOSIZE);
      GetWindowRect(GetDlgItem(hwnd,IDDI_VARIABLE_STEPS_LABEL),&rect);
      SetWindowPos(GetDlgItem(hwnd,IDDI_VARIABLE_STEPS_LABEL),HWND_TOP,rect.left - rectParent.left,rect.top - rectParent.top,0,0,SWP_NOSIZE);
      GetWindowRect(GetDlgItem(hwnd,IDDI_VARIABLE_STEPS),&rect);
      SetWindowPos(GetDlgItem(hwnd,IDDI_VARIABLE_STEPS),HWND_TOP,rect.left - rectParent.left,rect.top - rectParent.top,0,0,SWP_NOSIZE);
      GetWindowRect(GetDlgItem(hwnd,IDDI_VARIABLE_DOMAIN_EXPRESSION),&rect);
      SetWindowPos(GetDlgItem(hwnd,IDDI_VARIABLE_DOMAIN_EXPRESSION),HWND_TOP,rect.left - rectParent.left,rect.top - rectParent.top,0,0,SWP_NOSIZE);
      GetWindowRect(GetDlgItem(hwnd,IDDI_VARIABLE_VALUE_VARIABLE),&rect);
      SetWindowPos(GetDlgItem(hwnd,IDDI_VARIABLE_VALUE_VARIABLE),HWND_TOP,rect.left - rectParent.left,rect.top - rectParent.top,0,0,SWP_NOSIZE);
      GetWindowRect(GetDlgItem(hwnd,IDDI_VARIABLE_INDEPENDENT_VARIABLE),&rect);
      SetWindowPos(GetDlgItem(hwnd,IDDI_VARIABLE_INDEPENDENT_VARIABLE),HWND_TOP,rect.left - rectParent.left,rect.top - rectParent.top,0,0,SWP_NOSIZE);
      GetWindowRect(GetDlgItem(hwnd,IDDI_VARIABLE_DOMAIN_GROUP_NAME),&rect);
      SetWindowPos(GetDlgItem(hwnd,IDDI_VARIABLE_DOMAIN_GROUP_NAME),HWND_TOP,rect.left - rectParent.left,rect.top - rectParent.top,0,0,SWP_NOSIZE);
      GetWindowRect(GetDlgItem(hwnd,IDDI_VARIABLE_DOMAIN_INVALID),&rect);
      SetWindowPos(GetDlgItem(hwnd,IDDI_VARIABLE_DOMAIN_INVALID),HWND_TOP,rect.left - rectParent.left,rect.top - rectParent.top,0,0,SWP_NOSIZE);
      GetWindowRect(GetDlgItem(hwnd,IDDI_VARIABLE_DOMAIN_NAME),&rect);
      SetWindowPos(GetDlgItem(hwnd,IDDI_VARIABLE_DOMAIN_NAME),HWND_TOP,rect.left - rectParent.left,rect.top - rectParent.top,0,0,SWP_NOSIZE);

      }
      return LRESULT(TRUE);
 
   case WM_COMMAND: {

      if ( ! p ) break; 

      WORD controlID = LOWORD(wParam);
      WORD notifyCode = HIWORD(wParam);

      switch ( controlID ) {

      case IDDI_VARIABLE_VALUE_VARIABLE:
      case IDDI_VARIABLE_INDEPENDENT_VARIABLE: {

         p -> pIProperty_isIndependentVariable -> getWindowItemChecked(hwnd,IDDI_VARIABLE_INDEPENDENT_VARIABLE);

         EnableWindow(GetDlgItem(hwnd,IDDI_VARIABLE_VALUE),p -> isIndependentVariable ? FALSE : TRUE);
         EnableWindow(GetDlgItem(hwnd,IDDI_VARIABLE_DOMAIN_MIN),p -> isIndependentVariable ? TRUE : FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_VARIABLE_DOMAIN_MAX),p -> isIndependentVariable ? TRUE : FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_VARIABLE_STEPS),p -> isIndependentVariable ? TRUE : FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_VARIABLE_DOMAIN_EXPRESSION),p -> isIndependentVariable ? TRUE : FALSE);

         if ( ! p -> isIndependentVariable ) 
            SetFocus(GetDlgItem(hwnd,IDDI_VARIABLE_VALUE));

         }
         break;

      case IDDI_VARIABLE_VALUE:
         p -> pIProperty_valueExpression -> getWindowItemText(hwnd,controlID);
         break;

      case IDDI_VARIABLE_STEPS:
         p -> pIProperty_stepCount -> getWindowItemValue(hwnd,controlID);
         p -> setDomainExpression(hwnd);
         break;

      case IDDI_VARIABLE_DOMAIN_MIN:
         p -> pIProperty_minValue -> getWindowItemValue(hwnd,controlID);
         p -> setDomainExpression(hwnd);
         break;

      case IDDI_VARIABLE_DOMAIN_MAX:
         p -> pIProperty_maxValue -> getWindowItemValue(hwnd,controlID);
         p -> setDomainExpression(hwnd);
         break;

      case IDOK:
         p -> pIProperty_stepCount -> getWindowItemText(hwnd,IDDI_VARIABLE_STEPS);
         p -> pIProperty_stepExpression -> getWindowItemText(hwnd,IDDI_VARIABLE_DOMAIN_EXPRESSION);
         p -> pIProperty_minValue -> getWindowItemText(hwnd,IDDI_VARIABLE_DOMAIN_MIN);
         p -> pIProperty_maxValue -> getWindowItemText(hwnd,IDDI_VARIABLE_DOMAIN_MAX);
         p -> pIProperty_valueExpression -> getWindowItemText(hwnd,IDDI_VARIABLE_VALUE);

         p -> pIProperty_stepCount -> setWindowItemText(p -> hwndDialog,IDDI_VARIABLE_STEPS);
         p -> pIProperty_stepExpression -> setWindowItemText(p -> hwndDialog,IDDI_VARIABLE_DOMAIN_EXPRESSION);
         p -> pIProperty_minValue -> setWindowItemText(p -> hwndDialog,IDDI_VARIABLE_DOMAIN_MIN);
         p -> pIProperty_maxValue -> setWindowItemText(p -> hwndDialog,IDDI_VARIABLE_DOMAIN_MAX);
         p -> pIProperty_valueExpression -> setWindowItemText(p -> hwndDialog,IDDI_VARIABLE_VALUE);

         EndDialog(hwnd,0);
         break;

      case IDCANCEL:

         p -> pIProperty_stepCount -> getWindowItemText(p -> hwndDialog,IDDI_VARIABLE_STEPS);
         p -> pIProperty_stepExpression -> getWindowItemText(p -> hwndDialog,IDDI_VARIABLE_DOMAIN_EXPRESSION);
         p -> pIProperty_minValue -> getWindowItemText(p -> hwndDialog,IDDI_VARIABLE_DOMAIN_MIN);
         p -> pIProperty_maxValue -> getWindowItemText(p -> hwndDialog,IDDI_VARIABLE_DOMAIN_MAX);
         p -> pIProperty_valueExpression -> getWindowItemText(p -> hwndDialog,IDDI_VARIABLE_VALUE);

         EndDialog(hwnd,0);
         break;

      default:
         break;
      }
      }
      break;

   default:
      break;
   }
 
   return LRESULT(FALSE);
   }


   int V::getDomain(HWND hwndDestination) { 

   if ( ! iEvaluator ) 
      return E_UNEXPECTED;
 
   if ( ! hwndDestination ) 
      return E_UNEXPECTED;

   EVALUATE_SZ(iEvaluator,valueExpression,value.doubleValue);

   SetDlgItemText(hwndDestination,IDDI_VARIABLE_DOMAIN_INVALID,"");
   ShowWindow(GetDlgItem(hwndDestination,IDDI_VARIABLE_DOMAIN_INVALID),SW_HIDE);

   char* pszTemp = new char[strlen(name) + 10];
   sprintf(pszTemp,"<= %s <= ",name);
   SetWindowText(GetDlgItem(hwndDestination,IDDI_VARIABLE_DOMAIN_NAME),pszTemp);
   delete [] pszTemp;

   pIProperty_minValue -> setWindowItemText(hwndDestination,IDDI_VARIABLE_DOMAIN_MIN);
   pIProperty_stepCount -> setWindowItemText(hwndDestination,IDDI_VARIABLE_STEPS);
   pIProperty_stepExpression -> setWindowItemText(hwndDestination,IDDI_VARIABLE_DOMAIN_EXPRESSION);
   pIProperty_maxValue -> setWindowItemText(hwndDestination,IDDI_VARIABLE_DOMAIN_MAX);
   pIProperty_valueExpression -> setWindowItemText(hwndDestination,IDDI_VARIABLE_VALUE);

   SendDlgItemMessage(hwndDestination,IDDI_VARIABLE_VALUE_VARIABLE,BM_SETCHECK,isIndependentVariable ? FALSE : TRUE,0L);
   SendDlgItemMessage(hwndDestination,IDDI_VARIABLE_INDEPENDENT_VARIABLE,BM_SETCHECK,isIndependentVariable ? TRUE : FALSE,0L);

   EnableWindow(GetDlgItem(hwndDestination,IDDI_VARIABLE_VALUE),isIndependentVariable ? FALSE : TRUE);
   EnableWindow(GetDlgItem(hwndDestination,IDDI_VARIABLE_DOMAIN_MIN),isIndependentVariable ? TRUE : FALSE);
   EnableWindow(GetDlgItem(hwndDestination,IDDI_VARIABLE_DOMAIN_MAX),isIndependentVariable ? TRUE : FALSE);
   EnableWindow(GetDlgItem(hwndDestination,IDDI_VARIABLE_STEPS),isIndependentVariable ? TRUE : FALSE);
   EnableWindow(GetDlgItem(hwndDestination,IDDI_VARIABLE_DOMAIN_EXPRESSION),isIndependentVariable ? TRUE : FALSE);

   if ( ! isIndependentVariable ) 
      return 0;

   return setDomainExpression(hwndDestination);
   }


   int V::setDomainExpression(HWND hwndDestination) {

   char szDomainMin[MAX_PROPERTY_SIZE];

   GetDlgItemText(hwndDestination,IDDI_VARIABLE_DOMAIN_MIN,szDomainMin,MAX_PROPERTY_SIZE);

   if ( strlen(szDomainMin) == 0 ) {
      SetDlgItemText(hwndDestination,IDDI_VARIABLE_DOMAIN_INVALID,"No Min value!");
      ShowWindow(GetDlgItem(hwndDestination,IDDI_VARIABLE_DOMAIN_INVALID),SW_SHOW);
      return 0;
   }

   EVALUATE_SZ(iEvaluator,minValue,startVal)
 
   char szDomainMax[MAX_PROPERTY_SIZE];

   GetDlgItemText(hwndDestination,IDDI_VARIABLE_DOMAIN_MAX,szDomainMax,MAX_PROPERTY_SIZE);

   if ( 0 == strlen(szDomainMax) ) {
      SetDlgItemText(hwndDestination,IDDI_VARIABLE_DOMAIN_INVALID,"No Max value!");
      ShowWindow(GetDlgItem(hwndDestination,IDDI_VARIABLE_DOMAIN_INVALID),SW_SHOW);
      return 0;
   }

   EVALUATE_SZ(iEvaluator,maxValue,endVal)
 
   char szDomainSteps[MAX_PROPERTY_SIZE];

   GetDlgItemText(hwndDestination,IDDI_VARIABLE_STEPS,szDomainSteps,MAX_PROPERTY_SIZE);

   if ( 0 == strlen(szDomainSteps) ) {
      SetDlgItemText(hwndDestination,IDDI_VARIABLE_DOMAIN_INVALID,"No Steps value!");
      ShowWindow(GetDlgItem(hwndDestination,IDDI_VARIABLE_DOMAIN_INVALID),SW_SHOW);
      return 0;
   }

   SetDlgItemText(hwndDestination,IDDI_VARIABLE_DOMAIN_INVALID,"");

   long stepCount = atol(szDomainSteps);

   stepVal = (endVal - startVal) / (double)stepCount;

   sprintf(stepExpression,"%s = %s + (%s - %s)/%s",name,name,szDomainMax,szDomainMin,szDomainSteps);

   SetDlgItemText(hwndDestination,IDDI_VARIABLE_DOMAIN_EXPRESSION,stepExpression);
 
   value.doubleValue = startVal;

   return 0;
   }