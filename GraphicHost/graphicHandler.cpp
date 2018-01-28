
#include "GraphicHost.h"

   LRESULT EXPENTRY GraphicHost::graphicHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   GraphicHost *p = (GraphicHost *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   if ( WM_PAINT == msg ) {
      PAINTSTRUCT ps;
      BeginPaint(hwnd,&ps);
      EndPaint(hwnd,&ps);
      return (LRESULT)0;
   }

   if ( WM_SIZE == msg && p ) {
      RECT rect;
      GetWindowRect(hwnd,&rect);
      RECT rcAdjust{0,0,0,0};
      AdjustWindowRectEx(&rcAdjust,(DWORD)GetWindowLongPtr(hwnd,GWL_STYLE),FALSE,(DWORD)GetWindowLongPtr(hwnd,GWL_EXSTYLE));
      long cx = rect.right - rect.left - (rcAdjust.right - rcAdjust.left);
      long cy = rect.bottom - rect.top - (rcAdjust.bottom - rcAdjust.top);
      SIZEL sizel{cx,cy};
      pixelsToHiMetric(&sizel,&sizel);
      p -> pIOleObject_Graphic -> SetExtent(DVASPECT_CONTENT,&sizel);
   }

   return DefWindowProc(hwnd,msg,wParam,lParam);
   }