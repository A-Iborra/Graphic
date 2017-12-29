
#include "GraphicHost.h"

#define WM_SETUP_GRAPHIC      (WM_USER + 1)

   LRESULT __stdcall frameHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   switch ( msg ) {
   
   case WM_CREATE: {
      PostMessage(hwnd,WM_SETUP_GRAPHIC,0L,0L);
      }
      return (LRESULT)TRUE;

   case WM_PAINT: {
      PAINTSTRUCT ps;
      BeginPaint(hwnd,&ps);
      FillRect(ps.hdc,&ps.rcPaint,(HBRUSH)(COLOR_WINDOW + 1));
      EndPaint(hwnd,&ps);
      }
      break;

   case WM_SETUP_GRAPHIC: {
      pGraphicHost = new GraphicHost();
      }
      break;

   case WM_SYSCOMMAND: {
      if ( wParam == SC_CLOSE ) {
         delete pGraphicHost;
         PostQuitMessage(0L);
         return (LRESULT)0;
      }
      }
      break;

   default:
     break;

   }

   return DefWindowProc(hwnd,msg,wParam,lParam);
   }
