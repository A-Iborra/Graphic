/*

                       Copyright (c) 2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <stdio.h>

#include "utils.h"

#include "Graphic.h"
#include "ContainedDataSet.h"


   ContainedDataSet::ContainedDataSet(G* pp,long pID,HWND hwndT,IDataSet *pIDS,IUnknown* pIUnknownObject,REFIID riidEventsInterface) : 
      plotID(pID),
      pIDataSet(pIDS),
      ContainedObject(pp,hwndT,pIUnknownObject,riidEventsInterface) {};


   long __stdcall ContainedDataSet::QueryInterface(REFIID riid,void **ppv) {

   if ( IID_IDataSetEvents == riid ) 
      *ppv = static_cast<IDataSetEvents *>(this);
   else

   //if ( IID_IDispatch == riid ) 
   //   *ppv = static_cast<IDispatch*>(this);
   //else

      return ContainedObject::QueryInterface(riid,ppv);

   reinterpret_cast<IUnknown*>(*ppv) -> AddRef();
   return S_OK;
   }
   unsigned long __stdcall ContainedDataSet::AddRef() { return ContainedObject::AddRef(); }
   unsigned long __stdcall ContainedDataSet::Release() { return ContainedObject::Release(); }

#if 0
   HRESULT ContainedFunction::UndefinedVariable(BSTR bstrName) {
   return S_OK;
   }


   HRESULT ContainedFunction::UndefinedFunction(BSTR bstrName) {
   return S_OK;
   }

   HRESULT ContainedFunction::Clear() {
   return S_OK;
   }

   HRESULT ContainedFunction::Parsed() {
   return S_OK;
   }

   HRESULT ContainedFunction::Started(long expectedValues) {
   pParent -> PrepareForData(plotID);
   return S_OK;
   }


   HRESULT ContainedFunction::Paused() {
   return S_OK;
   }

   HRESULT ContainedFunction::Resumed() {
   return S_OK;
   }

   HRESULT ContainedFunction::Stopped() {
   return S_OK;
   }

   HRESULT ContainedFunction::TakeValues(long iterationNumber,long valueCount,SAFEARRAY** bstrNames,SAFEARRAY** arrayValues) {
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

   HRESULT ContainedFunction::TakeResults(long iterationNumber,BSTR bstrResults) {
   pParent -> TakeDataString(bstrResults,plotID);
   return S_OK;
   }

   HRESULT ContainedFunction::Finished() {
   pParent -> FinishedWithData(plotID);
   pParent -> Draw(plotID);
   return S_OK;
   }

#endif

