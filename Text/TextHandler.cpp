// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "text.h"

#include "Graphic_resource.h"

   LRESULT EXPENTRY Text::handler(HWND hwnd,UINT msg,WPARAM mp1,LPARAM mp2) {

   Text *p = (Text *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch (msg) {
   case WM_CREATE: {
      CREATESTRUCT *pc = (CREATESTRUCT *)mp2;
      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)pc -> lpCreateParams);
      }
      return LRESULT(FALSE);
 
   case WM_COMMAND: {
      switch ( LOWORD(mp1) ) {
      case IDMI_TEXT_PROPERTIES: {
         HWND hwndOwner = NULL;
         POINT ptCursor = {0};
         GetCursorPos(&ptCursor);
         hwndOwner = WindowFromPoint(ptCursor);
         IUnknown* pIUnknown;
         p -> QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknown));
         p -> pIProperties -> ShowProperties(hwndOwner,pIUnknown);
         pIUnknown -> Release();       
         if ( p -> pWhenChangedCallback ) 
            p -> pWhenChangedCallback(p -> pWhenChangedCallbackArg,p -> whenChangedCallbackCookie);
         }
         return LRESULT(TRUE);
 
      case IDMI_TEXT_DELETE: {
         if ( p -> pITextNotify ) {
            IText *pIText;
            p -> QueryInterface(IID_IText,reinterpret_cast<void **>(&pIText));
            pIText -> Release();
            p -> pITextNotify -> DeleteText(pIText);
         }
         if ( p -> pWhenChangedCallback ) 
            p -> pWhenChangedCallback(p -> pWhenChangedCallbackArg,p -> whenChangedCallbackCookie);
         }
         return LRESULT(TRUE);
 
      }
      }
      return LRESULT(TRUE);
 
   case WM_PAINT:
      Beep(1000,100);
 
   }
 
   return DefWindowProc(hwnd,msg,mp1,mp2);
   }