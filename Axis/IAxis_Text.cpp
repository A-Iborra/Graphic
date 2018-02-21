// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Axis.h"

   // Properties

   STDMETHODIMP Axis::put_DrawText(VARIANT_BOOL dt) {
   drawTickLabels = dt;
   return S_OK;
   }

   STDMETHODIMP Axis::get_DrawText(VARIANT_BOOL* pdt) {
   if ( ! pdt ) return E_POINTER;
   *pdt = drawTickLabels;
   return S_OK;
   }

   STDMETHODIMP Axis::put_LabelText(BSTR bstrLabel) {
   pLabel -> put_Text(bstrLabel);
   return S_OK;
   }


   STDMETHODIMP Axis::get_LabelText(BSTR *pbstrLabel) {
   if ( ! pbstrLabel ) 
      return E_POINTER;
   pLabel -> get_Text(pbstrLabel);
   return S_OK;
   }


   STDMETHODIMP Axis::get_RepresentativeText(IDispatch **pText) {
   if ( ! pText ) return E_POINTER;
   return pRepresentativeText -> QueryInterface(IID_IDispatch,reinterpret_cast<void**>(pText));
   }

   
   STDMETHODIMP Axis::put_TickLabelPrecision(double newPrecision) {
   propertyAxisPrecision -> put_doubleValue(newPrecision);
   return S_OK;
   }


   STDMETHODIMP Axis::get_TickLabelPrecision(double *pPrecision) {
   if ( ! pPrecision ) return E_POINTER;
   propertyAxisPrecision -> get_doubleValue(pPrecision);
   return S_OK;
   }


   // Methods

   STDMETHODIMP Axis::GetTextList(void **pList) {
   *pList = reinterpret_cast<void *>(&textList);
   return S_OK;
   }
 
 
   STDMETHODIMP Axis::PrepText() {

   std::list<IText *> theTickLabels;
   IText *pIText = NULL;
   while ( pIText = textList.GetNext(pIText) ) {
      pIText -> PrepData();
      theTickLabels.insert(theTickLabels.end(),pIText);
   }

#if 1
   bool overlap = false;

   while ( pIText = textList.GetNext(pIText) ) {

      RECT rcOuter;

      pIText -> get_GDIBoundingBox(&rcOuter);

      long cx = rcOuter.right - rcOuter.left;

      rcOuter.left = rcOuter.left - (long)(0.25 * (double)cx);
      rcOuter.right = rcOuter.right + (long)(0.25 * (double)cx);

      cx = rcOuter.bottom - rcOuter.top;

      rcOuter.top = rcOuter.top - (long)(0.25 * (double)cx);
      rcOuter.bottom = rcOuter.bottom + (long)(0.25 * (double)cx);

      for ( IText *pt : theTickLabels ) {

         if ( pt == pIText )
            continue;

         RECT rcInner;

         pt -> get_GDIBoundingBox(&rcInner);

         cx = rcInner.right - rcInner.left;

         rcInner.left = rcInner.left - (long)(0.25 * (double)cx);

         rcInner.right = rcInner.right + (long)(0.25 * (double)cx);

         cx = rcInner.bottom - rcInner.top;

         rcInner.top = rcInner.top - (long)(0.25 * (double)cx);

         rcInner.bottom = rcInner.bottom + (long)(0.25 * (double)cx);

         if ( rcInner.left > rcOuter.right || rcInner.right < rcOuter.left || rcInner.top > rcOuter.bottom || rcInner.bottom < rcOuter.top ) 
            continue;

         overlap = true;

         break;

      }
      if ( overlap )
         break;
   }

   pIText = NULL;

   if ( overlap ) {
      long k = 0;
      while ( pIText = textList.GetNext(pIText) ) {
         k++;
         if ( k % 2 ) 
            pIText -> LineUp();
      }
   }
#endif

   if ( S_OK == pLabel -> HasContent() ) {

      double decent,maxDecentX = DBL_MAX,maxDecentY = DBL_MAX;
      DataPoint dp;

      IText *pIText = NULL;
      while ( pIText = textList.GetNext(pIText) ) {
         pIText -> get_minX(&decent);
         maxDecentX = min(maxDecentX,decent);
         pIText -> get_minY(&decent);
         maxDecentY = min(maxDecentY,decent);
      }

      if ( DBL_MAX == maxDecentX && DBL_MAX == maxDecentY ) {
         maxDecentX = 0.0;
         maxDecentY = 0.0;
      }

      pLabel -> put_SizeProperty(propertyAxisLabelSize,propertyAxisLabelSizeUnits);

      switch ( type ) {
      case 'X':
         dp.x = origin.x + (endPoint.x - origin.x)/2.0;
         dp.y = maxDecentY;
         if ( drawTickLabels ) 
            dp.y -= tickBelow;
         dp.z = origin.z;
         break;
      case 'Y':
         dp.x = maxDecentX;
         dp.y = origin.y + (endPoint.y - origin.y)/2.0;
         if ( drawTickLabels ) 
            dp.x -= tickAbove;
         dp.z = origin.z;
         break;
      case 'Z':
         dp.x = maxDecentX;
         dp.y = maxDecentY;
         dp.z = origin.z + (endPoint.z - origin.z)/2.0;
         break;
      }
      pLabel -> put_PositionX(dp.x);
      pLabel -> put_PositionY(dp.y);
      pLabel -> put_PositionZ(dp.z);
      pLabel -> PrepData();
   }

   return S_OK;
   }
