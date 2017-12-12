/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <olectl.h>

#include <vector>

#include "Function.h"
#include "Graphic_resource.h"

#ifdef MULTITHREADED_EVALUATOR
#define UNMARSHALL_INTERFACE                                                                                             \
      IGSFunctioNaterEvents* p;                                                                                          \
      CreateStreamOnHGlobal(hglMarshalling,FALSE,&pIStream_Marshalling);                                                 \
      if ( S_OK != CoUnmarshalInterface(pIStream_Marshalling,IID_IDispatch,reinterpret_cast<void**>(&p)) )               \
         p = reinterpret_cast<IGSFunctioNaterEvents*>(connectData.pUnk);                                                 \
      pIStream_Marshalling -> Release();                                                                                 \
      pIStream_Marshalling = NULL;
#else
#define UNMARSHALL_INTERFACE                                                                                             \
      IGSFunctioNaterEvents* p = reinterpret_cast<IGSFunctioNaterEvents*>(connectData.pUnk);                             
#endif

#define STANDARD_NOARGUMENT_EVENT(id)                                                                                    \
   IEnumConnections* pIEnum;                                                                                             \
   CONNECTDATA connectData;                                                                                              \
   VARIANT varResult;                                                                                                    \
                                                                                                                         \
   pIConnectionPoint -> EnumConnections(&pIEnum);                                                                        \
                                                                                                                         \
   if ( ! pIEnum ) return;                                                                                               \
                                                                                                                         \
   while ( 1 ) {                                                                                                         \
                                                                                                                         \
      if ( pIEnum -> Next(1, &connectData, NULL) ) break;                                                                \
                                                                                                                         \
      UNMARSHALL_INTERFACE                                                                                               \
                                                                                                                         \
      VariantClear(&varResult);                                                                                          \
                                                                                                                         \
      DISPPARAMS disp = { NULL, NULL, 0, 0 };                                                                            \
      p -> Invoke(id, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);                    \
                                                                                                                         \
   }                                                                                                                     \
                                                                                                                         \
   pIEnum -> Release();                                                                                                  \


   STDMETHODIMP Function::EnumConnectionPoints(IEnumConnectionPoints **ppEnum) {
   _IConnectionPoint* connectionPoints[1];
   *ppEnum = NULL;
 
   if ( enumConnectionPoints ) delete enumConnectionPoints;
 
   connectionPoints[0] = pIConnectionPoint;
   enumConnectionPoints = new _IEnumConnectionPoints(this,connectionPoints,1);
 
   return enumConnectionPoints -> QueryInterface(IID_IEnumConnectionPoints,(void **)ppEnum);
   }
 
 
   STDMETHODIMP Function::FindConnectionPoint(REFIID riid,IConnectionPoint **ppCP) {
   *ppCP = NULL;
 
   if ( riid == DIID_IGSFunctioNaterEvents )
      return pIConnectionPoint -> QueryInterface(IID_IConnectionPoint,(void **)ppCP);
  
   return iProperties -> FindConnectionPoint(riid,ppCP);

   }
 
 
   long Function::fire_UndefinedVariable(BSTR bstrVariableName) {

   HRESULT hr = S_OK;

   IEnumConnections* pIEnum;
   CONNECTDATA connectData;
   VARIANT arg;
   VARIANT varResult;
   unsigned int errArg = 0;

   pIConnectionPoint -> EnumConnections(&pIEnum);

   if ( ! pIEnum ) return hr;

   while ( 1 ) {

      if ( pIEnum -> Next(1, &connectData, NULL) ) break;

      UNMARSHALL_INTERFACE

      memset(&arg,0,sizeof(VARIANT));
      memset(&varResult,0,sizeof(VARIANT));

      VariantClear(&arg);
      VariantClear(&varResult);

      arg.vt = VT_BSTR;
      arg.bstrVal = SysAllocString(bstrVariableName);

      DISPPARAMS disp = { &arg, NULL, 1, 0 };
      hr = p -> Invoke(functionEventID_UndefinedVariable, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, &errArg);

      SysFreeString(arg.bstrVal);

   }

   pIEnum -> Release();

   return hr;
   }


   void Function::fire_UndefinedFunction(BSTR functionName) {

   IEnumConnections* pIEnum;
   CONNECTDATA connectData;
   VARIANT variant; 
   VARIANT varResult;

   pIConnectionPoint -> EnumConnections(&pIEnum);

   if ( ! pIEnum ) return;

   while ( 1 ) {

      if ( pIEnum -> Next(1, &connectData, NULL) ) break;

      UNMARSHALL_INTERFACE

      VariantClear(&varResult); 
      variant.vt = VT_BSTR;
      variant.bstrVal = SysAllocString(functionName);
      DISPPARAMS disp = { &variant, NULL, 1, 0 };
      p -> Invoke(functionEventID_UndefinedFunction, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);

      SysFreeString(variant.bstrVal);

   }

   pIEnum -> Release();

   return;
   }



   void Function::fire_Clear() {
   STANDARD_NOARGUMENT_EVENT(functionEventID_Clear)
   return;
   }


   void Function::fire_Parsed() {
   STANDARD_NOARGUMENT_EVENT(functionEventID_Parsed)
   return;
   }


   void Function::fire_Started(long cntExpectedIterations) {

   IEnumConnections* pIEnum;
   CONNECTDATA connectData;
   VARIANT variant; 
   VARIANT varResult;

   pIConnectionPoint -> EnumConnections(&pIEnum);

   if ( ! pIEnum ) return;

   while ( 1 ) {

      if ( pIEnum -> Next(1, &connectData, NULL) ) break;

      UNMARSHALL_INTERFACE

      VariantClear(&varResult);
      variant.vt = VT_I4;
      variant.lVal = cntExpectedIterations;
      DISPPARAMS disp = { &variant, NULL, 1, 0 };
      p -> Invoke(functionEventID_Started, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);

   }

   pIEnum -> Release();

   return;
   }
 
 
   void Function::fire_Paused() {
   STANDARD_NOARGUMENT_EVENT(functionEventID_Paused)
   return;
   }
 
 
   void Function::fire_Resumed() {
   STANDARD_NOARGUMENT_EVENT(functionEventID_Resumed)
   return;
   }
 
 
   void Function::fire_Stopped() {
   STANDARD_NOARGUMENT_EVENT(functionEventID_Stopped)
   return;
   }
 
 
   void Function::fire_TakeValues(long iterationNumber,char* pszNames,char* pszValueString,char** pszCookedResults) {

   if ( pszCookedResults )
      *pszCookedResults = NULL;

   std::vector<BSTR *> theNames;
   std::vector<double *> theValues;
   std::vector<char *> szNames;
   std::vector<char *> szValues;
   long n;

   char *pszWorkingValues = new char[strlen(pszValueString) + 1];
   memset(pszWorkingValues,0,strlen(pszValueString) + 1);
   strcpy(pszWorkingValues,pszValueString);

   char *p,*s = strtok(pszWorkingValues,", ");
   long cntChars = 0;
   while ( s ) {
      cntChars += strlen(s) + 3;
      theValues.push_back(new double(atof(s)));
      p = new char[strlen(s) + 1];
      memset(p,0,strlen(s) + 1);
      strcpy(p,s);
      szValues.push_back(p);
      s = strtok(NULL,", ");
   }        

   delete [] pszWorkingValues;

   s = pszNames;
   while ( s[0] ) {
      cntChars += strlen(s) + 2;
      BSTR* bstrName = new BSTR;
      *bstrName = SysAllocStringLen(NULL,n = strlen(s));
      MultiByteToWideChar(CP_ACP,0,s,-1,*bstrName,n);
      theNames.push_back(bstrName);
      p = new char[n + 1];
      memset(p,0,n + 1);
      strcpy(p,s);
      szNames.push_back(p);
      s += strlen(s) + 1;
   }

   char *pszResults = new char[cntChars + 1];
   memset(pszResults,0,cntChars + 1);
   //char *pName = NULL;
   p = pszResults;
   int k = 0;
   for ( char *pName : szNames ) { //while ( pName = szNames.GetNext(pName) ) {
      char *pValue = szValues[k];
      p += sprintf(p,"%s = %s ",pName,pValue);
      k++;
   }
   
   BSTR bstrResults = SysAllocStringLen(NULL,strlen(pszResults) + 1);
   MultiByteToWideChar(CP_ACP,0,pszResults,-1,bstrResults,strlen(pszResults) + 1);

   SAFEARRAYBOUND rgb;
   double *pd;

   rgb.lLbound = 1;
   rgb.cElements = theValues.size();

   SAFEARRAY* saValues = SafeArrayCreate(VT_R8,1,&rgb);
   SAFEARRAY* saNames = SafeArrayCreate(VT_BSTR,1,&rgb);

   SafeArrayAccessData(saValues,reinterpret_cast<void**>(&pd));

   double *pd2 = (double*)NULL;
   for ( double *pd2 : theValues ) { //while ( pd2 = theValues.GetNext(pd2) ) {
      *pd = *pd2;
      pd++;
   }

   SafeArrayUnaccessData(saValues);

   //BSTR* pbstr = NULL;
   n = 0;
   for ( BSTR *pbstr : theNames ) { //while ( pbstr = theNames.GetFirst() ) {
      n++;
      SafeArrayPutElement(saNames,&n,*pbstr);
      SysFreeString(*pbstr);
      delete pbstr;
      //theNames.Remove(pbstr);
   }

   theNames.clear();

   IEnumConnections* pIEnum;

   pIConnectionPoint -> EnumConnections(&pIEnum);

   if ( ! pIEnum ) {
      if ( pszCookedResults ) 
         *pszCookedResults = pszResults;
      return;
   }
 
   while ( 1 ) {

      CONNECTDATA connectData;
   
      if ( pIEnum -> Next(1, &connectData, NULL) ) break;

      VARIANTARG pvars[4];
   
      VariantInit(&pvars[0]);
      VariantInit(&pvars[1]);
      VariantInit(&pvars[2]);
   
      pvars[0].vt = VT_BYREF | VT_ARRAY | VT_R8;
      pvars[0].pparray = &saValues;
   
      pvars[1].vt = VT_BYREF | VT_ARRAY | VT_BSTR;
      pvars[1].pparray = &saNames;
   
      pvars[2].vt = VT_I4;
      pvars[2].lVal = theValues.size();
   
      pvars[3].vt = VT_I4;
      pvars[3].lVal = iterationNumber;
   
      DISPPARAMS disp = { pvars, NULL, 4, 0 };
   
      VARIANT varResult = {VT_EMPTY};

      VariantClear(&varResult);
   
      UNMARSHALL_INTERFACE
   
      p -> Invoke(functionEventID_TakeValues,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_METHOD,&disp,&varResult,NULL,NULL);
   
      disp.cArgs = 2;

      VariantInit(&pvars[0]);
      VariantInit(&pvars[1]);
      pvars[0].vt = VT_BSTR;
      pvars[0].bstrVal = SysAllocString(bstrResults);
      pvars[1].vt = VT_I4;
      pvars[1].lVal = iterationNumber;

      VariantClear(&varResult);

      p -> Invoke(functionEventID_TakeResults,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_METHOD,&disp,&varResult,NULL,NULL);

   }

   SysFreeString(bstrResults);

   SafeArrayDestroy(saValues);
   
   for ( double *v : theValues )
      delete v;
   theValues.clear();

   for ( char *s : szNames ) 
      delete [] s;

   szNames.clear();

   for ( char *s : szValues )
      delete [] s;

   szValues.clear();

   pIEnum -> Release();

   if ( pszCookedResults ) 
      *pszCookedResults = pszResults;
   else 
      delete [] pszResults;

   return;
   }

 
   void Function::fire_Finished() {
   STANDARD_NOARGUMENT_EVENT(functionEventID_Finished)
   if ( hwndSpecDialog )
      EnableWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_PLOT_PROPERTIES),pIPlot ? TRUE : FALSE);
   return;
   }


   void Function::fire_DivideByZero() {
   STANDARD_NOARGUMENT_EVENT(functionEventID_DivideByZero)
   return;
   }


   void Function::fire_InvalidArgument(BSTR bstrFunctionName,double argumentValue) {

   IEnumConnections* pIEnum;
   CONNECTDATA connectData;
   VARIANT varResult; 

   pIConnectionPoint -> EnumConnections(&pIEnum);

   if ( ! pIEnum ) return;

   while ( 1 ) {

      if ( pIEnum -> Next(1, &connectData, NULL) ) break;

      UNMARSHALL_INTERFACE

      VARIANTARG pvars[2];

      memset(pvars,0,2 * sizeof(VARIANTARG));
 
//      VariantClear(&pvars[0]);
//      VariantClear(&pvars[1]);

      pvars[0].vt = VT_R8;
      pvars[0].dblVal = argumentValue;

      pvars[1].vt = VT_BSTR;
      pvars[1].bstrVal = SysAllocString(bstrFunctionName);

      VariantClear(&varResult);

      DISPPARAMS disp = { pvars, NULL, 2, 0 }; 

      p -> Invoke(functionEventID_InvalidArgument, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL); 

      SysFreeString(pvars[1].bstrVal);
   }

   pIEnum -> Release();

   return;
   }