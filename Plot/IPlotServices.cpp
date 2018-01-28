// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Plot.h"

   HRESULT Plot::GetPlotTypesInformation(SAFEARRAY **pp2DTypeIDs,SAFEARRAY **pp3DTypeIDs,SAFEARRAY **pp2DTypeNames,SAFEARRAY **pp3DTypeNames,
                                             SAFEARRAY **pp2DTypeInstanceNumbers,SAFEARRAY **pp3DTypeInstanceNumbers,
                                             SAFEARRAY **pp2DTypeInstances,SAFEARRAY **pp3DTypeInstances) {

   if ( ! pp2DTypeIDs || ! pp3DTypeIDs || ! pp2DTypeNames || ! pp3DTypeNames )
      return E_POINTER;

   SAFEARRAYBOUND rgb;

   rgb.lLbound = 1;
   rgb.cElements = (DWORD)plotType2DProviderNames.size();

   SAFEARRAY *pSA2DIds = SafeArrayCreate(VT_I4,1,&rgb);
   SAFEARRAY *pSA2DNames = SafeArrayCreate(VT_BSTR,1,&rgb);
   SAFEARRAY *pSA2DInstanceNumbers = SafeArrayCreate(VT_I4,1,&rgb);

   *pp2DTypeIDs = pSA2DIds;
   *pp2DTypeNames = pSA2DNames;
   *pp2DTypeInstanceNumbers = pSA2DInstanceNumbers;

   long *pvIDs = NULL;
   BSTR *pvNames = NULL;
   long *pvInstanceNumbers = NULL;

   SafeArrayAccessData(pSA2DIds,reinterpret_cast<void **>(&pvIDs));
   SafeArrayAccessData(pSA2DNames,reinterpret_cast<void **>(&pvNames));
   SafeArrayAccessData(pSA2DInstanceNumbers,reinterpret_cast<void **>(&pvInstanceNumbers));

   //memset(pvIDs,0,rgb.cElements * sizeof(long));
   //memset(pvNames,0,rgb.cElements * sizeof(BSTR));

   long k = 0;

   for ( std::pair<gc2DPlotTypes,char *> pPair : plotType2DProviderNames ) {

      pvIDs[k] = (long)pPair.first;

      long n = (DWORD)strlen(pPair.second) + 1;

      pvNames[k] = SysAllocStringLen(NULL,n);

      MultiByteToWideChar(CP_ACP,0,pPair.second,-1,pvNames[k],n);

      k++;
   }

   k = 0;

   for ( std::pair<gc2DPlotTypes,long> pPair : plotType2DInstanceNumber ) {
      pvInstanceNumbers[k] = pPair.second;
      k++;
   }

   SafeArrayUnaccessData(pSA2DIds);
   SafeArrayUnaccessData(pSA2DNames);
   SafeArrayUnaccessData(pSA2DInstanceNumbers);

   SAFEARRAY *pSA2DInstances = SafeArrayCreate(VT_UNKNOWN,1,&rgb);

   *pp2DTypeInstances = pSA2DInstances;

   IUnknown **pvIUnknowns = NULL;

   SafeArrayAccessData(pSA2DInstances,reinterpret_cast<void **>(&pvIUnknowns));

   k = 0;

   for ( std::pair<gc2DPlotTypes,IGSystemPlotType *> pPair : plotType2DProviderInstances ) {

      IUnknown *pIUnknown = NULL;

      pPair.second -> QueryInterface(IID_IUnknown,reinterpret_cast<void **>(&pIUnknown));

      pvIUnknowns[k] = pIUnknown;

      k++;
   }

   SafeArrayUnaccessData(pSA2DInstances);

   rgb.lLbound = 1;
   rgb.cElements = (DWORD)plotType3DProviderNames.size();

   SAFEARRAY *pSA3DIds = SafeArrayCreate(VT_I4,1,&rgb);
   SAFEARRAY *pSA3DNames = SafeArrayCreate(VT_BSTR,1,&rgb);
   SAFEARRAY *pSA3DInstanceNumbers = SafeArrayCreate(VT_I4,1,&rgb);

   *pp3DTypeIDs = pSA3DIds;
   *pp3DTypeNames = pSA3DNames;
   *pp3DTypeInstanceNumbers = pSA3DInstanceNumbers;

   SafeArrayAccessData(pSA3DIds,reinterpret_cast<void **>(&pvIDs));
   SafeArrayAccessData(pSA3DNames,reinterpret_cast<void **>(&pvNames));
   SafeArrayAccessData(pSA3DInstanceNumbers,reinterpret_cast<void **>(&pvInstanceNumbers));

   //memset(pvIDs,0,rgb.cElements * sizeof(VARIANT));
   //memset(pvNames,0,rgb.cElements * sizeof(VARIANT));

   k = 0;

   for ( std::pair<gc3DPlotTypes,char *> pPair : plotType3DProviderNames ) {

      pvIDs[k] = (long)pPair.first;

      long n = (DWORD)strlen(pPair.second) + 1;

      pvNames[k] = SysAllocStringLen(NULL,n);

      MultiByteToWideChar(CP_ACP,0,pPair.second,-1,pvNames[k],n);

      k++;
   }

   k = 0;

   for ( std::pair<gc3DPlotTypes,long> pPair : plotType3DInstanceNumber ) {
      pvInstanceNumbers[k] = pPair.second;
      k++;
   }

   SafeArrayUnaccessData(pSA3DIds);
   SafeArrayUnaccessData(pSA3DNames);
   SafeArrayUnaccessData(pSA3DInstanceNumbers);

   SAFEARRAY *pSA3DInstances = SafeArrayCreate(VT_UNKNOWN,1,&rgb);

   *pp3DTypeInstances = pSA3DInstances;

   pvIUnknowns = NULL;

   SafeArrayAccessData(pSA3DInstances,reinterpret_cast<void **>(&pvIUnknowns));

   k = 0;

   for ( std::pair<gc3DPlotTypes,IGSystemPlotType *> pPair : plotType3DProviderInstances ) {

      IUnknown *pIUnknown = NULL;

      pPair.second -> QueryInterface(IID_IUnknown,reinterpret_cast<void **>(&pIUnknown));

      pvIUnknowns[k] = pIUnknown;

      k++;
   }

   SafeArrayUnaccessData(pSA3DInstances);

   return S_OK;
   }
