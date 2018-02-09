// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "text.h"
#include <stdio.h>
#include <commctrl.h>

#include "Graphic_resource.h"

   LRESULT CALLBACK Text::orientationHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   Text *p = (Text *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch ( msg ) {
 
   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);

      p = (Text *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      p -> hwndOrientation = hwnd;

      }
      return LRESULT(TRUE);
 
   case WM_SHOWWINDOW: {

      if ( ! (BOOL)wParam )
         break;

      SendDlgItemMessage(hwnd,IDDI_TEXT_XYPLANE,BM_SETCHECK,(WPARAM)BST_UNCHECKED,0L);
      SendDlgItemMessage(hwnd,IDDI_TEXT_YXPLANE,BM_SETCHECK,(WPARAM)BST_UNCHECKED,0L);
      SendDlgItemMessage(hwnd,IDDI_TEXT_YZPLANE,BM_SETCHECK,(WPARAM)BST_UNCHECKED,0L);
      SendDlgItemMessage(hwnd,IDDI_TEXT_ZXPLANE,BM_SETCHECK,(WPARAM)BST_UNCHECKED,0L);
      SendDlgItemMessage(hwnd,IDDI_TEXT_ZYPLANE,BM_SETCHECK,(WPARAM)BST_UNCHECKED,0L);
      SendDlgItemMessage(hwnd,IDDI_TEXT_SCREENPLANE,BM_SETCHECK,(WPARAM)BST_UNCHECKED,0L);

      switch ( p -> coordinatePlane ) {
      case CoordinatePlane_XY:
         SendDlgItemMessage(hwnd,IDDI_TEXT_XYPLANE,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
         break;
      case CoordinatePlane_XZ:
         SendDlgItemMessage(hwnd,IDDI_TEXT_XZPLANE,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
         break;
      case CoordinatePlane_YX:
         SendDlgItemMessage(hwnd,IDDI_TEXT_YXPLANE,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
         break;
      case CoordinatePlane_YZ:
         SendDlgItemMessage(hwnd,IDDI_TEXT_YZPLANE,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
         break;
      case CoordinatePlane_ZX:
         SendDlgItemMessage(hwnd,IDDI_TEXT_ZXPLANE,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
         break;
      case CoordinatePlane_ZY:
         SendDlgItemMessage(hwnd,IDDI_TEXT_ZYPLANE,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
         break;
      case CoordinatePlane_screen:
         SendDlgItemMessage(hwnd,IDDI_TEXT_SCREENPLANE,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
         break;
      }

      if ( p -> flipVertical ) 
         SendDlgItemMessage(hwnd,IDDI_TEXT_FLIP_TB,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);

      if ( p -> flipHorizontal ) 
         SendDlgItemMessage(hwnd,IDDI_TEXT_FLIP_LR,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);

      SendDlgItemMessage(hwnd,IDDI_TEXT_PLANEHEIGHT,TBM_SETSEL,(WPARAM)FALSE,MAKELPARAM(0,100));
      SendDlgItemMessage(hwnd,IDDI_TEXT_PLANEHEIGHT,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)static_cast<long>(100.0 * (1.0 - p -> planeHeight)));

      char szTemp[32];
      sprintf(szTemp,"%g",p -> dpStart.x);
      SetDlgItemText(hwnd,IDDI_TEXT_XCOORDINATE,szTemp);
      sprintf(szTemp,"%g",p -> dpStart.y);
      SetDlgItemText(hwnd,IDDI_TEXT_YCOORDINATE,szTemp);
      sprintf(szTemp,"%g",p -> dpStart.z);
      SetDlgItemText(hwnd,IDDI_TEXT_ZCOORDINATE,szTemp);

      EnableWindow(GetDlgItem(hwnd,IDDI_TEXT_XCOORDINATE),p -> enablePositionSettings);
      EnableWindow(GetDlgItem(hwnd,IDDI_TEXT_YCOORDINATE),p -> enablePositionSettings);
      EnableWindow(GetDlgItem(hwnd,IDDI_TEXT_ZCOORDINATE),p -> enablePositionSettings);

      TextFormat format;
      p -> propertyFormat -> get_longValue(reinterpret_cast<long*>(&format));

      if ( format & TEXT_COORDINATES_FROM_TOP ) 
         SendDlgItemMessage(hwnd,IDDI_TEXT_FORMAT_FROM_TOP,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
      else {
         if ( format & TEXT_COORDINATES_FROM_CENTER )
            SendDlgItemMessage(hwnd,IDDI_TEXT_FORMAT_FROM_CENTER,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
         else
            SendDlgItemMessage(hwnd,IDDI_TEXT_FORMAT_FROM_BOTTOM,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
      }
      if ( format & TEXT_FORMAT_CENTER )
         SendDlgItemMessage(hwnd,IDDI_TEXT_FORMAT_CENTER,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
      else {
         if ( format & TEXT_FORMAT_RIGHT )
            SendDlgItemMessage(hwnd,IDDI_TEXT_FORMAT_RIGHT,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
         else
            SendDlgItemMessage(hwnd,IDDI_TEXT_FORMAT_LEFT,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
      }
      }
      break;

   case WM_COMMAND: {
      if ( ! p ) 
         break;
      int id = LOWORD(wParam);
      int notificationCode = HIWORD(wParam);
      switch ( notificationCode ) {
      case EN_CHANGE: {
         char szTemp[32];
         switch ( id) {
         case IDDI_TEXT_XCOORDINATE:
            GetDlgItemText(hwnd,IDDI_TEXT_XCOORDINATE,szTemp,32);
            p -> propertyPositionX -> put_szValue(szTemp);
            break;
         case IDDI_TEXT_YCOORDINATE:
            GetDlgItemText(hwnd,IDDI_TEXT_YCOORDINATE,szTemp,32);
            p -> propertyPositionY -> put_szValue(szTemp);
            break;
         case IDDI_TEXT_ZCOORDINATE:
            GetDlgItemText(hwnd,IDDI_TEXT_ZCOORDINATE,szTemp,32);
            p -> propertyPositionZ -> put_szValue(szTemp);
            break;
         }
         }
         break;

      case BN_CLICKED: {
         switch ( id ) {
         case IDDI_TEXT_XYPLANE:
            if ( BST_CHECKED == SendDlgItemMessage(hwnd,id,BM_GETCHECK,0L,0L) )
               p -> coordinatePlane = CoordinatePlane_XY;
            break;
         case IDDI_TEXT_XZPLANE:
            if ( BST_CHECKED == SendDlgItemMessage(hwnd,id,BM_GETCHECK,0L,0L) )
               p -> coordinatePlane = CoordinatePlane_XZ;
            break;
         case IDDI_TEXT_YXPLANE:
            if ( BST_CHECKED == SendDlgItemMessage(hwnd,id,BM_GETCHECK,0L,0L) )
               p -> coordinatePlane = CoordinatePlane_YX;
            break;
         case IDDI_TEXT_YZPLANE:
            if ( BST_CHECKED == SendDlgItemMessage(hwnd,id,BM_GETCHECK,0L,0L) )
               p -> coordinatePlane = CoordinatePlane_YZ;
            break;
         case IDDI_TEXT_ZXPLANE:
            if ( BST_CHECKED == SendDlgItemMessage(hwnd,id,BM_GETCHECK,0L,0L) )
               p -> coordinatePlane = CoordinatePlane_ZX;
            break;
         case IDDI_TEXT_ZYPLANE:
            if ( BST_CHECKED == SendDlgItemMessage(hwnd,id,BM_GETCHECK,0L,0L) )
               p -> coordinatePlane = CoordinatePlane_ZY;
            break;
         case IDDI_TEXT_FLIP_LR: {
            long isChecked = (long)SendDlgItemMessage(hwnd,IDDI_TEXT_FLIP_LR,BM_GETCHECK,0L,0L);
            p -> flipHorizontal = isChecked == BST_CHECKED ? TRUE : FALSE;
            }
            break;
         case IDDI_TEXT_FLIP_TB: {
            long isChecked = (long)SendDlgItemMessage(hwnd,IDDI_TEXT_FLIP_TB,BM_GETCHECK,0L,0L);
            p -> flipVertical = isChecked == BST_CHECKED ? TRUE : FALSE;
            }
            break;
         case IDDI_TEXT_SCREENPLANE:
            if ( BST_CHECKED == SendDlgItemMessage(hwnd,id,BM_GETCHECK,0L,0L) )
               p -> coordinatePlane = CoordinatePlane_screen;
            break;

         case IDDI_TEXT_FORMAT_FROM_BOTTOM:
         case IDDI_TEXT_FORMAT_FROM_CENTER:
         case IDDI_TEXT_FORMAT_FROM_TOP: {
            long format;
            p -> propertyFormat -> get_longValue(&format);
            format = format & 0x000F;
            if ( IDDI_TEXT_FORMAT_FROM_BOTTOM == id ) {
               format |= TEXT_COORDINATES_FROM_BOTTOM;
            } else {
               if ( IDDI_TEXT_FORMAT_FROM_CENTER == id ) 
                  format |= TEXT_COORDINATES_FROM_CENTER;
               else 
                  format |= TEXT_COORDINATES_FROM_TOP;
            }
            p -> propertyFormat -> put_longValue(format);
            }
            break;

         case IDDI_TEXT_FORMAT_LEFT:
         case IDDI_TEXT_FORMAT_RIGHT:
         case IDDI_TEXT_FORMAT_CENTER: {
            long format;
            p -> propertyFormat -> get_longValue(&format);
            format = format & 0x00F0;
            if ( IDDI_TEXT_FORMAT_LEFT == id ) {
               format |= TEXT_FORMAT_LEFT;
            } else {
               if ( IDDI_TEXT_FORMAT_CENTER == id ) 
                  format |= TEXT_FORMAT_CENTER;
               else 
                  format |= TEXT_FORMAT_RIGHT;
            }
            p -> propertyFormat -> put_longValue(format);
            }
            break;

         }
         }
         break;
      }
      }
      break;
 
   case WM_NOTIFY: {
      if ( ! p ) break;
      int idControl = (int)wParam;
      if ( idControl == IDDI_TEXT_PLANEHEIGHT ) {
         p -> propertyPlaneHeight -> put_doubleValue(static_cast<double>(100 - SendDlgItemMessage(hwnd,IDDI_TEXT_PLANEHEIGHT,TBM_GETPOS,0L,0L))/100.0);
      }
      }
      break;
 
   default:
      break;
   }
 
   return LRESULT(FALSE);
   }

