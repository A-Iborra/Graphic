/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include "Plot.h"

#include "Graphic_resource.h"

   bool isSampleInUse = false;

   LRESULT CALLBACK Plot::dimensionHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   Plot *p = (Plot *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = (PROPSHEETPAGE *)lParam;

      p = (Plot *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      p -> hwndDimensionSettings = hwnd;

      if ( ! hwndSampleGraphic )
         hwndSampleGraphic = CreateWindowEx(WS_EX_CLIENTEDGE,"plot-sample-graphic","",WS_CHILD,0,0,0,0,hwnd,NULL,hModule,(void *)p);

      PlotViews parentPlotView;

      p -> pOwnerPropertyPlotView -> get_longValue((long*)&parentPlotView);

      if ( gcPlotView3D == parentPlotView ) 
         SetDlgItemText(p -> hwndDimensionSettings,IDDI_PLOT_DIMENSION_OVERRIDE_OWNER,"Override the main 3-D view settings:");
      else
         SetDlgItemText(p -> hwndDimensionSettings,IDDI_PLOT_DIMENSION_OVERRIDE_OWNER,"Override the main 2-D view settings:");

      SendMessage(GetDlgItem(p -> hwndDimensionSettings,IDDI_PLOT_DIMENSION_OVERRIDE_OWNER),BM_SETCHECK,p -> overrideOwnerView ? BST_CHECKED : BST_UNCHECKED,0L);

      if ( p -> overrideOwnerView ) {
         SendMessage(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_3D),BM_SETCHECK,gcPlotView3D == p -> plotView ? BST_CHECKED : BST_UNCHECKED,0L);
         SendMessage(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_2D),BM_SETCHECK,gcPlotView3D == p -> plotView ? BST_UNCHECKED : BST_CHECKED,0L);
         EnableWindow(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_3D),TRUE);
         EnableWindow(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_2D),TRUE);
      } else {
         p -> pOwnerPropertyPlotView -> get_longValue((long*)&parentPlotView);
         SendMessage(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_3D),BM_SETCHECK,gcPlotView3D == parentPlotView ? BST_CHECKED : BST_UNCHECKED,0L);
         SendMessage(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_2D),BM_SETCHECK,gcPlotView3D == parentPlotView ? BST_UNCHECKED : BST_CHECKED,0L);
         EnableWindow(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_3D),FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_2D),FALSE);
      }

      }
      return LRESULT(0);

   case WM_DESTROY:
      hwndSampleGraphic = NULL;
      break;

   case WM_SHOWWINDOW:

      if ( ! wParam )
         break;

      if ( lParam )
         break;

      SetParent(hwndSampleGraphic,hwnd);

      RECT rect,rectParent;
      GetWindowRect(hwnd,&rectParent);

      GetWindowRect(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_SAMPLEPOSITION),&rect);

      RECT rcSample;
      rcSample.left = rect.left - rectParent.left + 8;
      rcSample.top = rect.top - rectParent.top + 8;
      rcSample.right = rcSample.left + rectParent.right - rect.left - (rect.left - rectParent.left) - 16;
      rcSample.bottom = rcSample.top + rectParent.bottom - rectParent.top - (rect.top - rectParent.top) - 16;

      SendMessage(hwndSampleGraphic,WMG_POSITION_SAMPLE_GRAPHIC,0L,(LPARAM)&rcSample);

      return (LRESULT)0L;

   case WM_COMMAND: {
      int controlID = LOWORD(wParam);
      int notificationCode = HIWORD(wParam);
      if ( ! p ) break;
      switch ( controlID ) {

      case IDDI_PLOT_DIMENSION_OVERRIDE_OWNER:
         p -> propertyOverrideOwnerView -> getWindowChecked((HWND)lParam);
         if ( p -> overrideOwnerView ) {
            SendMessage(GetDlgItem(p -> hwndDimensionSettings,IDDI_PLOT_DIMENSION_3D),BM_SETCHECK,p -> plotView == gcPlotView3D ? BST_CHECKED : BST_UNCHECKED,0L);
            SendMessage(GetDlgItem(p -> hwndDimensionSettings,IDDI_PLOT_DIMENSION_2D),BM_SETCHECK,p -> plotView == gcPlotView3D ? BST_UNCHECKED : BST_CHECKED,0L);
            EnableWindow(GetDlgItem(p -> hwndDimensionSettings,IDDI_PLOT_DIMENSION_3D),TRUE);
            EnableWindow(GetDlgItem(p -> hwndDimensionSettings,IDDI_PLOT_DIMENSION_2D),TRUE);
         } else {
            PlotViews parentPlotView;
            p -> pOwnerPropertyPlotView -> get_longValue((long*)&parentPlotView);
            SendMessage(GetDlgItem(p -> hwndDimensionSettings,IDDI_PLOT_DIMENSION_3D),BM_SETCHECK,parentPlotView == gcPlotView3D ? BST_CHECKED : BST_UNCHECKED,0L);
            SendMessage(GetDlgItem(p -> hwndDimensionSettings,IDDI_PLOT_DIMENSION_2D),BM_SETCHECK,parentPlotView == gcPlotView3D ? BST_UNCHECKED : BST_CHECKED,0L);
            EnableWindow(GetDlgItem(p -> hwndDimensionSettings,IDDI_PLOT_DIMENSION_3D),FALSE);
            EnableWindow(GetDlgItem(p -> hwndDimensionSettings,IDDI_PLOT_DIMENSION_2D),FALSE);
         }

         break;

      case IDDI_PLOT_DIMENSION_3D:
            p -> plotView = gcPlotView3D;
         break;

      case IDDI_PLOT_DIMENSION_2D:
            p -> plotView = gcPlotView2D;
         break;
      }

      InvalidateRect(hwndSampleGraphic,NULL,TRUE);

      }
      return LRESULT(0);

   default:
      break;
   }
   return LRESULT(0);
   }
