/*

                       Copyright (c) 2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <olectl.h>
#include <CommCtrl.h>

#include "General.h"
#include "Graphic_resource.h"

#include "Function.h"

   LRESULT EXPENTRY Function::functionPropertiesVisibilityHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
   
   if ( WM_INITDIALOG == msg ) {

      PROPSHEETPAGE *pPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);

      Function *p = (Function *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);
      
      p -> hwndPropertiesVisibility = hwnd;

      p -> pIPropertyExpressionVisible -> setWindowItemChecked(hwnd,IDDI_FUNCTION_PROPERTIES_VISIBILITY_EXPRESSION);
      p -> pIPropertyResultsVisible -> setWindowItemChecked(hwnd,IDDI_FUNCTION_PROPERTIES_VISIBILITY_RESULTS);
      p -> pIPropertyVariablesVisible -> setWindowItemChecked(hwnd,IDDI_FUNCTION_PROPERTIES_VISIBILITY_VARIABLES);
      p -> pIPropertyControlsVisible -> setWindowItemChecked(hwnd,IDDI_FUNCTION_PROPERTIES_VISIBILITY_CONTROLS);
      p -> pIPropertyStartVisible -> setWindowItemChecked(hwnd,IDDI_FUNCTION_PROPERTIES_VISIBILITY_START);
      p -> pIPropertyPauseVisible -> setWindowItemChecked(hwnd,IDDI_FUNCTION_PROPERTIES_VISIBILITY_PAUSE);
      p -> pIPropertyResumeVisible -> setWindowItemChecked(hwnd,IDDI_FUNCTION_PROPERTIES_VISIBILITY_RESUME);
      p -> pIPropertyStopVisible -> setWindowItemChecked(hwnd,IDDI_FUNCTION_PROPERTIES_VISIBILITY_STOP);
      p -> pIPropertyPlotPropertiesVisible -> setWindowItemChecked(hwnd,IDDI_FUNCTION_PROPERTIES_VISIBILITY_PLOTPROPS);

      EnableWindow(GetDlgItem(hwnd,IDDI_FUNCTION_PROPERTIES_VISIBILITY_START),p -> controlsVisible);
      EnableWindow(GetDlgItem(hwnd,IDDI_FUNCTION_PROPERTIES_VISIBILITY_PAUSE),p -> controlsVisible);
      EnableWindow(GetDlgItem(hwnd,IDDI_FUNCTION_PROPERTIES_VISIBILITY_RESUME),p -> controlsVisible);
      EnableWindow(GetDlgItem(hwnd,IDDI_FUNCTION_PROPERTIES_VISIBILITY_STOP),p -> controlsVisible);
      EnableWindow(GetDlgItem(hwnd,IDDI_FUNCTION_PROPERTIES_VISIBILITY_PLOTPROPS),p -> controlsVisible);

      SetWindowLongPtr(p -> hwndPropertiesVisibility,GWL_STYLE,GetWindowLongPtr(p -> hwndPropertiesVisibility,GWL_STYLE) & ~WS_CAPTION);

      return (LRESULT)FALSE;
   }

   return functionPropertiesHandler(hwnd,msg,wParam,lParam);
   }


   LRESULT EXPENTRY Function::functionPropertiesHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   Function *p = (Function *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch (msg) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = (PROPSHEETPAGE *)lParam;

      p = (Function *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);
      
      p -> hwndProperties = hwnd;

      p -> pPropertyDialogVariableList = new VList(p -> evaluator);
      p -> pPropertyDialogVariableList -> SetHwnds(p -> hwndProperties,0);

      p -> pIPropertyExpression -> setWindowItemText(p -> hwndProperties,IDDI_FUNCTION_PROPERTIES_EQUATION_ENTRY);
      p -> pIPropertyExpressionLabel -> setWindowItemText(p -> hwndProperties,IDDI_FUNCTION_EXPRESSION_LABEL);
      p -> pIPropertyResultsLabel -> setWindowItemText(p -> hwndProperties,IDDI_FUNCTION_RESULT_LABEL);

      if ( p -> isDesignMode ) {
         p -> pIPropertyPropertiesVisible -> setWindowItemChecked(p -> hwndProperties,IDDI_FUNCTION_PROPERTIES_ALLOWPROPERTIES);
         p -> pIPropertyPropertiesControlVisibility -> setWindowItemChecked(p -> hwndProperties,IDDI_FUNCTION_PROPERTIES_ALLOWCTLVISPROPS);
      } else {
         ShowWindow(GetDlgItem(p -> hwndProperties,IDDI_FUNCTION_PROPERTIES_ALLOWPROPERTIES),SW_HIDE);
         ShowWindow(GetDlgItem(p -> hwndProperties,IDDI_FUNCTION_PROPERTIES_ALLOWCTLVISPROPS),SW_HIDE);
      }

      SetWindowLongPtr(p -> hwndProperties,GWL_STYLE,GetWindowLongPtr(p -> hwndProperties,GWL_STYLE) & ~WS_CAPTION);

      }
      return LRESULT(1);

   case WM_COMMAND: {
      WORD controlID = LOWORD(wParam);
      WORD notifyCode = HIWORD(wParam);
      if ( ! p ) break; 
      switch ( controlID ) {
      case IDDI_FUNCTION_PROPERTIES_EQUATION_ENTRY:
         switch ( notifyCode ) {
         case EN_CHANGE: {

            char szTemp[MAX_PROPERTY_SIZE];
            GetDlgItemText(hwnd,IDDI_FUNCTION_PROPERTIES_EQUATION_ENTRY,szTemp,MAX_PROPERTY_SIZE);
            SendDlgItemMessage(p -> hwndProperties,IDDI_FUNCTION_PROPERTIES_VARIABLES,CB_RESETCONTENT,0L,0L);

            p -> pIPropertyExpression -> put_szValue(szTemp);

            p -> defineFunction(szTemp);

            if ( p -> hwndSpecDialog )
               p -> pIPropertyExpression -> setWindowItemText(p -> hwndSpecDialog,IDDI_FUNCTION_EQUATION_ENTRY);

            p -> pCurrentVariableList = p -> pPropertyDialogVariableList;

            IVariable* pManuallyAddedVariable = (IVariable*)NULL;

            while ( pManuallyAddedVariable = p -> pManuallyAddedVariables -> GetNext(pManuallyAddedVariable) ) {
               BSTR bstrName;
               long n;
               pManuallyAddedVariable -> get_Name(&bstrName);
               char* pszName = new char[n = (wcslen(bstrName) + 1)];
               memset(pszName,0,n);
               WideCharToMultiByte(CP_ACP,0,bstrName,-1,pszName,n,0,0);
               if ( CB_ERR == SendDlgItemMessage(hwnd,IDDI_FUNCTION_PROPERTIES_VARIABLES,CB_FINDSTRINGEXACT,-1L,(LPARAM)pszName) )
                  SendDlgItemMessage(hwnd,IDDI_FUNCTION_PROPERTIES_VARIABLES,CB_SETCURSEL,(WPARAM)SendDlgItemMessage(hwnd,IDDI_FUNCTION_PROPERTIES_VARIABLES,CB_ADDSTRING,0L,(LPARAM)pszName),0);
               delete [] pszName;
               SysFreeString(bstrName);
            }

            p -> defineFunction(szTemp);

            p -> pCurrentVariableList = p -> pVariableList;

            SetFocus(GetDlgItem(hwnd,IDDI_FUNCTION_PROPERTIES_EQUATION_ENTRY));
            }
            return LRESULT(0);
 
         default:
            return LRESULT(0);
         }

      case IDDI_FUNCTION_EXPRESSION_LABEL:
         switch ( notifyCode ) {
         case EN_CHANGE: {
            p -> pIPropertyExpressionLabel -> getWindowItemValue(hwnd,IDDI_FUNCTION_EXPRESSION_LABEL);
            if ( p -> hwndSpecDialog ) 
               p -> pIPropertyExpressionLabel -> setWindowItemText(p -> hwndSpecDialog,IDDI_FUNCTION_EXPRESSION_LABEL);
            }
            return LRESULT(0);
 
         default:
            return LRESULT(0);
         }

      case IDDI_FUNCTION_RESULT_LABEL:
         switch ( notifyCode ) {
         case EN_CHANGE:
            p -> pIPropertyResultsLabel -> getWindowItemValue(hwnd,IDDI_FUNCTION_RESULT_LABEL);
            if ( p -> hwndSpecDialog )
               p -> pIPropertyResultsLabel -> setWindowItemText(p -> hwndSpecDialog,IDDI_FUNCTION_RESULT_LABEL);
            return LRESULT(0);
 
         default:
            return LRESULT(0);
         }

      case IDDI_FUNCTION_PROPERTIES_ALLOWPROPERTIES:
         p -> pIPropertyPropertiesVisible -> getWindowItemChecked(hwnd,IDDI_FUNCTION_PROPERTIES_ALLOWPROPERTIES);
         break;

      case IDDI_FUNCTION_PROPERTIES_ALLOWCTLVISPROPS:
         p -> pIPropertyPropertiesControlVisibility -> getWindowItemChecked(hwnd,IDDI_FUNCTION_PROPERTIES_ALLOWCTLVISPROPS);
         break;

      case IDDI_FUNCTION_PROPERTIES_VARIABLE_EDIT: {

         char szTemp[64];

         if ( 0 == GetDlgItemText(hwnd,IDDI_FUNCTION_PROPERTIES_VARIABLES,szTemp,64) )
            break;

         IVariable* vTarget = p -> pCurrentVariableList -> Get(szTemp); 
         if ( ! vTarget ) 
            vTarget = p -> pManuallyAddedVariables -> Get(szTemp);

         IVariable* vSource = p -> pPropertyDialogVariableList -> Get(szTemp);
         if ( ! vSource ) 
            vSource = p -> pManuallyAddedVariables -> Get(szTemp);

         vSource -> CopyFrom(vTarget);

         vSource -> Edit();

         vTarget -> CopyFrom(vSource);

         }
         break;

      case IDDI_FUNCTION_PROPERTIES_VISIBILITY_EXPRESSION: {
         p -> pIPropertyExpressionVisible -> getWindowItemChecked(hwnd,controlID);
         }
         break;

      case IDDI_FUNCTION_PROPERTIES_VISIBILITY_RESULTS :
         p -> pIPropertyResultsVisible -> getWindowItemChecked(hwnd,controlID);
         break;

      case IDDI_FUNCTION_PROPERTIES_VISIBILITY_VARIABLES :
         p -> pIPropertyVariablesVisible -> getWindowItemChecked(hwnd,controlID);
         break;

      case IDDI_FUNCTION_PROPERTIES_VISIBILITY_CONTROLS : {
         short isVisible;
         p -> pIPropertyControlsVisible -> getWindowItemChecked(hwnd,controlID);
         p -> pIPropertyControlsVisible -> get_boolValue(&isVisible);
         EnableWindow(GetDlgItem(hwnd,IDDI_FUNCTION_PROPERTIES_VISIBILITY_START),isVisible);
         EnableWindow(GetDlgItem(hwnd,IDDI_FUNCTION_PROPERTIES_VISIBILITY_PAUSE),isVisible);
         EnableWindow(GetDlgItem(hwnd,IDDI_FUNCTION_PROPERTIES_VISIBILITY_RESUME),isVisible);
         EnableWindow(GetDlgItem(hwnd,IDDI_FUNCTION_PROPERTIES_VISIBILITY_STOP),isVisible);
         EnableWindow(GetDlgItem(hwnd,IDDI_FUNCTION_PROPERTIES_VISIBILITY_PLOTPROPS),isVisible);
         }
         break;

      case IDDI_FUNCTION_PROPERTIES_VISIBILITY_START :
         p -> pIPropertyStartVisible -> getWindowItemChecked(hwnd,controlID);
         break;

      case IDDI_FUNCTION_PROPERTIES_VISIBILITY_PAUSE :
         p -> pIPropertyPauseVisible -> getWindowItemChecked(hwnd,controlID);
         break;

      case IDDI_FUNCTION_PROPERTIES_VISIBILITY_RESUME :
         p -> pIPropertyResumeVisible -> getWindowItemChecked(hwnd,controlID);
         break;

      case IDDI_FUNCTION_PROPERTIES_VISIBILITY_STOP :
         p -> pIPropertyStopVisible -> getWindowItemChecked(hwnd,controlID);
         break;

      case IDDI_FUNCTION_PROPERTIES_VISIBILITY_PLOTPROPS :
         p -> pIPropertyPlotPropertiesVisible -> getWindowItemChecked(hwnd,controlID);
         break;

      }
      }
      return LRESULT(0);

   //case WM_PAINT:
   //   ShowWindow(GetDlgItem(hwnd,IDDI_FUNCTION_PROPERTIES_ALLOWVISIBILITYPROPERTIES),! p -> userMode ? SW_SHOW : SW_HIDE);
   //   break;

   }
 
   return 0;
   }