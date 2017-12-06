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


   LRESULT CALLBACK Text::orientationHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
   Text *p = (Text *)GetWindowLong(hwnd,GWL_USERDATA);
 
   switch ( msg ) {
 
   case WM_INITDIALOG: {
      PROPSHEETPAGE *pPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);
      p = (Text *)pPage -> lParam;
      SetWindowLong(hwnd,GWL_USERDATA,(long)p);
      }
      return LRESULT(TRUE);
 
   case WM_COMMAND: {
      if ( ! p ) break;
      int id = LOWORD(wParam);
      int notificationCode = HIWORD(wParam);
      switch ( notificationCode ) {
      case EN_CHANGE: {
         char szTemp[32];
         GetDlgItemText(hwnd,IDDI_TEXT_XCOORDINATE,szTemp,32);
         p -> propertyPositionX -> put_szValue(szTemp);
         GetDlgItemText(hwnd,IDDI_TEXT_YCOORDINATE,szTemp,32);
         p -> propertyPositionY -> put_szValue(szTemp);
         GetDlgItemText(hwnd,IDDI_TEXT_ZCOORDINATE,szTemp,32);
         p -> propertyPositionZ -> put_szValue(szTemp);
         }
         break;

      case BN_CLICKED: {
         switch ( id ) {
         case IDDI_TEXT_XYPLANE:
            p -> coordinatePlane = CoordinatePlane_XY;
            break;
         case IDDI_TEXT_XZPLANE:
            p -> coordinatePlane = CoordinatePlane_XZ;
            break;
         case IDDI_TEXT_YXPLANE:
            p -> coordinatePlane = CoordinatePlane_YX;
            break;
         case IDDI_TEXT_YZPLANE:
            p -> coordinatePlane = CoordinatePlane_YZ;
            break;
         case IDDI_TEXT_ZXPLANE:
            p -> coordinatePlane = CoordinatePlane_ZX;
            break;
         case IDDI_TEXT_ZYPLANE:
            p -> coordinatePlane = CoordinatePlane_ZY;
            break;
         case IDDI_TEXT_FLIP_LR: {
            long isChecked = SendDlgItemMessage(hwnd,IDDI_TEXT_FLIP_LR,BM_GETCHECK,0L,0L);
            p -> flipHorizontal = isChecked == BST_CHECKED ? TRUE : FALSE;
            }
            break;
         case IDDI_TEXT_FLIP_TB: {
            long isChecked = SendDlgItemMessage(hwnd,IDDI_TEXT_FLIP_TB,BM_GETCHECK,0L,0L);
            p -> flipVertical = isChecked == BST_CHECKED ? TRUE : FALSE;
            }
            break;
         case IDDI_TEXT_SCREENPLANE:
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

