// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"

#include "Graphic_resource.h"
 
   LRESULT CALLBACK G::axisHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   G *p = (G *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);

      p = (G *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      p -> hwndAxisSettings = hwnd;

      p -> propertyRenderOpenGLAxisText -> setWindowChecked(GetDlgItem(hwnd,IDDI_GRAPHIC_AXIIS_USE_OPENGL));

      BSTR bstrLabel;

      p -> xaxis -> get_LabelText(&bstrLabel);

      SetDlgItemTextW(hwnd,IDDI_GRAPHIC_AXIIS_XAXIS_TEXT,bstrLabel);

      SysFreeString(bstrLabel);

      p -> yaxis -> get_LabelText(&bstrLabel);

      SetDlgItemTextW(hwnd,IDDI_GRAPHIC_AXIIS_YAXIS_TEXT,bstrLabel);

      SysFreeString(bstrLabel);

      p -> zaxis -> get_LabelText(&bstrLabel);

      SetDlgItemTextW(hwnd,IDDI_GRAPHIC_AXIIS_ZAXIS_TEXT,bstrLabel);

      SysFreeString(bstrLabel);


      }
      return LRESULT(FALSE);
 
   case WM_COMMAND: {
      int notifyCode = HIWORD(wParam);
      switch ( LOWORD(wParam) ) {
      case IDDI_GRAPHIC_AXIIS_USE_OPENGL:
         p -> propertyRenderOpenGLAxisText -> getWindowChecked(GetDlgItem(hwnd,IDDI_GRAPHIC_AXIIS_USE_OPENGL));
         break;

      case IDDI_GRAPHIC_AXIIS_XAXIS_EDIT: {
         p -> xaxis -> ShowProperties(hwnd);
         BSTR bstrLabel;
         p -> xaxis -> get_LabelText(&bstrLabel);
         SetDlgItemTextW(hwnd,IDDI_GRAPHIC_AXIIS_XAXIS_TEXT,bstrLabel);
         SysFreeString(bstrLabel);
         }
         break;

      case IDDI_GRAPHIC_AXIIS_YAXIS_EDIT: {
         p -> yaxis -> ShowProperties(hwnd);
         BSTR bstrLabel;
         p -> yaxis -> get_LabelText(&bstrLabel);
         SetDlgItemTextW(hwnd,IDDI_GRAPHIC_AXIIS_YAXIS_TEXT,bstrLabel);
         SysFreeString(bstrLabel);
         }
         break;

      case IDDI_GRAPHIC_AXIIS_ZAXIS_EDIT: {
         p -> zaxis -> ShowProperties(hwnd);
         BSTR bstrLabel;
         p -> zaxis -> get_LabelText(&bstrLabel);
         SetDlgItemTextW(hwnd,IDDI_GRAPHIC_AXIIS_ZAXIS_TEXT,bstrLabel);
         SysFreeString(bstrLabel);
         }
         break;


      default:
         break;
      }
      }
      break;
 
   default:
      break;
   }
   return LRESULT(FALSE);
   }
 
 