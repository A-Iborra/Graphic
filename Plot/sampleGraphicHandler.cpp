
#include <stdio.h>

#include "Plot.h"
#include "GMESSAGE.H"

   LRESULT CALLBACK Plot::sampleGraphicHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   Plot *p = (Plot *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_CREATE: {

      p = (Plot *)((CREATESTRUCT *)lParam) -> lpCreateParams;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      HWND hwndCurrentOpenGL = p -> pIOpenGLImplementation -> TargetHWND();

      RECT rc;
      GetWindowRect(hwndCurrentOpenGL,&rc);

      p -> sizeMainGraphic.cx = rc.right - rc.left;
      p -> sizeMainGraphic.cy = rc.bottom - rc.top;

      }
      return (LRESULT)0L;
 
   case WMG_POSITION_SAMPLE_GRAPHIC: {

      double aspectRatio = (double)p -> sizeMainGraphic.cx / (double)p -> sizeMainGraphic.cy;

      RECT rcThis;

      memcpy(&rcThis,(BYTE *)lParam,sizeof(RECT));

      long cx = rcThis.right - rcThis.left;
      long cy = (long)((double)cx / aspectRatio);

      if ( rcThis.bottom - rcThis.top < cy ) {
         long cySubtract = cy - (rcThis.bottom - rcThis.top);
         cy = rcThis.bottom - rcThis.top;
         cx -= (long)((double)cySubtract * aspectRatio);
      }

      long x = (rcThis.right + rcThis.left - cx) / 2;
      long y = (rcThis.bottom + rcThis.top - cy) / 2;

      SetWindowPos(hwnd,HWND_TOP,x,y,cx,cy,SWP_SHOWWINDOW);

      if ( p -> hwndSampleGraphicSurface )
         DestroyWindow(p -> hwndSampleGraphicSurface);

      p -> hwndSampleGraphicSurface = CreateWindowEx(0L,"plot-sample-graphic-surface","",WS_CHILD | WS_VISIBLE,0,0,cx,cy,hwnd,NULL,hModule,(void *)p);

      return (LRESULT)0L;
      }

   case WM_ERASEBKGND:
   case WM_PAINT: {
      PAINTSTRUCT ps;
      BeginPaint(hwnd,&ps);
      EndPaint(hwnd,&ps);
      return (LRESULT)FALSE;
      }

   default:
      break;
   }
 
   return DefWindowProc(hwnd,msg,wParam,lParam);
   }


   LRESULT CALLBACK Plot::sampleGraphicSurfaceHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   Plot *p = (Plot *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_CREATE: {
      p = (Plot *)((CREATESTRUCT *)lParam) -> lpCreateParams;
      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);
      p -> pIOpenGLImplementation -> SetTargetWindow(hwnd);
      }
      break;

   case WM_ERASEBKGND:
   case WM_PAINT: {
      PAINTSTRUCT ps;
      BeginPaint(hwnd,&ps);
      sampleGraphicThread((void *)p);
      EndPaint(hwnd,&ps);
      return (LRESULT)FALSE;
      }

   }

   return DefWindowProc(hwnd,msg,wParam,lParam);
   }


   DWORD WINAPI Plot::sampleGraphicThread(void *pvArg) {

   Plot *p = (Plot *)pvArg;

   if ( p -> overrideOwnerView )
      p -> pIOpenGLImplementation -> SetUp(p -> pIDataSet,p -> propertyPlotView);
   else
      p -> pIOpenGLImplementation -> SetUp(p -> pIDataSet,p -> pOwnerPropertyPlotView);

   p -> pIGSGraphicServices -> ActivateLighting();

   p -> pIOpenGLImplementation -> Erase(p -> pOwnerPropertyBackgroundColor);

   long keepOkayToPlot;

   p -> get_OkToPlot(&keepOkayToPlot);

   p -> put_OkToPlot(1L);

   p -> Draw();

   p -> put_OkToPlot(keepOkayToPlot);

   p -> pIOpenGLImplementation -> Finalize();

   return 0L;
   }