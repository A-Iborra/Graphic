/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002,2008 Nathan T. Clark

*/

#include <windows.h>
#include <commctrl.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "Graphic_resource.h"
#include "utils.h"

#include "text.h"


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
         IUnknown* pIUnknown;
         p -> QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknown));
         p -> pIProperties -> ShowProperties(hwnd,pIUnknown);
         pIUnknown -> Release();       
         }
         return LRESULT(TRUE);
 
      case IDMI_TEXT_DELETE: {
         if ( p -> pITextNotify ) {
            IText *pIText;
            p -> QueryInterface(IID_IText,reinterpret_cast<void **>(&pIText));
            pIText -> Release();
            p -> pITextNotify -> DeleteText(pIText);
         }
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