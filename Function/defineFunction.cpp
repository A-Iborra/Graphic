/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <objbase.h>
#include <stdio.h>
#include <math.h>
#include <CommCtrl.h>

#include "Graphic_resource.h"
#include "gmessage.h"

#include "VList.h"
#include "Function.h"

#include "list.cpp"

   int Function::defineFunction(char* pszTheExpression) {

   if ( stopAllProcessing ) return 0;

//NTC: 12-01-2017: I am not sure what is supposed to be in the variableToDeleteList !?!

#if 0
   for ( IVariable **ppv : variableToDeleteList )
      (*ppv) -> Release();
#endif

   variableToDeleteList.clear();

   IVariable*v = (IVariable*)NULL;
   while ( v = pCurrentVariableList -> GetNext(v) )
      variableToDeleteList.push_back(new IVariable*(v));

   evaluator -> UndefineAllVariables();

   while ( v = pManuallyAddedVariables -> GetNext(v) ) 
      evaluator -> DefineVariable(v);

   char *pszTemp;
   long n;

   if ( pszTheExpression ) {
      n = strlen(pszTheExpression) + 1;
      pszTemp = new char[n];
      memset(pszTemp,0,n);
      strcpy(pszTemp,pszTheExpression);
   } else {
      n = SendDlgItemMessage(hwndSpecDialog,IDDI_FUNCTION_EQUATION_ENTRY,WM_GETTEXTLENGTH,0L,0L);
      pszTemp = new char[n + 1];
      memset(pszTemp,0,n + 1);
      GetDlgItemText(hwndSpecDialog,IDDI_FUNCTION_EQUATION_ENTRY,pszTemp,n + 1);
   }

   strcpy(expression,pszTemp);

   delete [] pszTemp;

   double dummyVal;
   expectEvalUndefines = TRUE;
   BSTR bstrExpression = SysAllocStringLen(NULL,n = (strlen(expression) + 1));
   MultiByteToWideChar(CP_ACP,0,expression,-1,bstrExpression,n);

   evaluator -> get_Eval(bstrExpression,&dummyVal);

   SysFreeString(bstrExpression);
   expectEvalUndefines = FALSE;
               
   for ( IVariable **toDelete : variableToDeleteList ) {
      if ( *toDelete ) {
         long k;
         v = (IVariable*)*toDelete;
         evaluator -> UndefineVariable(v);
         if ( hwndProperties ) {
            char* pszVariableName;
            BSTR bstrValue;
            v -> get_Name(&bstrValue);
            pszVariableName = new char[wcslen(bstrValue) + 1];
            memset(pszVariableName,0,wcslen(bstrValue) + 1);
            WideCharToMultiByte(CP_ACP,0,bstrValue,-1,pszVariableName,wcslen(bstrValue) + 1,0,0);
            SysFreeString(bstrValue);
            k = SendDlgItemMessage(hwndProperties,IDDI_FUNCTION_PROPERTIES_VARIABLES,CB_FINDSTRINGEXACT,-1L,(LPARAM)pszVariableName);
            delete [] pszVariableName;
            if ( CB_ERR != k ) 
               SendDlgItemMessage(hwndProperties,IDDI_FUNCTION_PROPERTIES_VARIABLES,CB_DELETESTRING,(WPARAM)k,0L);
         }
         pCurrentVariableList -> Remove(v,&currentShowingVariable);
      }
   }

   if ( currentShowingVariable && hwndVariablesTab ) {

      NMHDR pn;
      long ti;
      currentShowingVariable -> get_TabIndex(&ti);

      memset(&pn,0,sizeof(NMHDR));
      pn.hwndFrom = hwndVariablesTab;
      pn.idFrom = IDDI_FUNCTION_VARIABLES_TABS;

      pn.code = TCN_SELCHANGING;
      long k = SendMessage(hwndVariablesTab,TCM_GETCURSEL,0L,0L);
      SendMessage(hwndVariablesTab,WM_NOTIFY,0L,(LPARAM)&pn);

      pn.code = TCN_SELCHANGE;
      SendMessage(hwndVariablesTab,TCM_SETCURSEL,(WPARAM)ti,0L);
      SendMessage(hwndVariablesTab,WM_NOTIFY,0L,(LPARAM)&pn);
   }

   fire_Parsed();

   return TRUE;
   }
 
 
   IVariable* Function::UnknownVariable(char* variableName,bool* createdNew) {

   *createdNew = false;

   if ( expectEvalUndefines ) {
      if ( currentShowingVariable = pCurrentVariableList -> Get(variableName) ) {
         for ( IVariable **ppv : variableToDeleteList ) {
            if ( *ppv == currentShowingVariable ) {
               *ppv = 0;
               break;
            }
         }
      } else {
         hideAllTabs();
         currentShowingVariable = pCurrentVariableList -> Add(variableName);
         currentShowingVariable -> put_IsIndependent(TRUE);
         currentShowingVariable -> put_MinValueExpression(bstrDefaultMinValue);
         currentShowingVariable -> put_MaxValueExpression(bstrDefaultMaxValue);
         currentShowingVariable -> put_StepCount(defaultStepCount);
         showVariableTab(currentShowingVariable,variableName);
         *createdNew = true;
      }
      if ( hwndProperties ) {
         if ( CB_ERR == SendDlgItemMessage(hwndProperties,IDDI_FUNCTION_PROPERTIES_VARIABLES,CB_FINDSTRINGEXACT,-1L,(LPARAM)variableName) )
            SendDlgItemMessage(hwndProperties,IDDI_FUNCTION_PROPERTIES_VARIABLES,CB_SETCURSEL,(WPARAM)SendDlgItemMessage(hwndProperties,IDDI_FUNCTION_PROPERTIES_VARIABLES,CB_ADDSTRING,0L,(LPARAM)variableName),0);
      }
      evaluator -> DefineVariable(currentShowingVariable);
   }
   else
      return NULL;

   return currentShowingVariable;
   }
      

   unsigned long Function::evaluate(void* theFunction) {

   Function* f = (Function*)theFunction;

#ifdef MULTITHREADED_EVALUATOR
#endif

   if ( S_FALSE != f -> evaluator -> IsBusy() )
      return 0;

   f -> defineFunction();
   f -> fire_Clear();   
   IVariable* v = (IVariable *)NULL;
   while ( v = f -> pManuallyAddedVariables -> GetNext(v) ) 
      f -> evaluator -> DefineVariable(v);
   f -> evaluator -> Evaluate_szFunction(f -> expression);

#ifdef MULTITHREADED_EVALUATOR
#endif

   return 0;
   }
