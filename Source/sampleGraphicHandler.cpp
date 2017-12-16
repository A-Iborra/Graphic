/*

                       Copyright (c) 1996,1997,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
//#include <stdio.h>
#include <list>

#include "Graphic_resource.h"
#include "utils.h"

#include "Graphic.h"


   LRESULT CALLBACK G::sampleGraphicHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   G *p = (G *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch ( msg ) {
   case WM_CREATE: {
      CREATESTRUCT *pcs = (CREATESTRUCT *)lParam;
      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)pcs -> lpCreateParams);
      }
      return LRESULT(FALSE);
 
   case WM_PAINT: {
      PAINTSTRUCT ps;
      BeginPaint(hwnd,&ps);
      EndPaint(hwnd,&ps);
      if ( ! p ) break;
      p -> pIOpenGLImplementation -> SetTargetWindow(hwnd);
      p -> pIOpenGLImplementation -> SetUp(NULL,p -> propertyPlotView);
      p -> pIOpenGLImplementation -> Erase(p -> propertyBackgroundColor);
      VARIANT_BOOL doDrawText[3];
      p -> xaxis -> get_DrawText(&doDrawText[0]);
      p -> yaxis -> get_DrawText(&doDrawText[1]);
      p -> zaxis -> get_DrawText(&doDrawText[2]);
      p -> xaxis -> put_DrawText(VARIANT_FALSE);
      p -> yaxis -> put_DrawText(VARIANT_FALSE);
      p -> zaxis -> put_DrawText(VARIANT_FALSE);
      if ( GetParent(hwnd) == p -> hwndPlotSettings ) {
         long plotID = -1L;
         std::list<long> plotIDs;
         long itemCount = SendDlgItemMessage(GetParent(hwnd),IDDI_GRAPHIC_PLOTS_LIST,LVM_GETITEMCOUNT,0L,0L);
         if ( 0 < itemCount ) {
            long selectedCount = SendDlgItemMessage(GetParent(hwnd),IDDI_GRAPHIC_PLOTS_LIST,LVM_GETSELECTEDCOUNT,0L,0L);
            if ( 0 < selectedCount ) {
               LVITEM lvItem = {0};
               for ( int k = 0; k < itemCount; k++ ) {
                  memset(&lvItem,0,sizeof(LVITEM));
                  lvItem.iItem = k;
                  lvItem.mask = LVIF_STATE | LVIF_PARAM;
                  lvItem.stateMask = LVIS_SELECTED;
                  SendDlgItemMessage(GetParent(hwnd),IDDI_GRAPHIC_PLOTS_LIST,LVM_GETITEM,0L,(LPARAM)&lvItem);
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
      }
      return LRESULT(FALSE);
 
 
   default:
      break;
   }
 
   return DefWindowProc(hwnd,msg,wParam,lParam);
   }