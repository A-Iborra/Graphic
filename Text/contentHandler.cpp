// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "text.h"

#include "Graphic_resource.h"

   LRESULT CALLBACK Text::contentHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   Text *p = (Text *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = (PROPSHEETPAGE *)lParam;

      p = (Text *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      p -> hwndContent = hwnd;

      long n;

      p -> propertyContent -> setWindowText(GetDlgItem(hwnd,IDDI_TEXT_TEXT));

      p -> propertyDescription -> get_size(&n);

      if ( n > 1 ) {

         char *pszDescription = new char[n + 1];
         p -> propertyDescription -> get_szValue(pszDescription);
         SetWindowText(GetDlgItem(hwnd,IDDI_TEXT_DESCRIPTION),pszDescription);
         RECT rect;

         GetClientRect(GetDlgItem(hwnd,IDDI_TEXT_DESCRIPTION),&rect);
         SetWindowPos(GetDlgItem(hwnd,IDDI_TEXT_DESCRIPTION),HWND_TOP,rect.left,rect.top,rect.right - rect.left,rect.bottom - rect.top + 16,0L);

         GetClientRect(GetDlgItem(hwnd,IDDI_TEXT_TEXT),&rect);
         SetWindowPos(GetDlgItem(hwnd,IDDI_TEXT_TEXT),HWND_TOP,rect.left,rect.top + 16,rect.right - rect.left,rect.bottom - rect.top - 16,0L);

         delete [] pszDescription;
      }

      }
      return LRESULT(IDDI_TEXT_TEXT);
 
   case WM_SHOWWINDOW: {

      if ( ! (BOOL)wParam )
         break;

      p -> createFont(&p -> logicalFont);

#if 0
      HDC hdc = GetDC(GetDlgItem(hwnd,IDDI_TEXT_TEXT));

      float fv[4];
      BYTE *pb = (BYTE *)fv;

      p -> propertyTextColor -> get_binaryValue(4 * sizeof(long),(BYTE **)&pb);

      BYTE vb[3];
      COLORREF cr;
      vb[0] = (BYTE)(255.0f*fv[0]);
      vb[1] = (BYTE)(255.0f*fv[1]);
      vb[2] = (BYTE)(255.0f*fv[2]);
      cr = RGB(vb[0],vb[1],vb[2]);
   
      SetTextColor(hdc,cr);

      ReleaseDC(GetDlgItem(hwnd,IDDI_TEXT_TEXT),hdc);

#endif

      SendDlgItemMessage(hwnd,IDDI_TEXT_TEXT,WM_SETFONT,(WPARAM)p -> hFont,(LPARAM)TRUE);

      }
      break;

   case WM_SIZE: {                  
      int cx = LOWORD(lParam);
      int cy = HIWORD(lParam);
      RECT rect;
      GetClientRect(GetDlgItem(hwnd,IDDI_TEXT_TEXT),&rect);
      SetWindowPos(GetDlgItem(hwnd,IDDI_TEXT_TEXT),HWND_TOP,8,rect.top + 32,cx - 16,cy - 48 - rect.top,0L);
      }
      break;
 
   case WM_DESTROY: {
      RECT rectParent;
      GetWindowRect(GetParent(GetParent(hwnd)),&rectParent);
      p -> propertyPropertiesPosition -> put_binaryValue(sizeof(RECT),(BYTE*)&rectParent);
      p -> propertyContent -> getWindowItemValue(hwnd,IDDI_TEXT_TEXT);
      }
      break;
 
   case WM_COMMAND: {
 
      switch ( LOWORD(wParam) ) {
 
      case IDDI_TEXT_TEXT:
         p -> propertyContent -> getWindowItemValue(hwnd,IDDI_TEXT_TEXT);
         break;
 
      default:
         break;
      }
 
   default:
      break;
 
   }
 
   }
   return LRESULT(FALSE);
   }