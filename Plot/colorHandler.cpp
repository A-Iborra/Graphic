/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002,2008 Nathan T. Clark

*/

#include "Plot.h"
#include <commctrl.h>
#include <functional>

#include "Graphic_resource.h"

#include "utilities.h"

 #define GET_VALUES() {                                                                \
      float v[] = {0.0f,0.0f,0.0f,0.0f};                                               \
      char szTemp[32];                                                                 \
                                                                                       \
      GetDlgItemText(hwnd,IDDI_PLOT_COLOR_LINE_RED,szTemp,32);                         \
      v[0] = (float)atof(szTemp);                                                      \
      GetDlgItemText(hwnd,IDDI_PLOT_COLOR_LINE_GREEN,szTemp,32);                       \
      v[1] = (float)atof(szTemp);                                                      \
      GetDlgItemText(hwnd,IDDI_PLOT_COLOR_LINE_BLUE,szTemp,32);                        \
      v[2] = (float)atof(szTemp);                                                      \
      p -> propertyLineColor -> put_binaryValue(sizeof(v),(BYTE*)v);                   \
                                                                                       \
      GetDlgItemText(hwnd,IDDI_PLOT_COLOR_TOP_RED,szTemp,32);                          \
      v[0] = (float)atof(szTemp);                                                      \
      GetDlgItemText(hwnd,IDDI_PLOT_COLOR_TOP_GREEN,szTemp,32);                        \
      v[1] = (float)atof(szTemp);                                                      \
      GetDlgItemText(hwnd,IDDI_PLOT_COLOR_TOP_BLUE,szTemp,32);                         \
      v[2] = (float)atof(szTemp);                                                      \
      p -> propertyTopSurfaceColor -> put_binaryValue(sizeof(v),(BYTE*)v);             \
char szX[64];\
sprintf(szX,"%g %g %g\n",v[0],v[1],v[2]);\
OutputDebugString(szX); \
                                                                                       \
      GetDlgItemText(hwnd,IDDI_PLOT_COLOR_BOTTOM_RED,szTemp,32);                       \
      v[0] = (float)atof(szTemp);                                                      \
      GetDlgItemText(hwnd,IDDI_PLOT_COLOR_BOTTOM_GREEN,szTemp,32);                     \
      v[1] = (float)atof(szTemp);                                                      \
      GetDlgItemText(hwnd,IDDI_PLOT_COLOR_BOTTOM_BLUE,szTemp,32);                      \
      v[2] = (float)atof(szTemp);                                                      \
      p -> propertyBottomSurfaceColor -> put_binaryValue(sizeof(v),(BYTE*)v);          \
                                                                                       \
      }
  
 
