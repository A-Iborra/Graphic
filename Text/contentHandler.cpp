/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include "Graphic_resource.h"

#include "text.h"


   LRESULT CALLBACK Text::contentHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
   Text *p = (Text *)GetWindowLong(hwnd,GWL_USERDATA);
   switch ( msg ) {
   case WM_INITDIALOG: {
      PROPSHEETPAGE *pPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);
      p = (Text *)pPage -> lParam;
      SetWindowLong(hwnd,GWL_USERDATA,(long)p);
      }
      return LRESULT(IDDI_TEXT_TEXT);
 
   case WM_SIZE: {                  
      int cx = LOWORD(lParam);
      int cy = HIWORD(lParam);
      RECT rect;
      GetClientRect(GetDlgItem(hwnd,IDDI_TEXT_TEXT),&rect);
      SetWindowPos(GetDlgItem(hwnd,IDDI_TEXT_TEXT),HWND_TOP,8,rect.top + 32,cx - 16,cy - 48 - rect.top,0L);
      }
      break;
 
   case WM_DESTROY: {
      RECT rectParent;
      GetWindowRect(GetParent(GetParent(hwnd)),&rectParent);
      p -> propertyPropertiesPosition -> put_binaryValue(sizeof(RECT),(BYTE*)&rectParent);
      }
      break;
 
   case WM_COMMAND: {
 
      switch ( LOWORD(wParam) ) {
 
      case IDDI_TEXT_TEXT:
//FIXME         p -> propertyText -> getWindowItemValue((long)hwnd,IDDI_TEXT_TEXT);
         break;
 
      default:
         break;
      }
 
   default:
      break;
 
   }
 
   }
   return LRESULT(FALSE);
   }