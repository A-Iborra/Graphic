/*

                       Copyright (c) 2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include "Function.h"

#include "ConnectedEvaluator.h"

   STDAPI GSystemsEvaluatorDllGetClassObject(HMODULE hModule,REFCLSID clsid,REFIID riid,void** ppObject);

   IClassFactory* ConnectedEvaluator::pIClassFactory_Evaluator = NULL;;

   ConnectedEvaluator::ConnectedEvaluator(Function* pp) : 
      pParent(pp),
      evaluator(NULL),
      pIConnectionPoint(NULL),
      dwConnectionCookie(0),
      evaluatorEvents(pp) {

#ifdef EMBEDDED_EVALUATOR
   if ( ! ConnectedEvaluator::pIClassFactory_Evaluator ) 
      GSystemsEvaluatorDllGetClassObject(hModule,CLSID_Evaluator,IID_IClassFactory,reinterpret_cast<void **>(&ConnectedEvaluator::pIClassFactory_Evaluator));
#else
   if ( ! ConnectedEvaluator::pIClassFactory_Evaluator ) 
      CoGetClassObject(CLSID_Evaluator,CLSCTX_INPROC_SERVER,NULL,IID_IClassFactory,reinterpret_cast<void**>(&ConnectedEvaluator::pIClassFactory_Evaluator));
#endif
   pIClassFactory_Evaluator -> CreateInstance(NULL,IID_IEvaluator,reinterpret_cast<void **>(&evaluator));

   IConnectionPointContainer* pIConnectionPointContainer;
   evaluator -> QueryInterface(IID_IConnectionPointContainer,reinterpret_cast<void**>(&pIConnectionPointContainer));
   pIConnectionPointContainer -> FindConnectionPoint(IID_IEvaluatorEvents,&pIConnectionPoint);
   pIConnectionPoint -> Advise(&evaluatorEvents,&dwConnectionCookie);
   pIConnectionPointContainer -> Release();

   };


   ConnectedEvaluator::~ConnectedEvaluator() {
   if ( pIConnectionPoint ) {
      pIConnectionPoint -> Unadvise(dwConnectionCookie);
      pIConnectionPoint -> Release();
   }
   pIConnectionPoint = NULL;
   };
   

   long __stdcall ConnectedEvaluator::QueryInterface(REFIID riid,void **ppv) {
   return evaluator -> QueryInterface(riid,ppv);
   }
   unsigned long __stdcall ConnectedEvaluator::AddRef() {
   return evaluator -> AddRef();
   }
   unsigned long __stdcall ConnectedEvaluator::Release() {
   return evaluator -> Release();
   }


   HRESULT ConnectedEvaluator::GetTypeInfoCount(UINT * pctinfo) { 
   return evaluator -> GetTypeInfoCount(pctinfo);
   } 


   long __stdcall ConnectedEvaluator::GetTypeInfo(UINT itinfo,LCID lcid,ITypeInfo **pptinfo) { 
   return evaluator -> GetTypeInfo(itinfo,lcid,pptinfo);
   } 
 

   HRESULT ConnectedEvaluator::GetIDsOfNames(REFIID riid,OLECHAR** rgszNames,UINT cNames,LCID lcid,DISPID* rgdispid) { 
   return evaluator -> GetIDsOfNames(riid,rgszNames,cNames,lcid,rgdispid);
   }


   HRESULT ConnectedEvaluator::Invoke(DISPID dispidMember, REFIID riid, LCID lcid, 
                                           WORD wFlags,DISPPARAMS FAR* pdispparams, VARIANT FAR* pvarResult,
                                           EXCEPINFO FAR* pexcepinfo, UINT FAR* puArgErr) { 
   return evaluator -> Invoke(dispidMember,riid,lcid,wFlags,pdispparams,pvarResult,pexcepinfo,puArgErr);
   }


   HRESULT ConnectedEvaluator::get_Eval(BSTR bstrExpression,double* pTheResult) {
   return evaluator -> get_Eval(bstrExpression,pTheResult);
   }

 
   HRESULT ConnectedEvaluator::Evaluate_szExpression(char* szExpression,double* pvalue) {
   return evaluator -> Evaluate_szExpression(szExpression,pvalue);
   }

 
   HRESULT ConnectedEvaluator::Consume_szExpression(char* szExpression,double* pvalue) {
   return evaluator -> Consume_szExpression(szExpression,pvalue);
   }

 
   HRESULT ConnectedEvaluator::Evaluate_szFunction(char* szExpression) {
   return evaluator -> Evaluate_szFunction(szExpression);
   }

 
   HRESULT ConnectedEvaluator::UndefineVariable(IVariable *v) {
   return evaluator -> UndefineVariable(v);
   }
 
 
   HRESULT ConnectedEvaluator::DefineVariable(IVariable *v) {
   return evaluator -> DefineVariable(v);
   }
 
 
   HRESULT ConnectedEvaluator::UndefineAllVariables() {
   return evaluator -> UndefineAllVariables();
   }


   HRESULT ConnectedEvaluator::IsVariable(BSTR bstrName,BOOL* p) {
   return evaluator -> IsVariable(bstrName,p);
   }


   HRESULT ConnectedEvaluator::Terminate() {
   return evaluator -> Terminate();
   }


   HRESULT ConnectedEvaluator::Suspend() {
   return evaluator -> Suspend();
   }


   HRESULT ConnectedEvaluator::Resume() {
   return evaluator -> Resume();
   }


   HRESULT ConnectedEvaluator::IsBusy() {
   return evaluator -> IsBusy();
   }

   HRESULT ConnectedEvaluator::Evaluate_bstrExpression(BSTR bstrExpression,double *result) {
errorSound();
   return evaluator -> Evaluate_bstrExpression(bstrExpression,result);
   }
