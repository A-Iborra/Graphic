/*

                       Copyright (c) 1996,1997,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include <stdio.h>

#include "Graphic_resource.h"
#include "utils.h"

#include "Plot.h"

   LRESULT CALLBACK Plot::sampleGraphicHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   Plot *p = (Plot *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {
 
   case WM_PAINT: {
      PAINTSTRUCT ps;
      BeginPaint(hwnd,&ps);
      EndPaint(hwnd,&ps);
      p -> pIOpenGLImplementation -> SetTargetWindow(hwnd);
      p -> pIOpenGLImplementation -> SetUp(NULL,p -> propertyPlotView);
      p -> pIOpenGLImplementation -> Erase(p -> pOwnerPropertyBackgroundColor);
      p -> Draw();
      p -> pIOpenGLImplementation -> Finalize();
      return (LRESULT)FALSE;
      }
 
   //case WM_GETDLGCODE:
   //   return (LRESULT)0;
 
   default:
      break;
   }
 
   return CallWindowProc(p -> defaultStaticWindowHandler,hwnd,msg,wParam,lParam);
   }