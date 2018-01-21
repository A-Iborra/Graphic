// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"

#include "list.cpp"
#include "utils.h"

#define ENABLE_WINDOWS(enabled) {                                                        \
         EnableWindow(GetDlgItem(hwnd,IDDI_LIGHT_AMBIENT_RED),enabled);                  \
         EnableWindow(GetDlgItem(hwnd,IDDI_LIGHT_AMBIENT_GREEN),enabled);                \
         EnableWindow(GetDlgItem(hwnd,IDDI_LIGHT_AMBIENT_BLUE),enabled);                 \
         EnableWindow(GetDlgItem(hwnd,IDDI_LIGHT_AMBIENT_RED_SPIN),enabled);             \
         EnableWindow(GetDlgItem(hwnd,IDDI_LIGHT_AMBIENT_GREEN_SPIN),enabled);           \
         EnableWindow(GetDlgItem(hwnd,IDDI_LIGHT_AMBIENT_BLUE_SPIN),enabled);            \
         EnableWindow(GetDlgItem(hwnd,IDDI_LIGHT_AMBIENT_CHOOSE),enabled);               \
         EnableWindow(GetDlgItem(hwnd,IDDI_LIGHT_DIFFUSE_RED),enabled);                  \
         EnableWindow(GetDlgItem(hwnd,IDDI_LIGHT_DIFFUSE_GREEN),enabled);                \
         EnableWindow(GetDlgItem(hwnd,IDDI_LIGHT_DIFFUSE_BLUE),enabled);                 \
         EnableWindow(GetDlgItem(hwnd,IDDI_LIGHT_DIFFUSE_RED_SPIN),enabled);             \
         EnableWindow(GetDlgItem(hwnd,IDDI_LIGHT_DIFFUSE_GREEN_SPIN),enabled);           \
         EnableWindow(GetDlgItem(hwnd,IDDI_LIGHT_DIFFUSE_BLUE_SPIN),enabled);            \
         EnableWindow(GetDlgItem(hwnd,IDDI_LIGHT_DIFFUSE_CHOOSE),enabled);               \
         EnableWindow(GetDlgItem(hwnd,IDDI_LIGHT_SPECULAR_RED),enabled);                 \
         EnableWindow(GetDlgItem(hwnd,IDDI_LIGHT_SPECULAR_GREEN),enabled);               \
         EnableWindow(GetDlgItem(hwnd,IDDI_LIGHT_SPECULAR_BLUE),enabled);                \
         EnableWindow(GetDlgItem(hwnd,IDDI_LIGHT_SPECULAR_RED_SPIN),enabled);            \
         EnableWindow(GetDlgItem(hwnd,IDDI_LIGHT_SPECULAR_GREEN_SPIN),enabled);          \
         EnableWindow(GetDlgItem(hwnd,IDDI_LIGHT_SPECULAR_BLUE_SPIN),enabled);           \
         EnableWindow(GetDlgItem(hwnd,IDDI_LIGHT_SPECULAR_CHOOSE),enabled);              \
         EnableWindow(GetDlgItem(hwnd,IDDI_LIGHT_POS_X),enabled);                        \
         EnableWindow(GetDlgItem(hwnd,IDDI_LIGHT_POS_Y),enabled);                        \
         EnableWindow(GetDlgItem(hwnd,IDDI_LIGHT_POS_Z),enabled);                        \
      } 
                                                                                        
