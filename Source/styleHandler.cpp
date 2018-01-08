
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

      //EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_STYLE_SETVIEW),p -> plotView == gcPlotView3D ? TRUE : FALSE);


#if 0
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
#endif

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

#if 0
      EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_SURFACE),p -> plotView == gcPlotView3D);
      EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_WIREFRAME),p -> plotView == gcPlotView3D);
      EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_NATURAL),p -> plotView == gcPlotView3D);
      EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_STACKS),p -> plotView == gcPlotView3D);

      if ( p -> plotType == gcPlotTypeSurface ) SendDlgItemMessage(hwnd,IDDI_GRAPHIC_SUB_STYLE_SURFACE,BM_SETCHECK,1L,0);
      if ( p -> plotType == gcPlotTypeWireFrame ) SendDlgItemMessage(hwnd,IDDI_GRAPHIC_SUB_STYLE_WIREFRAME,BM_SETCHECK,1L,0);
      if ( p -> plotType == gcPlotTypeNatural ) SendDlgItemMessage(hwnd,IDDI_GRAPHIC_SUB_STYLE_NATURAL,BM_SETCHECK,1L,0);
      if ( p -> plotType == gcPlotTypeStacks ) SendDlgItemMessage(hwnd,IDDI_GRAPHIC_SUB_STYLE_STACKS,BM_SETCHECK,1L,0);
#endif
      if ( p -> plotView == gcPlotView3D )
         SendDlgItemMessage(hwnd,IDMI_GRAPHIC_VIEW_3D,BM_SETCHECK,1L,0);
      else
         SendDlgItemMessage(hwnd,IDMI_GRAPHIC_VIEW_2D,BM_SETCHECK,1L,0);

      break;

   case WM_COMMAND: {
      int idControl = LOWORD(wParam);
      long rv = SendMessage((HWND)lParam,BM_GETCHECK,0L,0L);
      switch ( idControl ) {
      case IDMI_GRAPHIC_VIEW_2D: {
         if ( rv == BST_CHECKED ) 
            p -> plotView = gcPlotView2D;
         else
            p -> plotView = gcPlotView3D;
#if 0
         EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_SURFACE),p -> plotView == gcPlotView3D);
         EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_WIREFRAME),p -> plotView == gcPlotView3D);
         EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_NATURAL),p -> plotView == gcPlotView3D);
         EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_STACKS),p -> plotView == gcPlotView3D);
#endif
         }
         break;
      case IDMI_GRAPHIC_VIEW_3D: {
         if ( rv == BST_CHECKED ) 
            p -> plotView = gcPlotView3D;
         else
            p -> plotView = gcPlotView2D;
#if 0
         EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_SURFACE),p -> plotView == gcPlotView3D);
         EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_WIREFRAME),p -> plotView == gcPlotView3D);
         EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_NATURAL),p -> plotView == gcPlotView3D);
         EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_STACKS),p -> plotView == gcPlotView3D);
#endif
         }
         break;
#if 0
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
#endif

      case IDDI_GRAPHIC_STYLE_SETVIEW: {
         p -> pIViewSet -> Properties(styleHandlerSomeObjectChanged,(void *)p);
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