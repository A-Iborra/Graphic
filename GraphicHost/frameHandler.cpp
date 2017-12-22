
#include "GraphicHost.h"

#define WM_SETUP_GRAPHIC      (WM_USER + 1)

   LRESULT __stdcall frameHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   switch ( msg ) {
   
   case WM_CREATE: {

      PostMessage(hwnd,WM_SETUP_GRAPHIC,0L,0L);

      }
      return (LRESULT)TRUE;

   case WM_SETUP_GRAPHIC: {

      hwndGraphic = CreateWindowEx(WS_EX_CLIENTEDGE,L"STATIC",L"",WS_CHILD | WS_VISIBLE,128,128,1300,768,hwndFrame,NULL,hInstance,NULL);//(LPVOID)pGraphicHost);

      pGraphicHost = new GraphicHost(hwndGraphic);

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
