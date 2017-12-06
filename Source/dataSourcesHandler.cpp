/*

                       Copyright (c) 1996,1997,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <commctrl.h>
#include <olectl.h>
#include <stdio.h>

#include "Graphic_resource.h"
#include "utils.h"

#include "Graphic.h"
#include "ContainedFunction.h"
#include "List.cpp"


   LRESULT CALLBACK G::dataSourcesHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   G *p = (G *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch ( msg ) {

   case WM_INITDIALOG: {
      p = (G *)lParam;
      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)lParam);
      p -> propertyAutoClear -> setWindowItemChecked(hwnd,IDDI_DATASOURCES_AUTOCLEAR);
      }
      return LRESULT(FALSE);
 
   case WM_PAINT:
      if ( p ) 
         p -> propertyAutoClear -> setWindowItemChecked(hwnd,IDDI_DATASOURCES_AUTOCLEAR);
      break;

   case WM_SIZE: {

      if ( ! p ) break;
      long x = 0,y = 0,cx = (long)LOWORD(lParam),cy = (long)HIWORD(lParam);
      if ( cx == 0 || cy == 0 ) break;

      SetWindowPos(GetDlgItem(hwnd,IDDI_DATASOURCES_TAB),HWND_TOP,0,0,cx - 8,cy - 8,SWP_NOMOVE);

      if ( p -> functionList.Count() > 0 ) {

         SIZEL sizeFunction;

         {
         IOleObject* pIOleObject_Function;
         int item = SendMessage(p->hwndDataSourcesFunctions,TCM_GETCURSEL,0L,0L);
         p -> functionList.GetByIndex(item) -> QueryInterface(IID_IOleObject,reinterpret_cast<void**>(&pIOleObject_Function));
         pIOleObject_Function -> GetExtent(DVASPECT_CONTENT,&sizeFunction);
         hiMetricToPixel(&sizeFunction,&sizeFunction);
         pIOleObject_Function -> Release();
         }

         cx -= sizeFunction.cx + 52;
         x += sizeFunction.cx + 52;

         SetWindowPos(p -> hwndDataSourcesFunctions,HWND_TOP,8,32,sizeFunction.cx + 16,min(cy - 74,sizeFunction.cy + 16),SWP_SHOWWINDOW);

         ContainedFunction* pContainedFunction = (ContainedFunction*)NULL;
         while ( pContainedFunction = p -> containedFunctionList.GetNext(pContainedFunction) ) 
            pContainedFunction -> ReSize();

         SetWindowPos(GetDlgItem(hwnd,IDDI_DATASOURCES_CLEAR),HWND_TOP,8,sizeFunction.cy + 16 + 32 + 10,0,0,SWP_NOSIZE);
         SetWindowPos(GetDlgItem(hwnd,IDDI_DATASOURCES_AUTOCLEAR),HWND_TOP,60,sizeFunction.cy + 16 + 32 + 10,0,0,SWP_NOSIZE);
         SetWindowPos(GetDlgItem(hwnd,IDDI_DATASOURCES_RESETDOMAIN),HWND_TOP,8,sizeFunction.cy + 16 + 32 + 10 + 16 + 4 + 16,0,0,SWP_NOSIZE);

      }

      }
      return LRESULT(FALSE);

   case WM_COMMAND: {
      int notifyCode = HIWORD(wParam);
      long idControl = LOWORD(wParam);
      switch ( idControl ) {
      case IDDI_DATASOURCES_AUTOCLEAR:
         if ( notifyCode == BN_CLICKED ) p -> propertyAutoClear -> getWindowChecked((HWND)lParam);
         break;
      case IDDI_DATASOURCES_CLEAR: {
         p -> clearData();
         p -> render(0);
         }
         break;
      case IDDI_DATASOURCES_RESETDOMAIN: {
         p -> pIDataSetMaster -> ReSet();
         p -> put_StatusText(0,"Extents: infinite");
         p -> erase();
         }
         break;
      default:
         break;
      }
      }
      break;

   case WM_NOTIFY: {
      if ( ! p ) break;
      if ( ! p -> isRunning ) break;
      NMHDR *pn = (NMHDR *)lParam;
      TC_ITEM tie;
      int k = SendMessage(pn -> hwndFrom,TCM_GETCURSEL,0,0);
      memset(&tie,0,sizeof(TC_ITEM));
      tie.mask = TCIF_PARAM;
      SendMessage(pn -> hwndFrom,TCM_GETITEM,(WPARAM)k,(LPARAM)&tie);
      switch ( pn -> code ) {
      case TCN_SELCHANGING:
         ShowWindow((HWND)tie.lParam,SW_HIDE);
         break;
 
      case TCN_SELCHANGE:
         ShowWindow((HWND)tie.lParam,SW_SHOW);
         break;
 
      default:
         break;
      }
 
      RECT rect;
      GetWindowRect(p -> hwndDataSourcesDialog,&rect);
      SendMessage(p -> hwndDataSourcesDialog,WM_SIZE,(WPARAM)SIZE_RESTORED,MAKELPARAM(rect.right - rect.left,rect.bottom - rect.top));

      }
      break;

   default:
      break;
   }
 
   return LRESULT(FALSE);
   }