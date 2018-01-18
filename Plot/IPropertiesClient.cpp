
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

   if ( propertyPlotTypesGlobal ) {

      propertyPlotTypesGlobal -> clearStorageObjects(); 

      for ( std::pair<gc2DPlotTypes,IGSystemPlotType *> pair : Plot::plotType2DProviderInstances ) {
         IUnknown *pIUnknown = NULL;
         pair.second -> QueryInterface(IID_IUnknown,reinterpret_cast<void **>(&pIUnknown));
         propertyPlotTypesGlobal -> addStorageObject(pIUnknown);
         pIUnknown -> Release();
      }

      for ( std::pair<gc3DPlotTypes,IGSystemPlotType *> pair : Plot::plotType3DProviderInstances ) {
         IUnknown *pIUnknown = NULL;
         pair.second -> QueryInterface(IID_IUnknown,reinterpret_cast<void **>(&pIUnknown));
         propertyPlotTypesGlobal -> addStorageObject(pIUnknown);
         pIUnknown -> Release();
      }

      propertyPlotTypesGlobal -> writeStorageObjects();
      propertyPlotTypesGlobal -> clearStorageObjects();

   }

   IStream *pIStream = NULL;

   CreateStreamOnHGlobal(NULL,FALSE,&pIStream);

   SAFEARRAYBOUND rgsaBound[1];
   memset(rgsaBound,0,sizeof(SAFEARRAYBOUND));
   rgsaBound[0].cElements = max(3,3 * plotType2DInstanceGUIDS.size());
   rgsaBound[0].lLbound = 0;

   SAFEARRAY *pArray = SafeArrayCreate(VT_BSTR,1,rgsaBound);

   BSTR *pValues = NULL;

   SafeArrayAccessData(pArray,(void **)&pValues);

   long k = 0;
   for ( std::pair<long,GUID> pair : plotType2DInstanceGUIDS ) {

      WCHAR szwType[32];
      swprintf(szwType,L"%ld",pair.first);
      pValues[k] = SysAllocString(szwType);
      k++;

      long instanceItem = plotType2DInstanceNumber[(gc2DPlotTypes)pair.first];
      swprintf(szwType,L"%ld",instanceItem);
      pValues[k] = SysAllocString(szwType);
      k++;

      BSTR bstr;
      StringFromCLSID(pair.second,&pValues[k]);
      k++;

      plotType2DProviderInstances[(gc2DPlotTypes)pair.first] -> Save(instanceItem,pair.second,(void *)pIStream);

   }

   if ( 0 == k ) {
      pValues[0] = SysAllocString(L"0");
      pValues[1] = SysAllocString(L"0");
      StringFromCLSID(GUID_NULL,(LPOLESTR *)&pValues[2]);
   }

   SafeArrayUnaccessData(pArray);

   propertyPlotTypes2DInstanceIdentifiers -> put_arrayValue(pArray);

   //
   //NTC: 01-14-2018: I am getting a trap on the following call to SafeArrayDestroy.
   // This MAY indicate accessing the underlying data in some invalid way, but I cannot find it, and the problem
   // may present itself elsewhere
   //
   //SafeArrayDestroy(pArray);

   memset(rgsaBound,0,sizeof(SAFEARRAYBOUND));
   rgsaBound[0].cElements = max(3,3 * plotType3DInstanceGUIDS.size());
   rgsaBound[0].lLbound = 0;

   pArray = SafeArrayCreate(VT_BSTR,1,rgsaBound);

   SafeArrayAccessData(pArray,(void **)&pValues);

   k = 0;
   for ( std::pair<long,GUID> pair : plotType3DInstanceGUIDS ) {

      WCHAR szwType[32];
      swprintf(szwType,L"%ld",pair.first);
      pValues[k] = SysAllocString(szwType);
      k++;

      long instanceItem = plotType3DInstanceNumber[(gc3DPlotTypes)pair.first];
      swprintf(szwType,L"%ld",instanceItem);
      pValues[k] = SysAllocString(szwType);
      k++;

      BSTR bstr;
      StringFromCLSID(pair.second,&pValues[k]);
      k++;

      plotType3DProviderInstances[(gc3DPlotTypes)pair.first] -> Save(instanceItem,pair.second,(void *)pIStream);

   }

   if ( 0 == k ) {
      pValues[0] = SysAllocString(L"0");
      pValues[1] = SysAllocString(L"0");
      StringFromCLSID(GUID_NULL,&pValues[2]);
   }

   SafeArrayUnaccessData(pArray);

   propertyPlotTypes3DInstanceIdentifiers -> put_arrayValue(pArray);

   //SafeArrayDestroy(pArray);

   HANDLE hPlotTypeData;

   GetHGlobalFromStream(pIStream,&hPlotTypeData);

   void *pPlotTypeData = GlobalLock(hPlotTypeData);

   propertyPlotTypesStorage -> put_binaryValue(GlobalSize(hPlotTypeData),(BYTE *)pPlotTypeData);

   pIStream -> Release();

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

   if ( propertyPlotTypesGlobal ) {

      long cntObjects;

      propertyPlotTypesGlobal -> get_storedObjectCount(&cntObjects);

      if ( cntObjects ) {

         propertyPlotTypesGlobal -> clearStorageObjects();

         for ( std::pair<gc2DPlotTypes,IGSystemPlotType *> pair : Plot::plotType2DProviderInstances ) {
            IUnknown *pIUnknown = NULL;
            pair.second -> QueryInterface(IID_IUnknown,reinterpret_cast<void **>(&pIUnknown));
            propertyPlotTypesGlobal -> addStorageObject(pIUnknown);
            pIUnknown -> Release();
         }

         for ( std::pair<gc3DPlotTypes,IGSystemPlotType *> pair : Plot::plotType3DProviderInstances ) {
            IUnknown *pIUnknown = NULL;
            pair.second -> QueryInterface(IID_IUnknown,reinterpret_cast<void **>(&pIUnknown));
            propertyPlotTypesGlobal -> addStorageObject(pIUnknown);
            pIUnknown -> Release();
         }

         propertyPlotTypesGlobal -> readStorageObjects();
         propertyPlotTypesGlobal -> clearStorageObjects();

      }

   }

   BYTE *pInstanceStorage = NULL;

   propertyPlotTypesStorage -> get_binaryData(&pInstanceStorage);

   if ( ! ( NULL == pInstanceStorage ) ) {

      long size = 0L;

      propertyPlotTypesStorage -> get_size(&size);

      HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE,size);

      VOID *pGlobal = GlobalLock(hGlobal);

      memcpy(pGlobal,pInstanceStorage,size);

      GlobalUnlock(hGlobal);

      IStream *pIStream = NULL;

      CreateStreamOnHGlobal(hGlobal,TRUE,&pIStream);

      SAFEARRAY *pArray = NULL;

      propertyPlotTypes2DInstanceIdentifiers -> get_arrayValue(&pArray);

      if ( pArray ) {

         BSTR *pValues;

         long lBound;
         long countElements;

         SafeArrayGetLBound(pArray,1,&lBound);

         SafeArrayGetUBound(pArray,1,&countElements);

         countElements = countElements - lBound + 1;
   
         SafeArrayAccessData(pArray,(void **)&pValues);

         for ( long k = 0; k < countElements; k += 3 ) {

            long index = _wtol(pValues[k]);

            if ( 0 == index )
               continue;

            long instanceNumber = _wtol(pValues[k + 1]);

            GUID theGUID;
            CLSIDFromString(pValues[k + 2],&theGUID);
            plotType2DInstanceGUIDS[index] = theGUID;

            plotType2DProviderInstances[(gc2DPlotTypes)index] -> InitNew(instanceNumber,theGUID);

            plotType2DProviderInstances[(gc2DPlotTypes)index] -> Load(instanceNumber,theGUID,(void *)pIStream);

         }

         SafeArrayUnaccessData(pArray);

         SafeArrayDestroy(pArray);

      }

      pArray = NULL;

      propertyPlotTypes3DInstanceIdentifiers -> get_arrayValue(&pArray);

      if ( pArray ) {

         BSTR *pValues;

         long lBound;
         long countElements;

         SafeArrayGetLBound(pArray,1,&lBound);
         SafeArrayGetUBound(pArray,1,&countElements);

         countElements = countElements - lBound + 1;

         SafeArrayAccessData(pArray,(void **)&pValues);

         for ( long k = 0; k < countElements; k += 3 ) {

            long index = _wtol(pValues[k]);

            if ( 0 == index )
               continue;

            long instanceNumber = _wtol(pValues[k + 1]);

            GUID theGUID;
            CLSIDFromString(pValues[k + 2],&theGUID);
            plotType3DInstanceGUIDS[index] = theGUID;

            plotType3DProviderInstances[(gc3DPlotTypes)index] -> InitNew(instanceNumber,theGUID);

            plotType2DProviderInstances[(gc2DPlotTypes)index] -> Load(instanceNumber,theGUID,(void *)pIStream);

         }

         SafeArrayUnaccessData(pArray);
   
         SafeArrayDestroy(pArray);

      }

      pIStream -> Release();

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