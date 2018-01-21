// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"

#include "Graphic_resource.h"
#include "utils.h"

   LRESULT CALLBACK G::posSizeHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   G *p = (G *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);

      p = (G *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      p -> hwndAppearanceSettings = hwnd;

      long k;
      char unitsArray[][32] = UNITS_ARRAY;

      HWND hwndTemp = GetDlgItem(hwnd,IDDI_GRAPHIC_SIZE_MARGINUNITS);

      k = 0;
      SendMessage(hwndTemp,CB_RESETCONTENT,0L,0L);
      while ( *unitsArray[k] ) 
         SendMessage(hwndTemp,CB_ADDSTRING,0,(LPARAM)unitsArray[k++]);

      p -> propertyPlotMarginUnits -> setWindowItemComboBoxSelection(p -> hwndAppearanceSettings,IDDI_GRAPHIC_SIZE_MARGINUNITS);
      p -> propertyPlotTopMargin -> setWindowItemText(p -> hwndAppearanceSettings,IDDI_GRAPHIC_SIZE_TOPMARGIN);
      p -> propertyPlotBottomMargin -> setWindowItemText(p -> hwndAppearanceSettings,IDDI_GRAPHIC_SIZE_BOTTOMMARGIN);
      p -> propertyPlotLeftMargin -> setWindowItemText(p -> hwndAppearanceSettings,IDDI_GRAPHIC_SIZE_LEFTMARGIN);
      p -> propertyPlotRightMargin -> setWindowItemText(p -> hwndAppearanceSettings,IDDI_GRAPHIC_SIZE_RIGHTMARGIN);
      p -> propertyPlotMarginsStretchAll -> setWindowItemChecked(p -> hwndAppearanceSettings,IDDI_GRAPHIC_SIZE_FITALL);
   
      }
      return LRESULT(FALSE);


   case WM_CHILDACTIVATE: {

      }
      return LRESULT(FALSE);
 
 
   case WM_COMMAND: {
      int controlID = LOWORD(wParam);
      switch ( controlID ) {
      case IDDI_GRAPHIC_SIZE_MARGINUNITS: {
         if ( LBN_SELCHANGE == HIWORD(wParam) ) {
            p -> propertyPlotMarginUnits -> getWindowItemComboBoxSelection(hwnd,IDDI_GRAPHIC_SIZE_MARGINUNITS);
         }
         }
         break;
      case IDDI_GRAPHIC_SIZE_FITALL:
         p -> propertyPlotMarginsStretchAll -> getWindowItemChecked(hwnd,IDDI_GRAPHIC_SIZE_FITALL);
         break;
      case IDDI_GRAPHIC_SIZE_LEFTMARGIN:
      case IDDI_GRAPHIC_SIZE_RIGHTMARGIN:
      case IDDI_GRAPHIC_SIZE_BOTTOMMARGIN:
      case IDDI_GRAPHIC_SIZE_TOPMARGIN: {
         IGProperty* pp = (IGProperty*)NULL;
         if ( EN_CHANGE == HIWORD(wParam) ) {
            switch ( controlID ) {
            case IDDI_GRAPHIC_SIZE_LEFTMARGIN:
               (pp = p -> propertyPlotLeftMargin) -> getWindowItemValue(hwnd,IDDI_GRAPHIC_SIZE_LEFTMARGIN);
               break;
            case IDDI_GRAPHIC_SIZE_RIGHTMARGIN:
               (pp = p -> propertyPlotRightMargin) -> getWindowItemValue(hwnd,IDDI_GRAPHIC_SIZE_RIGHTMARGIN);
               break;
            case IDDI_GRAPHIC_SIZE_BOTTOMMARGIN:
               (pp = p -> propertyPlotBottomMargin) -> getWindowItemValue(hwnd,IDDI_GRAPHIC_SIZE_BOTTOMMARGIN);
               break;
            case IDDI_GRAPHIC_SIZE_TOPMARGIN:
               (pp = p -> propertyPlotTopMargin) -> getWindowItemValue(hwnd,IDDI_GRAPHIC_SIZE_TOPMARGIN);
               break;
            }
            if ( strcmp(p -> plotMarginUnits,"percent") == 0 ) {
               long v;
               pp -> get_longValue(&v);
               if ( v > 100 ) {
                  MessageBeep(MB_ICONASTERISK);
                  pp -> put_longValue(100);
                  pp -> setWindowItemText(hwnd,controlID);
               }
            }
         }
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