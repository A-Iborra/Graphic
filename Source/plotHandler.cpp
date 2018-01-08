
#include "Graphic.h"
#include <stdio.h>
#include <functional>
#include <map>
#include <list>

#include "Graphic_resource.h"
#include "GMessage.h"

#include "utils.h"
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

   long max2DTypeControlId = 0;

   long max3DTypeControlId = 0;

   bool needsToScroll[] = {false,false,false};

   std::map<long,long> controlIdToType;
   static std::map<long,HWND> type2DToCheckBox;
   static std::map<long,HWND> type3DToCheckBox;

   LRESULT CALLBACK scrollPaneHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

   LRESULT CALLBACK G::plotHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   G *p = (G *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = (PROPSHEETPAGE *)lParam;

      p = (G *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      p -> hwndPlotSettings = hwnd;

      char szTemp[128];
      sprintf(szTemp,"There are %ld plots on this graphic",p -> plotList.Count());
      SetWindowText(GetDlgItem(hwnd,IDDI_GRAPHIC_PLOTS_COUNT),szTemp);

      RECT rect;
      LVCOLUMN lvColumn;

      GetWindowRect(GetDlgItem(hwnd,IDDI_GRAPHIC_PLOTS_LIST),&rect);
      memset(&lvColumn,0,sizeof(LVCOLUMN));
      lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
      lvColumn.fmt = LVCFMT_LEFT;
      lvColumn.cx = rect.right - rect.left - 4;
      lvColumn.pszText = "Plot";

      SendDlgItemMessage(hwnd,IDDI_GRAPHIC_PLOTS_LIST,LVM_INSERTCOLUMN,0,(LPARAM)&lvColumn);

      EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_PLOTS_EDIT),FALSE);

      char szInstructions[1024];

      LoadString(hModule,IDDI_GRAPHIC_PLOTS_INSTRUCTIONS,szInstructions,1024);

      SetDlgItemText(hwnd,IDDI_GRAPHIC_PLOTS_INSTRUCTIONS,szInstructions);

      TCITEM tcItem = {0};

      tcItem.mask = TCIF_TEXT;
      tcItem.pszText = "2 and 3-D";

      SendDlgItemMessage(hwnd,IDDI_GRAPHIC_PLOTS_ARITY_TABS,TCM_INSERTITEM,0L,(LPARAM)&tcItem);

      tcItem.pszText = "3-D Only";

      SendDlgItemMessage(hwnd,IDDI_GRAPHIC_PLOTS_ARITY_TABS,TCM_INSERTITEM,1L,(LPARAM)&tcItem);

      SendDlgItemMessage(hwnd,IDDI_GRAPHIC_PLOTS_ARITY_TABS,TCM_SETCURSEL,-1L,0L);

      SetWindowPos(GetDlgItem(hwnd,IDDI_GRAPHIC_PLOTS_ARITY_TABS),HWND_TOP,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);

      hwndScrollPanes[0] = hwnd2DTypesScrollPane = GetDlgItem(hwnd,IDDI_GRAPHIC_PLOTS_2D_TYPES_SCROLL_PANE);
      hwndScrollPanes[1] = hwnd3DTypesScrollPane = GetDlgItem(hwnd,IDDI_GRAPHIC_PLOTS_3D_TYPES_SCROLL_PANE);

      hwndScrollBars[0] = hwnd2DTypesScrollBar = GetDlgItem(hwnd,IDDI_GRAPHIC_PLOTS_2D_TYPES_SCROLL_BAR);
      hwndScrollBars[1] = hwnd3DTypesScrollBar = GetDlgItem(hwnd,IDDI_GRAPHIC_PLOTS_3D_TYPES_SCROLL_BAR);

      SAFEARRAY *p2DTypeIDs,*p2DTypeNames,*p3DTypeIDs,*p3DTypeNames;

      p -> pIPlotServicesObject -> GetPlotTypesInformation(&p2DTypeIDs,&p3DTypeIDs,&p2DTypeNames,&p3DTypeNames);

      long *pvIDs = NULL;
      BSTR *pvNames = NULL;

      long count2DTypes = 0;

      SafeArrayGetUBound(p2DTypeIDs,1,&count2DTypes);

      SafeArrayAccessData(p2DTypeIDs,reinterpret_cast<void **>(&pvIDs));
      SafeArrayAccessData(p2DTypeNames,reinterpret_cast<void **>(&pvNames));

      std::list<long> type2DIds;
      std::list<char *> type2DNames;

      for ( long k = 0; k < count2DTypes; k++ ) {
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
      return LRESULT(FALSE);
 
   case WM_DESTROY:
      p -> hwndSampleGraphic = NULL;
      break;

   case WM_SHOWWINDOW: {

      if ( ! wParam )
         break;

      if ( lParam )
         break;

      SendDlgItemMessage(hwnd,IDDI_GRAPHIC_PLOTS_LIST,LVM_DELETEALLITEMS,0L,(LPARAM)0L);

      LVITEM lvItem;
      IPlot *pIPlot = NULL;
      while ( pIPlot = p -> plotList.GetNext(pIPlot) ) {
         memset(&lvItem,0,sizeof(LVITEM));
         long n;
         BSTR bstrExpression;
         char szTemp[128];
         pIPlot -> get_Name(&bstrExpression);
         if ( 0 == wcslen(bstrExpression) ) 
            sprintf(szTemp,"Plot #%d",p -> plotList.ID(pIPlot));
         else         
            WideCharToMultiByte(CP_ACP,0,bstrExpression,-1,szTemp,64,0,0);
         SysFreeString(bstrExpression);
         lvItem.mask = LVIF_TEXT | LVIF_PARAM;
         lvItem.iItem = 32768;
         lvItem.state = LVIS_SELECTED;
         lvItem.stateMask = -1;
         lvItem.pszText = szTemp;
         lvItem.lParam = reinterpret_cast<long>(pIPlot);
         SendDlgItemMessage(hwnd,IDDI_GRAPHIC_PLOTS_LIST,LVM_INSERTITEM,0L,(LPARAM)&lvItem);
      }

      SetParent(hwndSampleGraphic,hwnd);

      RECT rcSample,rcDialog;
      GetWindowRect(hwnd,&rcDialog);
      GetWindowRect(GetDlgItem(hwnd,IDDI_GRAPHIC_STYLE_SAMPLE),&rcSample);

      long cx = rcDialog.right - rcDialog.left - (rcSample.left - rcDialog.left) - 32;
      long cy = rcDialog.bottom - rcDialog.top - (rcSample.top - rcDialog.top) - 32;
      rcSample.left = (rcSample.left - rcDialog.left) + 16;
      rcSample.top = rcSample.top - rcDialog.top + 16;
      rcSample.right = rcSample.left + cx;
      rcSample.bottom = rcSample.top + cy;

      G::sampleGraphicHandler(hwndSampleGraphic,WMG_POSITION_SAMPLE_GRAPHIC,1L,(LPARAM)&rcSample);

      RECT rcTabs;

      GetWindowRect(GetDlgItem(hwnd,IDDI_GRAPHIC_PLOTS_ARITY_TABS),&rcTabs);

      rcTabs.bottom = rcDialog.bottom - 24;

      SetWindowPos(GetDlgItem(hwnd,IDDI_GRAPHIC_PLOTS_DEFAULTS_BOX),HWND_TOP,0,0,rcTabs.right - rcTabs.left + 16,rcTabs.bottom - rcTabs.top + 32,SWP_NOMOVE);

      SetWindowPos(GetDlgItem(hwnd,IDDI_GRAPHIC_PLOTS_ARITY_TABS),HWND_TOP,0,0,rcTabs.right - rcTabs.left,rcTabs.bottom - rcTabs.top,SWP_NOMOVE);

      long cxScroll = GetSystemMetrics(SM_CXVSCROLL);

      for ( long k = 0; 1; k++ ) {

         if ( ! hwndScrollPanes[k] )
            break;

         SetWindowPos(hwndPaneHosts[k],HWND_TOP,rcTabs.left - rcDialog.left + 8,rcTabs.top - rcDialog.top + 32,rcTabs.right - rcTabs.left - 16 - cxScroll,rcTabs.bottom - rcTabs.top - 32 - 8,0L);
         SetWindowPos(hwndScrollBars[k],HWND_TOP,rcTabs.right - rcDialog.left - cxScroll - 4,rcTabs.top - rcDialog.top + 32,GetSystemMetrics(SM_CXVSCROLL),rcTabs.bottom - rcTabs.top - 32 - 8,0L);

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

      long currentIndex = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_PLOTS_ARITY_TABS,TCM_GETCURSEL,0L,0L);

      if ( -1 == currentIndex ) {
         SendDlgItemMessage(hwnd,IDDI_GRAPHIC_PLOTS_ARITY_TABS,TCM_SETCURSEL,0L,0L);
         currentIndex = 0;
      }

      ShowWindow(hwndPaneHosts[currentIndex],SW_SHOW);
      if ( needsToScroll[currentIndex] )
         ShowWindow(hwndScrollBars[currentIndex],SW_SHOW);

      gc2DPlotTypes pType2D = p -> default2DPlotType;

      SET_TYPES_2D

      gc3DPlotTypes pType3D = p -> default3DPlotType;

      SET_TYPES_3D

      }
      break;

   case WM_NOTIFY: {
      NMHDR *pHeader = (NMHDR *)lParam;
      switch ( wParam ) {
      case IDDI_GRAPHIC_PLOTS_LIST: {
         long itemCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_PLOTS_LIST,LVM_GETITEMCOUNT,0L,0L);
         long selectedCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_PLOTS_LIST,LVM_GETSELECTEDCOUNT,0L,0L);
         EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_PLOTS_EDIT),itemCount && selectedCount ? TRUE : FALSE);
         if ( pHeader -> code == LVN_ITEMCHANGED ) {
            InvalidateRect(hwndSampleGraphic,NULL,TRUE);
            UpdateWindow(hwndSampleGraphic);
         }
         }
         break;

      case IDDI_GRAPHIC_PLOTS_ARITY_TABS: {
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

   case WM_COMMAND: {

      long controlID = LOWORD(wParam);

      if ( IDDI_PLOT_TYPE_2D_MIN <= controlID && controlID <= max2DTypeControlId ) {

         if ( BST_CHECKED == SendMessage((HWND)lParam,BM_GETCHECK,0L,0L) )
            p -> default2DPlotType = (gc2DPlotTypes)((long)p -> default2DPlotType | controlIdToType[controlID]);
         else
            p -> default2DPlotType = (gc2DPlotTypes)((long)p -> default2DPlotType & ( ~ controlIdToType[controlID]));

         break;

      }

      if ( IDDI_PLOT_TYPE_3D_MIN <= controlID && controlID <= max3DTypeControlId ) {

         if ( BST_CHECKED == SendMessage((HWND)lParam,BM_GETCHECK,0L,0L) )
            p -> default3DPlotType = (gc3DPlotTypes)((long)p -> default3DPlotType | controlIdToType[controlID]);
         else
            p -> default3DPlotType = (gc3DPlotTypes)((long)p -> default3DPlotType & ( ~ controlIdToType[controlID]));

         break;

      }

      switch ( LOWORD(wParam) ) {

      case IDDI_GRAPHIC_PLOTS_EDIT: {
         long selectedCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_PLOTS_LIST,LVM_GETSELECTEDCOUNT,0L,0L);
         long itemCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_PLOTS_LIST,LVM_GETITEMCOUNT,0L,0L);
         LVITEM lvItem;
         if ( ! itemCount ) {
            MessageBox(GetParent(hwnd),"There aren't any plots to edit.","Note",MB_OK);
            break;
         }
         if ( ! selectedCount ) {
            MessageBox(GetParent(hwnd),"Please select one of the plots, then select Edit.","Note",MB_OK);
            break;
         }
         if ( 1 < selectedCount ) {
            MessageBox(GetParent(hwnd),"Please select only one of the plots, then select Edit.","Note",MB_OK);
            break;
         }
         for ( int k = 0; k < itemCount; k++ ) {
            memset(&lvItem,0,sizeof(LVITEM));
            lvItem.iItem = k;
            lvItem.mask = LVIF_STATE | LVIF_PARAM;
            lvItem.stateMask = LVIS_SELECTED;
            SendDlgItemMessage(hwnd,IDDI_GRAPHIC_PLOTS_LIST,LVM_GETITEM,0L,(LPARAM)&lvItem);
            if ( lvItem.state && LVIS_SELECTED ) {

               IPlot * pIPlot = (IPlot *)lvItem.lParam;
               IUnknown* pIUnknown;
               pIPlot -> QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknown));
               p -> pIGProperties -> ShowProperties(hwnd,pIUnknown);
               pIUnknown -> Release();

               G::sampleGraphicHandler(hwndSampleGraphic,WMG_POSITION_SAMPLE_GRAPHIC,1L,(LPARAM)NULL);

               char szTemp[MAX_PROPERTY_SIZE];
               BSTR bstr = NULL;
               pIPlot -> get_Name(&bstr);
               if ( 0 < wcslen(bstr) ) {
                  WideCharToMultiByte(CP_ACP,0,bstr,MAX_PROPERTY_SIZE,szTemp,MAX_PROPERTY_SIZE,0,0);
                  lvItem.mask = LVIF_TEXT;
                  lvItem.pszText = szTemp;
                  SendDlgItemMessage(hwnd,IDDI_GRAPHIC_PLOTS_LIST,LVM_SETITEM,0L,(LPARAM)&lvItem);
               }
               break;
            }
         }
         }
         break;

      }

      }

      InvalidateRect(hwndSampleGraphic,NULL,TRUE);

      break;
 
   default:
      break;
   }
 
   return LRESULT(FALSE);
   }


   LRESULT CALLBACK scrollPaneHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
 
   switch ( msg ) {

   case WM_COMMAND:
      return SendMessage(GetParent(GetParent(hwnd)),msg,wParam,lParam);

   default:
      break;

   }

   return DefWindowProc(hwnd,msg,wParam,lParam);
   }