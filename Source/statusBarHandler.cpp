// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"
#include <commctrl.h>

#include "utils.h"

   LRESULT CALLBACK G::statusBarHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
   G *p = (G *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {
   case WM_CREATE:
      SetWindowLongPtr(hwnd,GWLP_USERDATA,lParam);
      return (LRESULT)FALSE;

//   case WM_MOVE:
//ShowWindow(hwnd,SW_SHOW);
//   case WM_SHOWWINDOW:
//      Beep(2000,100);
//      break;

   case SB_SETTEXT: {
      CallWindowProc(p -> defaultStatusBarHandler,hwnd,msg,wParam,lParam);
      char *psz = (char *)lParam;
      delete [] psz;
      }
      return (LRESULT)FALSE;

   default:
      break;
   }
   return CallWindowProc(p -> defaultStatusBarHandler,hwnd,msg,wParam,lParam);
   }