// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Axis.h"
#include "General.h"

#include "Properties_i.h"
#include "Text_i.h"


   HRESULT Axis::SavePrep() {

   propertyTextCount -> put_longValue(textList.Count());

   propertyRepresentativeText -> clearStorageObjects();

   IUnknown *pIUnknown;

   pRepresentativeText -> QueryInterface(IID_IUnknown,reinterpret_cast<void **>(&pIUnknown));

   propertyRepresentativeText -> addStorageObject(pIUnknown);

   propertyRepresentativeText -> writeStorageObjects();

   pIUnknown -> Release();


   propertyLabel -> clearStorageObjects();

   pLabel -> QueryInterface(IID_IUnknown,reinterpret_cast<void **>(&pIUnknown));

   propertyLabel -> addStorageObject(pIUnknown);

   propertyLabel -> writeStorageObjects();

   pIUnknown -> Release();

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
 
//drawTickLabels = FALSE;

   //propertyLabel -> put_szValue("");

   pLabel -> put_Text(L"");
   pLabel -> TextColorProperty(propertyLabelColor);

   propertyAxisLabelColorTrackLineColor -> put_boolValue(TRUE);

   propertyAxisLabelSize -> put_doubleValue(16.0);
   propertyAxisLabelSizeUnits -> put_longValue(TEXT_SIZE_POINTS);

   propertyLabelPositionNatural -> put_boolValue(TRUE);   
   propertyLabelAnglePerpendicular -> put_boolValue(FALSE);
   propertyLabelAngleParallel -> put_boolValue(TRUE);     
   propertyLabelAngleOther -> put_boolValue(FALSE);
   propertyLabelAngleOtherValue -> put_szValue("0.0");

   propertyTickLabelSize -> put_doubleValue(16.0);
   propertyTickLabelSizeUnits  -> put_longValue(TEXT_SIZE_POINTS);

   propertyTickLabelColorTrackLineColor -> put_boolValue(TRUE);

   propertyDefaultFont -> put_szValue("Arial");
 
   propertyOriginAtMinpoint -> put_boolValue(TRUE);
   propertyEndpointAtMaxpoint -> put_boolValue(TRUE);

   propertyDeterminesDomain -> put_boolValue(TRUE);

   propertyOriginXValue -> put_szValue("Ox");
   propertyOriginYValue -> put_szValue("Oy");
   propertyOriginZValue -> put_szValue("Oz");

   pRepresentativeText -> TextColorProperty(propertyTickLabelColor);

   pLabel -> TextColorProperty(propertyLabelColor);

   switch ( type ) {
   case 'x':
   case 'X': {
      propertyEndpointXValue -> put_szValue("Mx");
      propertyEndpointYValue -> put_szValue("Oy");
      propertyEndpointZValue -> put_szValue("Oz");
      propertyLabelPositionXValue -> put_szValue("Ox + (Mx-Ox)/2");    
      propertyLabelPositionYValue -> put_szValue("Oy");    
      propertyLabelPositionZValue -> put_szValue("0z");
      propertyTickStraddleStyle -> put_longValue(TICK_STRADDLE_BELOW);
      propertyTicksAllPlanes -> put_boolValue(FALSE);
      pRepresentativeText -> put_CoordinatePlane(CoordinatePlane_XY);
      pRepresentativeText -> put_Format(TEXT_FORMAT_CENTER | TEXT_COORDINATES_FROM_TOP);

      pLabel -> put_CoordinatePlane(CoordinatePlane_XY);
      pLabel -> put_Format(TEXT_FORMAT_CENTER | TEXT_COORDINATES_FROM_TOP);

      }
      break;
 
   case 'y':
   case 'Y': {
      propertyLabelPositionXValue -> put_szValue("Ox");    
      propertyLabelPositionYValue -> put_szValue("Oy + (My-Oy)/2");    
      propertyLabelPositionZValue -> put_szValue("0z");    
      propertyEndpointXValue -> put_szValue("Ox");
      propertyEndpointYValue -> put_szValue("My");
      propertyEndpointZValue -> put_szValue("Oz");
      propertyTickStraddleStyle -> put_longValue(TICK_STRADDLE_BELOW);
      propertyTicksAllPlanes -> put_boolValue(FALSE);
      pRepresentativeText -> put_CoordinatePlane(CoordinatePlane_YX);
      pRepresentativeText -> put_FlipHorizontal(VARIANT_TRUE);
      pRepresentativeText -> put_Format(TEXT_FORMAT_CENTER | TEXT_COORDINATES_FROM_TOP);

      pLabel -> put_CoordinatePlane(CoordinatePlane_YX);
      pLabel -> put_Format(TEXT_FORMAT_CENTER | TEXT_COORDINATES_FROM_BOTTOM);

      }
      break;
 
   case 'z':
   case 'Z': {
      propertyLabelPositionXValue -> put_szValue("Ox");    
      propertyLabelPositionYValue -> put_szValue("Oy");    
      propertyLabelPositionZValue -> put_szValue("0z + (Mz-Oz)/2");    
      propertyEndpointXValue -> put_szValue("Ox");
      propertyEndpointYValue -> put_szValue("Oy");
      propertyEndpointZValue -> put_szValue("Mz");
      propertyTickStraddleStyle -> put_longValue(TICK_STRADDLE_BOTH);
      propertyTicksAllPlanes -> put_boolValue(TRUE);
      propertyTickLength -> put_doubleValue(DEFAULT_ZAXIS_TICK_PERCENT);
      pRepresentativeText -> put_CoordinatePlane(CoordinatePlane_XZ);
      pRepresentativeText -> put_FlipVertical(VARIANT_TRUE);
      pRepresentativeText -> put_Format(TEXT_FORMAT_RIGHT | TEXT_COORDINATES_FROM_CENTER);

      pLabel -> put_CoordinatePlane(CoordinatePlane_XZ);
      pLabel -> put_Format(TEXT_FORMAT_RIGHT | TEXT_COORDINATES_FROM_CENTER);

      }
      break;
 
   default:
      pRepresentativeText -> put_CoordinatePlane(CoordinatePlane_XY);
      break;
 
   }

#if 1
   float v[] = {CLR_BLACK};
   propertyLineColor -> put_binaryValue(sizeof(v),(BYTE*)v);
   propertyTickLabelColor -> put_binaryValue(sizeof(v),(BYTE*)v);
   propertyLabelColor -> put_binaryValue(sizeof(v),(BYTE*)v);
#endif 

   propertyTextCount -> put_longValue(0);
 
   propertyPlotType -> put_longValue(gcPlotType2DExternal1);

   if ( axisTickColorTrackLineColor )
      currentPropertyTickLabelColor = propertyLineColor;
   else 
      currentPropertyTickLabelColor = propertyTickLabelColor;
   
   return S_OK;//Loaded();
   }
 
 
   HRESULT Axis::Loaded() {

   long textCount = 0;
   char szDefaultFont[32];
 
   propertyDefaultFont -> get_szValue(szDefaultFont);

   if ( szDefaultFont[0] == '\0' )
      propertyDefaultFont -> put_szValue("Arial");
 
   propertyTextCount -> get_longValue(&textCount);

   propertyLabel -> clearStorageObjects();

   IUnknown *pIUnknown;

   pLabel -> QueryInterface(IID_IUnknown,reinterpret_cast<void **>(&pIUnknown));

   propertyLabel -> addStorageObject(pIUnknown);

   propertyLabel -> readStorageObjects();

   pIUnknown -> Release();


   propertyRepresentativeText -> clearStorageObjects();

   pRepresentativeText -> QueryInterface(IID_IUnknown,reinterpret_cast<void **>(&pIUnknown));

   propertyRepresentativeText -> addStorageObject(pIUnknown);

   propertyRepresentativeText -> readStorageObjects();

   pIUnknown -> Release();


   pRepresentativeText -> put_Description(L"Values to appear along the axis, separated by commas.\nTo include a comma, specify \\, (backslash - comma).");

   pRepresentativeText -> put_EnablePositionSettings(FALSE);

   pRepresentativeText -> put_ShowPositionSettings(FALSE);

   pRepresentativeText -> put_PartOfWorldDomain(FALSE);

   pRepresentativeText -> put_PartOfMainGraphic(FALSE);

   if ( axisTickColorTrackLineColor )
      currentPropertyTickLabelColor = propertyLineColor;
   else 
      currentPropertyTickLabelColor = propertyTickLabelColor;

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
