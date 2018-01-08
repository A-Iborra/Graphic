
#include "Plot.h"

   HRESULT Plot::GetPlotTypesInformation(SAFEARRAY **pp2DTypeIDs,SAFEARRAY **pp3DTypeIDs,SAFEARRAY **pp2DTypeNames,SAFEARRAY **pp3DTypeNames) {

   if ( ! pp2DTypeIDs || ! pp3DTypeIDs || ! pp2DTypeNames || ! pp3DTypeNames )
      return E_POINTER;

   SAFEARRAYBOUND rgb;

   rgb.lLbound = 1;
   rgb.cElements = plotType2DProviderNames.size();

   SAFEARRAY *pSA2DIds = SafeArrayCreate(VT_I4,1,&rgb);
   SAFEARRAY *pSA2DNames = SafeArrayCreate(VT_BSTR,1,&rgb);

   *pp2DTypeIDs = pSA2DIds;
   *pp2DTypeNames = pSA2DNames;

   long *pvIDs = NULL;
   BSTR *pvNames = NULL;

   SafeArrayAccessData(pSA2DIds,reinterpret_cast<void **>(&pvIDs));
   SafeArrayAccessData(pSA2DNames,reinterpret_cast<void **>(&pvNames));

   //memset(pvIDs,0,rgb.cElements * sizeof(long));
   //memset(pvNames,0,rgb.cElements * sizeof(BSTR));

   long k = 0;

   for ( std::pair<gc2DPlotTypes,char *> pPair : plotType2DProviderNames ) {

      pvIDs[k] = (long)pPair.first;

      long n = strlen(pPair.second) + 1;

      pvNames[k] = SysAllocStringLen(NULL,n);

      MultiByteToWideChar(CP_ACP,0,pPair.second,-1,pvNames[k],n);

      k++;
   }

   SafeArrayUnaccessData(pSA2DIds);
   SafeArrayUnaccessData(pSA2DNames);

   rgb.lLbound = 1;
   rgb.cElements = plotType3DProviderNames.size();

   SAFEARRAY *pSA3DIds = SafeArrayCreate(VT_I4,1,&rgb);
   SAFEARRAY *pSA3DNames = SafeArrayCreate(VT_BSTR,1,&rgb);

   *pp3DTypeIDs = pSA3DIds;
   *pp3DTypeNames = pSA3DNames;

   SafeArrayAccessData(pSA3DIds,reinterpret_cast<void **>(&pvIDs));
   SafeArrayAccessData(pSA3DNames,reinterpret_cast<void **>(&pvNames));

   //memset(pvIDs,0,rgb.cElements * sizeof(VARIANT));
   //memset(pvNames,0,rgb.cElements * sizeof(VARIANT));

   k = 0;

   for ( std::pair<gc3DPlotTypes,char *> pPair : plotType3DProviderNames ) {

      pvIDs[k] = (long)pPair.first;

      long n = strlen(pPair.second) + 1;

      pvNames[k] = SysAllocStringLen(NULL,n);

      MultiByteToWideChar(CP_ACP,0,pPair.second,-1,pvNames[k],n);

      k++;
   }

   SafeArrayUnaccessData(pSA3DIds);
   SafeArrayUnaccessData(pSA3DNames);

   return S_OK;
   }
