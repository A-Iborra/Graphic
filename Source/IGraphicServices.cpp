// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"

   HRESULT G::ActivateLighting() {
   pIOpenGLImplementation -> SetLighting(ppPropertyLightOn,ppPropertyAmbientLight,ppPropertyDiffuseLight,ppPropertySpecularLight,ppPropertyLightPos,propertyCountLights,propertyShinyness);
   return S_OK;
   }

   HRESULT G::AnyOpenGLHere(RECT *pRect) {

   long *pCallLists = NULL;

   getSegmentsExcludingAxiis(&pCallLists);

   if ( ! pCallLists )
      return S_FALSE;

   unsigned int *pHitTable = new unsigned int[HIT_TABLE_SIZE];
 
   unsigned int hitsCount = 0;

   pIOpenGLImplementation -> GetPickBoxHits((POINTL *)pRect,pRect -> right - pRect -> left,pRect -> bottom - pRect -> top,pHitTable,HIT_TABLE_SIZE,pCallLists,&hitsCount);

   delete [] pCallLists;
   delete [] pHitTable;

   return 0 < hitsCount ? S_OK : S_FALSE;
   }
 