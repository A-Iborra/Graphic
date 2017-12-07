/*

                       Copyright (c) 1996,1997,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include "utils.h"
#include "Graphic_resource.h"

#include "Graphic.h"

   LRESULT CALLBACK G::styleHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   G *p = (G *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {
   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);

      p = (G *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      p -> hwndStyleSettings = hwnd;

      RECT rect;
      GetClientRect(GetDlgItem(p -> hwndStyleSettings,IDDI_GRAPHIC_STYLE_SAMPLE),&rect);
      MapWindowPoints(GetDlgItem(p -> hwndStyleSettings,IDDI_GRAPHIC_STYLE_SAMPLE),p -> hwndStyleSettings,reinterpret_cast<POINT *>(&rect),2);
      HWND hwndTemp = CreateWindowEx(WS_EX_CLIENTEDGE,
                                               "G-plotSettingsGraphic",
                                               "",
                                               WS_VISIBLE | WS_CHILD,
                                               rect.left,rect.top,rect.right - rect.left,rect.bottom - rect.top,
                                               p -> hwndStyleSettings,NULL,hModule,(void *)p);
   
      DestroyWindow(GetDlgItem(p -> hwndStyleSettings,IDDI_GRAPHIC_STYLE_SAMPLE));

      SetWindowLongPtr(hwndTemp,GWLP_ID,IDDI_GRAPHIC_STYLE_SAMPLE);

      p -> propertyPlotView -> setWindowItemChecked(p -> hwndStyleSettings,IDMI_GRAPHIC_VIEW_3D);

      SendDlgItemMessage(p -> hwndStyleSettings,IDDI_GRAPHIC_SUB_STYLE_SURFACE,BM_SETCHECK,(WPARAM)BST_UNCHECKED,0L);
      SendDlgItemMessage(p -> hwndStyleSettings,IDDI_GRAPHIC_SUB_STYLE_WIREFRAME,BM_SETCHECK,(WPARAM)BST_UNCHECKED,0L);
      SendDlgItemMessage(p -> hwndStyleSettings,IDDI_GRAPHIC_SUB_STYLE_NATURAL,BM_SETCHECK,(WPARAM)BST_UNCHECKED,0L);
      SendDlgItemMessage(p -> hwndStyleSettings,IDDI_GRAPHIC_SUB_STYLE_STACKS,BM_SETCHECK,(WPARAM)BST_UNCHECKED,0L);

      EnableWindow(GetDlgItem(p -> hwndStyleSettings,IDDI_GRAPHIC_SUB_STYLE_SURFACE),p -> plotView == gcPlotView3D);
      EnableWindow(GetDlgItem(p -> hwndStyleSettings,IDDI_GRAPHIC_SUB_STYLE_WIREFRAME),p -> plotView == gcPlotView3D);
      EnableWindow(GetDlgItem(p -> hwndStyleSettings,IDDI_GRAPHIC_SUB_STYLE_NATURAL),p -> plotView == gcPlotView3D);
      EnableWindow(GetDlgItem(p -> hwndStyleSettings,IDDI_GRAPHIC_SUB_STYLE_STACKS),p -> plotView == gcPlotView3D);

      switch ( p -> plotType ) {
      case gcPlotTypeSurface:
         SendDlgItemMessage(p -> hwndStyleSettings,IDDI_GRAPHIC_SUB_STYLE_SURFACE,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
         break;
      case gcPlotTypeWireFrame:
         SendDlgItemMessage(p -> hwndStyleSettings,IDDI_GRAPHIC_SUB_STYLE_WIREFRAME,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
         break;
      case gcPlotTypeNatural:
         SendDlgItemMessage(p -> hwndStyleSettings,IDDI_GRAPHIC_SUB_STYLE_NATURAL,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
         break;
      case gcPlotTypeStacks:
         SendDlgItemMessage(p -> hwndStyleSettings,IDDI_GRAPHIC_SUB_STYLE_STACKS,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
         break;
      }

      }
      return LRESULT(FALSE);

   case WM_ERASEBKGND:
/* */
      EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_SURFACE),p -> plotView == gcPlotView3D);
      EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_WIREFRAME),p -> plotView == gcPlotView3D);
      EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_NATURAL),p -> plotView == gcPlotView3D);
      EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_STACKS),p -> plotView == gcPlotView3D);
/* */
      if ( p -> plotView == gcPlotView3D )
         SendDlgItemMessage(hwnd,IDMI_GRAPHIC_VIEW_3D,BM_SETCHECK,1L,0);
      else
         SendDlgItemMessage(hwnd,IDMI_GRAPHIC_VIEW_3D,BM_SETCHECK,0L,0);
/* */
      if ( p -> plotType == gcPlotTypeSurface ) SendDlgItemMessage(hwnd,IDDI_GRAPHIC_SUB_STYLE_SURFACE,BM_SETCHECK,1L,0);
      if ( p -> plotType == gcPlotTypeWireFrame ) SendDlgItemMessage(hwnd,IDDI_GRAPHIC_SUB_STYLE_WIREFRAME,BM_SETCHECK,1L,0);
      if ( p -> plotType == gcPlotTypeNatural ) SendDlgItemMessage(hwnd,IDDI_GRAPHIC_SUB_STYLE_NATURAL,BM_SETCHECK,1L,0);
      if ( p -> plotType == gcPlotTypeStacks ) SendDlgItemMessage(hwnd,IDDI_GRAPHIC_SUB_STYLE_STACKS,BM_SETCHECK,1L,0);
/* */
      break;

   case WM_COMMAND: {
      int idControl = LOWORD(wParam);
      long rv = SendMessage((HWND)lParam,BM_GETCHECK,0L,0L);
      switch ( idControl ) {
      case IDMI_GRAPHIC_VIEW_3D: {
         if ( rv == BST_CHECKED ) 
            p -> plotView = gcPlotView3D;
         else
            p -> plotView = gcPlotView2D;
/* */
         EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_SURFACE),p -> plotView == gcPlotView3D);
         EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_WIREFRAME),p -> plotView == gcPlotView3D);
         EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_NATURAL),p -> plotView == gcPlotView3D);
         EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_STACKS),p -> plotView == gcPlotView3D);
/* */
         }
         break;
/* */
      case IDDI_GRAPHIC_SUB_STYLE_SURFACE:
         if ( rv == BST_CHECKED ) p -> put_PlotType(gcPlotTypeSurface);
         break;
      case IDDI_GRAPHIC_SUB_STYLE_WIREFRAME:
         if ( rv == BST_CHECKED ) p -> put_PlotType(gcPlotTypeWireFrame);
         break;
      case IDDI_GRAPHIC_SUB_STYLE_NATURAL:
         if ( rv == BST_CHECKED ) p -> put_PlotType(gcPlotTypeNatural);
         break;
      case IDDI_GRAPHIC_SUB_STYLE_STACKS:
         if ( rv == BST_CHECKED ) p -> put_PlotType(gcPlotTypeStacks);
         break;
/* */
      default:
         break;
      }
      InvalidateRect(GetDlgItem(hwnd,IDDI_GRAPHIC_STYLE_SAMPLE),NULL,TRUE);
      UpdateWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_STYLE_SAMPLE));
      }
      return LRESULT(FALSE);

   default:
      break;
   }
   return LRESULT(FALSE);
   }