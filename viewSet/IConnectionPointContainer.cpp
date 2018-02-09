// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ViewSet.h"

#include "Graphic_resource.h"

   STDMETHODIMP ViewSet::EnumConnectionPoints(IEnumConnectionPoints **ppEnum) {

   _IConnectionPoint * connectionPoints[1];

   *ppEnum = NULL;
 
   if ( pIEnumConnectionPoints ) 
      delete pIEnumConnectionPoints;
 
   connectionPoints[0] = pIConnectionPoint;

   pIEnumConnectionPoints = new _IEnumConnectionPoints(this,connectionPoints,1);
 
   return pIEnumConnectionPoints -> QueryInterface(IID_IEnumConnectionPoints,(void **)ppEnum);
   }
 
 
   STDMETHODIMP ViewSet::FindConnectionPoint(REFIID riid,IConnectionPoint **ppCP) {
   *ppCP = NULL;
    if ( riid == IID_IGSGraphicEvents )
      return pIConnectionPoint -> QueryInterface(IID_IConnectionPoint,(void **)ppCP);
     return E_NOINTERFACE;
   }
 
 
   void ViewSet::fire_Clicked() {

   IEnumConnections* pIEnum;
   CONNECTDATA connectData;

   pIConnectionPoint -> EnumConnections(&pIEnum);

   if ( ! pIEnum ) 
      return;

   while ( 1 ) {

      if ( pIEnum -> Next(1, &connectData, NULL) ) 
         break;

      IGSGraphicEvents * p = reinterpret_cast<IGSGraphicEvents *>(connectData.pUnk);

      p -> Clicked();

   }

   pIEnum -> Release();

   return;
   }


   void ViewSet::fire_ReDraw() {

   IEnumConnections* pIEnum;
   CONNECTDATA connectData;

   pIConnectionPoint -> EnumConnections(&pIEnum);

   if ( ! pIEnum ) return;

   while ( 1 ) {

      if ( pIEnum -> Next(1, &connectData, NULL) ) 
         break;

      IGSGraphicEvents * p = reinterpret_cast<IGSGraphicEvents *>(connectData.pUnk);

      p -> ReDraw();

   }

   pIEnum -> Release();

   return;
   }
