/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002,2008 Nathan T. Clark

*/

#include <windows.h>
#include <stdio.h>
#include <commctrl.h>

#include "utils.h"

#include "Graphic_resource.h"

#include "plot.h"

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

   LRESULT CALLBACK Plot::colorHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   Plot *p = (Plot *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = (PROPSHEETPAGE *)lParam;

      p = (Plot *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      p -> hwndColorSettings = hwnd;

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
 
      if ( ! Plot::defaultPatchPainter ) 
         Plot::defaultPatchPainter = (WNDPROC)SetWindowLongPtr(GetDlgItem(hwnd,IDDI_PLOT_COLOR_LINE_BACKGROUND),GWLP_WNDPROC,(ULONG_PTR)patchPainterProc);
      else
         SetWindowLongPtr(GetDlgItem(hwnd,IDDI_PLOT_COLOR_LINE_BACKGROUND),GWLP_WNDPROC,(ULONG_PTR)Plot::patchPainterProc);

      SetWindowLongPtr(GetDlgItem(hwnd,IDDI_PLOT_COLOR_TOP_BACKGROUND),GWLP_WNDPROC,(ULONG_PTR)Plot::patchPainterProc);

      SetWindowLongPtr(GetDlgItem(hwnd,IDDI_PLOT_COLOR_BOTTOM_BACKGROUND),GWLP_WNDPROC,(ULONG_PTR)Plot::patchPainterProc);
 
      RECT rect,rectParent;
      GetWindowRect(hwnd,&rectParent);
      long cx = 300;
      long cy = 300;
      GetWindowRect(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_SAMPLEPOSITION),&rect);

      SetWindowPos(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_SAMPLEPOSITION),HWND_TOP,rect.left - rectParent.left + 8,rect.top - rectParent.top + 8,
                                       cx - 2 * (rect.left - rectParent.left) - 16,cy - (rect.top - rectParent.top) - 16,SWP_SHOWWINDOW);

      SetWindowLongPtr(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_SAMPLEPOSITION),GWLP_USERDATA,(ULONG_PTR)p);

      if ( defaultStaticWindowHandler )
         SetWindowLongPtr(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_SAMPLEPOSITION),GWLP_WNDPROC,(ULONG_PTR)&Plot::sampleGraphicHandler);
      else
         defaultStaticWindowHandler = (WNDPROC)SetWindowLongPtr(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_SAMPLEPOSITION),GWLP_WNDPROC,(ULONG_PTR)&Plot::sampleGraphicHandler);

      holdUpdates = TRUE;

      LOAD_VALUES();
 
      holdUpdates = FALSE;
 
      }
      return LRESULT(FALSE);
 
   //case WM_SHOWWINDOW: {

   //   if ( (BOOL)wParam ) {

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

   case WM_NOTIFY: { 
      if ( holdUpdates )
         break;
      NM_UPDOWN *pn = (NM_UPDOWN *)lParam;
      if ( pn -> hdr.code != UDN_DELTAPOS ) break;
      if ( pn -> hdr.idFrom >= (unsigned int)IDDI_PLOT_COLOR_LINE_RED_SPIN && pn -> hdr.idFrom <= (unsigned int)IDDI_PLOT_COLOR_BOTTOM_BLUE_SPIN ) {
         int k = pn -> hdr.idFrom - IDDI_PLOT_COLOR_LINE_RED_SPIN;
         HWND hwndEdit,hwndBackground;
         char szTemp[32];
         float x;
         switch ( k ) {
         case 0:
            hwndEdit = GetDlgItem(hwnd,IDDI_PLOT_COLOR_LINE_RED);
            hwndBackground = GetDlgItem(hwnd,IDDI_PLOT_COLOR_LINE_BACKGROUND);
            break;
         case 1:
            hwndEdit = GetDlgItem(hwnd,IDDI_PLOT_COLOR_LINE_GREEN);
            hwndBackground = GetDlgItem(hwnd,IDDI_PLOT_COLOR_LINE_BACKGROUND);
            break;
         case 2:
            hwndEdit = GetDlgItem(hwnd,IDDI_PLOT_COLOR_LINE_BLUE);
            hwndBackground = GetDlgItem(hwnd,IDDI_PLOT_COLOR_LINE_BACKGROUND);
            break;
         case 3:
            hwndEdit = GetDlgItem(hwnd,IDDI_PLOT_COLOR_TOP_RED);
            hwndBackground = GetDlgItem(hwnd,IDDI_PLOT_COLOR_TOP_BACKGROUND);
            break;
         case 4:
            hwndEdit = GetDlgItem(hwnd,IDDI_PLOT_COLOR_TOP_GREEN);
            hwndBackground = GetDlgItem(hwnd,IDDI_PLOT_COLOR_TOP_BACKGROUND);
            break;
         case 5:
            hwndEdit = GetDlgItem(hwnd,IDDI_PLOT_COLOR_TOP_BLUE);
            hwndBackground = GetDlgItem(hwnd,IDDI_PLOT_COLOR_TOP_BACKGROUND);
            break;
         case 6:
            hwndEdit = GetDlgItem(hwnd,IDDI_PLOT_COLOR_BOTTOM_RED);
            hwndBackground = GetDlgItem(hwnd,IDDI_PLOT_COLOR_BOTTOM_BACKGROUND);
            break;
         case 7:
            hwndEdit = GetDlgItem(hwnd,IDDI_PLOT_COLOR_BOTTOM_GREEN);
            hwndBackground = GetDlgItem(hwnd,IDDI_PLOT_COLOR_BOTTOM_BACKGROUND);
            break;
         case 8:
            hwndEdit = GetDlgItem(hwnd,IDDI_PLOT_COLOR_BOTTOM_BLUE);
            hwndBackground = GetDlgItem(hwnd,IDDI_PLOT_COLOR_BOTTOM_BACKGROUND);
            break;
         }
         GetWindowText(hwndEdit,szTemp,32);
         x = (float)atof(szTemp);
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
         
      }
 
      InvalidateRect(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_SAMPLEPOSITION),NULL,TRUE);

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
         switch ( notifyCode ) {
         case EN_CHANGE: {
            if ( ! holdUpdates ) {
               HWND hwndBackground;
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
            }
            break;
 
         default:
            break;
         }   

         }
         break;
 
      default:
         break;
      }

      InvalidateRect(GetDlgItem(hwnd,IDDI_PLOT_DIMENSION_SAMPLEPOSITION),NULL,TRUE);

      }
      return LRESULT(FALSE);

 
   default:
      break;
   }
 
   return LRESULT(FALSE);
   }
 
 
   LRESULT CALLBACK Plot::patchPainterProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   Plot *p = (Plot *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {
   case WM_PAINT: {

      IGProperty *pp = NULL;

      if ( hwnd == GetDlgItem(p -> hwndColorSettings,IDDI_PLOT_COLOR_LINE_BACKGROUND) )
         pp = p -> propertyLineColor;
      else 
         if ( hwnd == GetDlgItem(p -> hwndColorSettings,IDDI_PLOT_COLOR_TOP_BACKGROUND) )
            pp = p -> propertyTopSurfaceColor;
         else 
            if ( hwnd == GetDlgItem(p -> hwndColorSettings,IDDI_PLOT_COLOR_BOTTOM_BACKGROUND) )
               pp = p -> propertyBottomSurfaceColor;

      if ( NULL == pp )
         return CallWindowProc(Plot::defaultPatchPainter,hwnd,msg,wParam,lParam);

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