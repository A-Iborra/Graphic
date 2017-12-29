
#include "GraphicHost.h"

   LRESULT EXPENTRY GraphicHost::graphicHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   GraphicHost *p = (GraphicHost *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   if ( WM_PAINT == msg ) {
      PAINTSTRUCT ps;
      BeginPaint(hwnd,&ps);
      EndPaint(hwnd,&ps);
      return (LRESULT)0;
   }

   return DefWindowProc(hwnd,msg,wParam,lParam);
   }