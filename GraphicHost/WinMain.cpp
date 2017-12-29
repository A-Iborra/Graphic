
#define DEFINE_DATA

#include "GraphicHost.h"

#include "GraphicControl_i.c"

#include "GSystem_i.h"
#include "OpenGLImplementation_i.c"
#include "Properties_i.c"
#include "DataSet_i.c"
#include "Plot_i.c"

#include "utilities.h"

   int APIENTRY wWinMain(HINSTANCE hInst,HINSTANCE hPrevInstance,LPWSTR lpCmdLine,int nCmdShow) {

   hInstance = hInst;

   CoInitialize(NULL);

   char szTemp[MAX_PATH];

   GetCommonAppDataLocation(NULL,szTemp);

   sprintf(szApplicationDataDirectory,"%s\\InnoVisioNate\\Graphic",szTemp);

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

   MSG msg;
   while ( GetMessage(&msg,NULL,0L,0L) ) {
      if ( PeekMessage(&msg,NULL,WM_QUIT,WM_QUIT,PM_REMOVE) ) 
         break;
      if ( WM_KEYFIRST <= msg.message && msg.message <= WM_KEYLAST ) 
         TranslateMessage(&msg);
      DispatchMessage(&msg);
   }

   DestroyWindow(hwndFrame);

   CoUninitialize();

   return 0;
   }
