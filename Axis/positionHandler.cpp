// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "axis.h"

#include "Graphic_resource.h"
#include "utils.h"


   LRESULT CALLBACK Axis::positionHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   Axis *p = (Axis *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);
      p = (Axis *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      short b;

      p -> propertyOriginAtMinpoint -> setWindowItemChecked(hwnd,IDDI_AXIS_POSITION_ORIGIN_MINPOINT);
      p -> propertyOriginXValue -> setWindowItemText(hwnd,IDDI_AXIS_POSITION_ORIGIN_X);
      p -> propertyOriginYValue -> setWindowItemText(hwnd,IDDI_AXIS_POSITION_ORIGIN_Y);
      p -> propertyOriginZValue -> setWindowItemText(hwnd,IDDI_AXIS_POSITION_ORIGIN_Z);

      p -> propertyOriginAtMinpoint -> get_boolValue(&b);
      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_POSITION_ORIGIN_X),b ? FALSE : TRUE);
      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_POSITION_ORIGIN_Y),b ? FALSE : TRUE);
      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_POSITION_ORIGIN_Z),b ? FALSE : TRUE);
      p -> propertyOriginXValue -> setWindowItemText(hwnd,IDDI_AXIS_POSITION_ORIGIN_X);
      p -> propertyOriginYValue -> setWindowItemText(hwnd,IDDI_AXIS_POSITION_ORIGIN_Y);
      p -> propertyOriginZValue -> setWindowItemText(hwnd,IDDI_AXIS_POSITION_ORIGIN_Z);

      p -> propertyEndpointAtMaxpoint -> setWindowItemChecked(hwnd,IDDI_AXIS_POSITION_ENDPOINT_MAXPOINT);
      p -> propertyEndpointXValue -> setWindowItemText(hwnd,IDDI_AXIS_POSITION_ENDPOINT_X);
      p -> propertyEndpointYValue -> setWindowItemText(hwnd,IDDI_AXIS_POSITION_ENDPOINT_Y);
      p -> propertyEndpointZValue -> setWindowItemText(hwnd,IDDI_AXIS_POSITION_ENDPOINT_Z);

      p -> propertyEndpointAtMaxpoint -> get_boolValue(&b);
      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_POSITION_ENDPOINT_X),b ? FALSE : TRUE);
      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_POSITION_ENDPOINT_Y),b ? FALSE : TRUE);
      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_POSITION_ENDPOINT_Z),b ? FALSE : TRUE);
      p -> propertyEndpointXValue -> setWindowItemText(hwnd,IDDI_AXIS_POSITION_ENDPOINT_X);
      p -> propertyEndpointYValue -> setWindowItemText(hwnd,IDDI_AXIS_POSITION_ENDPOINT_Y);
      p -> propertyEndpointZValue -> setWindowItemText(hwnd,IDDI_AXIS_POSITION_ENDPOINT_Z);

      RECT rect;  
      GetWindowRect(hwnd,&rect);
      SetWindowPos(GetDlgItem(hwnd,IDDI_AXIS_POSITION_NOTES),HWND_TOP,0,0,rect.right - rect.left - 40,64,SWP_NOMOVE);

      p -> propertyDeterminesDomain -> setWindowItemChecked(hwnd,IDDI_AXIS_STYLE_DETERMINESDOMAIN);
      switch ( p -> type ) {
      case 'X':
         SetWindowText(GetDlgItem(hwnd,IDDI_AXIS_STYLE_DETERMINESDOMAIN),"These values determine the X domain");
         break;
      case 'Y':
         SetWindowText(GetDlgItem(hwnd,IDDI_AXIS_STYLE_DETERMINESDOMAIN),"These values determine the Y domain");
         break;
      case 'Z':
         SetWindowText(GetDlgItem(hwnd,IDDI_AXIS_STYLE_DETERMINESDOMAIN),"These values determine the Z domain");
         break;
      default:
         SetWindowText(GetDlgItem(hwnd,IDDI_AXIS_STYLE_DETERMINESDOMAIN),"These values determine the axis domain");
         break;
      }

      }
      return LRESULT(0);
 

   case WM_COMMAND: {
      int controlID = LOWORD(wParam);
      int notificationCode = HIWORD(wParam);
      HWND hwndControl = (HWND)lParam;
      if ( ! p ) break;
      switch ( controlID ) {
      case IDDI_AXIS_POSITION_ORIGIN_MINPOINT: {
         short b;
         p -> propertyOriginAtMinpoint -> getWindowChecked(hwndControl);
         p -> propertyOriginAtMinpoint -> get_boolValue(&b);
         EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_POSITION_ORIGIN_X),b ? FALSE : TRUE);
         EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_POSITION_ORIGIN_Y),b ? FALSE : TRUE);
         EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_POSITION_ORIGIN_Z),b ? FALSE : TRUE);
         }
         break;

      case IDDI_AXIS_POSITION_ORIGIN_X:
         p -> propertyOriginXValue -> getWindowText(hwndControl);
         break;

      case IDDI_AXIS_POSITION_ORIGIN_Y:
         p -> propertyOriginYValue -> getWindowText(hwndControl);
         break;

      case IDDI_AXIS_POSITION_ORIGIN_Z:
         p -> propertyOriginZValue -> getWindowText(hwndControl);
         break;

      case IDDI_AXIS_POSITION_ENDPOINT_MAXPOINT: {
         short b;
         p -> propertyEndpointAtMaxpoint -> getWindowChecked(hwndControl);
         p -> propertyEndpointAtMaxpoint -> get_boolValue(&b);
         EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_POSITION_ENDPOINT_X),b ? FALSE : TRUE);
         EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_POSITION_ENDPOINT_Y),b ? FALSE : TRUE);
         EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_POSITION_ENDPOINT_Z),b ? FALSE : TRUE);
         }
         break;

      case IDDI_AXIS_POSITION_ENDPOINT_X:
         p -> propertyEndpointXValue -> getWindowText(hwndControl);
         break;

      case IDDI_AXIS_POSITION_ENDPOINT_Y:
         p -> propertyEndpointYValue -> getWindowText(hwndControl);
         break;

      case IDDI_AXIS_POSITION_ENDPOINT_Z:
         p -> propertyEndpointZValue -> getWindowText(hwndControl);
         break;

      case IDDI_AXIS_STYLE_DETERMINESDOMAIN:
         p -> propertyDeterminesDomain -> getWindowChecked(hwndControl);
         break;

      default:
         break;
      }
      }
      return LRESULT(0);

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

      }

      }
      break;

   default:
      break;
   }
   return LRESULT(0);
   }
