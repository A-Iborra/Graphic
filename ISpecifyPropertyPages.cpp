// Copyright 2017 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"

   HRESULT G::GetPages(CAUUID *pCAUUID) {

   if ( ! pCAUUID )
      return E_POINTER;

   pCAUUID -> cElems = 8;

   pCAUUID -> pElems = reinterpret_cast<GUID *>(CoTaskMemAlloc(pCAUUID -> cElems * sizeof(GUID)));

   memset(pCAUUID -> pElems,0,pCAUUID -> cElems * sizeof(GUID));

   pCAUUID -> pElems[0] = CLSID_GSystemGraphicPropertiesPosSize;
   pCAUUID -> pElems[1] = CLSID_GSystemGraphicPropertiesStyle;
   pCAUUID -> pElems[2] = CLSID_GSystemGraphicPropertiesBackground;
   pCAUUID -> pElems[3] = CLSID_GSystemGraphicPropertiesText;
   pCAUUID -> pElems[4] = CLSID_GSystemGraphicPropertiesLighting;
   pCAUUID -> pElems[5] = CLSID_GSystemGraphicPropertiesAxis;
   pCAUUID -> pElems[6] = CLSID_GSystemGraphicPropertiesPlot;
   pCAUUID -> pElems[7] = CLSID_GSystemGraphicPropertiesFunctions;

   return S_OK;
   }