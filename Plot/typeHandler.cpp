/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/
#include <windows.h>

#include "Graphic_resource.h"
#include "utils.h"

#include "Plot.h"


   LRESULT CALLBACK Plot::typeHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
   Plot *p = (Plot *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch ( msg ) {
   case WM_INITDIALOG: {
      PROPSHEETPAGE *pPage = (PROPSHEETPAGE *)lParam;
      p = (Plot *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      p -> hwndTypeSettings = hwnd;

      PlotTypes pType;

      if ( p -> overrideOwnerType ) {
         pType = (PlotTypes)p -> plotType;
         EnableWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_OVERRIDE_OWNER),p -> pOwnerPropertyPlotType ? TRUE : FALSE);
      } else
         p -> pOwnerPropertyPlotType -> get_longValue(reinterpret_cast<long*>(&pType));

      p -> propertyOverrideOwnerType -> setWindowChecked(GetDlgItem(hwnd,IDDI_PLOT_TYPE_OVERRIDE_OWNER));

      SendMessage(GetDlgItem(hwnd,IDDI_PLOT_TYPE_NATURAL),BM_SETCHECK,(long)gcPlotTypeNatural & (long)pType ? BST_CHECKED : BST_UNCHECKED,0L);
      SendMessage(GetDlgItem(hwnd,IDDI_PLOT_TYPE_CONTOUR),BM_SETCHECK,(long)gcPlotTypeContour & (long)pType ? BST_CHECKED : BST_UNCHECKED,0L);
      SendMessage(GetDlgItem(hwnd,IDDI_PLOT_TYPE_PIE),BM_SETCHECK,(long)gcPlotTypePie & (long)pType ? BST_CHECKED : BST_UNCHECKED,0L);
      SendMessage(GetDlgItem(hwnd,IDDI_PLOT_TYPE_STACKS),BM_SETCHECK,(long)gcPlotTypeStacks & (long)pType ? BST_CHECKED : BST_UNCHECKED,0L);
      SendMessage(GetDlgItem(hwnd,IDDI_PLOT_TYPE_SURFACE),BM_SETCHECK,(long)gcPlotTypeSurface & (long)pType ? BST_CHECKED : BST_UNCHECKED,0L);
      SendMessage(GetDlgItem(hwnd,IDDI_PLOT_TYPE_WIREFRAME),BM_SETCHECK,(long)gcPlotTypeWireFrame & (long)pType ? BST_CHECKED : BST_UNCHECKED,0L);
      SendMessage(GetDlgItem(hwnd,IDDI_PLOT_TYPE_QUADS),BM_SETCHECK,(long)gcPlotTypeQuads & (long)pType ? BST_CHECKED : BST_UNCHECKED,0L);
      SendMessage(GetDlgItem(hwnd,IDDI_PLOT_TYPE_TRIANGLES),BM_SETCHECK,(long)gcPlotTypeTriangles & (long)pType ? BST_CHECKED : BST_UNCHECKED,0L);

      EnableWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_NATURAL),p -> overrideOwnerType ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_CONTOUR),p -> overrideOwnerType ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_PIE),p -> overrideOwnerType ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_STACKS),p -> overrideOwnerType ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_SURFACE),p -> overrideOwnerType ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_WIREFRAME),p -> overrideOwnerType ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_QUADS),p -> overrideOwnerType ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_TRIANGLES),p -> overrideOwnerType ? TRUE : FALSE);
      }

      return LRESULT(0);

   case WM_PAINT: {
      SetParent(p -> hwndSample,p -> hwndTypeSettings);
      RECT rect,rectParent;
      GetWindowRect(p -> hwndTypeSettings,&rectParent);
      long cx = 400;
      long cy = 400;
      GetWindowRect(GetDlgItem(p -> hwndTypeSettings,IDDI_PLOT_DIMENSION_SAMPLEPOSITION),&rect);
      SetWindowPos(p -> hwndSample,HWND_TOP,rect.left - rectParent.left + 8,rect.top - rectParent.top + 8,
                                          cx - 2 * (rect.left - rectParent.left) - 16,cy - (rect.top - rectParent.top) - 16,SWP_SHOWWINDOW);
      }
      break;

   case WM_COMMAND: {

      if ( ! p ) break;

      int controlID = LOWORD(wParam);
      int notificationCode = HIWORD(wParam);

      if ( controlID >= IDDI_PLOT_TYPE_MIN && controlID <= IDDI_PLOT_TYPE_MAX ) {

         long isChecked = SendMessage((HWND)lParam,BM_GETCHECK,0L,0L);
         long mask = 0x00000000;
         switch ( controlID ) {
         case IDDI_PLOT_TYPE_NATURAL:
            mask = (long)gcPlotTypeNatural;
            break;
         case IDDI_PLOT_TYPE_CONTOUR:
            mask = (long)gcPlotTypeContour;
            break;
         case IDDI_PLOT_TYPE_PIE:
            mask = (long)gcPlotTypePie;
            break;
         case IDDI_PLOT_TYPE_STACKS:
            mask = (long)gcPlotTypeStacks;
            break;
         case IDDI_PLOT_TYPE_SURFACE:
            mask = (long)gcPlotTypeSurface;
            break;
         case IDDI_PLOT_TYPE_WIREFRAME:
            mask = (long)gcPlotTypeWireFrame;
            break;
         case IDDI_PLOT_TYPE_QUADS:
            mask = (long)gcPlotTypeQuads;
            break;
         case IDDI_PLOT_TYPE_TRIANGLES:
            mask = (long)gcPlotTypeTriangles;
            break;
         }
               
         if ( isChecked )
            p -> plotType = p -> plotType | mask;
         else
            p -> plotType = p -> plotType & ~ mask;

      }

      else switch ( controlID ) {
      case IDDI_PLOT_TYPE_OVERRIDE_OWNER: {
         PlotTypes pType;

         p -> propertyOverrideOwnerType -> getWindowChecked((HWND)lParam);

         if ( p -> overrideOwnerType ) {
            pType = (PlotTypes)p -> plotType;
            EnableWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_OVERRIDE_OWNER),p -> pOwnerPropertyPlotType ? TRUE : FALSE);
         } else
            p -> pOwnerPropertyPlotType -> get_longValue(reinterpret_cast<long*>(&pType));

         SendMessage(GetDlgItem(hwnd,IDDI_PLOT_TYPE_NATURAL),BM_SETCHECK,(long)gcPlotTypeNatural & (long)pType ? BST_CHECKED : BST_UNCHECKED,0L);
         SendMessage(GetDlgItem(hwnd,IDDI_PLOT_TYPE_CONTOUR),BM_SETCHECK,(long)gcPlotTypeContour & (long)pType ? BST_CHECKED : BST_UNCHECKED,0L);
         SendMessage(GetDlgItem(hwnd,IDDI_PLOT_TYPE_PIE),BM_SETCHECK,(long)gcPlotTypePie & (long)pType ? BST_CHECKED : BST_UNCHECKED,0L);
         SendMessage(GetDlgItem(hwnd,IDDI_PLOT_TYPE_STACKS),BM_SETCHECK,(long)gcPlotTypeStacks & (long)pType ? BST_CHECKED : BST_UNCHECKED,0L);
         SendMessage(GetDlgItem(hwnd,IDDI_PLOT_TYPE_SURFACE),BM_SETCHECK,(long)gcPlotTypeSurface & (long)pType ? BST_CHECKED : BST_UNCHECKED,0L);
         SendMessage(GetDlgItem(hwnd,IDDI_PLOT_TYPE_WIREFRAME),BM_SETCHECK,(long)gcPlotTypeWireFrame & (long)pType ? BST_CHECKED : BST_UNCHECKED,0L);
         SendMessage(GetDlgItem(hwnd,IDDI_PLOT_TYPE_QUADS),BM_SETCHECK,(long)gcPlotTypeQuads & (long)pType ? BST_CHECKED : BST_UNCHECKED,0L);
         SendMessage(GetDlgItem(hwnd,IDDI_PLOT_TYPE_TRIANGLES),BM_SETCHECK,(long)gcPlotTypeTriangles & (long)pType ? BST_CHECKED : BST_UNCHECKED,0L);

         EnableWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_NATURAL),p -> overrideOwnerType ? TRUE : FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_CONTOUR),p -> overrideOwnerType ? TRUE : FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_PIE),p -> overrideOwnerType ? TRUE : FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_STACKS),p -> overrideOwnerType ? TRUE : FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_SURFACE),p -> overrideOwnerType ? TRUE : FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_WIREFRAME),p -> overrideOwnerType ? TRUE : FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_QUADS),p -> overrideOwnerType ? TRUE : FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_TRIANGLES),p -> overrideOwnerType ? TRUE : FALSE);
         }
         break;
      }

      InvalidateRect(p -> hwndSample,NULL,TRUE);
      return LRESULT(0);
      }

   default:
      break;
   }

   return LRESULT(0);
   }
