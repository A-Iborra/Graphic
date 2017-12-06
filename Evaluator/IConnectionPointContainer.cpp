/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <olectl.h>

#include "Evaluator.h"
#include "List.cpp"


   STDMETHODIMP Evaluator::EnumConnectionPoints(IEnumConnectionPoints **ppEnum) {
   _IConnectionPoint* connectionPoints[1];
   *ppEnum = NULL;
 
   if ( enumConnectionPoints ) delete enumConnectionPoints;
 
   connectionPoints[0] = &connectionPoint;
   enumConnectionPoints = new _IEnumConnectionPoints(this,connectionPoints,1);
 
   return enumConnectionPoints -> QueryInterface(IID_IEnumConnectionPoints,(void **)ppEnum);
   }
 
 
   STDMETHODIMP Evaluator::FindConnectionPoint(REFIID riid,IConnectionPoint **ppCP) {
   *ppCP = NULL;
 
   if ( riid == IID_IEvaluatorEvents )
      return connectionPoint.QueryInterface(IID_IConnectionPoint,(void **)ppCP);
  
   return CONNECT_E_NOCONNECTION;
   }
 
 
   void Evaluator::Fire_Started(long expectedIterations) {

   IEnumConnections* pIEnum;
   CONNECTDATA connectData;
 
   connectionPoint.EnumConnections(&pIEnum);

   if ( ! pIEnum ) return;
 
   while ( 1 ) {

      if ( pIEnum -> Next(1, &connectData, NULL) ) break;

      IEvaluatorEvents* pIEvaluatorEvents = reinterpret_cast<IEvaluatorEvents*>(connectData.pUnk);
 
      pIEvaluatorEvents -> Started(expectedIterations);
 
   }

   pIEnum -> Release();

   return;
   }
 

   void Evaluator::Fire_TakeResultString(long iterationCount,char* pszNames,char* pszResults) {

   IEnumConnections* pIEnum;
   CONNECTDATA connectData;

   connectionPoint.EnumConnections(&pIEnum);
 
   if ( ! pIEnum ) return;

   while ( 1 ) {
   
      if ( pIEnum -> Next(1, &connectData, NULL) ) break;

      IEvaluatorEvents* pIEvaluatorEvents = reinterpret_cast<IEvaluatorEvents*>(connectData.pUnk);

      pIEvaluatorEvents -> TakeResultString(iterationCount,pszNames,pszResults);

   }

   pIEnum -> Release();

   return;
   }
 
 
   void Evaluator::Fire_UnknownVariable(char* varName) {
   IEnumConnections* pIEnum;
   CONNECTDATA connectData;

   connectionPoint.EnumConnections(&pIEnum);
 
   if ( ! pIEnum ) return;

   while ( 1 ) {
   
      if ( pIEnum -> Next(1, &connectData, NULL) ) break;

      IEvaluatorEvents* pIEvaluatorEvents = reinterpret_cast<IEvaluatorEvents*>(connectData.pUnk);

      BSTR bstrVariableName = SysAllocStringLen(NULL,strlen(varName) + 1); 
      MultiByteToWideChar(CP_ACP,0,varName,-1,bstrVariableName,strlen(varName) + 1);

      pIEvaluatorEvents -> UnknownVariable(bstrVariableName);

      SysFreeString(bstrVariableName);

   }

   pIEnum -> Release();

   return;
   }
 
 
   void Evaluator::Fire_UnknownFunction(char* funName) {
   IEnumConnections* pIEnum;
   CONNECTDATA connectData;

   connectionPoint.EnumConnections(&pIEnum);

   if ( ! pIEnum ) return;
 
   while ( 1 ) {

      if ( pIEnum -> Next(1, &connectData, NULL) ) break;

      IEvaluatorEvents* pIEvaluatorEvents = reinterpret_cast<IEvaluatorEvents*>(connectData.pUnk);

      BSTR bstrFunctionName = SysAllocStringLen(NULL,strlen(funName) + 1); 
      MultiByteToWideChar(CP_ACP,0,funName,-1,bstrFunctionName,strlen(funName) + 1);

      pIEvaluatorEvents -> UnknownFunction(bstrFunctionName);
 
      SysFreeString(bstrFunctionName);

   }

   pIEnum -> Release();

   return;
   }

 
   void Evaluator::Fire_MathError(char* funName,double argument) {
   IEnumConnections* pIEnum;
   CONNECTDATA connectData;

   connectionPoint.EnumConnections(&pIEnum);

   if ( ! pIEnum ) return;
 
   while ( 1 ) {

      if ( pIEnum -> Next(1, &connectData, NULL) ) break;

      IEvaluatorEvents* pIEvaluatorEvents = reinterpret_cast<IEvaluatorEvents*>(connectData.pUnk);

      BSTR bstrFunctionName = SysAllocStringLen(NULL,strlen(funName) + 1); 
      MultiByteToWideChar(CP_ACP,0,funName,-1,bstrFunctionName,strlen(funName) + 1);

      pIEvaluatorEvents -> MathError(bstrFunctionName,argument);
 
      SysFreeString(bstrFunctionName);

   }

   pIEnum -> Release();

   return;
   }

 
   void Evaluator::Fire_Finished() {
   IEnumConnections* pIEnum;
   CONNECTDATA connectData;
 
   connectionPoint.EnumConnections(&pIEnum);

   if ( ! pIEnum ) return;
 
   while ( 1 ) {

      if ( pIEnum -> Next(1, &connectData, NULL) ) break;

      IEvaluatorEvents* pIEvaluatorEvents = reinterpret_cast<IEvaluatorEvents*>(connectData.pUnk);
 
      pIEvaluatorEvents -> Finished();

   }
 
   pIEnum -> Release();
 
   return;
   }


   void Evaluator::Fire_DivideByZero() {
   IEnumConnections* pIEnum;
   CONNECTDATA connectData;
 
   connectionPoint.EnumConnections(&pIEnum);

   if ( ! pIEnum ) return;
 
   while ( 1 ) {

      if ( pIEnum -> Next(1, &connectData, NULL) ) break;

      IEvaluatorEvents* pIEvaluatorEvents = reinterpret_cast<IEvaluatorEvents*>(connectData.pUnk);
 
      pIEvaluatorEvents -> DivideByZero();

   }
 
   pIEnum -> Release();
 
   return;
   }


   void Evaluator::Fire_InvalidArgument(char* funName,double invalidValue) {
   IEnumConnections* pIEnum;
   CONNECTDATA connectData;

   connectionPoint.EnumConnections(&pIEnum);

   if ( ! pIEnum ) return;
 
   while ( 1 ) {

      if ( pIEnum -> Next(1, &connectData, NULL) ) break;

      IEvaluatorEvents* pIEvaluatorEvents = reinterpret_cast<IEvaluatorEvents*>(connectData.pUnk);

      BSTR bstrFunctionName = SysAllocStringLen(NULL,strlen(funName) + 1); 
      MultiByteToWideChar(CP_ACP,0,funName,-1,bstrFunctionName,strlen(funName) + 1);

      pIEvaluatorEvents -> InvalidArgument(bstrFunctionName,invalidValue);
 
      SysFreeString(bstrFunctionName);

   }

   pIEnum -> Release();

   return;
   }

