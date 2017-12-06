/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002,2008 Nathan T. Clark

*/

#include <windows.h>
#include <stdio.h>

#include "general.h"

#include "Plot.h"


   HRESULT Plot::SavePrep() {

   DataPoint dp[2];
   pIDataSet -> GetDomain(&dp[0],&dp[1]);

   propertyDataExtents -> put_binaryValue(sizeof(dp),reinterpret_cast<byte *>(dp));

   return S_OK;
   }


   HRESULT Plot::InitNew() {

   pID = ++pCount;

   float fvBlack[] = {CLR_BLACK};
   float fvBlue[] = {CLR_BLUE};
   float fvGreen[] = {CLR_GREEN};
   float fvRed[] = {CLR_RED};
   double dm[9];
   long lv[4];
   DataPoint dp[2];

   dp[0].x = 0.0;
   dp[0].y = 0.0;
   dp[0].z = 0.0;
   dp[1].x = 0.0;
   dp[1].y = 0.0;
   dp[1].z = 0.0;
   propertyDataExtents -> put_binaryValue(sizeof(dp),reinterpret_cast<byte *>(dp));
 
   propertyLineColor -> put_binaryValue(sizeof(fvBlack),(BYTE*)fvBlack);
 
   switch ( pID % 3 ) {
   case 0:
      propertyTopSurfaceColor -> put_binaryValue(sizeof(fvBlue),(BYTE*)fvBlue);
      propertyBottomSurfaceColor -> put_binaryValue(sizeof(fvGreen),(BYTE*)fvGreen);
      break;
   case 1:
      propertyTopSurfaceColor -> put_binaryValue(sizeof(fvRed),(BYTE*)fvRed);
      propertyBottomSurfaceColor -> put_binaryValue(sizeof(fvBlue),(BYTE*)fvBlue);
      break;
   case 2:
      propertyTopSurfaceColor -> put_binaryValue(sizeof(fvGreen),(BYTE*)fvGreen);
      propertyBottomSurfaceColor -> put_binaryValue(sizeof(fvBlue),(BYTE*)fvBlue);
      break;
   default:
      propertyTopSurfaceColor -> put_binaryValue(sizeof(fvBlue),(BYTE*)fvBlue);
      propertyBottomSurfaceColor -> put_binaryValue(sizeof(fvGreen),(BYTE*)fvGreen);
      break;
   }
 
   memset(dm,0,9 * sizeof(double));
   memset(lv,0,4 * sizeof(long));
 
   propertyLineEnd -> put_longValue(1L);
   propertyLineJoin -> put_longValue(1L);
   propertyPlotView -> put_longValue((LONG)gcPlotView2D);
   propertyPlotType -> put_longValue((LONG)gcPlotTypeNone);

   propertyCustomColors -> put_binaryValue(9 * sizeof(double),(BYTE*)dm);
   propertyPropertiesPosition -> put_binaryValue(4 * sizeof(long),(BYTE*)lv);

   overrideOwnerView = false;
   overrideOwnerType = false;
 
   propertyName -> put_stringValue(L"");

   propertyDefaultSolidSizeInPercentDomain -> put_doubleValue(2.5);

   return Loaded();
   }
 
 
   HRESULT Plot::Loaded() {
   long v;
   DataPoint dp[2];
   BYTE *pdp = (BYTE *)&dp[0];

   propertyDataExtents -> get_binaryValue(sizeof(dp),reinterpret_cast<byte **>(&pdp));

   pIDataSet -> SetDomain(&dp[0],&dp[1]);

   haveAnyData = TRUE;

   propertyPlotView -> get_longValue(&v);
   put_PlotView(static_cast<PlotViews>(v));

   propertyPlotType -> get_longValue(&v);
   put_PlotType(static_cast<PlotTypes>(v));
 
   put_ColorProperty(propertyLineColor);

   return S_OK;
   }
 
 
   HRESULT Plot::Saved() {
   return S_OK;
   }


   HRESULT Plot::IsDirty() {
   return S_FALSE;
   }


   HRESULT Plot::GetClassID(BYTE *pCLSID) {
   *(CLSID*)pCLSID = CLSID_Plot;
   return S_OK;
   }