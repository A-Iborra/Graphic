/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <stdio.h>

#include "Graphic_resource.h"

#include "utils.h"

#include "Text.h"


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
   eraseBoundingBox();
   return S_OK;
   }
 
 
   HRESULT Text::Selector() {

   statusPosition();

   if ( ! moveCursor )
      moveCursor = SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZEALL)));

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

   pIBasePlot -> DrawGDI();
   pIBasePlotBoundingBox -> DrawGDI();

   return S_OK;
   }


   HRESULT Text::MouseMove(POINT* ptMouse) {

   eraseGDI();
   eraseBoundingBoxGDI();

   DataPoint dpNew,dpMouse = {(double)ptMouse -> x,(double)ptMouse -> y,0.0};

   pIOpenGLImplementation -> WindowToData(&dpMouse,&dpNew);

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

   pIBasePlot -> DrawGDI();

   pIBasePlotBoundingBox -> DrawGDI();

   statusPosition();

   return S_OK;
   }


   HRESULT Text::MouseRelease() {

   if ( moveCursor )
      SetCursor(moveCursor);

   moveCursor = NULL;
   eraseGDI();
   eraseBoundingBoxGDI();

   dpStart.x += dpSelectOffsetRestore.x;
   dpStart.y += dpSelectOffsetRestore.y;
   dpStart.z += dpSelectOffsetRestore.z;

   pIDataSet -> Translate(&dpSelectOffsetRestore);

   put_PositionX(dpStart.x);
   put_PositionY(dpStart.y);
   put_PositionZ(dpStart.z);

   pIDataSetBoundingBox -> Translate(&dpSelectOffsetRestore);

   pIBasePlot -> Draw();

   return S_OK;
   }