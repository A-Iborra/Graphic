// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Axis.h"
#include "General.h"

#include "Properties_i.h"
#include "Text_i.h"


   HRESULT Axis::SavePrep() {
   propertyTextCount -> put_longValue(textList.Count());
   return S_OK;
   }


   HRESULT Axis::InitNew() {
 
   propertyLineWeight -> put_longValue(DEFAULT_LINE_WEIGHT);
 
   propertyGridLinesPerTick -> put_longValue(0L);

   tickCount = DEFAULT_TICK_COUNT;
 
   propertyTickStraddleStyle -> put_longValue(DEFAULT_TICK_STRADDLE_STYLE);
   tickPctAbove = DEFAULT_TICK_STRADDLE_ABOVE;
 
   propertyTickLength -> put_doubleValue(DEFAULT_TICK_PERCENT);
   propertyTickLengthUnits -> put_longValue(UNIT_PERCENT);
 
   propertyAxisPrecision -> put_doubleValue(1.0);
   drawTickLabels = TRUE;
   doAutoPrecision = TRUE;
 
   propertyLabel -> put_szValue("");
   propertyDrawLabel -> put_boolValue(FALSE);
   propertyAxisLabelColorTrackLineColor -> put_boolValue(TRUE);

   propertyAxisLabelSize -> put_doubleValue(10.0);
   propertyAxisLabelSizeUnits -> put_longValue(TEXT_SIZE_POINTS);

   propertyLabelPositionNatural -> put_boolValue(TRUE);   
   propertyLabelAnglePerpendicular -> put_boolValue(FALSE);
   propertyLabelAngleParallel -> put_boolValue(TRUE);     
   propertyLabelAngleOther -> put_boolValue(FALSE);
   propertyLabelAngleOtherValue -> put_szValue("0.0");

   propertyTickLabelSize -> put_doubleValue(10.0);
   propertyTickLabelSizeUnits  -> put_longValue(TEXT_SIZE_POINTS);

   propertyTickLabelColorTrackLineColor -> put_boolValue(TRUE);

   propertyDefaultFont -> put_szValue("Arial");
 
   propertyOriginAtMinpoint -> put_boolValue(TRUE);
   propertyEndpointAtMaxpoint -> put_boolValue(TRUE);

   propertyDeterminesDomain -> put_boolValue(TRUE);

   propertyOriginXValue -> put_szValue("Ox");
   propertyOriginYValue -> put_szValue("Oy");
   propertyOriginZValue -> put_szValue("Oz");

   switch ( type ) {
   case 'x':
   case 'X': {
      float v[] = {CLR_RED};
      propertyLineColor -> put_binaryValue(sizeof(v),(BYTE*)v);
      propertyTickLabelColor -> put_binaryValue(sizeof(v),(BYTE*)v);
      propertyLabelColor -> put_binaryValue(sizeof(v),(BYTE*)v);
      propertyEndpointXValue -> put_szValue("Mx");
      propertyEndpointYValue -> put_szValue("Oy");
      propertyEndpointZValue -> put_szValue("Oz");
      propertyLabelPositionXValue -> put_szValue("Ox + (Mx-Ox)/2");    
      propertyLabelPositionYValue -> put_szValue("Oy");    
      propertyLabelPositionZValue -> put_szValue("0z");    
      }
      break;
 
   case 'y':
   case 'Y': {
      float v[] = {CLR_GREEN};
      propertyLineColor -> put_binaryValue(sizeof(v),(BYTE*)v);
      propertyTickLabelColor -> put_binaryValue(sizeof(v),(BYTE*)v);
      propertyLabelColor -> put_binaryValue(sizeof(v),(BYTE*)v);
      propertyLabelPositionXValue -> put_szValue("Ox");    
      propertyLabelPositionYValue -> put_szValue("Oy + (My-Oy)/2");    
      propertyLabelPositionZValue -> put_szValue("0z");    
      propertyEndpointXValue -> put_szValue("Ox");
      propertyEndpointYValue -> put_szValue("My");
      propertyEndpointZValue -> put_szValue("Oz");
      }
      break;
 
   case 'z':
   case 'Z': {
      float v[] = {CLR_BLUE};
      propertyLineColor -> put_binaryValue(sizeof(v),(BYTE*)v);
      propertyTickLabelColor -> put_binaryValue(sizeof(v),(BYTE*)v);
      propertyLabelColor -> put_binaryValue(sizeof(v),(BYTE*)v);
      propertyLabelPositionXValue -> put_szValue("Ox");    
      propertyLabelPositionYValue -> put_szValue("Oy");    
      propertyLabelPositionZValue -> put_szValue("0z + (Mz-Oz)/2");    
      propertyEndpointXValue -> put_szValue("Ox");
      propertyEndpointYValue -> put_szValue("Oy");
      propertyEndpointZValue -> put_szValue("Mz");
      }
      break;
 
   default:
      break;
 
   }

#if 1
   float v[] = {CLR_BLACK};
   propertyLineColor -> put_binaryValue(sizeof(v),(BYTE*)v);
   propertyTickLabelColor -> put_binaryValue(sizeof(v),(BYTE*)v);
   propertyLabelColor -> put_binaryValue(sizeof(v),(BYTE*)v);
#endif 
//   memset(propertyCustomColors -> ValueDoubleMatrix(),0,9 * sizeof(double));

   propertyTextCount -> put_longValue(0);
 
   propertyPlotType -> put_longValue(gcPlotType2DExternal1);
   
   return Loaded();
   }
 
 
   HRESULT Axis::Loaded() {
   long textCount = 0;
   char szDefaultFont[32];
 
   propertyDefaultFont -> get_szValue(szDefaultFont);

   if ( szDefaultFont[0] == '\0' )
      propertyDefaultFont -> put_szValue("Arial");
 
   propertyTextCount -> get_longValue(&textCount);
#if 0
   IText *t;
   for ( long tickNumber = 0; tickNumber < textCount; tickNumber++ ) {
 
      CoCreateInstance(CLSID_Text,
                       NULL,
                       CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,
                       IID_IText,
                       reinterpret_cast<void **>(&t));

      t -> Initialize(pIOpenGLImplementation,pIEvaluator,pIDataSetDomain,pParentPropertyFloor,pParentPropertyCeiling,NULL,NULL);

      textList.Add(t,NULL,tickNumber + 1);
 
   }
#endif 
   long n;

   propertyLabel -> get_size(&n);

   propertyDrawLabel -> put_boolValue(1 < n ? TRUE : FALSE);

   return S_OK;
   }
 
 
   HRESULT Axis::Saved() {
   return S_OK;
   }


   HRESULT Axis::IsDirty() {
   return S_FALSE;
   }


    HRESULT Axis::GetClassID(BYTE *pCLSID) {
   *(CLSID*)pCLSID = CLSID_GSystemAxis;
   return S_OK;
   }
