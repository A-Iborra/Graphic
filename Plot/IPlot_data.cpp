/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002,2008 Nathan T. Clark

*/

#include <windows.h>

#include "Utils.h"
#include "Plot.h"
 

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

   if ( 1 == n ) return S_FALSE;

   DataPoint currentPoint;

   char *data = new char[n];
 
   memset(data,0,n);

   WideCharToMultiByte(CP_ACP,0,input,-1,data,n,0,0);
 
   if ( * data ) {
      currentPoint.x = evalConsume(pIEvaluator,data);
      if ( * data ) {
         currentPoint.y = evalConsume(pIEvaluator,data);
         if ( * data ) {
            currentPoint.z = evalConsume(pIEvaluator,data);
            pIDataSet -> put_DataArity(DATA_ARITY_3D);
         }
         else {
            currentPoint.z = 0.0;
            pIDataSet -> put_DataArity(DATA_ARITY_2D);
         }
      } else {
         delete [] data;
         return S_FALSE;
      }
   } else {
      delete [] data;
      return S_FALSE;
   }
 
   pIDataSet -> pushDataPoint(&currentPoint);

   haveAnyData = true;
 
   delete [] data;

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


