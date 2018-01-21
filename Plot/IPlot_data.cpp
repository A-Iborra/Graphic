// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Plot.h"
#include "Utils.h"
 

   HRESULT Plot::PrepareForData() {
   pIDataSet -> ReSet();
   haveAnyData = false;
   return S_OK;
   }
 
 
   HRESULT Plot::TakeDataPoint(DataPoint *pdp) {
   if ( SUCCEEDED(pIDataSet -> pushDataPoint(pdp)) )
      haveAnyData = true;
   return S_OK;
   }


   HRESULT Plot::TakeDataString(BSTR input) {

   int n = wcslen(input) + 1;

   if ( 1 == n ) 
      return S_FALSE;

   DataPoint currentPoint;

   char *pszData = new char[n];
   char *pszRoot = pszData;

   memset(pszData,0,n);

   WideCharToMultiByte(CP_ACP,0,input,-1,pszData,n,0,0);
 
   if ( pszData[0] ) {
      currentPoint.x = evalConsume(pIEvaluator,pszData);
      if ( pszData[0] ) {
         currentPoint.y = evalConsume(pIEvaluator,pszData);
         if ( pszData[0] ) 
            currentPoint.z = evalConsume(pIEvaluator,pszData);
         else 
            currentPoint.z = 0.0;
      } else {
         delete [] pszRoot;
         return S_FALSE;
      }
   } else {
      delete [] pszRoot;
      return S_FALSE;
   }
 
   pIDataSet -> pushDataPoint(&currentPoint);

   haveAnyData = true;
 
   delete [] pszRoot;

   return S_OK;
   }


   HRESULT Plot::ResetData() {
   pIDataSet -> ReSet();
   haveAnyData = false;
   return S_OK;
   }


   HRESULT Plot::LastPoint(DataPoint* pLast) {
   DataList* pLastPoint;
   pIDataSet -> get_top(&pLastPoint);
   pLast -> x = pLastPoint -> data.x;
   pLast -> y = pLastPoint -> data.y;
   pLast -> z = pLastPoint -> data.z;
   return S_OK;
   }


   HRESULT Plot::PrepData() {
//   if ( ! haveAnyData ) return S_FALSE;
#if 0
   DataPoint extents[2];
   pIDataSet -> GetDomain(&extents[0],&extents[1]);
   if ( extents[0].x == extents[1].x ) extents[1].x += 0.1 * extents[0].x;
   if ( extents[0].x == extents[1].x ) extents[1].x = 1.0;
   if ( extents[0].y == extents[1].y ) extents[1].y += 0.1 * extents[0].y;
   if ( extents[0].y == extents[1].y ) extents[1].y = 1.0;
   if ( extents[0].z == extents[1].z ) extents[1].z += 0.1 * extents[0].z;
   if ( extents[0].z == extents[1].z ) extents[1].z = 1.0;
   pIDataSet -> SetDomain(&extents[0],&extents[1]);
#endif
   return S_OK;
   }


