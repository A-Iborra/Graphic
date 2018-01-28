// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "graphic.h"

#include "utils.h"
#include "Graphic_resource.h"
#include "GMessage.h"

   LRESULT CALLBACK G::styleHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   G *p = (G *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);

      p = (G *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      p -> hwndStyleSettings = hwnd;

      }
      return LRESULT(FALSE);

   case WM_DESTROY:
      p -> hwndSampleGraphic = NULL;
      break;

   case WM_SHOWWINDOW: {

      if ( ! wParam )
         break;

      if ( lParam )
         break;

      SetParent(hwndSampleGraphic,hwnd);

      RECT rcSample,rcParent;
      GetWindowRect(hwnd,&rcParent);
      GetWindowRect(GetDlgItem(hwnd,IDDI_GRAPHIC_STYLE_SAMPLE),&rcSample);

      long cx = rcParent.right - rcParent.left - (rcSample.left - rcParent.left) - 32;
      long cy = rcParent.bottom - rcParent.top - 32;
      rcSample.left = (rcSample.left - rcParent.left) + 16;
      rcSample.top = 16;
      rcSample.right = rcSample.left + cx;
      rcSample.bottom = rcSample.top + cy;

      SendMessage(hwndSampleGraphic,WMG_POSITION_SAMPLE_GRAPHIC,0L,(LPARAM)&rcSample);

      }
      break;

   case WM_ERASEBKGND:

      if ( p -> plotView == gcPlotView3D )
         SendDlgItemMessage(hwnd,IDMI_GRAPHIC_VIEW_3D,BM_SETCHECK,1L,0);
      else
         SendDlgItemMessage(hwnd,IDMI_GRAPHIC_VIEW_2D,BM_SETCHECK,1L,0);

      break;

   case WM_COMMAND: {
      int idControl = LOWORD(wParam);
      long rv = (long)SendMessage((HWND)lParam,BM_GETCHECK,0L,0L);
      switch ( idControl ) {
      case IDMI_GRAPHIC_VIEW_2D: {
         if ( rv == BST_CHECKED ) 
            p -> plotView = gcPlotView2D;
         else
            p -> plotView = gcPlotView3D;
         }
         break;
      case IDMI_GRAPHIC_VIEW_3D: {
         if ( rv == BST_CHECKED ) 
            p -> plotView = gcPlotView3D;
         else
            p -> plotView = gcPlotView2D;
         }
         break;
      case IDDI_GRAPHIC_STYLE_SETVIEW: {
         p -> pIViewSet -> Properties(p -> Canvas(),styleHandlerSomeObjectChanged,(void *)p);
         EnableWindow(hwnd,FALSE);
         }
         return (LRESULT)0L;

      default:
         break;
      }

      EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_STYLE_SETVIEW),p -> plotView == gcPlotView3D ? TRUE : FALSE);
      InvalidateRect(hwndSampleGraphic,NULL,TRUE);

      }
      return LRESULT(FALSE);

   default:
      break;
   }

   return LRESULT(FALSE);
   }