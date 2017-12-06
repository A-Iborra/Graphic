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
   Text *p = (Text *)GetWindowLong(hwnd,GWL_USERDATA);
 
   switch (msg) {
   case WM_CREATE: {
      CREATESTRUCT *pc = (CREATESTRUCT *)mp2;
      SetWindowLong(hwnd,GWL_USERDATA,(LONG)pc -> lpCreateParams);
      }
      return LRESULT(FALSE);
 
   case WM_COMMAND: {
      USHORT commandName = LOWORD(mp1);
      switch ( commandName ) {
      case IDMI_TEXT_PROPERTIES: {
         IUnknown* pIUnknown;
         p -> QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknown));
         p -> pIProperties -> EditProperties(hwnd,L"Text",pIUnknown);
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