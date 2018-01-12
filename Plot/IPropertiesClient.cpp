
#include "Plot.h"

   HRESULT Plot::SavePrep() {

   if ( pIDataSet ) {
      DataPoint dp[2];
      pIDataSet -> GetDomain(&dp[0],&dp[1]);
      propertyDataExtents -> put_binaryValue(sizeof(dp),reinterpret_cast<byte *>(dp));
   }

   if ( propertyDataSet ) {

      propertyDataSet -> clearStorageObjects();
      propertyDataSet -> addStorageObject(pIDataSet);
      propertyDataSet -> writeStorageObjects();
      propertyDataSet -> clearStorageObjects();

   }

   if ( propertyPlotTypes ) {

      propertyPlotTypes -> clearStorageObjects();

      for ( std::pair<gc2DPlotTypes,IGSystemPlotType *> pair : Plot::plotType2DProviderInstances ) {
         IUnknown *pIUnknown = NULL;
         pair.second -> QueryInterface(IID_IUnknown,reinterpret_cast<void **>(&pIUnknown));
         propertyPlotTypes -> addStorageObject(pIUnknown);
         pIUnknown -> Release();
      }

      for ( std::pair<gc3DPlotTypes,IGSystemPlotType *> pair : Plot::plotType3DProviderInstances ) {
         IUnknown *pIUnknown = NULL;
         pair.second -> QueryInterface(IID_IUnknown,reinterpret_cast<void **>(&pIUnknown));
         propertyPlotTypes -> addStorageObject(pIUnknown);
         pIUnknown -> Release();
      }

      propertyPlotTypes -> writeStorageObjects();
      propertyPlotTypes -> clearStorageObjects();

   }

   return S_OK;
   }


   HRESULT Plot::InitNew() {

   pID = instanceCount;

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
   propertyDataExtents -> put_binaryValue(sizeof(dp),(BYTE *)dp);
 
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
   property2DPlotType -> put_longValue((LONG)gcPlotTypeNone);
   property3DPlotType -> put_longValue((LONG)gcPlotType3DNone);
   propertyLineWeight -> put_longValue((LONG)DEFAULT_LINE_WEIGHT);
   propertyCustomColors -> put_binaryValue(9 * sizeof(double),(BYTE*)dm);
   propertyPropertiesPosition -> put_binaryValue(4 * sizeof(long),(BYTE*)lv);

   overrideOwnerView = false;
   overrideOwnerType = false;
 
   propertyName -> put_stringValue(L"");

   propertyDefaultSolidSizeInPercentDomain -> put_doubleValue(2.5);

   return Loaded();
   }
 
 
   HRESULT Plot::Loaded() {

   if ( pIDataSet ) {
      DataPoint dp[2];
      BYTE *pdp = (BYTE *)&dp[0];

      propertyDataExtents -> get_binaryValue(sizeof(dp),reinterpret_cast<byte **>(&pdp));

      pIDataSet -> SetDomain(&dp[0],&dp[1]);
   }

   haveAnyData = TRUE;

   long v;

   propertyPlotView -> get_longValue(&v);
   put_PlotView(static_cast<PlotViews>(v));

   property2DPlotType -> get_longValue(&v);
   put_PlotType2D(static_cast<gc2DPlotTypes>(v));
 
   property3DPlotType -> get_longValue(&v);
   put_PlotType3D(static_cast<gc3DPlotTypes>(v));
 
   put_ColorProperty(propertyLineColor);

   if ( propertyDataSet ) {

      long cntObjects;

      propertyDataSet -> get_storedObjectCount(&cntObjects);

      if ( cntObjects ) {
         propertyDataSet -> clearStorageObjects();
         propertyDataSet -> addStorageObject(pIDataSet);
         propertyDataSet-> readStorageObjects();
         propertyDataSet -> clearStorageObjects();
      }

   }

   if ( propertyPlotTypes ) {

      long cntObjects;

      propertyPlotTypes -> get_storedObjectCount(&cntObjects);

      if ( cntObjects ) {

         propertyPlotTypes -> clearStorageObjects();

         for ( std::pair<gc2DPlotTypes,IGSystemPlotType *> pair : Plot::plotType2DProviderInstances ) {
            IUnknown *pIUnknown = NULL;
            pair.second -> QueryInterface(IID_IUnknown,reinterpret_cast<void **>(&pIUnknown));
            propertyPlotTypes -> addStorageObject(pIUnknown);
            pIUnknown -> Release();
         }

         for ( std::pair<gc3DPlotTypes,IGSystemPlotType *> pair : Plot::plotType3DProviderInstances ) {
            IUnknown *pIUnknown = NULL;
            pair.second -> QueryInterface(IID_IUnknown,reinterpret_cast<void **>(&pIUnknown));
            propertyPlotTypes -> addStorageObject(pIUnknown);
            pIUnknown -> Release();
         }

         propertyPlotTypes -> readStorageObjects();
         propertyPlotTypes -> clearStorageObjects();

      }

   }

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