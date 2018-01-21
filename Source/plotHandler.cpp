// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"
#include <stdio.h>
#include <functional>
#include <map>
#include <list>

#include "Graphic_resource.h"
#include "GMessage.h"

#include "utils.h"
#include "utilities.h"

#define SET_TYPES_2D                                                                                              \
for ( std::pair<long,HWND> pPair : type2DToCheckBox ) {                                                           \
   long isSelected = (long)pPair.first & (long)pType2D;                                                           \
   SendMessage(pPair.second,BM_SETCHECK,isSelected ? BST_CHECKED : BST_UNCHECKED,0L);                             \
   HWND hwndProperties = type2DToProperties[pPair.first];                                                         \
   if ( ! isSelected ) {                                                                                          \
      ShowWindow(hwndProperties,SW_HIDE);                                                                         \
      InvalidateRect(hwndPaneHosts[0],NULL,TRUE);                                                                 \
      continue;                                                                                                   \
   }                                                                                                              \
   ShowWindow(hwndProperties,SW_SHOW);                                                                            \
}

#define SET_TYPES_3D \
for ( std::pair<long,HWND> pPair : type3DToCheckBox ) {                                                           \
   long isSelected = (long)pPair.first & (long)pType3D;                                                           \
   SendMessage(pPair.second,BM_SETCHECK,isSelected ? BST_CHECKED : BST_UNCHECKED,0L);                             \
   HWND hwndProperties = type3DToProperties[pPair.first];                                                         \
   if ( ! isSelected ) {                                                                                          \
      ShowWindow(hwndProperties,SW_HIDE);                                                                         \
      InvalidateRect(hwndPaneHosts[1],NULL,TRUE);                                                                 \
      continue;                                                                                                   \
   }                                                                                                              \
   ShowWindow(hwndProperties,SW_SHOW);                                                                            \
}

   HWND hwndScrollBars[] = {NULL,NULL,NULL};
   HWND hwndPaneHosts[] = {NULL,NULL,NULL};
   HWND hwndScrollPanes[] = {NULL,NULL,NULL};

   long max2DTypeControlId = 0;

   long max3DTypeControlId = 0;

   bool needsToScroll[] = {false,false,false};

   std::map<long,long> controlIdToType;
   std::map<long,long> controlIdToTypeInstanceNumber;
   static std::map<long,HWND> type2DToCheckBox;
   static std::map<long,HWND> type3DToCheckBox;
   static std::map<long,HWND> type2DToProperties;
   static std::map<long,HWND> type3DToProperties;

   std::list<IGSystemPlotType *> needingRelease2D;
   std::list<IGSystemPlotType *> needingRelease3D;

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

      hwndScrollPanes[0] = GetDlgItem(hwnd,IDDI_GRAPHIC_PLOTS_2D_TYPES_SCROLL_PANE);
      hwndScrollPanes[1] = GetDlgItem(hwnd,IDDI_GRAPHIC_PLOTS_3D_TYPES_SCROLL_PANE);

      hwndScrollBars[0] = GetDlgItem(hwnd,IDDI_GRAPHIC_PLOTS_2D_TYPES_SCROLL_BAR);
      hwndScrollBars[1] = GetDlgItem(hwnd,IDDI_GRAPHIC_PLOTS_3D_TYPES_SCROLL_BAR);

      SAFEARRAY *p2DTypeIDs,*p2DTypeNames,*p3DTypeIDs,*p3DTypeNames;
      SAFEARRAY *p2DTypeInstances,*p3DTypeInstances;
      SAFEARRAY *p2DTypeInstanceNumbers,*p3DTypeInstanceNumbers;

      p -> pIPlotServicesObject -> GetPlotTypesInformation(&p2DTypeIDs,&p3DTypeIDs,&p2DTypeNames,&p3DTypeNames,&p2DTypeInstanceNumbers,&p3DTypeInstanceNumbers,&p2DTypeInstances,&p3DTypeInstances);

      long *pvIDs = NULL;
      BSTR *pvNames = NULL;
      IUnknown **pvIUnknowns = NULL;
      long *pvInstanceNumbers = NULL;

      long count2DTypes = 0;

      SafeArrayGetUBound(p2DTypeIDs,1,&count2DTypes);

      SafeArrayAccessData(p2DTypeIDs,reinterpret_cast<void **>(&pvIDs));
      SafeArrayAccessData(p2DTypeNames,reinterpret_cast<void **>(&pvNames));
      SafeArrayAccessData(p2DTypeInstanceNumbers,reinterpret_cast<void **>(&pvInstanceNumbers));
      SafeArrayAccessData(p2DTypeInstances,reinterpret_cast<void **>(&pvIUnknowns));

      std::list<long> type2DIds;
      std::list<char *> type2DNames;
      std::list<IGSystemPlotType *> typePropertyInstance2D;

      needingRelease2D.clear();

      long controlId = IDDI_PLOT_TYPE_2D_MIN;

      for ( long k = 0; k < count2DTypes; k++ ) {

         type2DIds.push_back(pvIDs[k]);

         long n = wcslen(pvNames[k]);

         char *pszTemp = new char[n + 1];

         pszTemp[n] = '\0';

         WideCharToMultiByte(CP_ACP,0,pvNames[k],-1,pszTemp,n,0,0);

         type2DNames.push_back(pszTemp);

         IGSystemPlotType *pIGSystemPlotType = NULL;

         pvIUnknowns[k] -> QueryInterface(IID_IGSystemPlotType,reinterpret_cast<void **>(&pIGSystemPlotType));

         VARIANT_BOOL hasProperties = VARIANT_FALSE;

         pIGSystemPlotType -> get_HasProperties(pvInstanceNumbers[k],&hasProperties);

         needingRelease2D.push_back(pIGSystemPlotType);

         if ( VARIANT_FALSE == hasProperties )
            pIGSystemPlotType = NULL;

         controlIdToTypeInstanceNumber[controlId] = pvInstanceNumbers[k];

         typePropertyInstance2D.push_back(pIGSystemPlotType);

         controlId++;

      }

      SafeArrayUnaccessData(p2DTypeIDs);
      SafeArrayUnaccessData(p2DTypeNames);
      SafeArrayUnaccessData(p2DTypeInstanceNumbers);
      SafeArrayUnaccessData(p2DTypeInstances);

      SafeArrayDestroy(p2DTypeIDs);
      SafeArrayDestroy(p2DTypeNames);
      SafeArrayDestroy(p2DTypeInstanceNumbers);
      SafeArrayDestroy(p2DTypeInstances);

      long count3DTypes = 0;

      SafeArrayGetUBound(p3DTypeIDs,1,&count3DTypes);

      SafeArrayAccessData(p3DTypeIDs,reinterpret_cast<void **>(&pvIDs));
      SafeArrayAccessData(p3DTypeInstanceNumbers,reinterpret_cast<void **>(&pvInstanceNumbers));
      SafeArrayAccessData(p3DTypeNames,reinterpret_cast<void **>(&pvNames));
      SafeArrayAccessData(p3DTypeInstances,reinterpret_cast<void **>(&pvIUnknowns));

      std::list<long> type3DIds;
      std::list<char *> type3DNames;
      std::list<IGSystemPlotType *> typePropertyInstance3D;

      needingRelease3D.clear();

      controlId = IDDI_PLOT_TYPE_3D_MIN;

      for ( long k = 0; k < count3DTypes; k++ ) {
         type3DIds.push_back(pvIDs[k]);
         long n = wcslen(pvNames[k]);
         char *pszTemp = new char[n + 1];
         pszTemp[n] = '\0';
         WideCharToMultiByte(CP_ACP,0,pvNames[k],-1,pszTemp,n,0,0);
         type3DNames.push_back(pszTemp);

         IGSystemPlotType *pIGSystemPlotType = NULL;

         pvIUnknowns[k] -> QueryInterface(IID_IGSystemPlotType,reinterpret_cast<void **>(&pIGSystemPlotType));

         VARIANT_BOOL hasProperties = VARIANT_FALSE;

         pIGSystemPlotType -> get_HasProperties(pvInstanceNumbers[k],&hasProperties);

         needingRelease3D.push_back(pIGSystemPlotType);

         if ( VARIANT_FALSE == hasProperties )
            pIGSystemPlotType = NULL;

         controlIdToTypeInstanceNumber[controlId] = pvInstanceNumbers[k];

         typePropertyInstance3D.push_back(pIGSystemPlotType);

         controlId++;

      }

      SafeArrayUnaccessData(p3DTypeIDs);
      SafeArrayUnaccessData(p3DTypeInstanceNumbers);
      SafeArrayUnaccessData(p3DTypeNames);
      SafeArrayUnaccessData(p3DTypeInstances);

      SafeArrayDestroy(p3DTypeIDs);
      SafeArrayDestroy(p3DTypeNames);
      SafeArrayDestroy(p3DTypeInstanceNumbers);
      SafeArrayDestroy(p3DTypeInstances);

      controlId = IDDI_PLOT_TYPE_2D_MIN;

      RECT rcCheckbox,rcParent;
      GetWindowRect(hwnd,&rcParent);
      GetWindowRect(GetDlgItem(hwnd,controlId),&rcCheckbox);

      long cxCheckbox = rcCheckbox.right - rcCheckbox.left;
      long cyCheckbox = rcCheckbox.bottom - rcCheckbox.top;
      long xCheckbox = rcCheckbox.left - rcParent.left;
      long yCheckbox = 4;

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

         HWND hwndProperties = CreateWindowEx(0L,"BUTTON","...",BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,xCheckbox + 80,yCheckbox - 2,32,cyCheckbox + 4,hwndScrollPanes[0],(HMENU)(controlId + 1000),hModule,NULL);

         ShowWindow(hwndProperties,SW_HIDE);

         IGSystemPlotType *pIGSystemPlotType = typePropertyInstance2D.front();

         SetWindowLongPtr(hwndProperties,GWL_USERDATA,(ULONG_PTR)pIGSystemPlotType);

         SendMessage(hwndProperties,WM_SETFONT,(LPARAM)hGUIFont,0L);

         type2DToProperties[controlIdToType[controlId]] = hwndProperties;

         typePropertyInstance2D.pop_front();

         yCheckbox += cyCheckbox + 8;

         max2DTypeControlId = controlId;

         controlId++;

      }

      GetWindowRect(hwndScrollPanes[0],&rcCheckbox);

      SetWindowPos(hwndScrollPanes[0],HWND_TOP,0,0,rcCheckbox.right - rcCheckbox.left,yCheckbox,SWP_NOMOVE);

      controlId = IDDI_PLOT_TYPE_3D_MIN;

      GetWindowRect(GetDlgItem(hwnd,controlId),&rcCheckbox);

      cxCheckbox = rcCheckbox.right - rcCheckbox.left;
      cyCheckbox = rcCheckbox.bottom - rcCheckbox.top;
      xCheckbox = rcCheckbox.left - rcParent.left;
      yCheckbox = 4;

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

         HWND hwndProperties = CreateWindowEx(0L,"BUTTON","...",BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,xCheckbox + 80,yCheckbox - 2,32,cyCheckbox + 4,hwndScrollPanes[1],(HMENU)(controlId + 1000),hModule,NULL);

         ShowWindow(hwndProperties,SW_HIDE);
         IGSystemPlotType *pIGSystemPlotType = typePropertyInstance3D.front();

         SetWindowLongPtr(hwndProperties,GWL_USERDATA,(ULONG_PTR)pIGSystemPlotType);

         SendMessage(hwndProperties,WM_SETFONT,(LPARAM)hGUIFont,0L);

         type3DToProperties[controlIdToType[controlId]] = hwndProperties;

         typePropertyInstance3D.pop_front();

         controlId++;

         yCheckbox += cyCheckbox + 8;

      }

      GetWindowRect(hwndScrollPanes[1],&rcCheckbox);

      SetWindowPos(hwndScrollPanes[1],HWND_TOP,0,0,rcCheckbox.right - rcCheckbox.left,yCheckbox,SWP_NOMOVE);

      hwndPaneHosts[0] = CreateWindowEx(0,"STATIC","",WS_VISIBLE | WS_CHILD,0,0,0,0,hwnd,NULL,hModule,(void *)p);
      hwndPaneHosts[1] = CreateWindowEx(0,"STATIC","",WS_VISIBLE | WS_CHILD,0,0,0,0,hwnd,NULL,hModule,(void *)p);

      SetParent(hwndScrollPanes[0],hwndPaneHosts[0]);
      SetWindowPos(hwndScrollPanes[0],HWND_TOP,0,0,0,0,SWP_NOSIZE);

      SetWindowLongPtr(hwndScrollPanes[0],GWLP_USERDATA,(ULONG_PTR)p);
      SetWindowLongPtr(hwndScrollPanes[0],GWLP_WNDPROC,(ULONG_PTR)scrollPaneHandler);

      SetParent(hwndScrollPanes[1],hwndPaneHosts[1]);
      SetWindowPos(hwndScrollPanes[1],HWND_TOP,0,0,0,0,SWP_NOSIZE);

      SetWindowLongPtr(hwndScrollPanes[1],GWLP_USERDATA,(ULONG_PTR)p);
      SetWindowLongPtr(hwndScrollPanes[1],GWLP_WNDPROC,(ULONG_PTR)scrollPaneHandler);

      }
      return LRESULT(FALSE);
 
   case WM_DESTROY:
      p -> hwndSampleGraphic = NULL;
      for ( IGSystemPlotType *ppt : needingRelease2D )
         ppt -> Release();
      needingRelease2D.clear();
      for ( IGSystemPlotType *ppt : needingRelease3D )
         ppt -> Release();
      needingRelease3D.clear();
      break;

   case WM_SHOWWINDOW: {

      if ( ! wParam )
         break;

      if ( lParam )
         break;

      if ( ! ( GetParent(hwndSampleGraphic) == hwnd ) )
         SetParent(hwndSampleGraphic,hwnd);

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

      //SetWindowPos(GetDlgItem(hwnd,IDDI_GRAPHIC_PLOTS_DEFAULTS_BOX),HWND_TOP,0,0,rcTabs.right - rcTabs.left + 16,rcTabs.bottom - rcTabs.top + 32,SWP_NOMOVE);

      SetWindowPos(GetDlgItem(hwnd,IDDI_GRAPHIC_PLOTS_ARITY_TABS),HWND_TOP,0,0,rcTabs.right - rcTabs.left,rcTabs.bottom - rcTabs.top,SWP_NOMOVE);

      long cxScroll = GetSystemMetrics(SM_CXVSCROLL);

      long currentIndex = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_PLOTS_ARITY_TABS,TCM_GETCURSEL,0L,0L);

      if ( -1 == currentIndex ) {
         SendDlgItemMessage(hwnd,IDDI_GRAPHIC_PLOTS_ARITY_TABS,TCM_SETCURSEL,0L,0L);
         currentIndex = 0;
      }

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

         if ( ! ( k == currentIndex ) ) 
            ShowWindow(hwndPaneHosts[k],SW_HIDE);

         ShowWindow(hwndScrollBars[k],SW_HIDE);

      }

      ShowWindow(hwndPaneHosts[currentIndex],SW_SHOW);
      ShowWindow(hwndScrollPanes[currentIndex],SW_SHOW);

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

      if ( ! ( hwndScrollBar == hwndScrollBars[0] ) && ! ( hwndScrollBar == hwndScrollBars[1] ) ) 
         return LRESULT(FALSE);

      if ( LOWORD(wParam) == SB_ENDSCROLL ) 
         return LRESULT(FALSE);

      long sbIndex = 0;
      if ( hwndScrollBar == hwndScrollBars[1] )
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

         G::plotHandler(hwnd,WM_SHOWWINDOW,1L,0L);

         break;

      }

      if ( IDDI_PLOT_TYPE_3D_MIN <= controlID && controlID <= max3DTypeControlId ) {

         if ( BST_CHECKED == SendMessage((HWND)lParam,BM_GETCHECK,0L,0L) )
            p -> default3DPlotType = (gc3DPlotTypes)((long)p -> default3DPlotType | controlIdToType[controlID]);
         else
            p -> default3DPlotType = (gc3DPlotTypes)((long)p -> default3DPlotType & ( ~ controlIdToType[controlID]));

         G::plotHandler(hwnd,WM_SHOWWINDOW,1L,0L);

         break;

      }

      if ( (IDDI_PLOT_TYPE_2D_MIN + 1000) <= controlID && controlID <= (max2DTypeControlId + 1000) ) {

         ((IGSystemPlotType *)GetWindowLongPtr((HWND)lParam,GWLP_USERDATA)) -> 
            ShowProperties(controlIdToTypeInstanceNumber[controlID - 1000],hwnd,GUID_NULL,NULL,NULL,0);

         G::plotHandler(hwnd,WM_SHOWWINDOW,1L,0L);

         InvalidateRect(hwndSampleGraphic,NULL,TRUE);

         break;
      }

      if ( (IDDI_PLOT_TYPE_3D_MIN + 1000) <= controlID && controlID <= (max3DTypeControlId + 1000) ) {

         ((IGSystemPlotType *)GetWindowLongPtr((HWND)lParam,GWLP_USERDATA)) -> 
            ShowProperties(controlIdToTypeInstanceNumber[controlID - 1000],hwnd,GUID_NULL,NULL,NULL,0);

         G::plotHandler(hwnd,WM_SHOWWINDOW,1L,0L);

         InvalidateRect(hwndSampleGraphic,NULL,TRUE);

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

               G::plotHandler(hwnd,WM_SHOWWINDOW,1L,0L);

               SendDlgItemMessage(hwnd,IDDI_GRAPHIC_PLOTS_LIST,LVM_SETITEM,0L,(LPARAM)&lvItem);

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