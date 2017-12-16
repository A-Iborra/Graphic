
#include "GraphicHost.h"

   int pixelFormat;
   void doCommand(HWND hwnd);

   GLint viewPort[4] = {0, 0, 256,256};
   GLdouble projectionMatrix[16];
   GLdouble modelMatrix[16];

   LRESULT __stdcall handler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   switch ( msg ) {
   
   case WM_SETSIZES: {

      RECT rcClient,rcFrame;
      GetWindowRect(hwndFrame,&rcFrame);
      GetWindowRect(hwndClient1,&rcClient);

      SetWindowPos(GetDlgItem(hwnd,1),HWND_TOP,rcClient.left - rcFrame.left,rcClient.bottom - rcFrame.top + 16,0,0,SWP_NOSIZE);

      GetWindowRect(hwndClient2,&rcClient);
      
      SetWindowPos(GetDlgItem(hwnd,2),HWND_TOP,rcClient.left - rcFrame.left,rcClient.bottom - rcFrame.top + 16,0,0,SWP_NOSIZE);

      }
      return (LRESULT)FALSE;

   case WM_COMMAND: {
      doCommand(GetDlgItem(hwnd,10 * LOWORD(wParam)));
      }
      break;

   case WM_PAINT: {
      PAINTSTRUCT ps;
      BeginPaint(hwnd,&ps);
      EndPaint(hwnd,&ps);
      }
      return (LRESULT)FALSE;

   case WM_SYSCOMMAND: {
      if ( wParam == SC_CLOSE ) {
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


   void doCommand(HWND hwnd) {

   DataPoint dp;
   DataPoint dpOrigin {0.0,0.0,0.0};
   DataPoint dpMove {-DBL_MAX,-DBL_MAX,-DBL_MAX};

   pIPlot -> PrepareForData();

   pIPlot -> TakeDataPoint(&dpOrigin);

   dp.x = 1.0;
   dp.y = 0.0;
   dp.z = 0.0;

   pIPlot -> TakeDataPoint(&dp);

   pIPlot -> TakeDataPoint(&dpMove);

   pIPlot -> TakeDataPoint(&dpOrigin);

   dp.x -= 1.0;
   dp.y = 1.0;

   pIPlot -> TakeDataPoint(&dp);

   pIPlot -> TakeDataPoint(&dpMove);

   pIPlot -> TakeDataPoint(&dpOrigin);

   dp.y -= 1.0;
   dp.z = 1.0;

   pIPlot -> TakeDataPoint(&dp);

   IDataSet *pIDataSet = NULL;

   pIPlot -> get_DataSet(&pIDataSet);

   pIOpenGLImplementation -> SetTargetWindow(hwnd);

   pIOpenGLImplementation -> SetUp(pIDataSet,pIPropertyPlotView,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);

   pIOpenGLImplementation -> Erase(pIPropertyBackground);

   pIPlot -> Draw();

   return;
   }