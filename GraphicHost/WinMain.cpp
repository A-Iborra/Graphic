
#define DEFINE_DATA

#include "GraphicHost.h"

#include "GraphicControl_i.c"

#include "GSystem_i.h"
#include "OpenGLImplementation_i.c"
#include "Properties_i.c"
#include "DataSet_i.c"
#include "Plot_i.c"

   int APIENTRY wWinMain(HINSTANCE hInst,HINSTANCE hPrevInstance,LPWSTR lpCmdLine,int nCmdShow) {

   hInstance = hInst;

   CoInitialize(NULL);

   WNDCLASS gClass;

   memset(&gClass,0,sizeof(WNDCLASS));
   gClass.style = CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW;
   gClass.lpfnWndProc = (WNDPROC)frameHandler;
   gClass.cbClsExtra = 32;
   gClass.cbWndExtra = 32;
   gClass.hInstance = hInstance;
   gClass.hIcon = NULL;
   gClass.hCursor = NULL;
   gClass.hbrBackground = 0;
   gClass.lpszMenuName = NULL;
   gClass.lpszClassName = L"Graphic-Host";
  
   RegisterClass(&gClass);

   hwndFrame = CreateWindowEx(0L,L"Graphic-Host",L"InnoVisioNate Graphic Host",WS_OVERLAPPEDWINDOW | WS_VISIBLE,0,0,1600,1024,NULL,NULL,hInstance,NULL);

   //CreateWindowEx(0L,WC_BUTTON,L"Draw",WS_CHILD | WS_VISIBLE,0,0,48,24,hwndFrame,(HMENU)1,hInstance,NULL);
   //CreateWindowEx(0L,WC_BUTTON,L"Draw",WS_CHILD | WS_VISIBLE,0,0,48,24,hwndFrame,(HMENU)2,hInstance,NULL);

   //hwndClient1 = CreateWindowEx(WS_EX_CLIENTEDGE,WC_STATIC,L"",WS_CHILD | WS_VISIBLE,32,32,512,512,hwndFrame,(HMENU)10,hInstance,NULL);
   //hwndClient2 = CreateWindowEx(WS_EX_CLIENTEDGE,WC_STATIC,L"",WS_CHILD | WS_VISIBLE,32 + 512 + 32,32 + 128,256,256,hwndFrame,(HMENU)20,hInstance,NULL);

   //PostMessage(hwndFrame,WM_SETSIZES,0L,0L);

   MSG msg;
   while ( GetMessage(&msg,NULL,0L,0L) ) {
      if ( PeekMessage(&msg,NULL,WM_QUIT,WM_QUIT,PM_REMOVE) ) break;
      DispatchMessage(&msg);
   }

   return 0;
   }
