// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Text.h"

#include <stdio.h>
#include <commctrl.h>

#include "Graphic_resource.h"

   extern HMODULE hModule;

#define GET_SPINNERS {                                                                                   \
      char szTemp[32];                                                                                   \
      float fv[4];                                                                                       \
      GetDlgItemText(hwnd,IDDI_TEXT_TEXTCOLOR_RED,szTemp,32); fv[0] = (float)atof(szTemp);               \
      GetDlgItemText(hwnd,IDDI_TEXT_TEXTCOLOR_GREEN,szTemp,32); fv[1] = (float)atof(szTemp);             \
      GetDlgItemText(hwnd,IDDI_TEXT_TEXTCOLOR_BLUE,szTemp,32); fv[2] = (float)atof(szTemp);              \
      fv[3] = 1.0;                                                                                       \
      p -> propertyTextColor -> put_binaryValue(4 * sizeof(float),(BYTE*)fv);                            \
      }

#define SET_SPINNERS                                                                                     \
      {                                                                                                  \
      float fv[4];                                                                                       \
      char szTemp[32];                                                                                   \
      BYTE *pb = (BYTE *)fv;                                                                             \
      p -> propertyTextColor -> get_binaryValue(4 * sizeof(float),(BYTE**)&pb);                          \
      sprintf(szTemp,"%3.1f",fv[0]); SetDlgItemText(hwnd,IDDI_TEXT_TEXTCOLOR_RED,szTemp);                \
      sprintf(szTemp,"%3.1f",fv[1]); SetDlgItemText(hwnd,IDDI_TEXT_TEXTCOLOR_GREEN,szTemp);              \
      sprintf(szTemp,"%3.1f",fv[2]); SetDlgItemText(hwnd,IDDI_TEXT_TEXTCOLOR_BLUE,szTemp);               \
      }


   static int holdUpdates = FALSE;

   LRESULT CALLBACK Text::styleHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   Text *p = (Text *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);

      p = (Text *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      p -> hwndStyle = hwnd;

      holdUpdates = TRUE;

      if ( p -> hwndSample ) 
         DestroyWindow(p -> hwndSample);

      RECT rect,rcColor;

      GetWindowRect(hwnd,&rect);

      GetWindowRect(GetDlgItem(hwnd,IDDI_TEXT_TEXTCOLOR_BACKGROUND),&rcColor);

      long bottomColor = rcColor.bottom - rect.top;

      p -> hwndSample = CreateWindowEx(WS_EX_CLIENTEDGE,"STATIC","",WS_CHILD | WS_VISIBLE,
                                          4,bottomColor + 8,
                                             rect.right - rect.left - 12,rect.bottom - rect.top - bottomColor - 16,hwnd,NULL,hModule,(void *)p);

      SetWindowLongPtr(p -> hwndSample,GWLP_USERDATA,(ULONG_PTR)p);

      p -> createFont(&p -> logicalFont);

      p -> oldSampleHandler = (WNDPROC)SetWindowLongPtr(p -> hwndSample,GWLP_WNDPROC,(ULONG_PTR)Text::sampleHandler);

      SetWindowLongPtr(GetDlgItem(hwnd,IDDI_TEXT_TEXTCOLOR_BACKGROUND),GWLP_USERDATA,(ULONG_PTR)p);

      if ( ! p -> defaultPatchPainter ) 
         p -> defaultPatchPainter = (WNDPROC)SetWindowLongPtr(GetDlgItem(hwnd,IDDI_TEXT_TEXTCOLOR_BACKGROUND),GWLP_WNDPROC,(ULONG_PTR)patchPainterProc);
      else
         SetWindowLongPtr(GetDlgItem(hwnd,IDDI_TEXT_TEXTCOLOR_BACKGROUND),GWLP_WNDPROC,(ULONG_PTR)patchPainterProc);

      SendDlgItemMessage(hwnd,IDDI_TEXT_FONTSIZE_SPINNER,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_TEXT_TEXTCOLOR_RED_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_TEXT_TEXTCOLOR_GREEN_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      SendDlgItemMessage(hwnd,IDDI_TEXT_TEXTCOLOR_BLUE_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));

      HWND hwndListBox = GetDlgItem(hwnd,IDDI_TEXT_FONTLIST);
   
      HDC hdc = GetDC(0);

      LOGFONT logFont;
    
      memset(&logFont,0,sizeof(LOGFONT));
    
      logFont.lfCharSet = DEFAULT_CHARSET;

      logFont.lfFaceName[0] = '\0';
    
      EnumFontFamiliesEx(hdc,&logFont,fontListEnumerator,(LPARAM)hwndListBox,0L);
   
      DeleteDC(hdc); 
   
      long n = (DWORD)strlen(p -> szFace);
      char *pszFontName = new char[n + 1];
      char *pszFontStyle;
      memset(pszFontName,0,(n + 1) * sizeof(char));
      strcpy(pszFontName,p -> szFace);
   
      n = (long)SendMessage(hwndListBox,CB_FINDSTRINGEXACT,-1L,(LPARAM)pszFontName);

      SendMessage(hwndListBox,CB_SETCURSEL,(WPARAM)n,0L);
   
      SendMessage(hwnd,WM_COMMAND,MAKEWPARAM(IDDI_TEXT_FONTLIST,CBN_SELCHANGE),(LPARAM)GetDlgItem(hwnd,IDDI_TEXT_FONTLIST));
   
      delete [] pszFontName;
   
      p -> propertyFaceStyle -> get_size(&n);
      if ( n ) {
         pszFontStyle = new char[n + 1];
         p -> propertyFaceStyle -> get_szValue(pszFontStyle);
      } else {
         pszFontStyle = new char[32];
         sprintf(pszFontStyle,"Regular");
      }
   
      n = (long)SendMessage(GetDlgItem(hwnd,IDDI_TEXT_FONTSTYLELIST),CB_FINDSTRINGEXACT,-1L,(LPARAM)pszFontStyle);
      if ( n == CB_ERR ) {
         p -> propertyFaceStyle -> put_szValue("Regular");
         n = (long)SendMessage(GetDlgItem(hwnd,IDDI_TEXT_FONTSTYLELIST),CB_SELECTSTRING,-1L,(LPARAM)"Regular");
      }
      SendMessage(GetDlgItem(hwnd,IDDI_TEXT_FONTSTYLELIST),CB_SETCURSEL,n,0L);
   
      delete [] pszFontStyle;
   
      char szTemp[32];
      long fontSizeUnits;
      sprintf(szTemp,"%5.1f",p -> fontSize);
      SetDlgItemText(hwnd,IDDI_TEXT_FONTSIZE,szTemp);
   
      long k = 0;
      char unitsArray[][32] = TEXT_UNITS_ARRAY;
      SendMessage(GetDlgItem(hwnd,IDDI_TEXT_FONTSIZEUNITSLIST),CB_RESETCONTENT,0L,0L);
      while ( *unitsArray[k] ) 
         SendMessage(GetDlgItem(hwnd,IDDI_TEXT_FONTSIZEUNITSLIST),CB_ADDSTRING,0,(LPARAM)unitsArray[k++]);
   
      p -> propertySizeUnits -> get_longValue(&fontSizeUnits);

      SendMessage(GetDlgItem(hwnd,IDDI_TEXT_FONTSIZEUNITSLIST),CB_SETCURSEL,fontSizeUnits,0L);

      p -> propertyOpenGLRendering -> setWindowItemChecked(hwnd,IDDI_TEXT_RENDEROPENGL);

      SET_SPINNERS

      holdUpdates = FALSE;

      }
      return LRESULT(TRUE);

   case WM_NOTIFY: {
      if ( holdUpdates )
         break;
      NM_UPDOWN *pn = (NM_UPDOWN *)lParam;
      if ( pn -> hdr.code != UDN_DELTAPOS ) 
         break;
      char szTemp[32];
      if ( IDDI_TEXT_FONTSIZE_SPINNER == pn -> hdr.idFrom ) {
         GetDlgItemText(hwnd,IDDI_TEXT_FONTSIZE,szTemp,32);
         float x = (float)atof(szTemp);
         x += (float)pn -> iDelta;
         sprintf(szTemp,"%3.1f",x);
         SetDlgItemText(hwnd,IDDI_TEXT_FONTSIZE,szTemp);
         break;
      }
      if ( pn -> hdr.idFrom < (unsigned int)IDDI_TEXT_TEXTCOLOR_RED_SPIN || pn -> hdr.idFrom > (unsigned int)IDDI_TEXT_TEXTCOLOR_BLUE_SPIN ) 
         break;
      int k = (int)pn -> hdr.idFrom - IDDI_TEXT_TEXTCOLOR_RED_SPIN;
      HWND hwndEdit,hwndBackground;
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
         GET_SPINNERS
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

//      p -> drawSample();

      p -> createFont(&p -> logicalFont);

      //InvalidateRect(p -> hwndSample,NULL,TRUE);

      return LRESULT(FALSE);
 
   case WM_COMMAND: {

      switch ( LOWORD(wParam) ) {
 
      case IDDI_TEXT_RENDEROPENGL:
         p -> propertyOpenGLRendering -> getWindowItemChecked(hwnd,IDDI_TEXT_RENDEROPENGL);
         break;

      case IDDI_TEXT_FONTLIST: {
 
         switch ( HIWORD(wParam) ) {
 
         case CBN_SELCHANGE: {

            HWND hwndAvailableStyles = GetDlgItem(hwnd,IDDI_TEXT_FONTSTYLELIST);
            long itemIndex = (long)SendMessage((HWND)lParam,CB_GETCURSEL,0L,0L);
            char *fontStyle = NULL;
            RECT rect;
            fontListData *flData = (fontListData *)SendMessage((HWND)lParam,CB_GETITEMDATA,itemIndex,0L);
            LOGFONTA *lfa = NULL;
            SendMessage((HWND)lParam,CB_GETLBTEXT,itemIndex,(LPARAM)p -> szFace);
            SendMessage(hwndAvailableStyles,CB_RESETCONTENT,0L,0L);
            if ( flData ) {
               for ( long k = 0; k < flData -> countVariations; k++ ) {
                  lfa = &flData -> lf[k];
                  if ( lfa -> lfWeight & FW_REGULAR ) SendMessage(hwndAvailableStyles,CB_ADDSTRING,-1L,(LPARAM)"Regular");
                  if ( lfa -> lfWeight & FW_BOLD ) SendMessage(hwndAvailableStyles,CB_ADDSTRING,-1L,(LPARAM)"Bold");
                  if ( lfa -> lfWeight & FW_BLACK ) SendMessage(hwndAvailableStyles,CB_ADDSTRING,-1L,(LPARAM)"Black");
                  if ( lfa -> lfItalic ) SendMessage(hwndAvailableStyles,CB_ADDSTRING,-1L,(LPARAM)"Italic");
                  if ( lfa -> lfUnderline ) SendMessage(hwndAvailableStyles,CB_ADDSTRING,-1L,(LPARAM)"Underline");
               }
               GetWindowRect(hwndAvailableStyles,&rect);
               rect.bottom = 16 + ((long)SendMessage(hwndAvailableStyles,CB_GETCOUNT,0,0) + 1) * (long)SendMessage(hwndAvailableStyles,CB_GETITEMHEIGHT,0,0);
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
            itemIndex = (long)SendMessage(hwndAvailableStyles,CB_FINDSTRINGEXACT,-1L,(LPARAM)fontStyle);
            if ( itemIndex == CB_ERR ) {
               p -> propertyFaceStyle -> put_szValue("Regular");
               itemIndex = (long)SendMessage(hwndAvailableStyles,CB_SELECTSTRING,-1L,(LPARAM)"Regular");
            }
            SendMessage(hwndAvailableStyles,CB_SETCURSEL,itemIndex,0L);

            p -> createFont(&p -> logicalFont);

            //InvalidateRect(p -> hwndSample,NULL,TRUE);

//            p -> drawSample(); 

            if ( fontStyle ) 
               delete [] fontStyle;

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
            long itemIndex = (long)SendMessage((HWND)lParam,CB_GETCURSEL,0L,0L);
            SendMessage((HWND)lParam,CB_GETLBTEXT,itemIndex,reinterpret_cast<LPARAM>(fontStyle));
            p -> propertyFaceStyle -> put_szValue(fontStyle);
         }
         }

         p -> createFont(&p -> logicalFont);

         }
         break;
 
      case IDDI_TEXT_FONTSIZEUNITSLIST: {
         switch ( HIWORD(wParam) ) {
         case CBN_SELENDOK:
            p -> propertySizeUnits -> put_longValue((long)SendMessage((HWND)lParam,CB_GETCURSEL,0L,0L));
         }

         p -> createFont(&p -> logicalFont);

         }
         break;
 
      case IDDI_TEXT_FONTSIZE: {
         switch ( HIWORD(wParam) ) {
         case EN_CHANGE: {
            char szTemp[32];
            SendMessage((HWND)lParam,WM_GETTEXT,32L,(LPARAM)szTemp);
            p -> fontSize = atof(szTemp);
         }
         }

         p -> createFont(&p -> logicalFont);

         }
         break;
 
      case IDDI_TEXT_TEXTCOLOR_CHOOSE: {
         CHOOSECOLOR ci;
         GET_SPINNERS
         float fv[4];
         BYTE *pb = (BYTE *)fv;
         p -> propertyTextColor -> get_binaryValue(4 * sizeof(float),(BYTE**)&pb);
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
            p -> propertyTextColor -> put_binaryValue(4 * sizeof(float),(BYTE*)fv);
            holdUpdates = TRUE;
            SET_SPINNERS
            holdUpdates = FALSE;
            RECT rect,rect2;
            long cx,cy;
            GetWindowRect(GetDlgItem(hwnd,IDDI_TEXT_TEXTCOLOR_BACKGROUND),&rect);
            GetWindowRect(hwnd,&rect2);
            cx = rect.right - rect.left;
            cy = rect.bottom - rect.top;
            rect.left = rect.left - rect2.left;
            rect.top = rect.top - rect2.top;
            rect.right = rect.left + cx;
            rect.bottom = rect.top + cy;
            InvalidateRect(hwnd,&rect,TRUE);
         }

         p -> createFont(&p -> logicalFont);

         }
         return LRESULT(0);

      case IDDI_TEXT_TEXTCOLOR_RED:
      case IDDI_TEXT_TEXTCOLOR_GREEN:
      case IDDI_TEXT_TEXTCOLOR_BLUE: {
         switch ( HIWORD(wParam) ) {
         case EN_CHANGE: {
            if ( holdUpdates ) break;
            GET_SPINNERS
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

            p -> createFont(&p -> logicalFont);

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
   Text *p = (Text *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
   switch ( msg ) {
   case WM_PAINT: {
      PAINTSTRUCT ps;
      HBRUSH hb;
      HDC hdc;
      float fv[4];
      BYTE *pb = (BYTE *)fv;
      p -> propertyTextColor -> get_binaryValue(4 * sizeof(long),(BYTE**)&pb);
      BYTE vb[3];
      COLORREF cr;
      vb[0] = (BYTE)(255.0f*fv[0]);
      vb[1] = (BYTE)(255.0f*fv[1]);
      vb[2] = (BYTE)(255.0f*fv[2]);
      cr = RGB(vb[0],vb[1],vb[2]);
      hb = CreateSolidBrush(cr);
      hdc = BeginPaint(hwnd,&ps);
      FillRect(hdc,&ps.rcPaint,hb);
      DeleteObject(hb);
      EndPaint(hwnd,&ps);
      }
      return LRESULT(FALSE);
 
   default:
      break;
   }
   return CallWindowProc(p -> defaultPatchPainter,hwnd,msg,wParam,lParam);
   }


   LRESULT CALLBACK Text::sampleHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   Text *p = (Text *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch ( msg ) {

   case WM_PAINT: {

      PAINTSTRUCT ps;
      BeginPaint(hwnd,&ps);

      float fv[4];
      BYTE *pb = (BYTE *)fv;

      p -> propertyTextColor -> get_binaryValue(4 * sizeof(long),(BYTE **)&pb);

      BYTE vb[3];
      COLORREF cr;
      vb[0] = (BYTE)(255.0f*fv[0]);
      vb[1] = (BYTE)(255.0f*fv[1]);
      vb[2] = (BYTE)(255.0f*fv[2]);
      cr = RGB(vb[0],vb[1],vb[2]);
   
      SetTextColor(ps.hdc,cr);

      cr = GetPixel(ps.hdc,1,1);

      SetBkColor(ps.hdc,cr);

      HBRUSH hb = CreateSolidBrush(cr);

      FillRect(ps.hdc,&ps.rcPaint,hb);

      DeleteObject(hb);

      HGDIOBJ oldFont = SelectObject(ps.hdc,p -> hFont);

      RECT rcAvailable,rcText;

      GetClientRect(hwnd,&rcAvailable);

      memset(&rcText,0,sizeof(RECT));

      DrawTextEx(ps.hdc,(char *)p -> propertyContent -> pointer(),-1L,&rcText,DT_CALCRECT,NULL);

      long cxText = min(rcText.right - rcText.left,rcAvailable.right - rcAvailable.left);
      long cyText = min(rcText.bottom - rcText.top,rcAvailable.bottom - rcAvailable.top);

      rcText.left = (rcAvailable.right - rcAvailable.left - cxText) / 2;
      rcText.top = (rcAvailable.bottom - rcAvailable.top - cyText) / 2;
      rcText.right = rcText.left + cxText;
      rcText.bottom = rcText.top + cyText;

      DrawTextEx(ps.hdc,(char *)p -> propertyContent -> pointer(),-1L,&rcText,DT_CENTER,NULL);

      SelectObject(ps.hdc,oldFont);

      EndPaint(hwnd,&ps);

      }
      return (LRESULT)FALSE;
   
   case WM_DESTROY:
      p -> hwndSample = NULL;

   default:
      break;
 
   }

   if ( p )
      return CallWindowProc(p -> oldSampleHandler,hwnd,msg,wParam,lParam);

   return LRESULT(FALSE);
   }