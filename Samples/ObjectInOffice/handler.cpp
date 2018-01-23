
#include "ObjectInOffice.h"

#include <stdio.h>

   LRESULT EXPENTRY ObjectInOffice::handler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   ObjectInOffice *p = (ObjectInOffice *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

char szX[256];
sprintf_s(szX,256,"hwnd: %p,msg %ld,p: %p\n",hwnd,msg,p);
OutputDebugStringA(szX);

   switch ( msg ) {

   case WM_CREATE: {
      CREATESTRUCT *cp = (CREATESTRUCT *)lParam;
      p = (ObjectInOffice *)cp -> lpCreateParams;
      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);
      }
      return (LRESULT)FALSE;

   case WM_SIZE:
      return (LRESULT)FALSE;

   case WM_PAINT: {
      PAINTSTRUCT ps;
COLORREF red = RGB(255,0,0);

      HBRUSH hb = GetSysColorBrush(COLOR_APPWORKSPACE);
      HDC hdc = BeginPaint(hwnd,&ps);
      FillRect(hdc,&ps.rcPaint,hb);
      EndPaint(hwnd,&ps);
      }
      return (LRESULT)FALSE;

   default:
      break;
   }

   return DefWindowProc(hwnd,msg,wParam,lParam);
   }