// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "axis.h"
#include "Graphic_resource.h"


   LRESULT EXPENTRY Axis::handler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   Axis *p = (Axis *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch (msg) {
   case WM_CREATE: {
      CREATESTRUCT *pc = (CREATESTRUCT *)lParam;
      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)pc -> lpCreateParams);
      }
      return LRESULT(FALSE);
 
   case WM_COMMAND: {

      switch ( LOWORD(wParam) ) {

      case IDMI_AXIS_SET_PROPERTIES: {
         HWND hwndOwner = NULL;
         POINT ptCursor = {0};
         GetCursorPos(&ptCursor);
         hwndOwner = WindowFromPoint(ptCursor);
         IUnknown* pIUnknown;
         p -> QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknown));
         p -> pIGProperties -> ShowProperties(hwndOwner ? hwndOwner : GetForegroundWindow(),pIUnknown);
         pIUnknown -> Release();
         if ( p -> pWhenChangedCallback ) 
            p -> pWhenChangedCallback(p -> pWhenChangedCallbackArg,p -> whenChangedCallbackCookie);
         }
         }
         return LRESULT(TRUE);
 
      }
      return LRESULT(TRUE);
 
   }
 
   return DefWindowProc(hwnd,msg,wParam,lParam);
   }