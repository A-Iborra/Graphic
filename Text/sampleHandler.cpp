/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include "Graphic_resource.h"
#include "utils.h"

#include "text.h"

   extern HMODULE hModule;

   static int holdUpdates = FALSE;

   LRESULT CALLBACK Text::sampleHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
   Text *p = (Text *)GetWindowLong(hwnd,GWL_USERDATA);
 
   switch ( msg ) {
   case WM_INITDIALOG:
      return LRESULT(TRUE);
 
   case WM_PAINT: {
      PAINTSTRUCT ps;
      BeginPaint(hwnd,&ps);
      EndPaint(hwnd,&ps);
      if ( p ) p -> drawSample();
      }
      return LRESULT(TRUE);

   default:
      break;
 
   }

   if ( p )
      return CallWindowProc(p -> oldSampleHandler,hwnd,msg,wParam,lParam);

   return LRESULT(FALSE);
   }