#define GET_VALUES(k)  {                                                                                 \
      float fv[4];                                                                                       \
      p -> ppPropertyLightOn[k] -> getWindowItemChecked(hwnd,IDDI_LIGHT_ENABLED);                        \
      char szTemp[32];                                                                                   \
      GetDlgItemText(hwnd,IDDI_LIGHT_AMBIENT_RED,szTemp,32); fv[0] = (float)atof(szTemp);                \
      GetDlgItemText(hwnd,IDDI_LIGHT_AMBIENT_GREEN,szTemp,32); fv[1] = (float)atof(szTemp);              \
      GetDlgItemText(hwnd,IDDI_LIGHT_AMBIENT_BLUE,szTemp,32); fv[2] = (float)atof(szTemp);               \
      fv[3] = 1.0;                                                                                       \
      p -> ppPropertyAmbientLight[k] -> put_binaryValue(4 * sizeof(float),(BYTE*)fv);                    \
      GetDlgItemText(hwnd,IDDI_LIGHT_DIFFUSE_RED,szTemp,32); fv[0] = (float)atof(szTemp);                \
      GetDlgItemText(hwnd,IDDI_LIGHT_DIFFUSE_GREEN,szTemp,32); fv[1] = (float)atof(szTemp);              \
      GetDlgItemText(hwnd,IDDI_LIGHT_DIFFUSE_BLUE,szTemp,32); fv[2] = (float)atof(szTemp);               \
      p -> ppPropertyDiffuseLight[k] -> put_binaryValue(4 * sizeof(float),(BYTE*)fv);                    \
      GetDlgItemText(hwnd,IDDI_LIGHT_SPECULAR_RED,szTemp,32); fv[0] = (float)atof(szTemp);               \
      GetDlgItemText(hwnd,IDDI_LIGHT_SPECULAR_GREEN,szTemp,32); fv[1] = (float)atof(szTemp);             \
      GetDlgItemText(hwnd,IDDI_LIGHT_SPECULAR_BLUE,szTemp,32); fv[2] = (float)atof(szTemp);              \
      p -> ppPropertySpecularLight[k] -> put_binaryValue(4 * sizeof(float),(BYTE*)fv);                   \
      char szPosition[96];                                                                               \
      long cb = 0;                                                                                       \
      GetDlgItemText(hwnd,IDDI_LIGHT_POS_X,szTemp,32);                                                   \
      cb = sprintf(szPosition,"%s,",szTemp);                                                             \
      GetDlgItemText(hwnd,IDDI_LIGHT_POS_Y,szTemp,32);                                                   \
      cb += sprintf(&szPosition[cb],"%s,",szTemp);                                                       \
      GetDlgItemText(hwnd,IDDI_LIGHT_POS_Z,szTemp,32);                                                   \
      sprintf(&szPosition[cb],"%s",szTemp);                                                              \
      p -> ppPropertyLightPos[k] -> put_szValue(szPosition);                                             \
      }                                                 
                                                                                         
