// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Text.h"
#include <math.h>

#include "Graphic_resource.h"

   HRESULT Text::LeftMouse() {
   return S_OK;  
   }
 
 
   HRESULT Text::RightMouse() {
   return S_OK;
   }
 
 
   HRESULT Text::MenuRequest() {
   statusPosition();
   POINT ptlMouse;
   GetCursorPos(&ptlMouse);
   if ( ! pITextNotify ) {
      EnableMenuItem(hMenu,IDMI_TEXT_DELETE,MF_BYCOMMAND | MF_GRAYED);
   } else {
      EnableMenuItem(hMenu,IDMI_TEXT_DELETE,MF_BYCOMMAND | MF_ENABLED);
   }
   TrackPopupMenu(hMenu,TPM_LEFTALIGN | TPM_RIGHTBUTTON,ptlMouse.x,ptlMouse.y,0,hwndObjectWindow,NULL);
   return 1;
   }
 
 
   HRESULT Text::Unselector() {
   eraseBoundingBoxGDI();
   return S_OK;
   }
 
 
   static double totalXMovement = 0.0;

   HRESULT Text::Selector() {

   statusPosition();

   if ( ! moveCursor )
      moveCursor = SetCursor(LoadCursor(NULL,IDC_SIZEALL));

   pIOpenGLImplementation -> get_MousePositionClient(&ptSelectPoint);

   pIOpenGLImplementation -> DataToWindow(&dpStart,UNIT_PIXEL,&dpSelectOffsetRestore);

   ptSelectOffset.x = (long)dpSelectOffsetRestore.x - ptSelectPoint.x;
   ptSelectOffset.y = (long)dpSelectOffsetRestore.y - ptSelectPoint.y;

   dpSelectOffsetRestore.x = (double)ptSelectPoint.x;
   dpSelectOffsetRestore.y = (double)ptSelectPoint.y;
   dpSelectOffsetRestore.z = 0.0;

   pIOpenGLImplementation -> WindowToData(&dpSelectOffsetRestore,&dpSelectOffsetRestore);

   dpSelectOffsetRestore.x = dpStart.x - dpSelectOffsetRestore.x;
   dpSelectOffsetRestore.y = dpStart.y - dpSelectOffsetRestore.y;
   dpSelectOffsetRestore.z = dpStart.z - dpSelectOffsetRestore.z;

   Erase();

   saveBoundingBoxBackground();

   pIBasePlotBoundingBox -> DrawGDI();

   totalXMovement = 0.0;

   return S_OK;
   }


   HRESULT Text::MouseMove(POINT* ptMouse) {

   eraseBoundingBoxGDI();

   DataPoint dpNew,dpMouse = {(double)ptMouse -> x,(double)ptMouse -> y,0.0};

   pIOpenGLImplementation -> WindowToData(&dpMouse,&dpNew);

   totalXMovement += fabs(dpNew.x - dpStart.x);

   DataPoint dpTranslate = {dpNew.x - dpStart.x,dpNew.y - dpStart.y,dpNew.z - dpStart.z};

   pIDataSet -> Translate(&dpTranslate);

   pIDataSetBoundingBox -> Translate(&dpTranslate);

   pIDataSet -> GenerateGDICoordinates(pIOpenGLImplementation);

   pIDataSetBoundingBox -> GenerateGDICoordinates(pIOpenGLImplementation);

   pIDataSet -> TranslateGDI(&ptSelectOffset);

   pIDataSetBoundingBox -> TranslateGDI(&ptSelectOffset);

   dpStart.x = dpNew.x;
   dpStart.y = dpNew.y;
   dpStart.z = dpNew.z;

   put_PositionX(dpStart.x);
   put_PositionY(dpStart.y);
   put_PositionZ(dpStart.z);

   saveBoundingBoxBackground();

   pIBasePlotBoundingBox -> DrawGDI();

   //Draw();

   statusPosition();

   return S_OK;
   }


   HRESULT Text::MouseRelease() {

   if ( moveCursor )
      SetCursor(moveCursor);

   moveCursor = NULL;

   eraseBoundingBoxGDI();

   if ( 0.0 < totalXMovement ) {

      dpStart.x += dpSelectOffsetRestore.x;
      dpStart.y += dpSelectOffsetRestore.y;
      dpStart.z += dpSelectOffsetRestore.z;

      pIDataSet -> Translate(&dpSelectOffsetRestore);

      put_PositionX(dpStart.x);
      put_PositionY(dpStart.y);
      put_PositionZ(dpStart.z);

      pIDataSetBoundingBox -> Translate(&dpSelectOffsetRestore);

   }

   Draw();

   return S_OK;
   }


   HRESULT Text::DefaultAction() {
   HWND hwndOwner = NULL;
   POINT ptCursor = {0};
   GetCursorPos(&ptCursor);
   hwndOwner = WindowFromPoint(ptCursor);
   IUnknown* pIUnknown;
   QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknown));
   pIProperties -> ShowProperties(hwndOwner ? hwndOwner : GetForegroundWindow(),pIUnknown);
   pIUnknown -> Release();
   if ( pWhenChangedCallback ) 
      pWhenChangedCallback(pWhenChangedCallbackArg,whenChangedCallbackCookie);
   return S_OK;
   }