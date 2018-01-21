// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Evaluator.h"

#include "gmessage.h"
#include "utils.h"
#include "VList.h"
#include "list.cpp"


   HRESULT Evaluator::Evaluate_szFunction(char* szExpression) {

   if ( WAIT_OBJECT_0 != WaitForSingleObject(functionIsBusy,0) ) return E_UNEXPECTED;

   pszFunctionExpression = new char[strlen(szExpression) + 1];

   strcpy(pszFunctionExpression,szExpression);

   char *c;
   if ( (c = strpbrk(pszFunctionExpression,"=")) != NULL ) {
      if ( pszResultName ) delete [] pszResultName;
      *c = '\0';
      pszResultName = new char[strlen(pszFunctionExpression) + 1];
      memset(pszResultName,0,strlen(pszFunctionExpression) + 1);
      strcpy(pszResultName,pszFunctionExpression);
      trim(pszResultName);
      *c = '=';
   }

   long n,cntIterations = 1;
   double minValue,maxValue,currentValue,deltaValue;
   BSTR bstrValue;
   char *pszName;

   long cntNameChars = 0;
   IVariable* v = (IVariable *)NULL;
   while ( v = GetNext(v) ) {

      v -> InitializeDomain();

      v -> get_MinValueExpression(&bstrValue);
      get_Eval(bstrValue,&minValue);
      SysFreeString(bstrValue);

      v -> get_MaxValueExpression(&bstrValue);
      get_Eval(bstrValue,&maxValue);
      SysFreeString(bstrValue);

      v -> get_Name(&bstrValue);
      pszName = new char[n = (wcslen(bstrValue) + 64)];
      memset(pszName,0,n);
      WideCharToMultiByte(CP_ACP,0,bstrValue,-1,pszName,n,0,0);
      SysFreeString(bstrValue);

      cntNameChars += strlen(pszName) + 1;

      sprintf(pszName + strlen(pszName)," = %lf",minValue);
      currentValue = eval(pszName);

      delete [] pszName;

#if 1
      long cntThisVar = 0L;
      v -> get_StepCount(&cntThisVar);
      cntIterations *= cntThisVar;
#else
      v -> get_DomainExpression(&bstrValue);
      get_Eval(bstrValue,&deltaValue);
      SysFreeString(bstrValue);

      deltaValue -= currentValue;

      if ( deltaValue != 0.0 && maxValue != minValue )
         cntIterations *= (long)(fabs(maxValue - minValue) / deltaValue);
#endif

   }

   cntNameChars += strlen(pszResultName) + 2;
   char *p,*pszNames = new char[cntNameChars];
   memset(pszNames,0,cntNameChars);

   p = pszNames;
   while ( v = GetNext(v) ) {
      v -> get_Name(&bstrValue);
      n = wcslen(bstrValue) + 1;
      pszName = new char[n];
      memset(pszName,0,n);
      WideCharToMultiByte(CP_ACP,0,bstrValue,-1,pszName,n,0,0);
      SysFreeString(bstrValue);
      strcpy(p,pszName);
      p += strlen(pszName) + 1;
   }

   strcpy(p,pszResultName);

   functionIterationCount = 0;

   Fire_Started(cntIterations);

   if ( Count() > 0 ) 
      doVar(pszNames,GetFirst(),GetLast());

   terminateNow = FALSE;

   delete [] pszFunctionExpression;

   pszFunctionExpression = NULL;

   Fire_Finished();

   delete [] pszNames;

   ReleaseMutex(functionIsBusy);

   return S_OK;
   }
 
 
   int Evaluator::doVar(char *pszNames,IVariable *v,IVariable *lastV) {
   double theValue,maxValue,minValue;
   short isLess;
   short exitNext = false;

   if ( v != lastV ) {
      BOOL isIndependent;
      v -> get_IsIndependent(&isIndependent);
      if ( isIndependent ) {
         v -> get_MaxValue(&maxValue);
         v -> get_MinValue(&minValue);
         minValue = 0.000001 * fabs(maxValue - minValue);
         v -> Start();
      } else {
         v -> get_Value(&theValue);
      }
      while ( true ) {

         WaitForSingleObject(threadControl,INFINITE);
         ReleaseMutex(threadControl);

         if ( terminateNow ) break;

         doVar(pszNames,GetNext(v),lastV);

         if ( ! isIndependent ) break;

         v -> Step();
         v -> get_Value(&theValue);

         if ( exitNext ) break;

         v -> get_IsLess(maxValue,theValue,&isLess);

         if ( isLess ) {
            if ( theValue > (maxValue + minValue) ) break;
            v -> put_Value(maxValue);
            exitNext = true;
         }

      }
      return TRUE;
   }
 
   ULONG cbStringSize;
   char* pszResults;
   IVariable *vList;
   BOOL isIndependent;
   static char szTest[128];
   double functionValue;

   lastV -> get_IsIndependent(&isIndependent);
   if ( isIndependent ) {
      lastV -> get_MaxValue(&maxValue);
      lastV -> get_MinValue(&minValue);
  } else
      lastV -> get_Value(&maxValue);

   minValue = 0.000001 * fabs(maxValue - minValue);

   lastV -> Start(); 
   while ( true ) {

      WaitForSingleObject(threadControl,INFINITE);
      ReleaseMutex(threadControl);

      if ( terminateNow ) break;

      cbStringSize = 1;
      vList = (IVariable *)NULL;
      while ( vList = GetNext(vList) ) {
         vList -> get_Value(&theValue);
         cbStringSize += sprintf(szTest,"%g, ",theValue);
      }

      functionValue = eval(pszFunctionExpression);

      cbStringSize += sprintf(szTest,"%g, ",functionValue);

      char *p = (pszResults = new char[cbStringSize]);
      memset(pszResults,0,cbStringSize);
      vList = (IVariable *)NULL;
      while ( vList = GetNext(vList) ) {
         vList -> get_Value(&theValue);
         p += sprintf(p,"%g, ",theValue);
      }
 
      if ( fabs(theValue - maxValue) < minValue ) exitNext = true;

      theValue = eval(pszFunctionExpression);

      p += sprintf(p,"%g,",functionValue);

      Fire_TakeResultString(++functionIterationCount,pszNames,pszResults);

      delete [] pszResults;

      if ( ! isIndependent ) break;

      lastV -> Step();

      lastV -> get_Value(&theValue); 

      if ( exitNext ) break;

      lastV -> get_IsLess(maxValue,theValue,&isLess);
      if ( isLess ) {
         if ( theValue > (maxValue + minValue)) break;
         lastV -> put_Value(maxValue);
         exitNext = true;
      }
            
   }

   return TRUE;
   }
