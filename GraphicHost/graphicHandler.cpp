
#include "GraphicHost.h"

   LRESULT EXPENTRY GraphicHost::graphicHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   GraphicHost *p = (GraphicHost *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   return CallWindowProc(nativeStaticHandler,hwnd,msg,wParam,lParam);
   }