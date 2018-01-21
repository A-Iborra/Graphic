// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"

#include <list>

#include "Graphic_resource.h"
#include "GMessage.h"

#include "utils.h"

   bool plotSelectedPlots = false;

   LRESULT CALLBACK G::sampleGraphicHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   G *p = (G *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch ( msg ) {
   case WM_CREATE: {

      CREATESTRUCT *pcs = (CREATESTRUCT *)lParam;
      p = (G *)pcs -> lpCreateParams;
      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      HWND hwndCurrentOpenGL = p -> pIOpenGLImplementation -> TargetHWND();

      RECT rc;
      GetWindowRect(hwndCurrentOpenGL,&rc);

      p -> sizeMainGraphic.cx = rc.right - rc.left;
      p -> sizeMainGraphic.cy = rc.bottom - rc.top;

      }
      return LRESULT(FALSE);
 
   case WMG_POSITION_SAMPLE_GRAPHIC: {

      plotSelectedPlots = 1L == wParam ? true : false;

      double aspectRatio = (double)p -> sizeMainGraphic.cx / (double)p -> sizeMainGraphic.cy;

      RECT rcThis;

      if ( NULL == lParam ) {
         RECT rcParent;
         GetWindowRect(GetParent(hwnd),&rcParent);
         GetWindowRect(hwnd,&rcThis);
         rcThis.right -= rcParent.left;
         rcThis.left -= rcParent.left;
         rcThis.top -= rcParent.top;
         rcThis.bottom -= rcParent.bottom;
      } else
         memcpy(&rcThis,(BYTE *)lParam,sizeof(RECT));

      long cx = rcThis.right - rcThis.left;
      long cy = (long)((double)cx / aspectRatio);

      if ( rcThis.bottom - rcThis.top < cy ) {
         long cySubtract = cy - (rcThis.bottom - rcThis.top);
         cy = rcThis.bottom - rcThis.top;
         cx -= (long)((double)cySubtract * aspectRatio);
      }

      long x = (rcThis.right + rcThis.left - cx) / 2;
      long y = (rcThis.bottom + rcThis.top - cy) / 2;

      SetWindowPos(hwnd,HWND_TOP,x,y,cx,cy,SWP_SHOWWINDOW);

      if ( p -> hwndSampleGraphicSurface )
         DestroyWindow(p -> hwndSampleGraphicSurface);

      p -> hwndSampleGraphicSurface = CreateWindowEx(0L,"G-plotSettingsGraphic-Surface","",WS_CHILD | WS_VISIBLE,0,0,cx,cy,hwnd,NULL,hModule,(void *)p);

      p -> pIOpenGLImplementation -> SetTargetWindow(p -> hwndSampleGraphicSurface);

      return (LRESULT)0L;
      }

   case WM_PAINT: {
      PAINTSTRUCT ps;
      BeginPaint(hwnd,&ps);
      EndPaint(hwnd,&ps);
      }
      return LRESULT(FALSE);
 
 
   default:
      break;
   }
 
   return DefWindowProc(hwnd,msg,wParam,lParam);
   }


   LRESULT CALLBACK G::sampleGraphicSurfaceHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   G *p = (G *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_CREATE: {
      p = (G *)((CREATESTRUCT *)lParam) -> lpCreateParams;
      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);
      p -> pIOpenGLImplementation -> SetTargetWindow(hwnd);
      }
      break;

   case WM_ERASEBKGND:
   case WM_PAINT: {
      PAINTSTRUCT ps;
      BeginPaint(hwnd,&ps);

      p -> pIOpenGLImplementation -> SetUp(NULL,p -> propertyPlotView);
      p -> pIOpenGLImplementation -> Erase(p -> propertyBackgroundColor);

      VARIANT_BOOL doDrawText[3];
      p -> xaxis -> get_DrawText(&doDrawText[0]);
      p -> yaxis -> get_DrawText(&doDrawText[1]);
      p -> zaxis -> get_DrawText(&doDrawText[2]);
      p -> xaxis -> put_DrawText(VARIANT_FALSE);
      p -> yaxis -> put_DrawText(VARIANT_FALSE);
      p -> zaxis -> put_DrawText(VARIANT_FALSE);

      if ( plotSelectedPlots ) {
         long plotID = -1L;
         std::list<long> plotIDs;
         long itemCount = SendDlgItemMessage(GetParent(GetParent(hwnd)),IDDI_GRAPHIC_PLOTS_LIST,LVM_GETITEMCOUNT,0L,0L);
         if ( 0 < itemCount ) {
            long selectedCount = SendDlgItemMessage(GetParent(GetParent(hwnd)),IDDI_GRAPHIC_PLOTS_LIST,LVM_GETSELECTEDCOUNT,0L,0L);
            if ( 0 < selectedCount ) {
               LVITEM lvItem = {0};
               for ( int k = 0; k < itemCount; k++ ) {
                  memset(&lvItem,0,sizeof(LVITEM));
                  lvItem.iItem = k;
                  lvItem.mask = LVIF_STATE | LVIF_PARAM;
                  lvItem.stateMask = LVIS_SELECTED;
                  SendDlgItemMessage(GetParent(GetParent(hwnd)),IDDI_GRAPHIC_PLOTS_LIST,LVM_GETITEM,0L,(LPARAM)&lvItem);
                  if ( lvItem.state && LVIS_SELECTED ) {
                     IPlot * pIPlot = (IPlot *)lvItem.lParam;
                     plotID = p -> plotList.ID(pIPlot);
                     plotIDs.push_front(plotID);
                  }
               }
               for ( long id : plotIDs )
                  p -> render(id);
            }
         }
      } else
         p -> render(0);

      p -> xaxis -> put_DrawText(doDrawText[0]);
      p -> yaxis -> put_DrawText(doDrawText[1]);
      p -> zaxis -> put_DrawText(doDrawText[2]);

      EndPaint(hwnd,&ps);
      return (LRESULT)FALSE;
      }

   }

   return DefWindowProc(hwnd,msg,wParam,lParam);
   }