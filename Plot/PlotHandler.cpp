
#include "plot.h"

#include "Graphic_resource.h"

   LRESULT EXPENTRY Plot::handler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   Plot *p = (Plot *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch (msg) {

   case WM_CREATE: {
      CREATESTRUCT *pc = (CREATESTRUCT *)lParam;
      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)pc -> lpCreateParams);
      }
      return LRESULT(FALSE);

   case WM_COMMAND: {

      switch ( LOWORD(wParam) ) {
      case IDMI_PLOT_PROPERTIES: {
         HWND hwndOwner = NULL;
         POINT ptCursor = {0};
         GetCursorPos(&ptCursor);
         hwndOwner = WindowFromPoint(ptCursor);
         IUnknown* pIUnknown;
         p -> QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknown));
         p -> pIProperties -> ShowProperties(hwndOwner ? hwndOwner : GetForegroundWindow(),pIUnknown);
         pIUnknown -> Release();
         if ( p -> pWhenChangedCallback ) {
            p -> pWhenChangedCallback(p -> pWhenChangedCallbackArg);
         }
         p -> hwndObjectWindow = NULL;
         DestroyWindow(hwnd);
         }
         return LRESULT(TRUE);
 
      case IDMI_PLOT_DELETE: {
         if ( p -> pIPlotNotify ) {
            IPlot *pIPlot;
            p -> QueryInterface(IID_IPlot,reinterpret_cast<void **>(&pIPlot));
            pIPlot -> Release();
            p -> pIPlotNotify -> DeletePlot(pIPlot);
         }
         if ( p -> pWhenChangedCallback ) {
            p -> pWhenChangedCallback(p -> pWhenChangedCallbackArg);
         }
         p -> hwndObjectWindow = NULL;
         DestroyWindow(hwnd);
         }
         break;
 
      }
 
      }
      return LRESULT(TRUE);
   }
 
   return DefWindowProc(hwnd,msg,wParam,lParam);
   }