#define SET_WINDOWS(k,noUpdate)                                                                          \
      {                                                                                                  \
      SHORT v;                                                                                           \
      float fv[4];                                                                                       \
      char szTemp[32];                                                                                   \
      p -> ppPropertyLightOn[k] -> get_boolValue(&v);                                                    \
      p -> ppPropertyLightOn[k] -> setWindowItemChecked(hwnd,IDDI_LIGHT_ENABLED);                        \
      if ( v )  {                                                                                        \
         ENABLE_WINDOWS(TRUE);                                                                           \
      } else {                                                                                           \
         ENABLE_WINDOWS(FALSE);                                                                          \
      }                                                                                                  \
      sprintf(szTemp,"Light # %d",k + 1);                                                                \
      SetDlgItemText(hwnd,IDDI_LIGHT_ENABLED_LABEL,szTemp);                                              \
      BYTE *pb = (BYTE *)fv;                                                                             \
      p -> ppPropertyAmbientLight[k] -> get_binaryValue(4 * sizeof(float),(BYTE**)&pb);                  \
      sprintf(szTemp,"%4.1f",fv[0]); SetDlgItemText(hwnd,IDDI_LIGHT_AMBIENT_RED,szTemp);                 \
      sprintf(szTemp,"%4.1f",fv[1]); SetDlgItemText(hwnd,IDDI_LIGHT_AMBIENT_GREEN,szTemp);               \
      sprintf(szTemp,"%4.1f",fv[2]); SetDlgItemText(hwnd,IDDI_LIGHT_AMBIENT_BLUE,szTemp);                \
      p -> ppPropertyDiffuseLight[k] -> get_binaryValue(4 * sizeof(float),(BYTE**)&pb);                  \
      sprintf(szTemp,"%4.1f",fv[0]); SetDlgItemText(hwnd,IDDI_LIGHT_DIFFUSE_RED,szTemp);                 \
      sprintf(szTemp,"%4.1f",fv[1]); SetDlgItemText(hwnd,IDDI_LIGHT_DIFFUSE_GREEN,szTemp);               \
      sprintf(szTemp,"%4.1f",fv[2]); SetDlgItemText(hwnd,IDDI_LIGHT_DIFFUSE_BLUE,szTemp);                \
      p -> ppPropertySpecularLight[k] -> get_binaryValue(4 * sizeof(float),(BYTE**)&pb);                 \
      sprintf(szTemp,"%4.1f",fv[0]); SetDlgItemText(hwnd,IDDI_LIGHT_SPECULAR_RED,szTemp);                \
      sprintf(szTemp,"%4.1f",fv[1]); SetDlgItemText(hwnd,IDDI_LIGHT_SPECULAR_GREEN,szTemp);              \
      sprintf(szTemp,"%4.1f",fv[2]); SetDlgItemText(hwnd,IDDI_LIGHT_SPECULAR_BLUE,szTemp);               \
      long n;                                                                                            \
      p -> ppPropertyLightPos[k] -> get_size(&n);                                                        \
      char *pszTemp = new char[n];                                                                       \
      p -> ppPropertyLightPos[k] -> get_szValue(pszTemp);                                                \
      char *pp = strtok(pszTemp,",");                                                                    \
      double dValue;                                                                                     \
      if ( pp ) {                                                                                        \
         if ( ! ( noUpdate == IDDI_LIGHT_POS_X ) )                                                       \
            SetDlgItemText(hwnd,IDDI_LIGHT_POS_X,pp);                                                    \
         p -> pIEvaluator -> Evaluate_szExpression(pp,&dValue);                                          \
         sprintf_s(szPosition,64,szPositionFormat,dValue);                                               \
         SetDlgItemText(hwnd,IDDI_LIGHT_POS_X_EVALED,szPosition);                                        \
         ShowWindow(GetDlgItem(hwnd,IDDI_LIGHT_POS_X_EVALED),SW_SHOW);                                   \
         pp = strtok(NULL,",");                                                                          \
         if ( pp ) {                                                                                     \
            if ( ! ( noUpdate == IDDI_LIGHT_POS_Y ) )                                                    \
               SetDlgItemText(hwnd,IDDI_LIGHT_POS_Y,pp);                                                 \
            p -> pIEvaluator -> Evaluate_szExpression(pp,&dValue);                                       \
            sprintf_s(szPosition,64,szPositionFormat,dValue);                                            \
            SetDlgItemText(hwnd,IDDI_LIGHT_POS_Y_EVALED,szPosition);                                     \
            ShowWindow(GetDlgItem(hwnd,IDDI_LIGHT_POS_Y_EVALED),SW_SHOW);                                \
            pp = strtok(NULL,",");                                                                       \
            if ( pp ) {                                                                                  \
               if ( ! ( noUpdate == IDDI_LIGHT_POS_Z ) )                                                 \
                  SetDlgItemText(hwnd,IDDI_LIGHT_POS_Z,pp);                                              \
               p -> pIEvaluator -> Evaluate_szExpression(pp,&dValue);                                    \
               sprintf_s(szPosition,64,szPositionFormat,dValue);                                         \
               SetDlgItemText(hwnd,IDDI_LIGHT_POS_Z_EVALED,szPosition);                                  \
               ShowWindow(GetDlgItem(hwnd,IDDI_LIGHT_POS_Z_EVALED),SW_SHOW);                             \
            } else {                                                                                     \
               SetDlgItemText(hwnd,IDDI_LIGHT_POS_Z,"0.0");                                              \
               ShowWindow(GetDlgItem(hwnd,IDDI_LIGHT_POS_Z_EVALED),SW_HIDE);                             \
            }                                                                                            \
         } else {                                                                                        \
            SetDlgItemText(hwnd,IDDI_LIGHT_POS_Y,"0.0");                                                 \
            SetDlgItemText(hwnd,IDDI_LIGHT_POS_Z,"0.0");                                                 \
            ShowWindow(GetDlgItem(hwnd,IDDI_LIGHT_POS_Y_EVALED),SW_HIDE);                                \
            ShowWindow(GetDlgItem(hwnd,IDDI_LIGHT_POS_Z_EVALED),SW_HIDE);                                \
         }                                                                                               \
      } else {                                                                                           \
         SetDlgItemText(hwnd,IDDI_LIGHT_POS_X,"0.0");                                                    \
         SetDlgItemText(hwnd,IDDI_LIGHT_POS_Y,"0.0");                                                    \
         SetDlgItemText(hwnd,IDDI_LIGHT_POS_Z,"0.0");                                                    \
         ShowWindow(GetDlgItem(hwnd,IDDI_LIGHT_POS_X_EVALED),SW_HIDE);                                   \
         ShowWindow(GetDlgItem(hwnd,IDDI_LIGHT_POS_Y_EVALED),SW_HIDE);                                   \
         ShowWindow(GetDlgItem(hwnd,IDDI_LIGHT_POS_Z_EVALED),SW_HIDE);                                   \
      }                                                                                                  \
                                                                                                         \
      p -> ppPropertyLightOn[k] -> setWindowItemChecked(hwnd,IDDI_LIGHT_ENABLED);                        \
      delete [] pszTemp;                                                                                 \
      }                                                                                                  
 
   static int holdUpdates = FALSE;
 
   char szPosition[64];
   char szPositionFormat[64];

   LRESULT CALLBACK G::lightingHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   G *p = (G *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = (PROPSHEETPAGE *)lParam;
      p = (G *)pPage -> lParam;
      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      p -> hwndLightingSettings = hwnd;

      holdUpdates = TRUE;
 
      SendDlgItemMessage(hwnd,IDDI_LIGHT_AMBIENT_RED_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_LIGHT_AMBIENT_GREEN_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_LIGHT_AMBIENT_BLUE_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_LIGHT_DIFFUSE_RED_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_LIGHT_DIFFUSE_GREEN_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_LIGHT_DIFFUSE_BLUE_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_LIGHT_SPECULAR_RED_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_LIGHT_SPECULAR_GREEN_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_LIGHT_SPECULAR_BLUE_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
 
      SetWindowLong(GetDlgItem(hwnd,IDDI_LIGHT_AMBIENT_BACKGROUND),GWL_USERDATA,(long)p);
      SetWindowLong(GetDlgItem(hwnd,IDDI_LIGHT_DIFFUSE_BACKGROUND),GWL_USERDATA,(long)p);
      SetWindowLong(GetDlgItem(hwnd,IDDI_LIGHT_SPECULAR_BACKGROUND),GWL_USERDATA,(long)p);
 
      if ( ! p -> defaultPatchPainter ) 
         p -> defaultPatchPainter = (WNDPROC)SetWindowLong(GetDlgItem(hwnd,IDDI_LIGHT_AMBIENT_BACKGROUND),GWL_WNDPROC,(LONG)patchPainterProc);
      else
         SetWindowLong(GetDlgItem(hwnd,IDDI_LIGHT_AMBIENT_BACKGROUND),GWL_WNDPROC,(LONG)patchPainterProc);
 
      SetWindowLong(GetDlgItem(hwnd,IDDI_LIGHT_DIFFUSE_BACKGROUND),GWL_WNDPROC,(LONG)patchPainterProc);
      SetWindowLong(GetDlgItem(hwnd,IDDI_LIGHT_SPECULAR_BACKGROUND),GWL_WNDPROC,(LONG)patchPainterProc);

      GetWindowText(GetDlgItem(hwnd,IDDI_LIGHT_POS_X_EVALED),szPositionFormat,64);
 
      SET_WINDOWS(0,0)

      holdUpdates = FALSE;
 
      SCROLLINFO si;
      si.cbSize = sizeof(si);
      si.fMask = SIF_RANGE | SIF_POS | SIF_PAGE; 
      si.nMin = 0;
      si.nMax = SUPPORTED_LIGHT_COUNT - 1;
      si.nPos = 0;
      si.nPage = 1;
      SendDlgItemMessage(hwnd,IDDI_CHOOSE_LIGHT_NO,SBM_SETSCROLLINFO,(WPARAM)TRUE,(LPARAM)&si);
      SendDlgItemMessage(hwnd,IDDI_CHOOSE_LIGHT_NO,SBM_ENABLE_ARROWS,(WPARAM)ESB_DISABLE_UP,0L);

      p -> ppPropertyLightOn[0] -> setWindowItemChecked(p -> hwndLightingSettings,IDDI_LIGHT_ENABLED);

      }

      return LRESULT(FALSE);
 
   case WM_VSCROLL: {

      HWND hwndScrollBar = (HWND)lParam;

      if ( hwndScrollBar != GetDlgItem(hwnd,IDDI_CHOOSE_LIGHT_NO) ) 
         return LRESULT(FALSE);

      if ( LOWORD(wParam) == SB_ENDSCROLL ) 
         return LRESULT(FALSE);

      SCROLLINFO si;
      memset(&si,0,sizeof(SCROLLINFO));
      si.cbSize = sizeof(SCROLLINFO);
      si.fMask = SIF_POS | SIF_RANGE;
      SendMessage(hwndScrollBar,SBM_GETSCROLLINFO,(WPARAM)0,(LPARAM)&si);

      GET_VALUES(si.nPos)

      switch ( LOWORD(wParam) ) {
      case SB_LINEDOWN:
      case SB_PAGEDOWN:
         si.nPos++;
         break;

      case SB_LINEUP:
      case SB_PAGEUP:
         si.nPos--;
         break;

      case SB_THUMBPOSITION:
      case SB_THUMBTRACK:
         si.nPos = HIWORD(wParam);
         break;

      default:
         return LRESULT(FALSE);
      }

      if ( si.nPos <= si.nMin )
         si.nPos = si.nMin;

      if ( si.nPos >= si.nMax )
         si.nPos = si.nMax;

      holdUpdates = TRUE;
      SET_WINDOWS(si.nPos,0);
      holdUpdates = FALSE;

      si.fMask = SIF_POS;
      SetScrollInfo((HWND)lParam,SB_CTL,&si,TRUE);

      RECT rectOwner,rectTop,rectBottom;
      GetWindowRect(hwnd,&rectOwner);
      GetWindowRect(GetDlgItem(hwnd,IDDI_LIGHT_AMBIENT_BACKGROUND),&rectTop);
      GetWindowRect(GetDlgItem(hwnd,IDDI_LIGHT_SPECULAR_BACKGROUND),&rectBottom);
      long cx,cy;
      cx = rectTop.right - rectTop.left;
      cy = rectBottom.bottom - rectTop.top;
 
      rectTop.left = rectTop.left - rectOwner.left;
      rectTop.top = rectTop.top - rectOwner.top;
      rectTop.right = rectTop.left + cx;
      rectTop.bottom = rectTop.top + cy;
      InvalidateRect(hwnd,&rectTop,TRUE);
 
      if ( si.nPos == si.nMin ) {
         SendMessage(hwndScrollBar,SBM_ENABLE_ARROWS,(WPARAM)ESB_DISABLE_UP,0L);
      } else {
         if ( si.nPos == si.nMax ) {
            SendMessage(hwndScrollBar,SBM_ENABLE_ARROWS,(WPARAM)ESB_DISABLE_DOWN,0L);
         } else
            SendMessage(hwndScrollBar,SBM_ENABLE_ARROWS,(WPARAM)ESB_ENABLE_BOTH,0L);
      }

      return LRESULT(FALSE);
      }
      break;
 
   case WM_NOTIFY: {
      NM_UPDOWN *pn = (NM_UPDOWN *)lParam;
      if ( pn -> hdr.code != UDN_DELTAPOS ) break;
      if ( pn -> hdr.idFrom < (unsigned int)IDDI_LIGHT_AMBIENT_RED_SPIN || pn -> hdr.idFrom > (unsigned int)IDDI_LIGHT_SPECULAR_BLUE_SPIN ) 
         break;
      int k = pn -> hdr.idFrom - IDDI_LIGHT_AMBIENT_RED_SPIN;
      HWND hwndEdit,hwndBackground;
      char szTemp[32];
      float x;
      switch ( k ) {
      case 0:
         hwndEdit = GetDlgItem(hwnd,IDDI_LIGHT_AMBIENT_RED);
         hwndBackground = GetDlgItem(hwnd,IDDI_LIGHT_AMBIENT_BACKGROUND);
         break;
      case 1:
         hwndEdit = GetDlgItem(hwnd,IDDI_LIGHT_AMBIENT_GREEN);
         hwndBackground = GetDlgItem(hwnd,IDDI_LIGHT_AMBIENT_BACKGROUND);
         break;
      case 2:
         hwndEdit = GetDlgItem(hwnd,IDDI_LIGHT_AMBIENT_BLUE);
         hwndBackground = GetDlgItem(hwnd,IDDI_LIGHT_AMBIENT_BACKGROUND);
         break;
      case 3:
         hwndEdit = GetDlgItem(hwnd,IDDI_LIGHT_DIFFUSE_RED);
         hwndBackground = GetDlgItem(hwnd,IDDI_LIGHT_DIFFUSE_BACKGROUND);
         break;
      case 4:
         hwndEdit = GetDlgItem(hwnd,IDDI_LIGHT_DIFFUSE_GREEN);
         hwndBackground = GetDlgItem(hwnd,IDDI_LIGHT_DIFFUSE_BACKGROUND);
         break;
      case 5:
         hwndEdit = GetDlgItem(hwnd,IDDI_LIGHT_DIFFUSE_BLUE);
         hwndBackground = GetDlgItem(hwnd,IDDI_LIGHT_DIFFUSE_BACKGROUND);
         break;
      case 6:
         hwndEdit = GetDlgItem(hwnd,IDDI_LIGHT_SPECULAR_RED);
         hwndBackground = GetDlgItem(hwnd,IDDI_LIGHT_SPECULAR_BACKGROUND);
         break;
      case 7:
         hwndEdit = GetDlgItem(hwnd,IDDI_LIGHT_SPECULAR_GREEN);
         hwndBackground = GetDlgItem(hwnd,IDDI_LIGHT_SPECULAR_BACKGROUND);
         break;
      case 8:
         hwndEdit = GetDlgItem(hwnd,IDDI_LIGHT_SPECULAR_BLUE);
         hwndBackground = GetDlgItem(hwnd,IDDI_LIGHT_SPECULAR_BACKGROUND);
         break;
      }
      GetWindowText(hwndEdit,szTemp,32);
      x = (float)atof(szTemp);
      x += (float)pn -> iDelta * 0.1f;
      if ( x >= 0.0f && x <= 1.0 ) {
         sprintf(szTemp,"%3.1f",x);
         SetWindowText(hwndEdit,szTemp);
         int j = SendMessage(GetDlgItem(hwnd,IDDI_CHOOSE_LIGHT_NO),SBM_GETPOS,0,0);
         GET_VALUES(j)
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

//if ( IsWindowVisible(hwnd) )
//p -> render(0);

      return LRESULT(FALSE);
 
   case WM_COMMAND: {
      int notifyCode = HIWORD(wParam);
      switch ( LOWORD(wParam) ) {
      case IDDI_LIGHT_ENABLED:
         if ( notifyCode == BN_CLICKED ) {
            int k = SendMessage(GetDlgItem(hwnd,IDDI_CHOOSE_LIGHT_NO),SBM_GETPOS,0,0);
            p -> ppPropertyLightOn[k] -> getWindowItemChecked(hwnd,IDDI_LIGHT_ENABLED);
            if ( SendMessage((HWND)lParam,BM_GETCHECK,0,0) != BST_CHECKED ) {
               ENABLE_WINDOWS(FALSE);
            } else {
               ENABLE_WINDOWS(TRUE);
            }
         }
         break;
 
      case IDDI_LIGHT_POS_X:
      case IDDI_LIGHT_POS_Y:
      case IDDI_LIGHT_POS_Z: 
         if ( notifyCode == EN_CHANGE ) {
             if ( holdUpdates ) break;
             int k = SendMessage(GetDlgItem(hwnd,IDDI_CHOOSE_LIGHT_NO),SBM_GETPOS,0,0);
             GET_VALUES(k)
             holdUpdates = true;
             SET_WINDOWS(k,LOWORD(wParam));
             holdUpdates = false;
         }
         break;
 
      case IDDI_LIGHT_AMBIENT_CHOOSE:
      case IDDI_LIGHT_DIFFUSE_CHOOSE:
      case IDDI_LIGHT_SPECULAR_CHOOSE: {
         IGProperty *pp;
         int k = SendMessage(GetDlgItem(hwnd,IDDI_CHOOSE_LIGHT_NO),SBM_GETPOS,0,0);
         HWND hwndBackground;
         switch ( LOWORD(wParam) ) {
         case IDDI_LIGHT_AMBIENT_CHOOSE:
            pp = p -> ppPropertyAmbientLight[k]; 
            hwndBackground = GetDlgItem(hwnd,IDDI_LIGHT_AMBIENT_BACKGROUND);
            break;
         case IDDI_LIGHT_DIFFUSE_CHOOSE:
            pp = p -> ppPropertyDiffuseLight[k]; 
            hwndBackground = GetDlgItem(hwnd,IDDI_LIGHT_DIFFUSE_BACKGROUND);
            break;
         case IDDI_LIGHT_SPECULAR_CHOOSE:
            pp = p -> ppPropertySpecularLight[k]; 
            hwndBackground = GetDlgItem(hwnd,IDDI_LIGHT_SPECULAR_BACKGROUND);
            break;
         }
         CHOOSECOLOR ci;
         GET_VALUES(k);
         float fv[4];
         BYTE *pb = (BYTE *)fv;
         pp -> get_binaryValue(4 * sizeof(float),(BYTE**)&pb);
         BYTE vb[3];
         BYTE ccData[128];
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
            SET_WINDOWS(k,0)
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
         p -> propertyCustomColors -> put_binaryValue(128,reinterpret_cast<byte *>(ci.lpCustColors));
         }
         break;
 
      case IDDI_LIGHT_AMBIENT_RED:
      case IDDI_LIGHT_AMBIENT_GREEN:
      case IDDI_LIGHT_AMBIENT_BLUE:
      case IDDI_LIGHT_DIFFUSE_RED:
      case IDDI_LIGHT_DIFFUSE_GREEN:
      case IDDI_LIGHT_DIFFUSE_BLUE:
      case IDDI_LIGHT_SPECULAR_RED:
      case IDDI_LIGHT_SPECULAR_GREEN:
      case IDDI_LIGHT_SPECULAR_BLUE: {
         switch ( notifyCode ) {
         case EN_CHANGE: {
            if ( holdUpdates ) break;
            int k = SendMessage(GetDlgItem(hwnd,IDDI_CHOOSE_LIGHT_NO),SBM_GETPOS,0,0);
            GET_VALUES(k)
            RECT rect,rect2;
            HWND hwndBackground;
            long cx,cy;
            switch ( LOWORD(wParam) ) {
               case IDDI_LIGHT_AMBIENT_RED:
                  hwndBackground = GetDlgItem(hwnd,IDDI_LIGHT_AMBIENT_BACKGROUND);
                  break;
               case IDDI_LIGHT_AMBIENT_GREEN:
                  hwndBackground = GetDlgItem(hwnd,IDDI_LIGHT_AMBIENT_BACKGROUND);
                  break;
               case IDDI_LIGHT_AMBIENT_BLUE:
                  hwndBackground = GetDlgItem(hwnd,IDDI_LIGHT_AMBIENT_BACKGROUND);
                  break;
               case IDDI_LIGHT_DIFFUSE_RED:
                  hwndBackground = GetDlgItem(hwnd,IDDI_LIGHT_DIFFUSE_BACKGROUND);
                  break;
               case IDDI_LIGHT_DIFFUSE_GREEN:
                  hwndBackground = GetDlgItem(hwnd,IDDI_LIGHT_DIFFUSE_BACKGROUND);
                  break;
               case IDDI_LIGHT_DIFFUSE_BLUE:
                  hwndBackground = GetDlgItem(hwnd,IDDI_LIGHT_DIFFUSE_BACKGROUND);
                  break;
               case IDDI_LIGHT_SPECULAR_RED:
                  hwndBackground = GetDlgItem(hwnd,IDDI_LIGHT_SPECULAR_BACKGROUND);
                  break;
               case IDDI_LIGHT_SPECULAR_GREEN:
                  hwndBackground = GetDlgItem(hwnd,IDDI_LIGHT_SPECULAR_BACKGROUND);
                  break;
               case IDDI_LIGHT_SPECULAR_BLUE:
                  hwndBackground = GetDlgItem(hwnd,IDDI_LIGHT_SPECULAR_BACKGROUND);
                  break;
            }
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
            break;
         }   
 
         }

         return LRESULT(FALSE);
 
      default:
         break;
      }

//if ( IsWindowVisible(hwnd) )
//p -> render(0);

      }
      return LRESULT(FALSE);
 
 
   default:
      break;
   }
 
   return LRESULT(FALSE);
   }

