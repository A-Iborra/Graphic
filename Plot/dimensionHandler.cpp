/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/
#include <windows.h>

#include "Graphic_resource.h"
#include "utils.h"

#include "Plot.h"

   bool isSampleInUse = false;

   LRESULT CALLBACK Plot::dimensionHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   Plot *p = (Plot *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = (PROPSHEETPAGE *)lParam;

      p = (Plot *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      p -> hwndDimensionSettings = hwnd;

      PlotViews parentPlotView;

      p -> pOwnerPropertyPlotView -> get_longValue((long*)&parentPlotView);

      if ( gcPlotView3D == parentPlotView ) 
         SetDlgItemText(p -> hwndDimensionSettings,IDDI_PLOT_DIMENSION_OVERRIDE_OWNER,"Override the global 3-D transformation settings?");
      else
         SetDlgItemText(p -> hwndDimensionSettings,IDDI_PLOT_DIMENSION_OVERRIDE_OWNER,"Override the global 2-D transformation settings?");

      SendMessage(GetDlgItem(p -> hwndDimensionSettings,IDDI_PLOT_DIMENSION_OVERRIDE_OWNER),BM_SETCHECK,p -> overrideOwnerView ? BST_CHECKED : BST_UNCHECKED,0L);

      if ( p -> overrideOwnerView ) {
         SendMessage(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_3D),BM_SETCHECK,p -> plotView == gcPlotView3D ? BST_CHECKED : BST_UNCHECKED,0L);
         SendMessage(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_2D),BM_SETCHECK,p -> plotView == gcPlotView3D ? BST_UNCHECKED : BST_CHECKED,0L);
         EnableWindow(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_3D),TRUE);
         EnableWindow(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_2D),TRUE);
      } else {
         p -> pOwnerPropertyPlotView -> get_longValue((long*)&parentPlotView);
         SendMessage(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_3D),BM_SETCHECK,parentPlotView == gcPlotView3D ? BST_CHECKED : BST_UNCHECKED,0L);
         SendMessage(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_2D),BM_SETCHECK,parentPlotView == gcPlotView3D ? BST_UNCHECKED : BST_CHECKED,0L);
         EnableWindow(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_3D),FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_2D),FALSE);
      }

      RECT rect,rectParent;
      GetWindowRect(hwnd,&rectParent);
      long cx = 400;
      long cy = 400;
      GetWindowRect(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_SAMPLEPOSITION),&rect);

      SetWindowPos(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_SAMPLEPOSITION),HWND_TOP,rect.left - rectParent.left + 8,rect.top - rectParent.top + 8,
                                       cx - 2 * (rect.left - rectParent.left) - 16,cy - (rect.top - rectParent.top) - 16,SWP_SHOWWINDOW);

      SetWindowLongPtr(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_SAMPLEPOSITION),GWLP_USERDATA,(ULONG_PTR)p);

      if ( defaultStaticWindowHandler )
         SetWindowLongPtr(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_SAMPLEPOSITION),GWLP_WNDPROC,(ULONG_PTR)&Plot::sampleGraphicHandler);
      else
         defaultStaticWindowHandler = (WNDPROC)SetWindowLongPtr(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_SAMPLEPOSITION),GWLP_WNDPROC,(ULONG_PTR)&Plot::sampleGraphicHandler);

      }
      return LRESULT(0);

   //case WM_SHOWWINDOW: {

   //   if ( (BOOL)wParam && 0 == lParam ) {

   //      p -> pIOpenGLImplementation -> SetTargetWindow(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_SAMPLEPOSITION));

   //      p -> pIOpenGLImplementation -> SetUp(p -> pIDataSet,p -> overrideOwnerView ? p -> propertyPlotView : p -> pOwnerPropertyPlotView,
   //                                             p -> pOwnerPropertyTheta,p -> pOwnerPropertyPhi,p -> pOwnerPropertySpin);

   //      p -> pIOpenGLImplementation -> SetLighting(p -> pOwnerPropertiesLightOn,
   //                                                   p -> pOwnerPropertiesAmbientLight,
   //                                                   p -> pOwnerPropertiesDiffuseLight,
   //                                                   p -> pOwnerPropertiesSpecularLight,
   //                                                   p -> pOwnerPropertiesLightPosition,
   //                                                   p -> pOwnerPropertyCountLights,NULL);
   //   }

   //   }
   //   break;

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

      InvalidateRect(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_SAMPLEPOSITION),NULL,TRUE);

      }
      return LRESULT(0);

   default:
      break;
   }
   return LRESULT(0);
   }
