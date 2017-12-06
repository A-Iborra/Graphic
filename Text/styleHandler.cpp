/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <process.h>
#include <math.h>

#include <commctrl.h>

#include "Graphic_resource.h"
#include "utils.h"

#include "text.h"

   extern HMODULE hModule;

#define GET_VALUES {                                                                                     \
      char szTemp[32];                                                                                   \
      float fv[4];                                                                                       \
      GetDlgItemText(hwnd,IDDI_TEXT_TEXTCOLOR_RED,szTemp,32); fv[0] = (float)atof(szTemp);               \
      GetDlgItemText(hwnd,IDDI_TEXT_TEXTCOLOR_GREEN,szTemp,32); fv[1] = (float)atof(szTemp);             \
      GetDlgItemText(hwnd,IDDI_TEXT_TEXTCOLOR_BLUE,szTemp,32); fv[2] = (float)atof(szTemp);              \
      fv[3] = 1.0;                                                                                       \
      p -> propertyTextColor -> put_binaryValue(4 * sizeof(float),(BYTE*)fv);                            \
      }

#define SET_WINDOWS                                                                                      \
      {                                                                                                  \
      float fv[4];                                                                                       \
      char szTemp[32];                                                                                   \
      BYTE *pb = (BYTE *)fv;                                                                             \
      p -> propertyTextColor -> get_binaryValue(4 * sizeof(float),(BYTE**)&pb);                          \
      sprintf(szTemp,"%4.1f",fv[0]); SetDlgItemText(hwnd,IDDI_TEXT_TEXTCOLOR_RED,szTemp);                \
      sprintf(szTemp,"%4.1f",fv[1]); SetDlgItemText(hwnd,IDDI_TEXT_TEXTCOLOR_GREEN,szTemp);              \
      sprintf(szTemp,"%4.1f",fv[2]); SetDlgItemText(hwnd,IDDI_TEXT_TEXTCOLOR_BLUE,szTemp);               \
      }


   static int holdUpdates = FALSE;

   LRESULT CALLBACK Text::styleHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
   Text *p = (Text *)GetWindowLong(hwnd,GWL_USERDATA);
 
   switch ( msg ) {
   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);
      p = (Text *)pPage -> lParam;
      SetWindowLong(hwnd,GWL_USERDATA,(long)p);

      holdUpdates = TRUE;

      if ( p -> hwndSample ) DestroyWindow(p -> hwndSample);
      RECT rect;
      GetClientRect(GetParent(hwnd),&rect);
      p -> hwndSample = CreateWindowEx(WS_EX_CLIENTEDGE,"STATIC","",WS_CHILD | WS_VISIBLE,
                                          4,rect.bottom - (int)((double)((rect.bottom - rect.top)/1.8)) - 4,
                                             rect.right - rect.left - 8,(int)((double)((rect.bottom-rect.top)/1.8)) - 4,hwnd,NULL,hModule,(void*)lParam);
      p -> oldSampleHandler = (WNDPROC)SetWindowLong(p -> hwndSample,GWL_WNDPROC,(long)Text::sampleHandler);
      SetWindowLong(p -> hwndSample,GWL_USERDATA,(long)p);

      SetWindowLong(GetDlgItem(hwnd,IDDI_TEXT_TEXTCOLOR_BACKGROUND),GWL_USERDATA,(long)p);

      if ( ! p -> defaultPatchPainter ) 
         p -> defaultPatchPainter = (WNDPROC)SetWindowLong(GetDlgItem(hwnd,IDDI_TEXT_TEXTCOLOR_BACKGROUND),GWL_WNDPROC,(LONG)patchPainterProc);
      else
         SetWindowLong(GetDlgItem(hwnd,IDDI_TEXT_TEXTCOLOR_BACKGROUND),GWL_WNDPROC,(LONG)patchPainterProc);

      SendDlgItemMessage(hwnd,IDDI_TEXT_TEXTCOLOR_RED_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_TEXT_TEXTCOLOR_GREEN_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_TEXT_TEXTCOLOR_BLUE_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));

      SET_WINDOWS

      holdUpdates = FALSE;

      }
      return LRESULT(TRUE);
 
   case WM_NOTIFY: {
      NM_UPDOWN *pn = (NM_UPDOWN *)lParam;
      if ( pn -> hdr.code != UDN_DELTAPOS ) break;
      if ( pn -> hdr.idFrom < (unsigned int)IDDI_TEXT_TEXTCOLOR_RED_SPIN || pn -> hdr.idFrom > (unsigned int)IDDI_TEXT_TEXTCOLOR_BLUE_SPIN ) 
         break;
      int k = pn -> hdr.idFrom - IDDI_TEXT_TEXTCOLOR_RED_SPIN;
      HWND hwndEdit,hwndBackground;
      char szTemp[32];
      float x;
      switch ( k ) {
      case 0:
         hwndEdit = GetDlgItem(hwnd,IDDI_TEXT_TEXTCOLOR_RED);
         hwndBackground = GetDlgItem(hwnd,IDDI_TEXT_TEXTCOLOR_BACKGROUND);
         break;
      case 1:
         hwndEdit = GetDlgItem(hwnd,IDDI_TEXT_TEXTCOLOR_GREEN);
         hwndBackground = GetDlgItem(hwnd,IDDI_TEXT_TEXTCOLOR_BACKGROUND);
         break;
      case 2:
         hwndEdit = GetDlgItem(hwnd,IDDI_TEXT_TEXTCOLOR_BLUE);
         hwndBackground = GetDlgItem(hwnd,IDDI_TEXT_TEXTCOLOR_BACKGROUND);
         break;
      }
      GetWindowText(hwndEdit,szTemp,32);
      x = (float)atof(szTemp);
      x += (float)pn -> iDelta * 0.1f;
      if ( x >= 0.0f && x <= 1.0 ) {
         sprintf(szTemp,"%3.1f",x);
         SetWindowText(hwndEdit,szTemp);
         GET_VALUES
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
      p -> drawSample();
      return LRESULT(FALSE);
 
   case WM_COMMAND: {
 
      switch ( LOWORD(wParam) ) {

      case IDDI_TEXT_FONTLIST: {
 
         switch ( HIWORD(wParam) ) {
 
         case CBN_SELCHANGE: {

            HWND hwndAvailableStyles = GetDlgItem(hwnd,IDDI_TEXT_FONTSTYLELIST);
            long itemIndex = SendMessage((HWND)lParam,CB_GETCURSEL,0L,0L);
            char fontFace[MAX_PATH],*fontStyle = NULL;
            RECT rect;
            fontListData *flData = reinterpret_cast<fontListData *>(SendMessage((HWND)lParam,CB_GETITEMDATA,itemIndex,0L));
            LOGFONTA *lfa;
            SendMessage((HWND)lParam,CB_GETLBTEXT,itemIndex,reinterpret_cast<LPARAM>(fontFace));
            strcpy(p -> szFace,fontFace);
            SendMessage(hwndAvailableStyles,CB_RESETCONTENT,0L,0L);
            if ( flData ) {
               for ( long k = 0; k < flData -> countVariations; k++ ) {
                  lfa = &flData -> lf[k];
                  if ( lfa -> lfWeight & FW_REGULAR ) SendMessage(hwndAvailableStyles,CB_ADDSTRING,-1L,(long)"Regular");
                  if ( lfa -> lfWeight & FW_BOLD ) SendMessage(hwndAvailableStyles,CB_ADDSTRING,-1L,(long)"Bold");
                  if ( lfa -> lfWeight & FW_BLACK ) SendMessage(hwndAvailableStyles,CB_ADDSTRING,-1L,(long)"Black");
                  if ( lfa -> lfItalic ) SendMessage(hwndAvailableStyles,CB_ADDSTRING,-1L,(long)"Italic");
                  if ( lfa -> lfUnderline ) SendMessage(hwndAvailableStyles,CB_ADDSTRING,-1L,(long)"Underline");
               }
               GetWindowRect(hwndAvailableStyles,&rect);
               rect.bottom = 16 + (SendMessage(hwndAvailableStyles,CB_GETCOUNT,0,0) + 1) * SendMessage(hwndAvailableStyles,CB_GETITEMHEIGHT,0,0);
               SetWindowPos(hwndAvailableStyles,HWND_TOP,0,0,rect.right - rect.left,rect.bottom,SWP_NOMOVE | SWP_NOZORDER);
            }
 
            long n;
            p -> propertyFaceStyle -> get_size(&n);
            if ( n ) {
               fontStyle = new char[n + 1];
               p -> propertyFaceStyle -> get_szValue(fontStyle);
            } else {
               fontStyle = new char[32];
               strcpy(fontStyle,"Regular");
            }
            itemIndex = SendMessage(hwndAvailableStyles,CB_FINDSTRINGEXACT,-1L,reinterpret_cast<LPARAM>(fontStyle));
            if ( itemIndex == CB_ERR ) {
               p -> propertyFaceStyle -> put_szValue("Regular");
               itemIndex = SendMessage(hwndAvailableStyles,CB_SELECTSTRING,-1L,reinterpret_cast<LPARAM>("Regular"));
            }
            SendMessage(hwndAvailableStyles,CB_SETCURSEL,itemIndex,0L);
            p -> drawSample(); 
            if ( fontStyle ) delete [] fontStyle;
            }
            return LRESULT(TRUE);
 
         default:
            break;
         }
         }
         return LRESULT(TRUE);
 
      case IDDI_TEXT_FONTSTYLELIST: {
         switch ( HIWORD(wParam) ) {
         case CBN_SELENDOK: {
            char fontStyle[32];
            long itemIndex = SendMessage((HWND)lParam,CB_GETCURSEL,0L,0L);
            SendMessage((HWND)lParam,CB_GETLBTEXT,itemIndex,reinterpret_cast<LPARAM>(fontStyle));
            p -> propertyFaceStyle -> put_szValue(fontStyle);
         }
         }
         p -> drawSample();
         }
         break;
 

      case IDDI_TEXT_FONTSIZEUNITSLIST: {
         switch ( HIWORD(wParam) ) {
         case CBN_SELENDOK:
            p -> propertySizeUnits -> put_longValue(SendMessage((HWND)lParam,CB_GETCURSEL,0L,0L));
         }
         p -> drawSample();
         }
         break;
 

      case IDDI_TEXT_FONTSIZE: {
         switch ( HIWORD(wParam) ) {
         case EN_CHANGE: {
            char szTemp[32];
            SendMessage((HWND)lParam,WM_GETTEXT,32L,reinterpret_cast<LPARAM>(szTemp));
            p -> fontSize = atof(szTemp);
         }
         }
         p -> drawSample();
         }
         break;
 
      case IDDI_TEXT_TEXTCOLOR_CHOOSE: {
         IGProperty *pp;
         HWND hwndBackground;
         switch ( LOWORD(wParam) ) {
         case IDDI_TEXT_TEXTCOLOR_CHOOSE:
            pp = p -> propertyTextColor;
            hwndBackground = GetDlgItem(hwnd,IDDI_TEXT_TEXTCOLOR_BACKGROUND);
            break;
         }
         CHOOSECOLOR ci;
         GET_VALUES
         float fv[4];
         BYTE *pb = (BYTE *)fv;
         pp -> get_binaryValue(4 * sizeof(float),(BYTE**)&pb);
         BYTE vb[3];
         BYTE ccData[128];
         vb[0] = (BYTE)(255.0f*fv[0]);
         vb[1] = (BYTE)(255.0f*fv[1]);
         vb[2] = (BYTE)(255.0f*fv[2]);
         memset(ccData,0,128 * sizeof(BYTE));
         memset(&ci,0,sizeof(ci));
         ci.lStructSize = sizeof(ci);
         ci.hwndOwner = hwnd;
         ci.rgbResult = RGB(vb[0],vb[1],vb[2]);
         ci.lpCustColors = (unsigned long*)ccData;
         ci.Flags = CC_RGBINIT | CC_SHOWHELP | CC_FULLOPEN;
         if ( ChooseColor(&ci) ) {
            fv[0] = (float)GetRValue(ci.rgbResult) / 255.0f;
            fv[1] = (float)GetGValue(ci.rgbResult) / 255.0f;
            fv[2] = (float)GetBValue(ci.rgbResult) / 255.0f;
            pp -> put_binaryValue(4 * sizeof(float),(BYTE*)fv);
            holdUpdates = TRUE;
            SET_WINDOWS
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
         p -> drawSample();
         }
         return LRESULT(0);


      case IDDI_TEXT_TEXTCOLOR_RED:
      case IDDI_TEXT_TEXTCOLOR_GREEN:
      case IDDI_TEXT_TEXTCOLOR_BLUE: {
         switch ( HIWORD(wParam) ) {
         case EN_CHANGE: {
            if ( holdUpdates ) break;
            GET_VALUES
            RECT rect,rect2;
            HWND hwndBackground;
            long cx,cy;
            switch ( LOWORD(wParam) ) {
               case IDDI_TEXT_TEXTCOLOR_RED:
                  hwndBackground = GetDlgItem(hwnd,IDDI_TEXT_TEXTCOLOR_BACKGROUND);
                  break;
               case IDDI_TEXT_TEXTCOLOR_GREEN:
                  hwndBackground = GetDlgItem(hwnd,IDDI_TEXT_TEXTCOLOR_BACKGROUND);
                  break;
               case IDDI_TEXT_TEXTCOLOR_BLUE:
                  hwndBackground = GetDlgItem(hwnd,IDDI_TEXT_TEXTCOLOR_BACKGROUND);
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
            p -> drawSample();
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
 
   default:
      break;
 
   }
 
   }
   return LRESULT(FALSE);
   }


   LRESULT CALLBACK Text::patchPainterProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
   Text *p = (Text *)GetWindowLong(hwnd,GWL_USERDATA);
   switch ( msg ) {
   case WM_PAINT: {
      PAINTSTRUCT ps;
      HBRUSH hb;
      HDC hdc;
      IGProperty *pp;
      if ( hwnd == GetDlgItem(p -> hwndStyle,IDDI_TEXT_TEXTCOLOR_BACKGROUND) ) {
         pp = p -> propertyTextColor;
      }
      else 
         return LRESULT(FALSE);
      float fv[4];
      BYTE *pb = (BYTE *)fv;
      pp -> get_binaryValue(4 * sizeof(long),(BYTE**)&pb);
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
      }
      return LRESULT(FALSE);
 
   default:
      break;
   }
   return p -> defaultPatchPainter(hwnd,msg,wParam,lParam);
   }