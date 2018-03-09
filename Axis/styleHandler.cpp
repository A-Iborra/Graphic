// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "axis.h"

#include "Graphic_resource.h"
#include "utils.h"

   LRESULT CALLBACK Axis::styleHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   Axis *p = (Axis *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch ( msg ) {

   case WM_INITDIALOG: {
 
      PROPSHEETPAGE *pPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);

      p = (Axis *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      HWND hwndTemp;
      long k;
      char unitsArray[][32] = UNITS_ARRAY;
      char szTemp[128];
      long v;
 
      hwndTemp = GetDlgItem(hwnd,IDDI_AXIS_STYLE_TICKLENGTH_UNITS);
      k = 0;
      while ( *unitsArray[k] ) {
         SendMessage(hwndTemp,CB_ADDSTRING,0,(LPARAM)unitsArray[k]);
         k++;
      } 

      p -> propertyLineWeight -> setWindowItemText(hwnd,IDDI_AXIS_STYLE_LINEWEIGHT);

      p -> propertyGridLinesPerTick -> setWindowItemText(hwnd,IDDI_AXIS_STYLE_GRIDSPERTICK);

      sprintf(szTemp,"%6.2lf",p -> tickLength);
      SetWindowText(GetDlgItem(hwnd,IDDI_AXIS_STYLE_TICKLENGTH),szTemp);

      p -> propertyTickLengthUnits -> get_longValue(&v);

      SendMessage(GetDlgItem(hwnd,IDDI_AXIS_STYLE_TICKLENGTH_UNITS),CB_GETLBTEXT,v,(LPARAM)szTemp);
      SendMessage(GetDlgItem(hwnd,IDDI_AXIS_STYLE_TICKLENGTH_UNITS),CB_SELECTSTRING,0,(LPARAM)szTemp);

      p -> propertyTickCount -> setWindowItemText(hwnd,IDDI_AXIS_STYLE_TICKCOUNT);

      p -> propertyTicksAllPlanes -> setWindowItemChecked(hwnd,IDDI_AXIS_STYLE_TICKS_ON_BOTH_PLANES);

      p -> propertyTickStraddleStyle -> get_longValue(&v);

      p -> propertyIsHidden -> setWindowItemChecked(hwnd,IDDI_AXIS_STYLE_HIDE);

      switch ( v ) {
      case TICK_STRADDLE_ABOVE:
         SendMessage(GetDlgItem(hwnd,IDDI_AXIS_STYLE_TICKSABOVE),BM_SETCHECK,(WPARAM)BST_CHECKED,0);
         break;
      case TICK_STRADDLE_BOTH:
         SendMessage(GetDlgItem(hwnd,IDDI_AXIS_STYLE_TICKSACCROSS),BM_SETCHECK,(WPARAM)BST_CHECKED,0);
         ShowWindow(GetDlgItem(hwnd,IDDI_AXIS_STYLE_MSG_PCNTABOVE),SW_SHOW);
         ShowWindow(GetDlgItem(hwnd,IDDI_AXIS_STYLE_PCNTABOVE),SW_SHOW);
         break;
      case TICK_STRADDLE_BELOW:
         SendMessage(GetDlgItem(hwnd,IDDI_AXIS_STYLE_TICKSBELOW),BM_SETCHECK,(WPARAM)BST_CHECKED,0);
         break;
      }

      sprintf(szTemp,"%5.1lf",p -> tickPctAbove);
      SetWindowText(GetDlgItem(hwnd,IDDI_AXIS_STYLE_PCNTABOVE),szTemp);

      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_STYLE_TICKLENGTH),p -> tickCount ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_STYLE_TICKLENGTH_UNITS),p -> tickCount ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_STYLE_TICKSABOVE),p -> tickCount ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_STYLE_TICKSACCROSS),p -> tickCount ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_STYLE_TICKSBELOW),p -> tickCount ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_STYLE_PCNTABOVE),p -> tickCount ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_STYLE_GRIDSPERTICK),p -> tickCount ? TRUE : FALSE);

      }
      return LRESULT(0);
 

   case WM_COMMAND: {
      HWND hwndControl = (HWND)lParam;
      int controlID = LOWORD(wParam);
      int notificationCode = HIWORD(wParam);
      if ( ! p ) break;
      switch ( controlID ) {

      case IDDI_AXIS_STYLE_LINEWEIGHT:
         p -> propertyLineWeight -> getWindowItemValue(hwnd,IDDI_AXIS_STYLE_LINEWEIGHT);
         break;

      case IDDI_AXIS_STYLE_TICKCOUNT:
         p -> propertyTickCount -> getWindowItemValue(hwnd,IDDI_AXIS_STYLE_TICKCOUNT);
         EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_STYLE_TICKLENGTH),p -> tickCount ? TRUE : FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_STYLE_TICKLENGTH_UNITS),p -> tickCount ? TRUE : FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_STYLE_TICKSABOVE),p -> tickCount ? TRUE : FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_STYLE_TICKSACCROSS),p -> tickCount ? TRUE : FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_STYLE_TICKSBELOW),p -> tickCount ? TRUE : FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_STYLE_PCNTABOVE),p -> tickCount ? TRUE : FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_STYLE_GRIDSPERTICK),p -> tickCount ? TRUE : FALSE);
         break;

      case IDDI_AXIS_STYLE_TICKSABOVE:
      case IDDI_AXIS_STYLE_TICKSACCROSS:
      case IDDI_AXIS_STYLE_TICKSBELOW: {
         if ( BN_CLICKED == HIWORD(wParam) ) {
            if ( controlID == IDDI_AXIS_STYLE_TICKSACCROSS ) {
               ShowWindow(GetDlgItem(hwnd,IDDI_AXIS_STYLE_MSG_PCNTABOVE),SW_SHOW);
               ShowWindow(GetDlgItem(hwnd,IDDI_AXIS_STYLE_PCNTABOVE),SW_SHOW);
            } else {
               ShowWindow(GetDlgItem(hwnd,IDDI_AXIS_STYLE_MSG_PCNTABOVE),SW_HIDE);
               ShowWindow(GetDlgItem(hwnd,IDDI_AXIS_STYLE_PCNTABOVE),SW_HIDE);
            }
         }
         if ( SendMessage(GetDlgItem(hwnd,IDDI_AXIS_STYLE_TICKSABOVE),BM_GETSTATE,0,0) & BST_CHECKED ) 
            p -> propertyTickStraddleStyle -> put_longValue(TICK_STRADDLE_ABOVE);
         else {
            if ( SendMessage(GetDlgItem(hwnd,IDDI_AXIS_STYLE_TICKSACCROSS),BM_GETSTATE,0,0) & BST_CHECKED )
               p -> propertyTickStraddleStyle -> put_longValue(TICK_STRADDLE_BOTH);
            else {
               if ( SendMessage(GetDlgItem(hwnd,IDDI_AXIS_STYLE_TICKSBELOW),BM_GETSTATE,0,0) & BST_CHECKED )
                  p -> propertyTickStraddleStyle -> put_longValue(TICK_STRADDLE_BELOW);
            }
         }
         }
         break;
 
      case IDDI_AXIS_STYLE_PCNTABOVE:
         p -> propertyTickStraddleAbove -> getWindowItemValue(hwnd,IDDI_AXIS_STYLE_PCNTABOVE);
         break;

      case IDDI_AXIS_STYLE_TICKLENGTH:
         p -> propertyTickLength -> getWindowItemValue(hwnd,IDDI_AXIS_STYLE_TICKLENGTH);
         break;

      case IDDI_AXIS_STYLE_TICKLENGTH_UNITS:
         if ( CBN_SELCHANGE == notificationCode ) 
            p -> propertyTickLengthUnits -> put_longValue((long)SendMessage(hwndControl,CB_GETCURSEL,0L,0L));
         break;

      case IDDI_AXIS_STYLE_GRIDSPERTICK:
         p -> propertyGridLinesPerTick -> getWindowItemValue(hwnd,IDDI_AXIS_STYLE_GRIDSPERTICK); 
         break;

      case IDDI_AXIS_STYLE_TICKS_ON_BOTH_PLANES:
         p -> propertyTicksAllPlanes -> getWindowItemChecked(hwnd,IDDI_AXIS_STYLE_TICKS_ON_BOTH_PLANES);
         break;

      case IDDI_AXIS_STYLE_HIDE:
         p -> propertyIsHidden -> getWindowItemChecked(hwnd,IDDI_AXIS_STYLE_HIDE);
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
