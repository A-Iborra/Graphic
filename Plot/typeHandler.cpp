
#include "Plot.h"

#include <map>

#include "Graphic_resource.h"
#include "utilities.h"

#define SET_TYPES_2D \
   for ( std::pair<long,HWND> pPair : type2DToCheckBox ) \
      SendMessage(pPair.second,BM_SETCHECK,(long)pPair.first & (long)pType2D ? BST_CHECKED : BST_UNCHECKED,0L);

#define SET_TYPES_3D \
   for ( std::pair<long,HWND> pPair : type3DToCheckBox ) \
      SendMessage(pPair.second,BM_SETCHECK,(long)pPair.first & (long)pType3D ? BST_CHECKED : BST_UNCHECKED,0L);

   HWND hwndScrollPanes[] = {NULL,NULL,NULL};
   HWND hwndScrollBars[] = {NULL,NULL,NULL};

   HWND hwnd2DTypesScrollPane = NULL;
   HWND hwnd2DTypesPaneHost = NULL;
   HWND hwnd2DTypesScrollBar = NULL;

   HWND hwnd3DTypesScrollPane = NULL;
   HWND hwnd3DTypesPaneHost = NULL;
   HWND hwnd3DTypesScrollBar = NULL;

   HWND hwndPaneHosts[] = {NULL,NULL,NULL};

   HWND hwndArityTabs = NULL;

   long max2DTypeControlId = 0;

   long max3DTypeControlId = 0;

   bool needsToScroll[] = {false,false,false};

   std::map<long,long> controlIdToType;
   static std::map<long,HWND> type2DToCheckBox;
   static std::map<long,HWND> type3DToCheckBox;

   LRESULT CALLBACK Plot::typeHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   Plot *p = (Plot *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = (PROPSHEETPAGE *)lParam;
      p = (Plot *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      p -> hwndTypeSettings = hwnd;

      if ( ! hwndSampleGraphic )
         hwndSampleGraphic = CreateWindowEx(WS_EX_CLIENTEDGE,"plot-sample-graphic","",WS_CHILD,0,0,0,0,hwnd,NULL,hModule,(void *)p);

      TCITEM tcItem = {0};

      tcItem.mask = TCIF_TEXT;
      tcItem.pszText = "2 and 3-D";

      hwndArityTabs = GetDlgItem(hwnd,IDDI_PLOT_TYPE_ARITY_TABS);

      SendMessage(hwndArityTabs,TCM_INSERTITEM,0L,(LPARAM)&tcItem);

      tcItem.pszText = "3-D Only";

      SendMessage(hwndArityTabs,TCM_INSERTITEM,1L,(LPARAM)&tcItem);

      SendMessage(hwndArityTabs,TCM_SETCURSEL,-1L,0L);

      hwndScrollPanes[0] = hwnd2DTypesScrollPane = GetDlgItem(hwnd,IDDI_PLOT_TYPE_2D_TYPES_SCROLL_PANE);
      hwndScrollPanes[1] = hwnd3DTypesScrollPane = GetDlgItem(hwnd,IDDI_PLOT_TYPE_3D_TYPES_SCROLL_PANE);

      hwndScrollBars[0] = hwnd2DTypesScrollBar = GetDlgItem(hwnd,IDDI_PLOT_TYPE_2D_TYPES_SCROLL_BAR);
      hwndScrollBars[1] = hwnd3DTypesScrollBar = GetDlgItem(hwnd,IDDI_PLOT_TYPE_3D_TYPES_SCROLL_BAR);

      SAFEARRAY *p2DTypeIDs,*p2DTypeNames,*p3DTypeIDs,*p3DTypeNames;

      p -> GetPlotTypesInformation(&p2DTypeIDs,&p3DTypeIDs,&p2DTypeNames,&p3DTypeNames);

      long *pvIDs = NULL;
      BSTR *pvNames = NULL;

      long count2DTypes = 0;

      SafeArrayGetUBound(p2DTypeIDs,1,&count2DTypes);

      SafeArrayAccessData(p2DTypeIDs,reinterpret_cast<void **>(&pvIDs));
      SafeArrayAccessData(p2DTypeNames,reinterpret_cast<void **>(&pvNames));

      std::list<long> type2DIds;
      std::list<char *> type2DNames;

      for ( long k = 1; k < count2DTypes; k++ ) {
         type2DIds.push_back(pvIDs[k]);
         long n = wcslen(pvNames[k]);
         char *pszTemp = new char[n + 1];
         pszTemp[n] = '\0';
         WideCharToMultiByte(CP_ACP,0,pvNames[k],-1,pszTemp,n,0,0);
         type2DNames.push_back(pszTemp);
      }

      SafeArrayUnaccessData(p2DTypeIDs);
      SafeArrayUnaccessData(p2DTypeNames);

      SafeArrayDestroy(p2DTypeIDs);
      SafeArrayDestroy(p2DTypeNames);

      long count3DTypes = 0;

      SafeArrayGetUBound(p3DTypeIDs,1,&count3DTypes);

      SafeArrayAccessData(p3DTypeIDs,reinterpret_cast<void **>(&pvIDs));
      SafeArrayAccessData(p3DTypeNames,reinterpret_cast<void **>(&pvNames));

      std::list<long> type3DIds;
      std::list<char *> type3DNames;

      for ( long k = 0; k < count3DTypes; k++ ) {
         type3DIds.push_back(pvIDs[k]);
         long n = wcslen(pvNames[k]);
         char *pszTemp = new char[n + 1];
         pszTemp[n] = '\0';
         WideCharToMultiByte(CP_ACP,0,pvNames[k],-1,pszTemp,n,0,0);
         type3DNames.push_back(pszTemp);
      }

      SafeArrayUnaccessData(p3DTypeIDs);
      SafeArrayUnaccessData(p3DTypeNames);

      SafeArrayDestroy(p3DTypeIDs);
      SafeArrayDestroy(p3DTypeNames);

      long controlId = IDDI_PLOT_TYPE_2D_MIN;

      RECT rcCheckbox,rcParent;
      GetWindowRect(hwnd,&rcParent);
      GetWindowRect(GetDlgItem(hwnd,controlId),&rcCheckbox);

      long cxCheckbox = rcCheckbox.right - rcCheckbox.left;
      long cyCheckbox = rcCheckbox.bottom - rcCheckbox.top;
      long xCheckbox = rcCheckbox.left - rcParent.left;
      long yCheckbox = 0;

      HFONT hGUIFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

      while ( 0 < type2DNames.size() ) {

         char *pszTemp = type2DNames.front();
         type2DNames.pop_front();

         controlIdToType[controlId] = type2DIds.front();
         type2DIds.pop_front();

         HWND hwndCheckbox = CreateWindowEx(0L,"BUTTON",pszTemp,BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE,xCheckbox,yCheckbox,cxCheckbox,cyCheckbox,hwndScrollPanes[0],(HMENU)controlId,hModule,(void *)p);
   
         SendMessage(hwndCheckbox,WM_SETFONT,(LPARAM)hGUIFont,0L);

         type2DToCheckBox[controlIdToType[controlId]] = hwndCheckbox;

         delete [] pszTemp;

         yCheckbox += cyCheckbox + 8;

         max2DTypeControlId = controlId;

         controlId++;

      }

      GetWindowRect(hwnd2DTypesScrollPane,&rcCheckbox);

      SetWindowPos(hwnd2DTypesScrollPane,HWND_TOP,0,0,rcCheckbox.right - rcCheckbox.left,yCheckbox,SWP_NOMOVE);

      controlId = IDDI_PLOT_TYPE_3D_MIN;

      GetWindowRect(GetDlgItem(hwnd,controlId),&rcCheckbox);

      cxCheckbox = rcCheckbox.right - rcCheckbox.left;
      cyCheckbox = rcCheckbox.bottom - rcCheckbox.top;
      xCheckbox = rcCheckbox.left - rcParent.left;
      yCheckbox = 0;

      while ( 0 < type3DNames.size() ) {

         char *pszTemp = type3DNames.front();
         type3DNames.pop_front();

         controlIdToType[controlId] = type3DIds.front();
         type3DIds.pop_front();

         HWND hwndCheckbox = CreateWindowEx(0L,"BUTTON",pszTemp,BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE,xCheckbox,yCheckbox,cxCheckbox,cyCheckbox,hwndScrollPanes[1],(HMENU)controlId,hModule,(void *)p);

         SendMessage(hwndCheckbox,WM_SETFONT,(LPARAM)hGUIFont,0L);

         type3DToCheckBox[controlIdToType[controlId]] = hwndCheckbox;

         delete [] pszTemp;

         max3DTypeControlId = controlId;

         controlId++;

         yCheckbox += cyCheckbox + 8;

      }

      GetWindowRect(hwnd3DTypesScrollPane,&rcCheckbox);

      SetWindowPos(hwnd3DTypesScrollPane,HWND_TOP,0,0,rcCheckbox.right - rcCheckbox.left,yCheckbox,SWP_NOMOVE);

      hwndPaneHosts[0] = hwnd2DTypesPaneHost = CreateWindowEx(0,"STATIC","",WS_VISIBLE | WS_CHILD,0,0,0,0,hwnd,NULL,hModule,(void *)p);
      hwndPaneHosts[1] = hwnd3DTypesPaneHost = CreateWindowEx(0,"STATIC","",WS_VISIBLE | WS_CHILD,0,0,0,0,hwnd,NULL,hModule,(void *)p);

      SetParent(hwnd2DTypesScrollPane,hwnd2DTypesPaneHost);
      SetWindowPos(hwnd2DTypesScrollPane,HWND_TOP,0,0,0,0,SWP_NOSIZE);

      SetWindowLongPtr(hwnd2DTypesScrollPane,GWLP_USERDATA,(ULONG_PTR)p);
      SetWindowLongPtr(hwnd2DTypesScrollPane,GWLP_WNDPROC,(ULONG_PTR)scrollPaneHandler);

      SetParent(hwnd3DTypesScrollPane,hwnd3DTypesPaneHost);
      SetWindowPos(hwnd3DTypesScrollPane,HWND_TOP,0,0,0,0,SWP_NOSIZE);

      SetWindowLongPtr(hwnd3DTypesScrollPane,GWLP_USERDATA,(ULONG_PTR)p);
      SetWindowLongPtr(hwnd3DTypesScrollPane,GWLP_WNDPROC,(ULONG_PTR)scrollPaneHandler);

      }
      return LRESULT(0);

   case WM_DESTROY:
      hwndSampleGraphic = NULL;
      break;

   case WM_SHOWWINDOW: {

      if ( ! wParam )
         break;

      if ( lParam )
         break;

      SetParent(hwndSampleGraphic,hwnd);

      long pType2D;
      long pType3D;

      if ( p -> overrideOwnerType ) {
         pType2D = p -> plotType2D;
         pType3D = p -> plotType3D;
         EnableWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_OVERRIDE_OWNER),p -> pOwnerProperty2DPlotType ? TRUE : FALSE);
      } else {
         p -> pOwnerProperty2DPlotType -> get_longValue(reinterpret_cast<long*>(&pType2D));
         p -> pOwnerProperty3DPlotType -> get_longValue(reinterpret_cast<long *>(&pType3D));
      }

      p -> propertyOverrideOwnerType -> setWindowChecked(GetDlgItem(hwnd,IDDI_PLOT_TYPE_OVERRIDE_OWNER));

      enum DataArity dataArity = p -> pIDataSet -> DataArity();

      //std::function<void(HWND)> *pEnable = new std::function<void(HWND)>( [=](HWND hwndChild) { EnableWindow(hwndChild,DATA_ARITY_3D == dataArity ? TRUE : FALSE); });
      //EnableWindow(hwnd3DTypes,DATA_ARITY_3D == dataArity ? TRUE : FALSE);
      //DoOnWindowDescendants(hwnd3DTypes,pEnable);

      long isOverride = 0L;

      p -> propertyOverrideOwnerType -> get_longValue(&isOverride);

      RECT rcDialog;
      RECT rcSample;

      GetWindowRect(hwnd,&rcDialog);

      if ( ! isOverride ) {

         ShowWindow(hwndArityTabs,SW_HIDE);
         ShowWindow(hwndPaneHosts[0],SW_HIDE);
         ShowWindow(hwndPaneHosts[1],SW_HIDE);
         ShowWindow(hwndScrollBars[0],SW_HIDE);
         ShowWindow(hwndScrollBars[1],SW_HIDE);

         ShowWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_NONE_SELECTED),SW_HIDE);

         rcSample.left = 16;
         rcSample.top = 48;
         rcSample.right = rcSample.left + rcDialog.right - rcDialog.left - 32;
         rcSample.bottom = rcSample.top + rcDialog.bottom - rcDialog.top - 48 - 16;

      } else {

         long maxScrollPaneBottom = 0;

         for ( long k = 0; 1; k++ ) {

            if ( ! hwndScrollPanes[k] )
               break;

            RECT rcScrollPane;

            GetWindowRect(hwndScrollPanes[k],&rcScrollPane);
            
            maxScrollPaneBottom = max(maxScrollPaneBottom,rcScrollPane.bottom);

         }
         RECT rcTabs;

         GetWindowRect(hwndArityTabs,&rcTabs);

         rcTabs.bottom = min(rcDialog.bottom - 24,maxScrollPaneBottom);

         SetWindowPos(hwndArityTabs,HWND_TOP,0,0,rcTabs.right - rcTabs.left,rcTabs.bottom - rcTabs.top,SWP_NOMOVE | SWP_SHOWWINDOW);

         long cxScroll = GetSystemMetrics(SM_CXVSCROLL);

         for ( long k = 0; 1; k++ ) {

            if ( ! hwndScrollPanes[k] )
               break;

            SetWindowPos(hwndPaneHosts[k],HWND_TOP,rcTabs.left - rcDialog.left + 8,rcTabs.top - rcDialog.top + 32,rcTabs.right - rcTabs.left - 16 - cxScroll,rcTabs.bottom - rcTabs.top - 32 - 8,SWP_SHOWWINDOW);
            SetWindowPos(hwndScrollBars[k],HWND_TOP,rcTabs.right - rcDialog.left - cxScroll - 4,rcTabs.top - rcDialog.top + 32,GetSystemMetrics(SM_CXVSCROLL),rcTabs.bottom - rcTabs.top - 32 - 8,SWP_SHOWWINDOW);

            RECT rcPane;

            GetWindowRect(hwndScrollPanes[k],&rcPane);

            SCROLLINFO si = {0};
            si.cbSize = sizeof(si);
            si.fMask = SIF_RANGE | SIF_POS | SIF_PAGE; 
            si.nMin = 0;
            si.nMax = rcPane.bottom - rcPane.top;
            si.nPos = 0;
            si.nPage = (rcTabs.bottom - rcTabs.top - 32);

            needsToScroll[k] = si.nMax > si.nPage;

            SendMessage(hwndScrollBars[k],SBM_SETSCROLLINFO,(WPARAM)TRUE,(LPARAM)&si);
            SendMessage(hwndScrollBars[k],SBM_ENABLE_ARROWS,(WPARAM)ESB_DISABLE_UP,0L);

            ShowWindow(hwndScrollPanes[k],SW_SHOW);

            ShowWindow(hwndPaneHosts[k],SW_HIDE);
            ShowWindow(hwndScrollBars[k],SW_HIDE);

         }

         long currentIndex = SendMessage(hwndArityTabs,TCM_GETCURSEL,0L,0L);

         if ( -1 == currentIndex ) {
            SendMessage(hwndArityTabs,TCM_SETCURSEL,0L,0L);
            currentIndex = 0;
         }

         ShowWindow(hwndPaneHosts[currentIndex],SW_SHOW);
         if ( needsToScroll[currentIndex] )
            ShowWindow(hwndScrollBars[currentIndex],SW_SHOW);

         RECT rect;

         GetWindowRect(hwndArityTabs,&rect);

         rcSample.left = rect.right - rcDialog.left + 8;
         rcSample.top = 8;
         rcSample.right = rcSample.left + rcDialog.right - rcDialog.left - (rect.right - rcDialog.left) - 16;
         rcSample.bottom = rcDialog.bottom - rcDialog.top - 16;

         SET_TYPES_2D

         SET_TYPES_3D

         if ( 0 == pType2D && 0 == pType2D )
            ShowWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_NONE_SELECTED),SW_SHOW);
         else
            ShowWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_NONE_SELECTED),SW_HIDE);

      }

      SendMessage(hwndSampleGraphic,WMG_POSITION_SAMPLE_GRAPHIC,0L,(LPARAM)&rcSample);

      InvalidateRect(hwndSampleGraphic,NULL,TRUE);

      }
      break;

   case WM_NOTIFY: {
      NMHDR *pHeader = (NMHDR *)lParam;
      switch ( wParam ) {
      case IDDI_PLOT_TYPE_ARITY_TABS: {
         long currentIndex = SendMessage(pHeader -> hwndFrom,TCM_GETCURSEL,0L,0L);
         if ( TCN_SELCHANGING == pHeader -> code ) {
            if ( -1 < currentIndex ) {
               ShowWindow(hwndPaneHosts[currentIndex],SW_HIDE);
               ShowWindow(hwndScrollBars[currentIndex],SW_HIDE);
            }
            break;
         }
         if ( TCN_SELCHANGE == pHeader -> code ) {
            ShowWindow(hwndPaneHosts[currentIndex],SW_SHOW);
            if ( needsToScroll[currentIndex] )
               ShowWindow(hwndScrollBars[currentIndex],SW_SHOW);
         }
         }
         break;

      }
      }
      break;

   case WM_COMMAND: {

      if ( ! p ) break;

      int controlID = LOWORD(wParam);

      if ( IDDI_PLOT_TYPE_2D_MIN <= controlID && controlID <= max2DTypeControlId ) {

         if ( BST_CHECKED == SendMessage((HWND)lParam,BM_GETCHECK,0L,0L) )
            p -> plotType2D = p -> plotType2D | controlIdToType[controlID];
         else
            p -> plotType2D = p -> plotType2D & ( ~ controlIdToType[controlID] );

         if ( 0 == p -> plotType2D && 0 == p -> plotType3D )
            ShowWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_NONE_SELECTED),SW_SHOW);
         else
            ShowWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_NONE_SELECTED),SW_HIDE);

         InvalidateRect(hwndSampleGraphic,NULL,TRUE);

         break;
      }

      if ( IDDI_PLOT_TYPE_3D_MIN <= controlID && controlID <= max3DTypeControlId ) {

         if ( BST_CHECKED == SendMessage((HWND)lParam,BM_GETCHECK,0L,0L) )
            p -> plotType3D = p -> plotType3D | controlIdToType[controlID];
         else
            p -> plotType3D = p -> plotType3D & ( ~ controlIdToType[controlID] );

         if ( 0 == p -> plotType2D && 0 == p -> plotType3D )
            ShowWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_NONE_SELECTED),SW_SHOW);
         else
            ShowWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_NONE_SELECTED),SW_HIDE);

         InvalidateRect(hwndSampleGraphic,NULL,TRUE);

         break;
      }

      switch ( controlID ) {

      case IDDI_PLOT_TYPE_OVERRIDE_OWNER: {

         gc2DPlotTypes pType2D;
         gc3DPlotTypes pType3D;

         p -> propertyOverrideOwnerType -> getWindowChecked((HWND)lParam);

         if ( p -> overrideOwnerType ) {
            pType2D = (gc2DPlotTypes)p -> plotType2D;
            pType3D = (gc3DPlotTypes)p -> plotType3D;
            EnableWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_OVERRIDE_OWNER),p -> pOwnerProperty2DPlotType ? TRUE : FALSE);
         } else {
            p -> pOwnerProperty2DPlotType -> get_longValue(reinterpret_cast<long*>(&pType2D));
            p -> pOwnerProperty3DPlotType -> get_longValue(reinterpret_cast<long*>(&pType3D));
         }

         SET_TYPES_2D

         SET_TYPES_3D

         std::function<void(HWND)> *pEnable = new std::function<void(HWND)>( [=](HWND hwndChild) {EnableWindow(hwndChild,p -> overrideOwnerType ? TRUE : FALSE);});

         DoOnWindowDescendants(hwnd2DTypesScrollPane,pEnable);

         pEnable = new std::function<void(HWND)>( [=](HWND hwndChild) {EnableWindow(hwndChild,p -> overrideOwnerType ? TRUE : FALSE);});

         DoOnWindowDescendants(hwnd3DTypesScrollPane,pEnable);

         typeHandler(hwnd,WM_SHOWWINDOW,1L,0L);

         }
         break;
      }

      InvalidateRect(hwndSampleGraphic,NULL,TRUE);

      return LRESULT(0);
      }

   case WM_VSCROLL: {

      HWND hwndScrollBar = (HWND)lParam;

      if ( ! ( hwndScrollBar == hwnd2DTypesScrollBar ) && ! ( hwndScrollBar == hwnd3DTypesScrollBar ) ) 
         return LRESULT(FALSE);

      if ( LOWORD(wParam) == SB_ENDSCROLL ) 
         return LRESULT(FALSE);

      long sbIndex = 0;

      if ( hwndScrollBar == hwnd3DTypesScrollBar )
         sbIndex = 1;

      SCROLLINFO si;
      memset(&si,0,sizeof(SCROLLINFO));
      si.cbSize = sizeof(SCROLLINFO);
      si.fMask = SIF_POS | SIF_RANGE | SIF_PAGE;

      SendMessage(hwndScrollBar,SBM_GETSCROLLINFO,(WPARAM)0,(LPARAM)&si);

      switch ( LOWORD(wParam) ) {
      case SB_LINEDOWN:
         si.nPos++;
         break;

      case SB_PAGEDOWN:
         si.nPos += si.nPage;
         break;

      case SB_LINEUP:
         si.nPos--;
         break;

      case SB_PAGEUP:
         si.nPos -= si.nPage;
         break;

      case SB_THUMBPOSITION:
      case SB_THUMBTRACK:
         si.nPos = HIWORD(wParam);
         break;

      default:
         return LRESULT(FALSE);
      }

      si.fMask = SIF_POS;

      SetScrollInfo((HWND)lParam,SB_CTL,&si,TRUE);

      if ( si.nMax <= si.nPos )
         SendMessage(hwndScrollBar,SBM_ENABLE_ARROWS,(WPARAM)ESB_DISABLE_DOWN,0L);

      SetWindowPos(hwndScrollPanes[sbIndex],HWND_BOTTOM,0,-si.nPos,0,0,SWP_NOSIZE);

      }
      break;

   default:
      break;
   }

   return LRESULT(0);
   }



   LRESULT CALLBACK Plot::scrollPaneHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   Plot *p = (Plot *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch ( msg ) {

   case WM_VSCROLL: 
      return SendMessage(GetParent(GetParent(GetParent(hwnd))),msg,wParam,lParam);

   case WM_COMMAND:
      return SendMessage(GetParent(GetParent(hwnd)),msg,wParam,lParam);

   default:
      break;

   }

   return DefWindowProc(hwnd,msg,wParam,lParam);
   }