// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "DataSet.h"
#include <vector>

#include "Graphic_resource.h"

#define STANDARD_NOARGUMENT_EVENT(fn)                                                                                    \
   IEnumConnections* pIEnum;                                                                                             \
   CONNECTDATA connectData;                                                                                              \
                                                                                                                         \
   pIConnectionPoint -> EnumConnections(&pIEnum);                                                                        \
                                                                                                                         \
   if ( ! pIEnum ) return;                                                                                               \
                                                                                                                         \
   while ( 1 ) {                                                                                                         \
                                                                                                                         \
      if ( pIEnum -> Next(1, &connectData, NULL) ) break;                                                                \
                                                                                                                         \
      IDataSetEvents * p = reinterpret_cast<IDataSetEvents *>(connectData.pUnk);                                         \
                                                                                                                         \
      p -> fn();                                                                                                         \
                                                                                                                         \
   }                                                                                                                     \
                                                                                                                         \
   pIEnum -> Release();                                                                                                  \


   STDMETHODIMP DataSet::EnumConnectionPoints(IEnumConnectionPoints **ppEnum) {
   _IConnectionPoint* connectionPoints[1];
   *ppEnum = NULL;
 
   if ( enumConnectionPoints ) delete enumConnectionPoints;
 
   connectionPoints[0] = pIConnectionPoint;
   enumConnectionPoints = new _IEnumConnectionPoints(this,connectionPoints,1);
 
   return enumConnectionPoints -> QueryInterface(IID_IEnumConnectionPoints,(void **)ppEnum);
   }
 
 
   STDMETHODIMP DataSet::FindConnectionPoint(REFIID riid,IConnectionPoint **ppCP) {
   *ppCP = NULL;
 
   if ( riid == IID_IDataSetEvents )
      return pIConnectionPoint -> QueryInterface(IID_IConnectionPoint,(void **)ppCP);
  
   return pIGProperties -> FindConnectionPoint(riid,ppCP);

   }
 
   void DataSet::fire_Clear() {
   STANDARD_NOARGUMENT_EVENT(Clear)
   return;
   }
 
   void DataSet::fire_Started(long countValues) {

   IEnumConnections* pIEnum;

   pIConnectionPoint -> EnumConnections(&pIEnum);

   if ( ! pIEnum ) 
      return;
 
   while ( 1 ) {

      CONNECTDATA connectData;
   
      if ( pIEnum -> Next(1, &connectData, NULL) ) break;

      IDataSetEvents * p = reinterpret_cast<IDataSetEvents *>(connectData.pUnk);                                         \
  
      p -> Started(countValues);

   }

   pIEnum -> Release();

   //if ( hwndSpecDialog )
   //   EnableWindow(GetDlgItem(hwndSpecDialog,IDDI_FUNCTION_PLOT_PROPERTIES),pIPlot ? TRUE : FALSE);
   return;
   }

   void DataSet::fire_TakeValues(long iterationNumber,char* pszNames,char* pszValueString,char** pszCookedResults) {

   if ( pszCookedResults )
      *pszCookedResults = NULL;

   std::vector<BSTR *> theNames;
   std::vector<double *> theValues;
   std::vector<char *> szNames;
   std::vector<char *> szValues;
   long n;

   char *pszWorkingValues = new char[(DWORD)strlen(pszValueString) + 1];
   memset(pszWorkingValues,0,(DWORD)strlen(pszValueString) + 1);
   strcpy(pszWorkingValues,pszValueString);

   char *p,*s = strtok(pszWorkingValues,", ");
   long cntChars = 0;
   while ( s ) {
      cntChars += (DWORD)strlen(s) + 3;
      theValues.push_back(new double(atof(s)));
      p = new char[(DWORD)strlen(s) + 1];
      memset(p,0,(DWORD)strlen(s) + 1);
      strcpy(p,s);
      szValues.push_back(p);
      s = strtok(NULL,", ");
   }        

   delete [] pszWorkingValues;

   s = pszNames;
   while ( s[0] ) {
      cntChars += (DWORD)strlen(s) + 2;
      BSTR* bstrName = new BSTR;
      *bstrName = SysAllocStringLen(NULL,n = (DWORD)strlen(s));
      MultiByteToWideChar(CP_ACP,0,s,-1,*bstrName,n);
      theNames.push_back(bstrName);
      p = new char[n + 1];
      memset(p,0,n + 1);
      strcpy(p,s);
      szNames.push_back(p);
      s += (DWORD)strlen(s) + 1;
   }

   char *pszResults = new char[cntChars + 1];
   memset(pszResults,0,cntChars + 1);
   p = pszResults;
   int k = 0;
   for ( char *pName : szNames ) {
      char *pValue = szValues[k];
      p += sprintf(p,"%s = %s ",pName,pValue);
      k++;
   }
   
   BSTR bstrResults = SysAllocStringLen(NULL,(DWORD)strlen(pszResults) + 1);
   MultiByteToWideChar(CP_ACP,0,pszResults,-1,bstrResults,(DWORD)strlen(pszResults) + 1);

   SAFEARRAYBOUND rgb;
   double *pd;

   rgb.lLbound = 1;
   rgb.cElements = (DWORD)theValues.size();

   SAFEARRAY* saValues = SafeArrayCreate(VT_R8,1,&rgb);
   SAFEARRAY* saNames = SafeArrayCreate(VT_BSTR,1,&rgb);

   SafeArrayAccessData(saValues,reinterpret_cast<void**>(&pd));

   double *pd2 = (double*)NULL;
   for ( double *pd2 : theValues ) {
      *pd = *pd2;
      pd++;
   }

   SafeArrayUnaccessData(saValues);

   n = 0;
   for ( BSTR *pbstr : theNames ) {
      n++;
      SafeArrayPutElement(saNames,&n,*pbstr);
      SysFreeString(*pbstr);
      delete pbstr;
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

      IDataSetEvents * p = reinterpret_cast<IDataSetEvents *>(connectData.pUnk);                                         \
  
      p -> TakeValues(iterationNumber,(DWORD)theValues.size(),&saNames,&saValues);

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

 
   void DataSet::fire_Finished() {

   STANDARD_NOARGUMENT_EVENT(Finished)

   if ( pOneShotOnFinishedLambda ) {
      (*pOneShotOnFinishedLambda)();
      delete pOneShotOnFinishedLambda;
      pOneShotOnFinishedLambda = NULL;
   }

   return;
   }
