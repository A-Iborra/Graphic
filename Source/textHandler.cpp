/*

                       Copyright (c) 1996,1997,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include "Graphic_resource.h"

#include "Graphic.h"

   LRESULT CALLBACK G::textHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
   G *p = (G *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
   switch ( msg ) {
   case WM_INITDIALOG: {
      PROPSHEETPAGE *pPage = (PROPSHEETPAGE *)lParam;
      p = (G *)pPage -> lParam;
      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);
      p -> hwndTextSettings = hwnd;
      }
      return LRESULT(FALSE);
 
   default:
      break;
   }
   return LRESULT(FALSE);
   }
