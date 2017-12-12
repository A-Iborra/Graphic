/*

                       Copyright (c) 1996,1997,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include <stdio.h>

#include "Graphic_resource.h"
#include "utils.h"

#include "Graphic.h"


   LRESULT CALLBACK G::sampleGraphicHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
   G *p = reinterpret_cast<G *>(GetWindowLong(hwnd,GWL_USERDATA));
 
   switch ( msg ) {
   case WM_CREATE: {
      CREATESTRUCT *pcs = reinterpret_cast<CREATESTRUCT *>(lParam);
      SetWindowLong(hwnd,GWL_USERDATA,reinterpret_cast<long>(pcs -> lpCreateParams));
      }
      return LRESULT(FALSE);
 
   case WM_PAINT: {
      PAINTSTRUCT ps;
      BeginPaint(hwnd,&ps);
      EndPaint(hwnd,&ps);
      if ( ! p ) break;
      RECT oldMargins;
      memcpy(&oldMargins,&p -> margins,sizeof(RECT));

      HWND hwndOldTarget = p -> pIOpenGLImplementation -> TargetHWND();

      p -> pIOpenGLImplementation -> SetTargetWindow(hwnd);

      p -> margins.left = 8;
      p -> margins.top = 8;
      p -> margins.right = 8;
      p -> margins.bottom = 8;
      p -> render(0);

      p -> pIOpenGLImplementation -> SetTargetWindow(hwndOldTarget);

      memcpy(&p -> margins,&oldMargins,sizeof(RECT));
      }
      return LRESULT(FALSE);
 
 
   default:
      break;
   }
 
   return DefWindowProc(hwnd,msg,wParam,lParam);
   }