#define LOAD_VALUES() {                                                                \
      float v[4];                                                                      \
      char szTemp[32];                                                                 \
                                                                                       \
      BYTE *pb = (BYTE *)v;                                                            \
      p -> propertyLineColor -> get_binaryValue(sizeof(v),(BYTE**)&pb);                \
      sprintf(szTemp,"%3.1f",v[0]);                                                    \
      SetDlgItemText(hwnd,IDDI_PLOT_COLOR_LINE_RED,szTemp);                            \
      sprintf(szTemp,"%3.1f",v[1]);                                                    \
      SetDlgItemText(hwnd,IDDI_PLOT_COLOR_LINE_GREEN,szTemp);                          \
      sprintf(szTemp,"%3.1f",v[2]);                                                    \
      SetDlgItemText(hwnd,IDDI_PLOT_COLOR_LINE_BLUE,szTemp);                           \
                                                                                       \
      p -> propertyTopSurfaceColor -> get_binaryValue(sizeof(v),(BYTE**)&pb);          \
      sprintf(szTemp,"%3.1f",v[0]);                                                    \
      SetDlgItemText(hwnd,IDDI_PLOT_COLOR_TOP_RED,szTemp);                             \
      sprintf(szTemp,"%3.1f",v[1]);                                                    \
      SetDlgItemText(hwnd,IDDI_PLOT_COLOR_TOP_GREEN,szTemp);                           \
      sprintf(szTemp,"%3.1f",v[2]);                                                    \
      SetDlgItemText(hwnd,IDDI_PLOT_COLOR_TOP_BLUE,szTemp);                            \
                                                                                       \
      p -> propertyBottomSurfaceColor -> get_binaryValue(sizeof(v),(BYTE**)&pb);       \
      sprintf(szTemp,"%3.1f",v[0]);                                                    \
      SetDlgItemText(hwnd,IDDI_PLOT_COLOR_BOTTOM_RED,szTemp);                          \
      sprintf(szTemp,"%3.1f",v[1]);                                                    \
      SetDlgItemText(hwnd,IDDI_PLOT_COLOR_BOTTOM_GREEN,szTemp);                        \
      sprintf(szTemp,"%3.1f",v[2]);                                                    \
      SetDlgItemText(hwnd,IDDI_PLOT_COLOR_BOTTOM_BLUE,szTemp);                         \
                                                                                       \
      }
                                                                    

   static int holdUpdates = FALSE;

   static HWND hwndGroupLine = NULL;
   static HWND hwndGroupTop = NULL;
   static HWND hwndGroupBottom = NULL;
   static HWND hwndHidableGroups[] = {NULL,NULL};

   LRESULT CALLBACK Plot::colorHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   Plot *p = (Plot *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = (PROPSHEETPAGE *)lParam;

      p = (Plot *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      p -> hwndColorSettings = hwnd;

      if ( ! hwndSampleGraphic )
         hwndSampleGraphic = CreateWindowEx(WS_EX_CLIENTEDGE,"plot-sample-graphic","",WS_CHILD,0,0,0,0,hwnd,NULL,hModule,(void *)p);

      SendDlgItemMessage(hwnd,IDDI_PLOT_COLOR_LINE_RED_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_PLOT_COLOR_LINE_GREEN_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_PLOT_COLOR_LINE_BLUE_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_PLOT_COLOR_TOP_RED_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_PLOT_COLOR_TOP_GREEN_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_PLOT_COLOR_TOP_BLUE_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_PLOT_COLOR_BOTTOM_RED_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_PLOT_COLOR_BOTTOM_GREEN_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_PLOT_COLOR_BOTTOM_BLUE_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
 
      SetWindowLongPtr(GetDlgItem(hwnd,IDDI_PLOT_COLOR_LINE_BACKGROUND),GWLP_USERDATA,(ULONG_PTR)p);
      SetWindowLongPtr(GetDlgItem(hwnd,IDDI_PLOT_COLOR_TOP_BACKGROUND),GWLP_USERDATA,(ULONG_PTR)p);
      SetWindowLongPtr(GetDlgItem(hwnd,IDDI_PLOT_COLOR_BOTTOM_BACKGROUND),GWLP_USERDATA,(ULONG_PTR)p);
 
      Plot::defaultPatchPainter = (WNDPROC)SetWindowLongPtr(GetDlgItem(hwnd,IDDI_PLOT_COLOR_LINE_BACKGROUND),GWLP_WNDPROC,(ULONG_PTR)Plot::patchPainterProc);

      SetWindowLongPtr(GetDlgItem(hwnd,IDDI_PLOT_COLOR_TOP_BACKGROUND),GWLP_WNDPROC,(ULONG_PTR)Plot::patchPainterProc);

      SetWindowLongPtr(GetDlgItem(hwnd,IDDI_PLOT_COLOR_BOTTOM_BACKGROUND),GWLP_WNDPROC,(ULONG_PTR)Plot::patchPainterProc);

      holdUpdates = TRUE;

      LOAD_VALUES();
 
      holdUpdates = FALSE;

      hwndGroupLine = GetDlgItem(hwnd,IDDI_PLOT_COLOR_GROUP_LINE);
      hwndGroupTop = GetDlgItem(hwnd,IDDI_PLOT_COLOR_GROUP_TOP);
      hwndGroupBottom = GetDlgItem(hwnd,IDDI_PLOT_COLOR_GROUP_BOTTOM);

      hwndHidableGroups[0] = hwndGroupTop;
      hwndHidableGroups[1] = hwndGroupBottom;

      }
      return (LRESULT)0L;

   case WM_DESTROY:
      hwndSampleGraphic = NULL;
      break;

   case WM_SHOWWINDOW:

      if ( ! wParam )
         break;

      if ( lParam )
         break;

      SetParent(hwndSampleGraphic,hwnd);

      gc3DPlotTypes pType3D;

      if ( p -> overrideOwnerType )
         pType3D = (gc3DPlotTypes)p -> plotType3D;
      else
         p -> pOwnerProperty3DPlotType -> get_longValue(reinterpret_cast<long*>(&pType3D));

      RECT rcColors,rcDialog;

      GetWindowRect(hwnd,&rcDialog);
      GetWindowRect(hwndGroupLine,&rcColors);

      long swFlag;

      RECT rcSample;
      VARIANT_BOOL hasSurfaces;

#if 0
      p -> get_PlotTypeHasSurfaces(pType,&hasSurfaces);
#else
{
hasSurfaces = VARIANT_TRUE;
}
Beep(2000,100);
#endif

      if ( VARIANT_TRUE == hasSurfaces ) {

         ShowWindow(hwndGroupTop,SW_SHOW);
         ShowWindow(hwndGroupBottom,SW_SHOW);

         swFlag = SW_SHOW;

         rcSample.left = 16;
         rcSample.top = 2 * (rcColors.bottom - rcDialog.top + 8);
         rcSample.right = rcSample.left + rcDialog.right - rcDialog.left - 32;
         rcSample.bottom = rcSample.top + rcDialog.bottom - rcDialog.top - 2 * (rcColors.bottom - rcDialog.top + 8) - 16;

      } else {

         ShowWindow(hwndGroupTop,SW_HIDE);
         ShowWindow(hwndGroupBottom,SW_HIDE);

         swFlag = SW_HIDE;

         rcSample.left = 16;
         rcSample.top = rcColors.bottom - rcDialog.top + 8;
         rcSample.right = rcSample.left + rcDialog.right - rcDialog.left - 32;
         rcSample.bottom = rcSample.top + rcDialog.bottom - rcDialog.top - (rcColors.bottom - rcDialog.top + 8) - 16;

      }

      SendMessage(hwndSampleGraphic,WMG_POSITION_SAMPLE_GRAPHIC,0L,(LPARAM)&rcSample);

      for ( long k = 0; k < 2; k++ ) {

         std::function<void(HWND)> *pShowChildren = new std::function<void(HWND)>( [=](HWND hwndChild) {
               if ( hwndChild == hwndHidableGroups[k] )
                  return;
               RECT rcThis,rcPane;
               GetWindowRect(hwndChild,&rcThis);
               GetWindowRect(hwndHidableGroups[k],&rcPane);
               if ( rcThis.right < rcPane.left || rcThis.left > rcPane.right || rcThis.bottom < rcPane.top || rcThis.top > rcPane.bottom )
                  return;
               if ( rcThis.left < rcPane.left || rcThis.right > rcPane.right || rcThis.top < rcPane.top || rcThis.bottom > rcPane.bottom )
                  return;
               ShowWindow(hwndChild,swFlag);
               return;
               });

         DoOnWindowDescendants(hwnd,pShowChildren);

      }

      break;

   case WM_NOTIFY: { 

      if ( holdUpdates )
         break;

      NM_UPDOWN *pn = (NM_UPDOWN *)lParam;

      if ( pn -> hdr.code != UDN_DELTAPOS ) 
         break;
   
      if ( 0 == pn -> iDelta )
         break;

      char szTemp[32];
      HWND hwndEdit = (HWND)SendMessage(pn -> hdr.hwndFrom,UDM_GETBUDDY,(WPARAM)0L,(LPARAM)0L);
      sprintf_s(szTemp,32,"%04ld",GetWindowLongPtr(hwndEdit,GWLP_ID));
      szTemp[3] = '\0';
      long backGroundId = 10 * atol(szTemp) + 1;
      HWND hwndBackground = GetDlgItem(GetParent(hwndEdit),backGroundId);

      GetWindowText(hwndEdit,szTemp,32);

      float x = (float)atof(szTemp);

      x += (float)pn -> iDelta * 0.1f;

      if ( x >= 0.0f && x <= 1.0 ) {
         sprintf(szTemp,"%3.1f",x);
         SetWindowText(hwndEdit,szTemp);
         GET_VALUES()
            RECT rect,rect2;
            GetWindowRect(hwndBackground,&rect);
            GetWindowRect(hwnd,&rect2);
            long cx,cy;
            cx = rect.right - rect.left;
            cy = rect.bottom - rect.top;
            rect.left = rect.left - rect2.left;
            rect.top = rect.top - rect2.top;
            rect.right = rect.left + cx;
            rect.bottom = rect.top + cy;
            InvalidateRect(hwnd,&rect,TRUE);
      }

      InvalidateRect(hwndSampleGraphic,NULL,TRUE);

      }
      break;

   case WM_COMMAND: {

      int notifyCode = HIWORD(wParam);

      switch ( LOWORD(wParam) ) {
      case IDDI_PLOT_COLOR_LINE_CHOOSE:
      case IDDI_PLOT_COLOR_TOP_CHOOSE:
      case IDDI_PLOT_COLOR_BOTTOM_CHOOSE: {
 
         IGProperty *pp;
         HWND hwndBackground;
 
         switch ( LOWORD(wParam) ) {
         case IDDI_PLOT_COLOR_LINE_CHOOSE:
            pp = p -> propertyLineColor;
            hwndBackground = GetDlgItem(hwnd,IDDI_PLOT_COLOR_LINE_BACKGROUND);
            break;
         case IDDI_PLOT_COLOR_TOP_CHOOSE:
            pp = p -> propertyTopSurfaceColor; 
            hwndBackground = GetDlgItem(hwnd,IDDI_PLOT_COLOR_TOP_BACKGROUND);
            break;
         case IDDI_PLOT_COLOR_BOTTOM_CHOOSE:
            pp = p -> propertyBottomSurfaceColor;
            hwndBackground = GetDlgItem(hwnd,IDDI_PLOT_COLOR_BOTTOM_BACKGROUND);
            break;
         }
         CHOOSECOLOR ci;
         GET_VALUES();
         float fv[4];
         double dm[9];
         BYTE *pb = (BYTE *)fv;
         pp -> get_binaryValue(4 * sizeof(float),(BYTE**)&pb);
         BYTE vb[3];
         vb[0] = (BYTE)(255.0f*fv[0]);
         vb[1] = (BYTE)(255.0f*fv[1]);
         vb[2] = (BYTE)(255.0f*fv[2]);
         memset(&ci,0,sizeof(ci));
         ci.lStructSize = sizeof(ci);
         ci.hwndOwner = hwnd;
         ci.rgbResult = RGB(vb[0],vb[1],vb[2]);
         pb = (BYTE *)dm;
         p -> propertyCustomColors -> get_binaryValue(9 * sizeof(double),(BYTE**)&pb);
         ci.lpCustColors = (unsigned long *)dm;
         ci.Flags = CC_RGBINIT | CC_SHOWHELP | CC_FULLOPEN;
         if ( ChooseColor(&ci) ) {
            fv[0] = (float)GetRValue(ci.rgbResult) / 255.0f;
            fv[1] = (float)GetGValue(ci.rgbResult) / 255.0f;
            fv[2] = (float)GetBValue(ci.rgbResult) / 255.0f;
            pp -> put_binaryValue(3 * sizeof(float),(BYTE*)fv);
            holdUpdates = TRUE;
            LOAD_VALUES();
            holdUpdates = FALSE;
            RECT rect,rect2;
            long cx,cy;
            GetWindowRect(hwndBackground,&rect);
            GetWindowRect(hwnd,&rect2);
            cx = rect.right - rect.left;
            cy = rect.bottom - rect.top;
            rect.left = rect.left - rect2.left;
            rect.top = rect.top - rect2.top;
            rect.right = rect.left + cx;
            rect.bottom = rect.top + cy;
            InvalidateRect(hwnd,&rect,TRUE);
         }
         }
         break;

      case IDDI_PLOT_COLOR_LINE_RED:
      case IDDI_PLOT_COLOR_LINE_GREEN:
      case IDDI_PLOT_COLOR_LINE_BLUE: 
      case IDDI_PLOT_COLOR_TOP_RED:
      case IDDI_PLOT_COLOR_TOP_GREEN:
      case IDDI_PLOT_COLOR_TOP_BLUE: 
      case IDDI_PLOT_COLOR_BOTTOM_RED:
      case IDDI_PLOT_COLOR_BOTTOM_GREEN:
      case IDDI_PLOT_COLOR_BOTTOM_BLUE: {

         if ( ! ( EN_CHANGE == notifyCode ) )
            return (LRESULT)0L;

         if ( holdUpdates )
            return (LRESULT)0L;

         HWND hwndBackground = NULL;
         switch ( LOWORD(wParam) ) {
         case IDDI_PLOT_COLOR_LINE_RED:
         case IDDI_PLOT_COLOR_LINE_GREEN:
         case IDDI_PLOT_COLOR_LINE_BLUE:
            hwndBackground = GetDlgItem(hwnd,IDDI_PLOT_COLOR_LINE_BACKGROUND);
            break;
         case IDDI_PLOT_COLOR_TOP_RED:
         case IDDI_PLOT_COLOR_TOP_GREEN:
         case IDDI_PLOT_COLOR_TOP_BLUE:
            hwndBackground = GetDlgItem(hwnd,IDDI_PLOT_COLOR_TOP_BACKGROUND);
            break;
         case IDDI_PLOT_COLOR_BOTTOM_RED:
         case IDDI_PLOT_COLOR_BOTTOM_GREEN:
         case IDDI_PLOT_COLOR_BOTTOM_BLUE:
            hwndBackground = GetDlgItem(hwnd,IDDI_PLOT_COLOR_BOTTOM_BACKGROUND);
            break;
         }
 
         GET_VALUES()

         RECT rect,rect2;
         long cx,cy;
         GetWindowRect(hwndBackground,&rect);
         GetWindowRect(hwnd,&rect2);
         cx = rect.right - rect.left;
         cy = rect.bottom - rect.top;
         rect.left = rect.left - rect2.left;
         rect.top = rect.top - rect2.top;
         rect.right = rect.left + cx;
         rect.bottom = rect.top + cy;
         InvalidateRect(hwnd,&rect,TRUE);

         }
         break;
 
      default:
         return (LRESULT)0L;
      }

      InvalidateRect(hwndSampleGraphic,NULL,TRUE);

      }
      return (LRESULT)0L;

 
   default:
      break;
   }
 
   return (LRESULT)0L;
   }
 
 
   LRESULT CALLBACK Plot::patchPainterProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   Plot *p = (Plot *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_PAINT: {

      IGProperty *pp = NULL;

      switch ( GetWindowLongPtr(hwnd,GWLP_ID) ) {
      case IDDI_PLOT_COLOR_LINE_BACKGROUND:
         pp = p -> propertyLineColor;
         break;
      case IDDI_PLOT_COLOR_TOP_BACKGROUND:
         pp = p -> propertyTopSurfaceColor;
         break;
      case IDDI_PLOT_COLOR_BOTTOM_BACKGROUND:
         pp = p -> propertyBottomSurfaceColor;
         break;
      default:
         break;
      }

      float fv[4];
      BYTE *pb = (BYTE *)fv;
      pp -> get_binaryValue(4 * sizeof(long),(BYTE**)&pb);
      BYTE vb[3];
      COLORREF cr;
      vb[0] = (BYTE)(255.0f*fv[0]);
      vb[1] = (BYTE)(255.0f*fv[1]);
      vb[2] = (BYTE)(255.0f*fv[2]);
 
      PAINTSTRUCT ps;
      HBRUSH hb;
      HDC hdc;

      cr = RGB(vb[0],vb[1],vb[2]);
      hb = CreateSolidBrush(cr);
 
      hdc = BeginPaint(hwnd,&ps);
      FillRect(hdc,&ps.rcPaint,hb);
      EndPaint(hwnd,&ps);

      DeleteObject(hb);
 
      }
      return LRESULT(FALSE);
 
   default:
      break;
   }

   return CallWindowProc(Plot::defaultPatchPainter,hwnd,msg,wParam,lParam);
   }