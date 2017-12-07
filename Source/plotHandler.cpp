/*

                       Copyright (c) 1996,1997,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include <stdio.h>

#include "Graphic_resource.h"
#include "utils.h"

#include "Graphic.h"


   LRESULT CALLBACK G::plotHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   G *p = (G *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = (PROPSHEETPAGE *)lParam;

      p = (G *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      p -> hwndPlotSettings = hwnd;

      char szTemp[32];
      sprintf(szTemp,"There are %ld plots on this graphic",p -> plotList.Count());
      SetWindowText(GetDlgItem(hwnd,IDDI_GRAPHIC_PLOTS_COUNT),szTemp);

      RECT rect;

      GetClientRect(GetDlgItem(p -> hwndPlotSettings,IDDI_GRAPHIC_STYLE_SAMPLE),&rect);

      HWND hwndPlotSettingsGraphic;
   
      GetClientRect(GetDlgItem(p -> hwndPlotSettings,IDDI_GRAPHIC_PLOTS_GRAPHIC),&rect);

      MapWindowPoints(GetDlgItem(p -> hwndPlotSettings,IDDI_GRAPHIC_PLOTS_GRAPHIC),p -> hwndPlotSettings,(POINT *)&rect,2);
    
      hwndPlotSettingsGraphic = CreateWindowEx(WS_EX_CLIENTEDGE,
                                               "G-plotSettingsGraphic",
                                               "",
                                               WS_VISIBLE | WS_CHILD,
                                               rect.left,rect.top,rect.right - rect.left,rect.bottom - rect.top,
                                               p -> hwndPlotSettings,NULL,hModule,(void *)p);
      }
      return LRESULT(FALSE);
 
   case WM_PAINT: {
      PAINTSTRUCT ps;
      BeginPaint(hwnd,&ps);
      p -> pIOpenGLImplementation -> SetTargetWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_PLOTS_GRAPHIC));
      p -> stubDraw();
      p -> pIOpenGLImplementation -> ResetTargetWindow();
      EndPaint(hwnd,&ps);
      }
      break;

   case WM_COMMAND:
      switch ( LOWORD(wParam) ) {
      case IDDI_GRAPHIC_PLOTS_ADD: {
/*
         IPlot *pPlot;
         char szTemp[64];
 
         pPlot = p -> newPlot(p -> plotList.Count() + 1);
 
         sprintf(szTemp,"There are %ld plots on this graphic",p -> plotList.Count());
         SetWindowText(GetDlgItem(hwnd,IDDI_GRAPHIC_PLOTS_COUNT),szTemp);
 
         p -> pIOpenGLImplementation -> SetTargetWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_PLOTS_GRAPHIC));
         p -> render(0);
         p -> pIOpenGLImplementation -> ResetTargetWindow();
*/ 
      }
      break;
 
      case IDDI_GRAPHIC_PLOTS_EDIT:
         break;

      case IDDI_GRAPHIC_PLOTS_DELETE:
         break;
      }
      break;
 
   default:
      break;
   }
 
   return LRESULT(FALSE);
   }
