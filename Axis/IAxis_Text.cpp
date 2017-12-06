/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002,2008 Nathan T. Clark

*/

#include <windows.h>
#include <math.h>
#include <list>

#include "axis.h"



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
   propertyLabel -> put_stringValue(bstrLabel);
   if ( wcslen(bstrLabel) )
      drawAxisLabel = true;
   else
      drawAxisLabel = false;
   return S_OK;
   }


   STDMETHODIMP Axis::get_LabelText(BSTR *pbstrLabel) {
   if ( ! pbstrLabel ) return E_POINTER;
   propertyLabel -> get_stringValue(pbstrLabel);
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
      for ( std::list<IText *>::iterator it = theTickLabels.begin(); it != theTickLabels.end(); it++ ) {
         if ( (*it) == pIText )
            continue;
         RECT rcInner;
         (*it) -> get_GDIBoundingBox(&rcInner);
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
   long k = 0;

   if ( overlap ) while ( pIText = textList.GetNext(pIText) ) {
      k++;
      if ( k % 2 ) {
         pIText -> LineUp();
      }
   }

   if ( drawAxisLabel ) {
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
      BSTR bstrLabel;
      propertyLabel -> get_stringValue(&bstrLabel);
      pLabel -> put_Text(bstrLabel);
      SysFreeString(bstrLabel);
      pLabel -> TextColorProperty(propertyLabelColor);
      pLabel -> put_CoordinatePlane(CoordinatePlane_XY);
      switch ( type ) {
      case 'X':
         dp.x = origin.x + (endPoint.x - origin.x)/2.0;
         dp.y = maxDecentY;
         if ( drawTickLabels ) 
            dp.y -= tickBelow;
         pLabel -> put_Format(TEXT_FORMAT_CENTER | TEXT_COORDINATES_FROM_TOP);
         break;
      case 'Y':
         dp.x = maxDecentX;
         dp.y = origin.y + (endPoint.y - origin.y)/2.0;
         if ( drawTickLabels ) 
            dp.x -= tickAbove;
         pLabel -> put_Format(TEXT_FORMAT_CENTER | TEXT_COORDINATES_FROM_BOTTOM);
         pLabel -> put_CoordinatePlane(CoordinatePlane_YX);
         break;
      }
      dp.z = origin.z;
      pLabel -> put_PositionX(dp.x);
      pLabel -> put_PositionY(dp.y);
      pLabel -> put_PositionZ(dp.z);
      pLabel -> PrepData();
   }

   return S_OK;
   }
