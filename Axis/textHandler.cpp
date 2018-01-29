// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "axis.h"
#include <CommCtrl.h>

#include "Graphic_resource.h"
#include "utils.h"


   LRESULT CALLBACK Axis::textHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   Axis *p = (Axis *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);
      p = (Axis *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      char unitsArray[][32] = TEXT_UNITS_ARRAY;
 
      long k = 0;
      while ( *unitsArray[k] ) 
         SendMessage(GetDlgItem(hwnd,IDDI_AXIS_TEXT_TICK_LABEL_SIZE_UNITS),CB_ADDSTRING,0,(LPARAM)unitsArray[k++]);

      long v;
      double d;
      char szTemp[32];

      if ( p -> doAutoPrecision ) {
         SendMessage(GetDlgItem(hwnd,IDDI_AXIS_TEXT_AUTOPRECISION),BM_SETCHECK,(WPARAM)BST_CHECKED,0);
      } else {
         SendMessage(GetDlgItem(hwnd,IDDI_AXIS_TEXT_SPECIFICPRECISION),BM_SETCHECK,(WPARAM)BST_CHECKED,0);
      }

      p -> propertyAxisPrecision -> get_doubleValue(&d);
      sprintf(szTemp,"%8.3f",d);
      SetDlgItemText(hwnd,IDDI_AXIS_TEXT_AXISTICKLABELPRECISION_VALUE,szTemp);

      p -> propertyLabel -> setWindowText(GetDlgItem(hwnd,IDDI_AXIS_TEXT_LABELTEXT));
      long n;
      p -> propertyLabel -> get_size(&n);

      p -> propertyDrawLabel -> put_boolValue(1 < n ? TRUE : FALSE);

      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_LABEL_POSITION_NATURAL),p -> drawAxisLabel ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_LABEL_POSITION_X),p -> drawAxisLabel ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_LABEL_POSITION_Y),p -> drawAxisLabel ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_LABEL_POSITION_Z),p -> drawAxisLabel ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_LABEL_ANGLE_PERPENDICULAR),p -> drawAxisLabel ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_LABEL_ANGLE_PARALLEL),p -> drawAxisLabel ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_LABEL_ANGLE_ISOTHER),p -> drawAxisLabel ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_LABEL_ANGLE_OTHER),p -> drawAxisLabel ? TRUE : FALSE);

      p -> propertyLabelPositionNatural -> setWindowItemChecked(hwnd,IDDI_AXIS_TEXT_LABEL_POSITION_NATURAL);
      p -> propertyLabelAnglePerpendicular -> setWindowItemChecked(hwnd,IDDI_AXIS_TEXT_LABEL_ANGLE_PERPENDICULAR);
      p -> propertyLabelAngleParallel -> setWindowItemChecked(hwnd,IDDI_AXIS_TEXT_LABEL_ANGLE_PARALLEL);
      p -> propertyLabelAngleOther -> setWindowItemChecked(hwnd,IDDI_AXIS_TEXT_LABEL_ANGLE_ISOTHER);
      p -> propertyLabelAngleOtherValue -> setWindowItemText(hwnd,IDDI_AXIS_TEXT_LABEL_ANGLE_OTHER);

      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_LABEL_ANGLE_OTHER),p -> labelAngleOther ? TRUE : FALSE);

      p -> propertyLabelPositionNatural -> setWindowItemChecked(hwnd,IDDI_AXIS_TEXT_LABEL_POSITION_NATURAL);
      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_LABEL_POSITION_X),p -> labelPositionNatural ? FALSE : TRUE);
      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_LABEL_POSITION_Y),p -> labelPositionNatural ? FALSE : TRUE);
      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_LABEL_POSITION_Z),p -> labelPositionNatural ? FALSE : TRUE);

      p -> propertyTickLabelSize -> get_doubleValue(&d);

      sprintf(szTemp,"%6.1f",d);
      SetDlgItemText(hwnd,IDDI_AXIS_TEXT_TICK_LABEL_SIZE,szTemp);

      p -> propertyTickLabelSizeUnits -> get_longValue(&v);
      SendMessage(GetDlgItem(hwnd,IDDI_AXIS_TEXT_TICK_LABEL_SIZE_UNITS),CB_GETLBTEXT,v,(LPARAM)szTemp);
      SendMessage(GetDlgItem(hwnd,IDDI_AXIS_TEXT_TICK_LABEL_SIZE_UNITS),CB_SELECTSTRING,0,(LPARAM)szTemp);

      p -> propertyDrawText -> setWindowItemChecked(hwnd,IDDI_AXIS_TEXT_DISPLAYTICKLABELS);
      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_SPECIFICPRECISION),p -> drawTickLabels ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_AUTOPRECISION),p -> drawTickLabels ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_AXISTICKLABELPRECISION_VALUE),p -> drawTickLabels && ! p -> doAutoPrecision ? TRUE : FALSE);

      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_TICK_LABEL_SIZE),p -> drawTickLabels ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_TICK_LABEL_SIZE_UNITS),p -> drawTickLabels ? TRUE : FALSE);

      }
      return LRESULT(FALSE);
 
   case WM_COMMAND: {

      HWND hwndControl = (HWND)lParam;
      int notificationCode = HIWORD(wParam);
 
      switch ( LOWORD(wParam) ) {
 
      case IDDI_AXIS_TEXT_DISPLAYTICKLABELS: 

         if ( SendMessage(GetDlgItem(hwnd,IDDI_AXIS_TEXT_DISPLAYTICKLABELS),BM_GETCHECK,0,0) == (LRESULT)BST_CHECKED )  {

            p -> propertyDrawText -> put_boolValue(TRUE);

            EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_SPECIFICPRECISION),TRUE);
            EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_AUTOPRECISION),TRUE);
            EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_TICK_LABEL_SIZE),TRUE);
            EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_TICK_LABEL_SIZE_UNITS),TRUE);

            if ( SendMessage(GetDlgItem(hwnd,IDDI_AXIS_TEXT_SPECIFICPRECISION),BM_GETCHECK,0,0) == (LRESULT)BST_CHECKED )
               EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_AXISTICKLABELPRECISION_VALUE),TRUE);
            else
               EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_AXISTICKLABELPRECISION_VALUE),FALSE);
         } else {

            p -> propertyDrawText -> put_boolValue(FALSE);

            EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_SPECIFICPRECISION),FALSE);
            EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_AUTOPRECISION),FALSE);
            EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_AXISTICKLABELPRECISION_VALUE),FALSE);
            EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_TICK_LABEL_SIZE),FALSE);
            EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_TICK_LABEL_SIZE_UNITS),FALSE);
         }
         break;
 
      case IDDI_AXIS_TEXT_AUTOPRECISION: 
         if ( SendMessage(GetDlgItem(hwnd,IDDI_AXIS_TEXT_AUTOPRECISION),BM_GETCHECK,0,0) == (LRESULT)BST_CHECKED ) {
            p -> propertyAutoPrecision -> put_boolValue(TRUE);
            EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_AXISTICKLABELPRECISION_VALUE),FALSE);
         } else {
            p -> doAutoPrecision = FALSE;
            EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_AXISTICKLABELPRECISION_VALUE),TRUE);
         }
         break;
       
      case IDDI_AXIS_TEXT_SPECIFICPRECISION: 
         if ( SendMessage(GetDlgItem(hwnd,IDDI_AXIS_TEXT_SPECIFICPRECISION),BM_GETCHECK,0,0) == (LRESULT)BST_CHECKED ) {
            p -> doAutoPrecision = FALSE;
            EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_AXISTICKLABELPRECISION_VALUE),TRUE);
         } else {
            p -> doAutoPrecision = TRUE;
            EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_AXISTICKLABELPRECISION_VALUE),FALSE);
         }
         break;
 
      case IDDI_AXIS_TEXT_AXISTICKLABELPRECISION_VALUE: {
         char szTemp[32];
         GetDlgItemText(hwnd,IDDI_AXIS_TEXT_AXISTICKLABELPRECISION_VALUE,szTemp,32);
         double d = atof(szTemp);
         p -> propertyAxisPrecision -> put_doubleValue(d);
         }
         break;
 
      case IDDI_AXIS_TEXT_TICK_LABEL_SIZE:
         p -> propertyTickLabelSize -> getWindowValue(hwndControl);
         break;

      case IDDI_AXIS_TEXT_TICK_LABEL_SIZE_UNITS:
         if ( CBN_SELCHANGE == notificationCode ) 
            p -> propertyTickLabelSizeUnits -> put_longValue((long)SendMessage(hwndControl,CB_GETCURSEL,0L,0L));
         break;

      case IDDI_AXIS_TEXT_LABELTEXT: {
         long n;
         p -> propertyLabel -> getWindowItemText(hwnd,IDDI_AXIS_TEXT_LABELTEXT);
         p -> propertyLabel -> get_size(&n);
         p -> propertyDrawLabel -> put_boolValue(1 < n ? TRUE : FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_LABEL_POSITION_NATURAL),p -> drawAxisLabel ? TRUE : FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_LABEL_POSITION_X),p -> drawAxisLabel ? ! p -> labelPositionNatural : FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_LABEL_POSITION_Y),p -> drawAxisLabel ? ! p -> labelPositionNatural : FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_LABEL_POSITION_Z),p -> drawAxisLabel ? ! p -> labelPositionNatural : FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_LABEL_ANGLE_PERPENDICULAR),p -> drawAxisLabel ? TRUE : FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_LABEL_ANGLE_PARALLEL),p -> drawAxisLabel ? TRUE : FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_LABEL_ANGLE_ISOTHER),p -> drawAxisLabel ? TRUE : FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_LABEL_ANGLE_OTHER),p -> drawAxisLabel ? p -> labelAngleOther : FALSE);
         }
         break;

      case IDDI_AXIS_TEXT_LABEL_POSITION_NATURAL:
         p -> propertyLabelPositionNatural -> getWindowChecked(hwndControl);
         EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_LABEL_POSITION_X),p -> labelPositionNatural ? FALSE : TRUE);
         EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_LABEL_POSITION_Y),p -> labelPositionNatural ? FALSE : TRUE);
         EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_LABEL_POSITION_Z),p -> labelPositionNatural ? FALSE : TRUE);
         break;

      case IDDI_AXIS_TEXT_LABEL_POSITION_X:
         p -> propertyLabelPositionXValue -> getWindowValue(hwndControl);
         break;

      case IDDI_AXIS_TEXT_LABEL_POSITION_Y:
         p -> propertyLabelPositionYValue -> getWindowValue(hwndControl);
         break;

      case IDDI_AXIS_TEXT_LABEL_POSITION_Z:
         p -> propertyLabelPositionZValue -> getWindowValue(hwndControl);
         break;

      case IDDI_AXIS_TEXT_LABEL_ANGLE_PERPENDICULAR:
         p -> propertyLabelAnglePerpendicular -> getWindowChecked(hwndControl);
         p -> propertyLabelAngleOther -> getWindowItemChecked(hwnd,IDDI_AXIS_TEXT_LABEL_ANGLE_ISOTHER);
         EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_LABEL_ANGLE_OTHER),p -> labelAngleOther ? TRUE : FALSE);
         break;

      case IDDI_AXIS_TEXT_LABEL_ANGLE_PARALLEL:
         p -> propertyLabelAngleParallel -> getWindowChecked(hwndControl);
         p -> propertyLabelAngleOther -> getWindowItemChecked(hwnd,IDDI_AXIS_TEXT_LABEL_ANGLE_ISOTHER);
         EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_LABEL_ANGLE_OTHER),p -> labelAngleOther ? TRUE : FALSE);
         break;

      case IDDI_AXIS_TEXT_LABEL_ANGLE_ISOTHER:
         p -> propertyLabelAngleOther -> getWindowChecked(hwndControl);
         EnableWindow(GetDlgItem(hwnd,IDDI_AXIS_TEXT_LABEL_ANGLE_OTHER),p -> labelAngleOther ? TRUE : FALSE);
         break;

      case IDDI_AXIS_TEXT_LABEL_ANGLE_OTHER:
         p -> propertyLabelAngleOtherValue -> getWindowValue(hwndControl);
         break;

      default:
         break;
      }
      break;

      }
      break;

   case WM_NOTIFY: {
      NM_UPDOWN *pn = (NM_UPDOWN *)lParam;
      if ( pn -> hdr.code != UDN_DELTAPOS ) break;
      double x;
      char szTemp[32];
      if ( pn -> hdr.code != UDN_DELTAPOS ) break;
      if ( pn -> hdr.idFrom != (unsigned int)IDDI_AXIS_TEXT_AXISPRECISIONSPIN ) break;
      GetDlgItemText(hwnd,IDDI_AXIS_TEXT_AXISTICKLABELPRECISION_VALUE,szTemp,32);
      x = atof(szTemp);
      x -= (float)pn -> iDelta * 0.25;
      sprintf(szTemp,"%8.3f",x);
      p -> propertyAxisPrecision -> put_doubleValue(x);
      SetDlgItemText(hwnd,IDDI_AXIS_TEXT_AXISTICKLABELPRECISION_VALUE,szTemp);
      }
      break;
 
   default:
      break;
 
   }
 
   return LRESULT(FALSE);
   }
