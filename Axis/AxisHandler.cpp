/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <stdio.h>

#include "Graphic_resource.h"

#include "axis.h"


   LRESULT EXPENTRY Axis::handler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   Axis *p = (Axis *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch (msg) {
   case WM_CREATE: {
      CREATESTRUCT *pc = (CREATESTRUCT *)lParam;
      SetWindowLong(hwnd,GWL_USERDATA,(LONG)pc -> lpCreateParams);
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
         }
         }
         return LRESULT(TRUE);
 
      }
      return LRESULT(TRUE);
 
   }
 
   return DefWindowProc(hwnd,msg,wParam,lParam);
   }