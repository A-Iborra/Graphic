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

      if ( ! p )
         break;

      long x = 0,y = 0,cx = (long)LOWORD(lParam),cy = (long)HIWORD(lParam);

      if ( cx == 0 || cy == 0 )
         break;

      SetWindowPos(GetDlgItem(hwnd,IDDI_DATASOURCES_TAB),HWND_TOP,0,0,cx - 12,cy - 12,SWP_NOMOVE);

      SIZEL sizelFunctionsUI{0,0};

      long functionCount = SendMessage(p -> hwndDataSourcesFunctions,TCM_GETITEMCOUNT,0L,0L);

      for ( int k = 0; k < functionCount; k++ ) {

         TC_ITEM tie = {0};

         tie.mask = TCIF_PARAM;

         SendMessage(p -> hwndDataSourcesFunctions,TCM_GETITEM,(WPARAM)k,(LPARAM)&tie);

         SIZEL sizeFunction;

         ((ContainedFunction *)tie.lParam) -> GetExtentPixels(&sizeFunction);

         sizelFunctionsUI.cx = max(sizelFunctionsUI.cx,sizeFunction.cx);
         sizelFunctionsUI.cy = max(sizelFunctionsUI.cy,sizeFunction.cy);

      }

      SetWindowPos(p -> hwndDataSourcesFunctions,HWND_TOP,8,32,sizelFunctionsUI.cx + 16,min(cy - 74,sizelFunctionsUI.cy + 32),0L);

      for ( int k = 0; k < functionCount; k++ ) {

         TC_ITEM tie = {0};

         tie.mask = TCIF_PARAM;

         SendMessage(p -> hwndDataSourcesFunctions,TCM_GETITEM,(WPARAM)k,(LPARAM)&tie);

         ((ContainedFunction *)tie.lParam) -> ReSize();

      }

      long dataSetCount = SendMessage(p -> hwndDataSourcesDataSets,TCM_GETITEMCOUNT,0L,0L);

      SIZEL sizelDataSetsUI{0,0};

      for ( int k = 0; k < dataSetCount; k++ ) {

         TC_ITEM tie = {0};

         tie.mask = TCIF_PARAM;

         SendMessage(p -> hwndDataSourcesDataSets,TCM_GETITEM,(WPARAM)k,(LPARAM)&tie);

         SIZEL sizeDataSet;

         ((ContainedFunction *)tie.lParam) -> GetExtentPixels(&sizeDataSet);

         sizelDataSetsUI.cx = max(sizelDataSetsUI.cx,sizeDataSet.cx);
         sizelDataSetsUI.cy = max(sizelDataSetsUI.cy,sizeDataSet.cy);

      }

      for ( int k = 0; k < dataSetCount; k++ ) {

         TC_ITEM tie = {0};

         tie.mask = TCIF_PARAM;

         SendMessage(p -> hwndDataSourcesDataSets,TCM_GETITEM,(WPARAM)k,(LPARAM)&tie);

         ((ContainedFunction *)tie.lParam) -> ReSize();

      }

      if ( 0 < dataSetCount )
         SetWindowPos(p -> hwndDataSourcesDataSets,HWND_TOP,8,32,sizelDataSetsUI.cx + 16,min(cy - 74,sizelDataSetsUI.cy + 32),0L);

      long countTabs = SendMessage(p -> hwndDataSourcesTab,TCM_GETITEMCOUNT,0L,0L);

      long functionTabIndex = -1;
      long dataSetTabIndex = -1;

      TC_ITEM tie = {0};

      char szHeader[64];
      tie.mask = TCIF_TEXT;
      tie.cchTextMax = 64;
      tie.pszText = szHeader;

      for ( long k = 0; k < countTabs; k++ ) {
         SendMessage(p -> hwndDataSourcesTab,TCM_GETITEM,(WPARAM)k,(LPARAM)&tie);
         if ( 0 == strcmp(szHeader,"Functions") )
            functionTabIndex = k;
         if ( 0 == strcmp(szHeader,"DataSets") )
            dataSetTabIndex = k;
      }

      RECT rc,rcParent;

      if ( SendMessage(p -> hwndDataSourcesTab,TCM_GETCURSEL,0L,0L) == functionTabIndex )
         GetWindowRect(p -> hwndDataSourcesFunctions,&rc);
      else
         GetWindowRect(p -> hwndDataSourcesDataSets,&rc);

      GetWindowRect(hwnd,&rcParent);

      SetWindowPos(GetDlgItem(hwnd,IDDI_DATASOURCES_CLEAR),HWND_TOP,8,rc.bottom - rcParent.top + 16,0,0,SWP_NOSIZE);
      SetWindowPos(GetDlgItem(hwnd,IDDI_DATASOURCES_AUTOCLEAR),HWND_TOP,70,rc.bottom - rcParent.top + 16 + 4,0,0,SWP_NOSIZE);
      SetWindowPos(GetDlgItem(hwnd,IDDI_DATASOURCES_RESETDOMAIN),HWND_TOP,8,rc.bottom - rcParent.top + 48,0,0,SWP_NOSIZE);

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

      int k = SendMessage(pn -> hwndFrom,TCM_GETCURSEL,0,0);
      if ( -1 == k )
         break;

      if ( ! ( TCN_SELCHANGE == pn -> code ) && ! ( TCN_SELCHANGING == pn -> code ) )
         break;

      TC_ITEM tie;

      memset(&tie,0,sizeof(TC_ITEM));

      char szHeader[64];
      tie.mask = TCIF_TEXT | TCIF_PARAM;
      tie.cchTextMax = 64;
      tie.pszText = szHeader;
      HWND hwndTab = NULL;
      SendMessage(pn -> hwndFrom,TCM_GETITEM,(WPARAM)k,(LPARAM)&tie);

      if ( 0 == strcmp(szHeader,"Functions") ) {
         ShowWindow(p -> hwndDataSourcesDataSets,SW_HIDE);
         ShowWindow(p -> hwndDataSourcesFunctions,SW_SHOW);
         hwndTab = p -> hwndDataSourcesFunctions;
      } else if ( 0 == strcmp(szHeader,"DataSets") ) {
         ShowWindow(p -> hwndDataSourcesFunctions,SW_HIDE);
         ShowWindow(p -> hwndDataSourcesDataSets,SW_SHOW);
         hwndTab = p -> hwndDataSourcesDataSets;
      }

      if ( ! ( NULL == hwndTab ) ) {

         long currentIndex = SendMessage(hwndTab,TCM_GETCURSEL,0L,0L);

         if ( -1 == currentIndex )
            break;

         SendMessage(hwndTab,TCM_GETITEM,(WPARAM)currentIndex,(LPARAM)&tie);

         if ( tie.lParam )
            if ( TCN_SELCHANGING == pn -> code )
               ((ContainedObject *)tie.lParam) -> Hide();
            else
               ((ContainedObject *)tie.lParam) -> Show();

      } else {

         if ( tie.lParam )
            if ( TCN_SELCHANGING == pn -> code )
               ((ContainedObject *)tie.lParam) -> Hide();
            else
               ((ContainedObject *)tie.lParam) -> Show();

      }

      RECT rcDialog;
      GetWindowRect(p -> hwndDataSourcesDialog,&rcDialog);

      SendMessage(p -> hwndDataSourcesDialog,WM_SIZE,0L,MAKELPARAM(rcDialog.right - rcDialog.left,rcDialog.bottom - rcDialog.top));

      }
      break;

   default:
      break;

   }
 
   return LRESULT(FALSE);
   }