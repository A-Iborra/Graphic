// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Axis.h"
#include <CommCtrl.h>

#include "utils.h"
#include "Graphic_resource.h"
 
#define GET_VALUES(prop,redindex)    {               \
      float v[] = {0.0f,0.0f,0.0f,0.0f};             \
      char szTemp[32];                               \
                                                     \
      GetDlgItemText(hwnd,redindex,szTemp,32);       \
      v[0] = (float)atof(szTemp);                    \
      GetDlgItemText(hwnd,redindex + 1,szTemp,32);   \
      v[1] = (float)atof(szTemp);                    \
      GetDlgItemText(hwnd,redindex + 2,szTemp,32);   \
      v[2] = (float)atof(szTemp);                    \
      prop -> put_binaryValue(sizeof(v),(BYTE*)v);   \
      }
 
#define LOAD_VALUES(prop,redindex)     {             \
      float v[4];                                    \
      char szTemp[32];                               \
                                                     \
      BYTE *pb = (BYTE *)v;                          \
      prop -> get_binaryValue(sizeof(v),(BYTE**)&pb); \
      sprintf(szTemp,"%3.2f",v[0]); SetDlgItemText(hwnd,redindex,szTemp);      \
      sprintf(szTemp,"%3.2f",v[1]); SetDlgItemText(hwnd,redindex + 1,szTemp);  \
      sprintf(szTemp,"%3.2f",v[2]); SetDlgItemText(hwnd,redindex + 2,szTemp);  \
      }


   LRESULT CALLBACK Axis::colorHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   Axis *p = (Axis *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);

      p = (Axis *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      p -> hwndColorSettings = hwnd;

      SendDlgItemMessage(hwnd,IDDI_AXIS_COLOR_LINE_COLOR_RED_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_AXIS_COLOR_LINE_COLOR_GREEN_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_AXIS_COLOR_LINE_COLOR_BLUE_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
 
      SetWindowLongPtr(GetDlgItem(hwnd,IDDI_AXIS_COLOR_LINE_COLOR_BACKGROUND),GWLP_USERDATA,(ULONG_PTR)p);

      if ( NULL == p -> defaultPatchPainter ) 
         p -> defaultPatchPainter = (WNDPROC)SetWindowLongPtr(GetDlgItem(hwnd,IDDI_AXIS_COLOR_LINE_COLOR_BACKGROUND),GWLP_WNDPROC,(ULONG_PTR)patchPainterProc);
      else
         SetWindowLongPtr(GetDlgItem(hwnd,IDDI_AXIS_COLOR_LINE_COLOR_BACKGROUND),GWLP_WNDPROC,(ULONG_PTR)patchPainterProc);
 
      LOAD_VALUES(p -> propertyLineColor,IDDI_AXIS_COLOR_LINE_COLOR_RED);
 
      SendDlgItemMessage(hwnd,IDDI_AXIS_COLOR_TICK_LABEL_COLOR_RED_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_AXIS_COLOR_TICK_LABEL_COLOR_GREEN_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_AXIS_COLOR_TICK_LABEL_COLOR_BLUE_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
 
      SetWindowLongPtr(GetDlgItem(hwnd,IDDI_AXIS_COLOR_TICK_LABEL_COLOR_BACKGROUND),GWLP_USERDATA,(ULONG_PTR)p);

      SetWindowLongPtr(GetDlgItem(hwnd,IDDI_AXIS_COLOR_TICK_LABEL_COLOR_BACKGROUND),GWLP_WNDPROC,(ULONG_PTR)patchPainterProc);

      if ( p -> currentPropertyTickLabelColor ) 
         LOAD_VALUES(p -> currentPropertyTickLabelColor,IDDI_AXIS_COLOR_TICK_LABEL_COLOR_RED);

      SendDlgItemMessage(hwnd,IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_RED_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_GREEN_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_BLUE_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
 
      SetWindowLongPtr(GetDlgItem(hwnd,IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_BACKGROUND),GWLP_USERDATA,(ULONG_PTR)p);

      SetWindowLongPtr(GetDlgItem(hwnd,IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_BACKGROUND),GWLP_WNDPROC,(ULONG_PTR)patchPainterProc);

      if ( p -> currentPropertyAxisLabelColor )
         LOAD_VALUES(p -> currentPropertyAxisLabelColor,IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_RED);
   
      if ( p -> axisTickColorTrackLineColor )
         p -> currentPropertyTickLabelColor = p -> propertyLineColor;
      else 
         p -> currentPropertyTickLabelColor = p -> propertyTickLabelColor;

      if ( p -> axisLabelColorTrackLineColor )
         p -> currentPropertyAxisLabelColor = p -> propertyLineColor;
      else
         p -> currentPropertyAxisLabelColor = p -> propertyLabelColor;

      }
      return (LRESULT)FALSE;
 
   case WM_PAINT: {

      p -> propertyTickLabelColorTrackLineColor -> setWindowItemChecked(p -> hwndColorSettings,IDDI_AXIS_COLOR_TICK_LABEL_COLOR_TRACK);

      EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_TICK_LABEL_COLOR_TRACK),TRUE);
      EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_TICK_LABEL_COLOR_CHOOSE),! p -> axisTickColorTrackLineColor);
      EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_TICK_LABEL_COLOR_RED),! p -> axisTickColorTrackLineColor);
      EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_TICK_LABEL_COLOR_RED_SPIN),! p -> axisTickColorTrackLineColor);
      EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_TICK_LABEL_COLOR_GREEN),! p -> axisTickColorTrackLineColor);
      EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_TICK_LABEL_COLOR_GREEN_SPIN),! p -> axisTickColorTrackLineColor);
      EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_TICK_LABEL_COLOR_BLUE),! p -> axisTickColorTrackLineColor);
      EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_TICK_LABEL_COLOR_BLUE_SPIN),! p -> axisTickColorTrackLineColor);

      p -> propertyAxisLabelColorTrackLineColor -> setWindowItemChecked(p -> hwndColorSettings,IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_TRACK);

      EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_TRACK),TRUE);
      EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_CHOOSE),! p -> axisLabelColorTrackLineColor);
      EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_RED),! p -> axisLabelColorTrackLineColor);
      EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_RED_SPIN),! p -> axisLabelColorTrackLineColor);
      EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_GREEN),! p -> axisLabelColorTrackLineColor);
      EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_GREEN_SPIN),! p -> axisLabelColorTrackLineColor);
      EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_BLUE),! p -> axisLabelColorTrackLineColor);
      EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_BLUE_SPIN),! p -> axisLabelColorTrackLineColor);
      }
      break;

   case WM_NOTIFY: {

      NMHDR *pNotifyHeader = (NMHDR *)lParam;

      switch ( pNotifyHeader -> code ) {

      case PSN_APPLY: {

         PSHNOTIFY *pNotify = (PSHNOTIFY *)lParam;

         if ( pNotify -> lParam ) {
            p -> pIGProperties -> Pop();
            p -> pIGProperties -> Pop();
         } else {
            p -> pIGProperties -> Save();
            p -> pIGProperties -> Discard();
            p -> pIGProperties -> Discard();
         }

         p -> Draw();

         SetWindowLongPtr(hwnd,DWLP_MSGRESULT,PSNRET_NOERROR);

         }
         break;

      case UDN_DELTAPOS: {

         NM_UPDOWN *pn = (NM_UPDOWN *)lParam;

         int redIndex;
         int backgroundIndex;
         int spinIndex;
         if ( pn -> hdr.idFrom >= (unsigned int)IDDI_AXIS_COLOR_LINE_COLOR_RED_SPIN && pn -> hdr.idFrom <= (unsigned int)IDDI_AXIS_COLOR_LINE_COLOR_BLUE_SPIN ) {
            redIndex = IDDI_AXIS_COLOR_LINE_COLOR_RED;
            backgroundIndex = IDDI_AXIS_COLOR_LINE_COLOR_BACKGROUND;
            spinIndex = IDDI_AXIS_COLOR_LINE_COLOR_RED_SPIN;
         } else {
            if ( pn -> hdr.idFrom >= (unsigned int)IDDI_AXIS_COLOR_TICK_LABEL_COLOR_RED_SPIN && pn -> hdr.idFrom <= (unsigned int)IDDI_AXIS_COLOR_TICK_LABEL_COLOR_BLUE_SPIN ) {
               redIndex = IDDI_AXIS_COLOR_TICK_LABEL_COLOR_RED;
               backgroundIndex = IDDI_AXIS_COLOR_TICK_LABEL_COLOR_RED;
               spinIndex = IDDI_AXIS_COLOR_TICK_LABEL_COLOR_RED_SPIN;
            } else {
               if ( pn -> hdr.idFrom >= (unsigned int)IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_RED_SPIN && pn -> hdr.idFrom <= (unsigned int)IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_BLUE_SPIN ) {
                  redIndex = IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_RED;
                  backgroundIndex = IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_RED;
                  spinIndex = IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_RED_SPIN;
               } 
               else
                  break;
            }
         }

         int k = (int)pn -> hdr.idFrom - spinIndex;
         HWND hwndEdit;
         char szTemp[32];
         float x;
         switch ( k ) {
         case 0:
            hwndEdit = GetDlgItem(hwnd,redIndex);
            break;
         case 1:
            hwndEdit = GetDlgItem(hwnd,redIndex + 1);
            break;
         case 2:
            hwndEdit = GetDlgItem(hwnd,redIndex + 2);
            break;
         }

         GetWindowText(hwndEdit,szTemp,32);

         x = (float)atof(szTemp);
         x += (float)pn -> iDelta * 0.1f;
         if ( x >= 0.0f && x <= 1.0 ) {
            sprintf(szTemp,"%3.2f",x);
            SetWindowText(hwndEdit,szTemp);
         }

         }
         return LRESULT(FALSE);

      }

      }
      break;
 
   case WM_COMMAND: {

      HWND hwndControl = (HWND)lParam;
      int controlID = LOWORD(wParam);
      int notificationCode = HIWORD(wParam);

      switch ( controlID ) {

      case IDDI_AXIS_COLOR_LINE_COLOR_CHOOSE:
      case IDDI_AXIS_COLOR_TICK_LABEL_COLOR_CHOOSE:
      case IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_CHOOSE: {

         IGProperty *prop;
         int redIndex;
         int backgroundIndex;

         if ( controlID == IDDI_AXIS_COLOR_LINE_COLOR_CHOOSE ) {
            prop = p -> propertyLineColor;
            redIndex = IDDI_AXIS_COLOR_LINE_COLOR_RED;
            backgroundIndex = IDDI_AXIS_COLOR_LINE_COLOR_BACKGROUND;
         } else {
            if ( controlID == IDDI_AXIS_COLOR_TICK_LABEL_COLOR_CHOOSE ) {
               prop = p -> currentPropertyTickLabelColor;
               redIndex = IDDI_AXIS_COLOR_TICK_LABEL_COLOR_RED;
               backgroundIndex = IDDI_AXIS_COLOR_TICK_LABEL_COLOR_BACKGROUND;
            } else {
               if ( controlID == IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_CHOOSE ) {
                  prop = p -> currentPropertyAxisLabelColor;
                  redIndex = IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_RED;
                  backgroundIndex = IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_BACKGROUND;
               }
               else 
                  break;
            }
         }
 
         CHOOSECOLOR ci = {0};
         GET_VALUES(prop,redIndex);
         float fv[4];
         double dm[9];
         BYTE *pb = (BYTE *)fv;
         prop -> get_binaryValue(sizeof(fv),(BYTE**)&pb);
         BYTE vb[3];
         vb[0] = (BYTE)(255.0f*fv[0]);
         vb[1] = (BYTE)(255.0f*fv[1]);
         vb[2] = (BYTE)(255.0f*fv[2]);
         memset(&ci,0,sizeof(ci));
         ci.lStructSize = sizeof(ci);
         ci.hwndOwner = hwnd;
         ci.rgbResult = RGB(vb[0],vb[1],vb[2]);
         pb = (BYTE *)dm;
         prop -> get_binaryValue(9 * sizeof(double),(BYTE**)&pb);
         ci.lpCustColors = (unsigned long *)dm;
         ci.Flags = CC_RGBINIT | CC_SHOWHELP | CC_FULLOPEN;
         if ( ChooseColor(&ci) ) {
            fv[0] = (float)GetRValue(ci.rgbResult) / 255.0f;
            fv[1] = (float)GetGValue(ci.rgbResult) / 255.0f;
            fv[2] = (float)GetBValue(ci.rgbResult) / 255.0f;
            prop -> put_binaryValue(sizeof(fv),(BYTE*)fv);
            LOAD_VALUES(prop,redIndex);
            if ( controlID == IDDI_AXIS_COLOR_LINE_COLOR_CHOOSE ) {
               if ( p -> axisTickColorTrackLineColor ) {
                  LOAD_VALUES(p -> currentPropertyTickLabelColor,IDDI_AXIS_COLOR_TICK_LABEL_COLOR_RED);
               }
               if ( p -> axisLabelColorTrackLineColor ) {
                  LOAD_VALUES(p -> currentPropertyAxisLabelColor,IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_RED);
               }
            }
            InvalidateRect(hwnd,NULL,TRUE);
            UpdateWindow(hwnd);
         }
         }
         return LRESULT(0);

      case IDDI_AXIS_COLOR_LINE_COLOR_RED:
      case IDDI_AXIS_COLOR_LINE_COLOR_GREEN:
      case IDDI_AXIS_COLOR_LINE_COLOR_BLUE:
      case IDDI_AXIS_COLOR_TICK_LABEL_COLOR_RED:
      case IDDI_AXIS_COLOR_TICK_LABEL_COLOR_GREEN:
      case IDDI_AXIS_COLOR_TICK_LABEL_COLOR_BLUE:
      case IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_RED:
      case IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_GREEN:
      case IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_BLUE: {
         if ( notificationCode == EN_CHANGE ) {
            IGProperty *prop = NULL;
            int redIndex;
            int backgroundIndex;
            if ( controlID >= IDDI_AXIS_COLOR_LINE_COLOR_RED && controlID <= IDDI_AXIS_COLOR_LINE_COLOR_BLUE ) {
               prop = p -> propertyLineColor;
               redIndex = IDDI_AXIS_COLOR_LINE_COLOR_RED;
               backgroundIndex = IDDI_AXIS_COLOR_LINE_COLOR_BACKGROUND;
               if ( p -> axisTickColorTrackLineColor ) {
                  char szColor[32];
                  GetWindowText(hwndControl,szColor,32);
                  SendMessage(GetDlgItem(hwnd,IDDI_AXIS_COLOR_TICK_LABEL_COLOR_RED + (controlID - IDDI_AXIS_COLOR_LINE_COLOR_RED)),WM_SETTEXT,0L,(WPARAM)szColor);
               }
               if ( p -> axisLabelColorTrackLineColor ) {
                  char szColor[32];
                  GetWindowText(hwndControl,szColor,32);
                  SendMessage(GetDlgItem(hwnd,IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_RED + (controlID - IDDI_AXIS_COLOR_LINE_COLOR_RED)),WM_SETTEXT,0L,(WPARAM)szColor);
               }
            } else {
               if ( controlID >= IDDI_AXIS_COLOR_TICK_LABEL_COLOR_RED && controlID <= IDDI_AXIS_COLOR_TICK_LABEL_COLOR_BLUE ) {
                  prop = p -> currentPropertyTickLabelColor;
                  redIndex = IDDI_AXIS_COLOR_TICK_LABEL_COLOR_RED;
                  backgroundIndex = IDDI_AXIS_COLOR_TICK_LABEL_COLOR_BACKGROUND;
               } else {
                  if ( controlID >= IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_RED && controlID <= IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_BLUE ) {
                     prop = p -> currentPropertyAxisLabelColor;
                     redIndex = IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_RED;
                     backgroundIndex = IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_BACKGROUND;
                  }
                  else
                     return LRESULT(0);
               }
            }
            if ( ! prop )
               break;

            GET_VALUES(prop,redIndex)
InvalidateRect(hwnd,NULL,FALSE);
UpdateWindow(hwnd);
#if 0
            RECT rect,rect2;
            long cx,cy;
            GetWindowRect(GetDlgItem(hwnd,backgroundIndex),&rect);
            GetWindowRect(hwnd,&rect2);
            cx = rect.right - rect.left;
            cy = rect.bottom - rect.top;
            rect.left = rect.left - rect2.left;
            rect.top = rect.top - rect2.top;
            rect.right = rect.left + cx;
            rect.bottom = rect.top + cy;
            InvalidateRect(GetDlgItem(hwnd,backgroundIndex),&rect,TRUE);
            UpdateWindow(GetDlgItem(hwnd,backgroundIndex));
#endif
         }
         }
         return LRESULT(0);
 
      case IDDI_AXIS_COLOR_TICK_LABEL_COLOR_TRACK:
         p -> propertyTickLabelColorTrackLineColor -> getWindowChecked(hwndControl);
         EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_TICK_LABEL_COLOR_CHOOSE),p -> drawTickLabels ? ! p -> axisTickColorTrackLineColor  : FALSE);
         EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_TICK_LABEL_COLOR_RED),p -> drawTickLabels ? ! p -> axisTickColorTrackLineColor  : FALSE);
         EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_TICK_LABEL_COLOR_RED_SPIN),p -> drawTickLabels ? ! p -> axisTickColorTrackLineColor  : FALSE);
         EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_TICK_LABEL_COLOR_GREEN),p -> drawTickLabels ? ! p -> axisTickColorTrackLineColor  : FALSE);
         EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_TICK_LABEL_COLOR_GREEN_SPIN),p -> drawTickLabels ? ! p -> axisTickColorTrackLineColor  : FALSE);
         EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_TICK_LABEL_COLOR_BLUE),p -> drawTickLabels ? ! p -> axisTickColorTrackLineColor  : FALSE);
         EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_TICK_LABEL_COLOR_BLUE_SPIN),p -> drawTickLabels ? ! p -> axisTickColorTrackLineColor  : FALSE);
         if ( p -> axisTickColorTrackLineColor ) {
            p -> currentPropertyTickLabelColor = p -> propertyLineColor;
         } else {
            p -> currentPropertyTickLabelColor = p -> propertyTickLabelColor;
         }
         LOAD_VALUES(p -> currentPropertyTickLabelColor,IDDI_AXIS_COLOR_TICK_LABEL_COLOR_RED);
         break;

      case IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_TRACK: {
         p -> propertyAxisLabelColorTrackLineColor -> getWindowChecked(hwndControl);
         BOOL drawAxisLabel = (S_OK == p -> pLabel -> HasContent());
         EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_CHOOSE),drawAxisLabel ? ! p -> axisLabelColorTrackLineColor  : FALSE);
         EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_RED),drawAxisLabel ? ! p -> axisLabelColorTrackLineColor  : FALSE);
         EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_RED_SPIN),drawAxisLabel ? ! p -> axisLabelColorTrackLineColor  : FALSE);
         EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_GREEN),drawAxisLabel ? ! p -> axisLabelColorTrackLineColor  : FALSE);
         EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_GREEN_SPIN),drawAxisLabel ? ! p -> axisLabelColorTrackLineColor  : FALSE);
         EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_BLUE),drawAxisLabel ? ! p -> axisLabelColorTrackLineColor  : FALSE);
         EnableWindow(GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_BLUE_SPIN),drawAxisLabel ? ! p -> axisLabelColorTrackLineColor  : FALSE);
         if ( p -> axisLabelColorTrackLineColor ) {
            p -> currentPropertyAxisLabelColor = p -> propertyLineColor;
         } else {
            p -> currentPropertyAxisLabelColor = p -> propertyLabelColor;
         }
         LOAD_VALUES(p -> currentPropertyAxisLabelColor,IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_RED);
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
 
   return LRESULT(FALSE);
   }
 
 
 
   LRESULT CALLBACK Axis::patchPainterProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
   Axis *p = (Axis *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
   switch ( msg ) {
   case WM_PAINT: {
      PAINTSTRUCT ps;
      HBRUSH hb;
      HDC hdc;
      float fv[4];
      BYTE *pb = (BYTE *)fv;
      if ( hwnd == GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_LINE_COLOR_BACKGROUND) ) {
         p -> propertyLineColor -> get_binaryValue(sizeof(fv),(BYTE**)&pb);
      } else {
         if ( hwnd == GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_TICK_LABEL_COLOR_BACKGROUND) ) {
            p -> currentPropertyTickLabelColor -> get_binaryValue(sizeof(fv),(BYTE**)&pb);
         } else {
            if ( hwnd == GetDlgItem(p -> hwndColorSettings,IDDI_AXIS_COLOR_AXIS_LABEL_COLOR_BACKGROUND) ) {
               p -> currentPropertyAxisLabelColor -> get_binaryValue(sizeof(fv),(BYTE**)&pb);
            }
         }
      }
      BYTE vb[3];
      COLORREF cr;
      vb[0] = (BYTE)(255.0f*fv[0]);
      vb[1] = (BYTE)(255.0f*fv[1]);
      vb[2] = (BYTE)(255.0f*fv[2]);
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
   return CallWindowProc(p -> defaultPatchPainter,hwnd,msg,wParam,lParam);
   }