
#include <stdio.h>
#include <vector>

#include "graphic.h"
#include "utils.h"

#include "List.cpp"

   long tabIndex(HWND hwndTab,char *pszCompare) {

   long countTabs = SendMessage(hwndTab,TCM_GETITEMCOUNT,0L,0L);
   
   char szTest[32];

   TC_ITEM tie = {0};
   tie.mask = TCIF_TEXT;
   tie.cchTextMax = 32;
   tie.pszText = szTest;

   for ( long k = 0; k < countTabs; k++ ) {
      SendMessage(hwndTab,TCM_GETITEM,(WPARAM)k,(LPARAM)&tie);
      if ( 0 == strcmp(szTest,pszCompare) ) 
         return k;
   }

   return -1;
   }

   void G::setDataSourcesVisibility(IDataSet *pIDataSet_Relevant,IGSFunctioNater *pIFunction_Relevant) {

   if ( ! hwndFrame )
      return;

   RECT rcFrame;
   GetWindowRect(hwndFrame,&rcFrame);

   long cxGraphic = rcFrame.right - rcFrame.left;
   long cyGraphic = rcFrame.bottom - rcFrame.top;
   long xGraphic = 0;
   long yGraphic = 0;

   long cxDataSourcesAdjust = 0;

   bool anyDataSourceVisible = false;
   bool anyFunctionVisible = false;
   bool anyDataSetVisible = false;

   long entryTabIndex = SendMessage(hwndDataSourcesTab,TCM_GETCURSEL,(WPARAM)0L,(LPARAM)0L);

   if ( -1 == entryTabIndex )
      entryTabIndex = -2;

   long entryFunctionTabIndex = tabIndex(hwndDataSourcesTab,"Functions");

   long entryDataSetTabIndex = tabIndex(hwndDataSourcesTab,"DataSets");

   //long entryFunctionTabIndex = functionTabIndex;
   //long entryDataSetTabIndex = dataSetTabIndex;

   SIZEL sizeUI{0,0};

   std::vector<IGSFunctioNater *> visibleFunctions;

   IGSFunctioNater *pIFunction = NULL;

   while ( pIFunction = functionList.GetNext(pIFunction) ) {

      IOleObject* pIOleObject_Function;

      VARIANT_BOOL isAnyControlVisible;

      pIFunction -> get_AnyControlVisible(&isAnyControlVisible);

      if ( ! isAnyControlVisible ) 
         continue;

      visibleFunctions.push_back(pIFunction);

      anyFunctionVisible = true;

      anyDataSourceVisible = true;

      SIZEL sizeThisFunction{0,0};

      pIFunction -> QueryInterface(IID_IOleObject,reinterpret_cast<void**>(&pIOleObject_Function));

      pIOleObject_Function -> GetExtent(DVASPECT_CONTENT,&sizeThisFunction);

      if ( sizeUI.cx < sizeThisFunction.cx )
         sizeUI.cx = sizeThisFunction.cx;

      if ( sizeUI.cy < sizeThisFunction.cy )
         sizeUI.cy = sizeThisFunction.cy;

      pIOleObject_Function -> Release();

   }

   if ( ! anyFunctionVisible ) {

      SendMessage(hwndDataSourcesFunctions,TCM_DELETEALLITEMS,0L,0L);

      SendMessage(hwndDataSourcesTab,TCM_DELETEITEM,(WPARAM)tabIndex(hwndDataSourcesTab,"Functions"),0L);

   }

   IOleObject* pIOleObject_DataSet;

   IDataSet *pIDataSet = NULL;

   while ( pIDataSet = dataSetList.GetNext(pIDataSet) ) {

      anyDataSetVisible = true;

      anyDataSourceVisible = true;

      SIZEL sizeThisUI{0,0};

      pIDataSet -> QueryInterface(IID_IOleObject,reinterpret_cast<void**>(&pIOleObject_DataSet));

      pIOleObject_DataSet -> GetExtent(DVASPECT_CONTENT,&sizeThisUI);

      if ( sizeUI.cx < sizeThisUI.cx )
         sizeUI.cx = sizeThisUI.cx;

      if ( sizeUI.cy < sizeThisUI.cy )
         sizeUI.cy = sizeThisUI.cy;

      pIOleObject_DataSet -> Release();

   }

   if ( ! anyDataSetVisible ) {

      SendMessage(hwndDataSourcesDataSets,TCM_DELETEALLITEMS,0L,0L);

      SendMessage(hwndDataSourcesTab,TCM_DELETEITEM,(WPARAM)tabIndex(hwndDataSourcesTab,"DataSets"),0L);

   }

   if ( anyDataSourceVisible ) {

      hiMetricToPixel(&sizeUI,&sizeUI);
      cxDataSourcesAdjust = sizeUI.cx + 52;
      xGraphic = cxDataSourcesAdjust;
      cxGraphic -= cxDataSourcesAdjust;

      SendMessage(hwndDataSourcesTab,TCM_DELETEITEM,(WPARAM)tabIndex(hwndDataSourcesTab,"DataSets"),0L);

      SendMessage(hwndDataSourcesDataSets,TCM_DELETEALLITEMS,0L,0L);

      if ( anyDataSetVisible ) {

         TC_ITEM tie;
         memset(&tie,0,sizeof(TC_ITEM));
         tie.mask = TCIF_TEXT;
         tie.pszText = "DataSets";
         tie.cchTextMax = 18;
         long rc = SendMessage(hwndDataSourcesTab,TCM_INSERTITEM,(WPARAM)SendMessage(hwndDataSourcesTab,TCM_GETITEMCOUNT,0L,0L),(LPARAM)&tie);

         for ( long k = 0; k < containedDataSetList.Count(); k++ ) {
            ContainedDataSet * pIDataSet = containedDataSetList.GetByIndex(k);
            char szName[32];
            sprintf(szName,"DS%ld",k + 1);
            memset(&tie,0,sizeof(TC_ITEM));
            tie.mask = TCIF_PARAM | TCIF_TEXT;
            tie.lParam = (LPARAM)pIDataSet;
            tie.pszText = szName;
            SendMessage(hwndDataSourcesDataSets,TCM_INSERTITEM,(WPARAM)SendMessage(hwndDataSourcesDataSets,TCM_GETITEMCOUNT,0L,0L),(LPARAM)&tie);
         }

      }

      SendMessage(hwndDataSourcesTab,TCM_DELETEITEM,(WPARAM)tabIndex(hwndDataSourcesTab,"Functions"),0L);

      SendMessage(hwndDataSourcesFunctions,TCM_DELETEALLITEMS,0L,0L);

      if ( anyFunctionVisible ) {

         TC_ITEM tie;

         memset(&tie,0,sizeof(TC_ITEM));
         tie.mask = TCIF_TEXT;
         tie.pszText = "Functions";
         tie.cchTextMax = 18;
         SendMessage(hwndDataSourcesTab,TCM_INSERTITEM,(WPARAM)SendMessage(hwndDataSourcesTab,TCM_GETITEMCOUNT,0L,0L),(LPARAM)&tie);

         for ( long k = 0; k < visibleFunctions.size(); k++ ) {
            IGSFunctioNater *pf = visibleFunctions[k];
            ContainedFunction *pIFunction = NULL;
            while ( pIFunction = containedFunctionList.GetNext(pIFunction) )
               if ( pIFunction -> pFunction() == pf )
                  break;
            char szName[32];
            sprintf(szName,"F%ld",k + 1);
            memset(&tie,0,sizeof(TC_ITEM));
            tie.mask = TCIF_PARAM | TCIF_TEXT;
            tie.lParam = (LPARAM)pIFunction;
            tie.pszText = szName;
            SendMessage(hwndDataSourcesFunctions,TCM_INSERTITEM,(WPARAM)SendMessage(hwndDataSourcesFunctions,TCM_GETITEMCOUNT,0L,0L),(LPARAM)&tie);
         }

      }

      NMHDR notifyHeader = {0};

      if ( ! ( NULL == pIDataSet_Relevant ) ) {

         SendMessage(hwndDataSourcesTab,TCM_SETCURSEL,(WPARAM)tabIndex(hwndDataSourcesTab,"DataSets"),0L);

         long item = dataSetList.IndexOf(pIDataSet_Relevant);

         if ( -1 == item )
            item = 0;

         SendMessage(hwndDataSourcesDataSets,TCM_SETCURSEL,(WPARAM)item,0L);

         notifyHeader.hwndFrom = hwndDataSourcesDataSets;

      } else if ( ! ( NULL == pIFunction_Relevant ) ) {

         SendMessage(hwndDataSourcesTab,TCM_SETCURSEL,(WPARAM)tabIndex(hwndDataSourcesTab,"Functions"),0L);

         long item = 0;
         bool found = false;
         for ( IGSFunctioNater *pf : visibleFunctions ) {
            if ( pf == pIFunction_Relevant ) {
               found = true;
               break;
            }
            item++;
         }

         if ( ! found )
            item = 0;

         SendMessage(hwndDataSourcesFunctions,TCM_SETCURSEL,(WPARAM)item,0L);

         notifyHeader.hwndFrom = hwndDataSourcesFunctions;

      } else {

         if ( entryTabIndex == entryFunctionTabIndex ) {

            SendMessage(hwndDataSourcesTab,TCM_SETCURSEL,(WPARAM)tabIndex(hwndDataSourcesTab,"Functions"),0L);

            notifyHeader.hwndFrom = hwndDataSourcesFunctions;

         } else if ( entryTabIndex == entryDataSetTabIndex ) {

            SendMessage(hwndDataSourcesTab,TCM_SETCURSEL,(WPARAM)tabIndex(hwndDataSourcesTab,"DataSets"),0L);

            notifyHeader.hwndFrom = hwndDataSourcesDataSets;

         } else {

            SendMessage(hwndDataSourcesTab,TCM_SETCURSEL,(WPARAM)0,0L);

            if ( anyDataSetVisible ) {

               SendMessage(hwndDataSourcesDataSets,TCM_SETCURSEL,(WPARAM)0,0L);

               notifyHeader.hwndFrom = hwndDataSourcesDataSets;

            }

            if ( anyFunctionVisible ) {

               SendMessage(hwndDataSourcesFunctions,TCM_SETCURSEL,(WPARAM)0,0L);

               notifyHeader.hwndFrom = hwndDataSourcesFunctions;

            }

         }

      }

      if ( showStatusBar ) {
         RECT rectStatusBar;
         GetWindowRect(hwndStatusBar,&rectStatusBar);
         SetWindowPos(hwndDataSourcesDialog,HWND_TOP,0,0,cxDataSourcesAdjust,cyGraphic - (rectStatusBar.bottom - rectStatusBar.top),SWP_SHOWWINDOW);
      } else
         SetWindowPos(hwndDataSourcesDialog,HWND_TOP,0,0,cxDataSourcesAdjust,cyGraphic,SWP_SHOWWINDOW);

      RECT rcDialog;
      GetWindowRect(hwndDataSourcesDialog,&rcDialog);

      SendMessage(hwndDataSourcesDialog,WM_SIZE,0L,MAKELPARAM(rcDialog.right - rcDialog.left,rcDialog.bottom - rcDialog.top));

      notifyHeader.code = TCN_SELCHANGING;

      if ( notifyHeader.hwndFrom == hwndDataSourcesDataSets ) {

         notifyHeader.hwndFrom = hwndDataSourcesTab;
         SendMessage(hwndDataSourcesDialog,WM_NOTIFY,0L,(LPARAM)&notifyHeader);
         notifyHeader.hwndFrom = hwndDataSourcesFunctions;
         SendMessage(hwndDataSourcesDialog,WM_NOTIFY,0L,(LPARAM)&notifyHeader);
         notifyHeader.hwndFrom = hwndDataSourcesDataSets;

      } else {

         notifyHeader.hwndFrom = hwndDataSourcesTab;
         SendMessage(hwndDataSourcesDialog,WM_NOTIFY,0L,(LPARAM)&notifyHeader);
         notifyHeader.hwndFrom = hwndDataSourcesDataSets;
         SendMessage(hwndDataSourcesDialog,WM_NOTIFY,0L,(LPARAM)&notifyHeader);
         notifyHeader.hwndFrom = hwndDataSourcesFunctions;

      }
      notifyHeader.code = TCN_SELCHANGE;

      SendMessage(hwndDataSourcesDialog,WM_NOTIFY,0L,(LPARAM)&notifyHeader);

   } else

      ShowWindow(hwndDataSourcesDialog,SW_HIDE);

   if ( showStatusBar ) {

      RECT rectStatusBar;

      SetWindowPos(hwndStatusBar,HWND_TOP,0,0,cxGraphic + cxDataSourcesAdjust,cyGraphic,SWP_SHOWWINDOW);

      GetWindowRect(hwndStatusBar,&rectStatusBar);

      int statusWidth = statusTextWidth < 0 ? STANDARD_STATUS_TEXT_WIDTH : statusTextWidth[1];

      int sbWidths[2] = {cxGraphic + cxDataSourcesAdjust - statusWidth,-1};

      SendMessage(hwndStatusBar,SB_SETPARTS,(WPARAM)2,(LPARAM)sbWidths);

      rectStatusText.left = rectStatusBar.right - rectStatusBar.left - statusWidth;
      rectStatusText.right = rectStatusText.left + statusWidth;

      rectStatusText.top = 2;
      rectStatusText.bottom = rectStatusBar.bottom - rectStatusBar.top - 4;

      SetWindowPos(hwndGraphic,HWND_TOP,xGraphic,yGraphic,cxGraphic,cyGraphic - (rectStatusBar.bottom - rectStatusBar.top),0L);

   } else {

      ShowWindow(hwndStatusBar,SW_HIDE);

      SetWindowPos(hwndGraphic,HWND_TOP,xGraphic,yGraphic,cxGraphic,cyGraphic,0L);

   }

   HWND hwndCurrent = pIOpenGLImplementation -> TargetHWND();

   if ( hwndCurrent == hwndGraphic )
      pIOpenGLImplementation -> SetTargetWindow(hwndGraphic);


   return;
   }