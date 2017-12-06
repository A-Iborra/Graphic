
#include <windows.h>
#include <commctrl.h>

#include "Graphic.h"

#include "Graphic_resource.h"

   static int holdUpdates = FALSE;

   LRESULT CALLBACK G::backgroundHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   G *p = (G *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);

      p = (G *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      holdUpdates = TRUE;

      SendDlgItemMessage(hwnd,IDDI_LIGHT_BACKGROUND_RED_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_LIGHT_BACKGROUND_GREEN_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_LIGHT_BACKGROUND_BLUE_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
 
      SetWindowLongPtr(GetDlgItem(hwnd,IDDI_LIGHT_BACKGROUND_BACKGROUND),GWLP_USERDATA,(ULONG_PTR)p);
 
      if ( NULL == G::defaultPatchPainter ) 
         G::defaultPatchPainter = (WNDPROC)SetWindowLongPtr(GetDlgItem(hwnd,IDDI_LIGHT_BACKGROUND_BACKGROUND),GWLP_WNDPROC,(ULONG_PTR)G::patchPainterProc);
      else
         SetWindowLongPtr(GetDlgItem(hwnd,IDDI_LIGHT_BACKGROUND_BACKGROUND),GWLP_WNDPROC,(ULONG_PTR)G::patchPainterProc);
 
#define LOAD_VALUES()      {                                                                          \
      char szTemp[32];                                                                                \
      float fv[4];                                                                                    \
      BYTE *pb = (BYTE *)fv;                                                                          \
      p -> propertyBackgroundColor -> get_binaryValue(4 * sizeof(float),(BYTE**)&pb);                 \
      sprintf(szTemp,"%4.1f",fv[0]); SetDlgItemText(hwnd,IDDI_LIGHT_BACKGROUND_RED,szTemp);           \
      sprintf(szTemp,"%4.1f",fv[1]); SetDlgItemText(hwnd,IDDI_LIGHT_BACKGROUND_GREEN,szTemp);         \
      sprintf(szTemp,"%4.1f",fv[2]); SetDlgItemText(hwnd,IDDI_LIGHT_BACKGROUND_BLUE,szTemp);          \
      }
 
 
#define GET_VALUES()       {                                                           \
      float fv[] = {0.0f,0.0f,0.0f,0.0f};                                              \
      char szTemp[32];                                                                 \
      GetDlgItemText(hwnd,IDDI_LIGHT_BACKGROUND_RED,szTemp,32);                        \
      fv[0] = (float)atof(szTemp);                                                     \
      GetDlgItemText(hwnd,IDDI_LIGHT_BACKGROUND_GREEN,szTemp,32);                      \
      fv[1] = (float)atof(szTemp);                                                     \
      GetDlgItemText(hwnd,IDDI_LIGHT_BACKGROUND_BLUE,szTemp,32);                       \
      fv[2] = (float)atof(szTemp);                                                     \
      p -> propertyBackgroundColor -> put_binaryValue(4 * sizeof(float),(BYTE*)fv);    \
      }

      LOAD_VALUES();
 
      holdUpdates = FALSE;

      }
      return LRESULT(FALSE);

   case WM_NOTIFY: {
       NM_UPDOWN *pn = (NM_UPDOWN *)lParam;
       if ( pn -> hdr.code != UDN_DELTAPOS ) break;
       if ( pn -> hdr.idFrom >= (unsigned int)IDDI_LIGHT_BACKGROUND_RED_SPIN && pn -> hdr.idFrom <= (unsigned int)IDDI_LIGHT_BACKGROUND_BLUE_SPIN ) {
          int k = pn -> hdr.idFrom - IDDI_LIGHT_BACKGROUND_RED_SPIN;
          HWND hwndEdit;
          char szTemp[32];
          float x;
          switch ( k ) {
          case 0:
             hwndEdit = GetDlgItem(hwnd,IDDI_LIGHT_BACKGROUND_RED);
             break;
          case 1:
             hwndEdit = GetDlgItem(hwnd,IDDI_LIGHT_BACKGROUND_GREEN);
             break;
          case 2:
             hwndEdit = GetDlgItem(hwnd,IDDI_LIGHT_BACKGROUND_BLUE);
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
             long cx,cy;
             GetWindowRect(GetDlgItem(hwnd,IDDI_LIGHT_BACKGROUND_BACKGROUND),&rect);
             GetWindowRect(hwnd,&rect2);
             cx = rect.right - rect.left;
             cy = rect.bottom - rect.top;
             rect.left = rect.left - rect2.left;
             rect.top = rect.top - rect2.top;
             rect.right = rect.left + cx;
             rect.bottom = rect.top + cy;
             InvalidateRect(hwnd,&rect,TRUE);
          }
          return LRESULT(FALSE);
       }
      }
      break;
 
   case WM_COMMAND: {
      int notifyCode = HIWORD(wParam);
      switch ( LOWORD(wParam) ) {
      case IDNI_GRAPHIC_OK:
      case IDNI_GRAPHIC_APPLY: {
         GET_VALUES();
         }
         return LRESULT(FALSE);
 
      case IDNI_GRAPHIC_CANCEL:
         break;
 
      case IDDI_LIGHT_BACKGROUND_CHOOSE: {
         IGProperty *pp = p -> propertyBackgroundColor;
         CHOOSECOLOR ci;
         GET_VALUES();
         float fv[4];
         BYTE ccData[128];
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
         pb = (BYTE *)ccData;
         p -> propertyCustomColors -> get_binaryValue(128,(BYTE**)&pb);
         ci.lpCustColors = reinterpret_cast<unsigned long *>(ccData);
         ci.Flags = CC_RGBINIT | CC_SHOWHELP | CC_FULLOPEN;
         if ( ChooseColor(&ci) ) {
            fv[0] = (float)GetRValue(ci.rgbResult) / 255.0f;
            fv[1] = (float)GetGValue(ci.rgbResult) / 255.0f;
            fv[2] = (float)GetBValue(ci.rgbResult) / 255.0f;
            pp -> put_binaryValue(4 * sizeof(float),(BYTE*)fv);
            holdUpdates = TRUE;
            LOAD_VALUES()
            holdUpdates = FALSE;
            RECT rect,rect2;
            long cx,cy;
            GetWindowRect(GetDlgItem(hwnd,IDDI_LIGHT_BACKGROUND_BACKGROUND),&rect);
            GetWindowRect(hwnd,&rect2);
            cx = rect.right - rect.left;
            cy = rect.bottom - rect.top;
            rect.left = rect.left - rect2.left;
            rect.top = rect.top - rect2.top;
            rect.right = rect.left + cx;
            rect.bottom = rect.top + cy;
            InvalidateRect(hwnd,&rect,TRUE);
         }
         p -> propertyCustomColors -> put_binaryValue(128,reinterpret_cast<byte *>(ci.lpCustColors));
         }
         return LRESULT(FALSE);
 
      case IDDI_LIGHT_BACKGROUND_RED:
      case IDDI_LIGHT_BACKGROUND_GREEN:
      case IDDI_LIGHT_BACKGROUND_BLUE: {
         switch ( notifyCode ) {
         case EN_CHANGE: {
            if ( ! holdUpdates ) {
               GET_VALUES()
               RECT rect,rect2;
               long cx,cy;
               GetWindowRect(GetDlgItem(hwnd,IDDI_LIGHT_BACKGROUND_BACKGROUND),&rect);
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
         return LRESULT(FALSE);
 
      default:
         break;
      }
      }
      return LRESULT(FALSE);

   default:
      break;
   }
   return LRESULT(FALSE);
   }
 