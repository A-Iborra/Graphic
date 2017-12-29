/*

                       Copyright (c) 1996,1997,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <commctrl.h>

#include "utils.h"
#include "Graphic.h"

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