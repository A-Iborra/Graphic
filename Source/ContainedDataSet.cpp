// Copyright 2017 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"

#include "utils.h"

#include "Graphic.h"
#include "ContainedDataSet.h"


   ContainedDataSet::ContainedDataSet(G* pp,ULONG_PTR pID,HWND hwndT,IDataSet *pIDS,IUnknown* pIUnknownObject,REFIID riidEventsInterface) : 
      plotID(pID),
      pIDataSet(pIDS),
      ContainedObject(pp,hwndT,pIUnknownObject,riidEventsInterface) {};


   long __stdcall ContainedDataSet::QueryInterface(REFIID riid,void **ppv) {

   if ( IID_IDataSetEvents == riid ) {
      *ppv = static_cast<IDataSetEvents *>(this);
      AddRef();
      return S_OK;
   }

   if ( S_OK == ContainedObject::QueryInterface(riid,ppv) ) 
      return S_OK;

   return pDataSet() -> QueryInterface(riid,ppv);
   }
   unsigned long __stdcall ContainedDataSet::AddRef() { return ContainedObject::AddRef(); }
   unsigned long __stdcall ContainedDataSet::Release() { return ContainedObject::Release(); }

   HRESULT ContainedDataSet::Clear() {
   return S_OK;
   }

   HRESULT ContainedDataSet::Started(long expectedValues) {
   //NTC: A data set would have already populated itself, so, unlike other objects, a PrepareForData call is not necessary here, but setting OkayToPlot is necessary
   IPlot *pIPlot = pParent -> plotList.Get(plotID);
   pIPlot -> put_OkToPlot(1);
   return S_OK;
   }

   HRESULT ContainedDataSet::TakeValues(long iterationNumber,long valueCount,SAFEARRAY** bstrNames,SAFEARRAY** arrayValues) {
   long index,lBound;
   long n = 0;
   static char szTest[128];
   double v;

   SafeArrayGetLBound(*arrayValues,1,&lBound);
   index = lBound - 1;
   for ( long k = 0; k < valueCount; k++ ) {
      index++;
      SafeArrayGetElement(*arrayValues,&index,&v);
      n += sprintf(szTest,"%g, ",v);
   }

   char* p,*pszValues = new char[n + 1];

   p = pszValues;
   memset(pszValues,0,n + 1);
   memset(pszValues,' ',n);
   index = lBound - 1;
   for ( long k = 0; k < valueCount; k++ ) {
      index++;
      SafeArrayGetElement(*arrayValues,&index,&v);
      p += sprintf(p,"%g, ",v);

   }      
   BSTR bstrValues = SysAllocStringLen(NULL,n + 1);
   MultiByteToWideChar(CP_ACP,0,pszValues,-1,bstrValues,n + 1);
   pParent -> TakeDataString(bstrValues,plotID);
   delete [] pszValues;
   SysFreeString(bstrValues);
   return S_OK;
   }

   HRESULT ContainedDataSet::Finished() {
   pParent -> FinishedWithData(plotID);
   pParent -> Draw(plotID);
   return S_OK;
   }
