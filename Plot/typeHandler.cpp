// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Plot.h"

#include <map>

#include "Graphic_resource.h"
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
   if ( 0 == GetWindowLongPtr(hwndProperties,GWLP_USERDATA) )                                                     \
      continue;                                                                                                   \
   ShowWindow(hwndProperties,SW_SHOW);                                                                            \
   if ( ! ( p -> plotType2DInstanceGUIDS.find(pPair.first) == p -> plotType2DInstanceGUIDS.end() ) )              \
      continue;                                                                                                   \
   GUID theGUID;                                                                                                  \
   CoCreateGuid(&theGUID);                                                                                        \
   p -> plotType2DInstanceGUIDS[pPair.first] = theGUID;                                                           \
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
   if ( 0 == GetWindowLongPtr(hwndProperties,GWLP_USERDATA) )                                                     \
      continue;                                                                                                   \
   ShowWindow(hwndProperties,SW_SHOW);                                                                            \
   if ( ! ( p -> plotType3DInstanceGUIDS.find(pPair.first) == p -> plotType3DInstanceGUIDS.end() ) )              \
      continue;                                                                                                   \
   GUID theGUID;                                                                                                  \
   CoCreateGuid(&theGUID);                                                                                        \
   p -> plotType3DInstanceGUIDS[pPair.first] = theGUID;                                                           \
}

   HWND hwndScrollPanes[] = {NULL,NULL,NULL};
   HWND hwndScrollBars[] = {NULL,NULL,NULL};

   HWND hwndPaneHosts[] = {NULL,NULL,NULL};

   HWND hwndArityTabs = NULL;

   long max2DTypeControlId = 0;

   long max3DTypeControlId = 0;

   bool needsToScroll[] = {false,false,false};

   std::map<long,long> controlIdToType;
   static std::map<long,HWND> type2DToCheckBox;
   static std::map<long,HWND> type3DToCheckBox;
   static std::map<long,HWND> type2DToProperties;
   static std::map<long,HWND> type3DToProperties;

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

      hwndScrollPanes[0] = GetDlgItem(hwnd,IDDI_PLOT_TYPE_2D_TYPES_SCROLL_PANE);
      hwndScrollPanes[1] = GetDlgItem(hwnd,IDDI_PLOT_TYPE_3D_TYPES_SCROLL_PANE);

      hwndScrollBars[0] = GetDlgItem(hwnd,IDDI_PLOT_TYPE_2D_TYPES_SCROLL_BAR);
      hwndScrollBars[1] = GetDlgItem(hwnd,IDDI_PLOT_TYPE_3D_TYPES_SCROLL_BAR);

      SAFEARRAY *p2DTypeIDs,*p2DTypeNames,*p3DTypeIDs,*p3DTypeNames;
      SAFEARRAY *p2DTypeInstances,*p3DTypeInstances;
      SAFEARRAY *p2DTypeInstanceNumbers,*p3DTypeInstanceNumbers;

      p -> GetPlotTypesInformation(&p2DTypeIDs,&p3DTypeIDs,&p2DTypeNames,&p3DTypeNames,
                                       &p2DTypeInstanceNumbers,&p3DTypeInstanceNumbers,
                                       &p2DTypeInstances,&p3DTypeInstances);

      long *pvIDs = NULL;
      BSTR *pvNames = NULL;

      long count2DTypes = 0;

      SafeArrayGetUBound(p2DTypeIDs,1,&count2DTypes);

      SafeArrayAccessData(p2DTypeIDs,reinterpret_cast<void **>(&pvIDs));
      SafeArrayAccessData(p2DTypeNames,reinterpret_cast<void **>(&pvNames));

      std::list<long> type2DIds;
      std::list<char *> type2DNames;
      std::list<IGSystemPlotType *> typePropertyInstance2D;

      for ( long k = 0; k < count2DTypes; k++ ) {
         type2DIds.push_back(pvIDs[k]);
         long n = (DWORD)wcslen(pvNames[k]);
         char *pszTemp = new char[n + 1];
         pszTemp[n] = '\0';
         WideCharToMultiByte(CP_ACP,0,pvNames[k],-1,pszTemp,n,0,0);
         type2DNames.push_back(pszTemp);
// Take this from the safearray
         IGSystemPlotType *pIGSystemPlotType = Plot::plotType2DProviderInstances[(gc2DPlotTypes)pvIDs[k]];
         VARIANT_BOOL hasProperties = VARIANT_FALSE;
         if ( ! ( NULL == pIGSystemPlotType ) )
            pIGSystemPlotType -> get_HasProperties(Plot::plotType2DInstanceNumber[(gc2DPlotTypes)pvIDs[k]],&hasProperties);
         if ( VARIANT_FALSE == hasProperties )
            pIGSystemPlotType = NULL;
         typePropertyInstance2D.push_back(pIGSystemPlotType);
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
      std::list<IGSystemPlotType *> typePropertyInstance3D;

      for ( long k = 0; k < count3DTypes; k++ ) {
         type3DIds.push_back(pvIDs[k]);
         long n = (DWORD)wcslen(pvNames[k]);
         char *pszTemp = new char[n + 1];
         pszTemp[n] = '\0';
         WideCharToMultiByte(CP_ACP,0,pvNames[k],-1,pszTemp,n,0,0);
         type3DNames.push_back(pszTemp);
         IGSystemPlotType *pIGSystemPlotType = Plot::plotType3DProviderInstances[(gc3DPlotTypes)pvIDs[k]];
         VARIANT_BOOL hasProperties = VARIANT_FALSE;
         if ( ! ( NULL == pIGSystemPlotType ) )
            pIGSystemPlotType -> get_HasProperties(Plot::plotType3DInstanceNumber[(gc3DPlotTypes)pvIDs[k]],&hasProperties);
         if ( VARIANT_FALSE == hasProperties )
            pIGSystemPlotType = NULL;
         typePropertyInstance3D.push_back(pIGSystemPlotType);
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
      long yCheckbox = 4;

      HFONT hGUIFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

      while ( 0 < type2DNames.size() ) {

         char *pszTemp = type2DNames.front();
         type2DNames.pop_front();

         controlIdToType[controlId] = type2DIds.front();
         type2DIds.pop_front();

         HWND hwndCheckbox = CreateWindowEx(0L,"BUTTON",pszTemp,BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE,xCheckbox,yCheckbox,cxCheckbox,cyCheckbox,hwndScrollPanes[0],(HMENU)(ULONG_PTR)controlId,hModule,NULL);
   
         SendMessage(hwndCheckbox,WM_SETFONT,(LPARAM)hGUIFont,0L);

         HWND hwndProperties = CreateWindowEx(0L,"BUTTON","...",BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,xCheckbox + 80,yCheckbox - 2,32,cyCheckbox + 4,hwndScrollPanes[0],(HMENU)(ULONG_PTR)(controlId + 1000),hModule,NULL);

         ShowWindow(hwndProperties,SW_HIDE);

         SetWindowLongPtr(hwndProperties,GWLP_USERDATA,(ULONG_PTR)typePropertyInstance2D.front());

         SendMessage(hwndProperties,WM_SETFONT,(LPARAM)hGUIFont,0L);

         type2DToProperties[controlIdToType[controlId]] = hwndProperties;

         typePropertyInstance2D.pop_front();

         type2DToCheckBox[controlIdToType[controlId]] = hwndCheckbox;

         delete [] pszTemp;

         yCheckbox += cyCheckbox + 8;

         max2DTypeControlId = controlId;

         controlId++;

      }

      GetWindowRect(hwndScrollPanes[0],&rcCheckbox);

      SetWindowPos(hwndScrollPanes[0],HWND_TOP,0,0,rcCheckbox.right - rcCheckbox.left,yCheckbox + 8,SWP_NOMOVE);

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

         HWND hwndCheckbox = CreateWindowEx(0L,"BUTTON",pszTemp,BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE,xCheckbox,yCheckbox,cxCheckbox,cyCheckbox,hwndScrollPanes[1],(HMENU)(ULONG_PTR)controlId,hModule,NULL);

         SendMessage(hwndCheckbox,WM_SETFONT,(LPARAM)hGUIFont,0L);

         HWND hwndProperties = CreateWindowEx(0L,"BUTTON","...",BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,xCheckbox + 80,yCheckbox - 2,32,cyCheckbox + 4,hwndScrollPanes[1],(HMENU)(ULONG_PTR)(controlId + 1000),hModule,NULL);

         ShowWindow(hwndProperties,SW_HIDE);

         SetWindowLongPtr(hwndProperties,GWLP_USERDATA,(ULONG_PTR)typePropertyInstance3D.front());

         SendMessage(hwndProperties,WM_SETFONT,(LPARAM)hGUIFont,0L);

         type3DToProperties[controlIdToType[controlId]] = hwndProperties;

         typePropertyInstance3D.pop_front();

         type3DToCheckBox[controlIdToType[controlId]] = hwndCheckbox;

         delete [] pszTemp;

         max3DTypeControlId = controlId;

         controlId++;

         yCheckbox += cyCheckbox + 8;

      }

      GetWindowRect(hwndScrollPanes[1],&rcCheckbox);

      SetWindowPos(hwndScrollPanes[1],HWND_TOP,0,0,rcCheckbox.right - rcCheckbox.left,yCheckbox + 8,SWP_NOMOVE);

      hwndPaneHosts[0] = CreateWindowEx(0,"STATIC","",WS_VISIBLE | WS_CHILD,0,0,0,0,hwnd,NULL,hModule,NULL);
      hwndPaneHosts[1] = CreateWindowEx(0,"STATIC","",WS_VISIBLE | WS_CHILD,0,0,0,0,hwnd,NULL,hModule,NULL);

      SetParent(hwndScrollPanes[0],hwndPaneHosts[0]);
      SetWindowPos(hwndScrollPanes[0],HWND_TOP,0,0,0,0,SWP_NOSIZE);

      SetWindowLongPtr(hwndScrollPanes[0],GWLP_USERDATA,(ULONG_PTR)p);
      SetWindowLongPtr(hwndScrollPanes[0],GWLP_WNDPROC,(ULONG_PTR)scrollPaneHandler);

      SetParent(hwndScrollPanes[1],hwndPaneHosts[1]);
      SetWindowPos(hwndScrollPanes[1],HWND_TOP,0,0,0,0,SWP_NOSIZE);

      SetWindowLongPtr(hwndScrollPanes[1],GWLP_USERDATA,(ULONG_PTR)p);
      SetWindowLongPtr(hwndScrollPanes[1],GWLP_WNDPROC,(ULONG_PTR)scrollPaneHandler);

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

      if ( ! ( GetParent(hwndSampleGraphic) == hwnd ) )
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

      if ( 0 == pType2D && 0 == pType3D )
         ShowWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_NONE_SELECTED),SW_SHOW);
      else
         ShowWindow(GetDlgItem(hwnd,IDDI_PLOT_TYPE_NONE_SELECTED),SW_HIDE);

      p -> propertyOverrideOwnerType -> setWindowChecked(GetDlgItem(hwnd,IDDI_PLOT_TYPE_OVERRIDE_OWNER));

      enum DataArity dataArity = p -> pIDataSet -> DataArity();

      std::function<void(HWND)> *pEnable = new std::function<void(HWND)>( [=](HWND hwndChild) { EnableWindow(hwndChild,DATA_ARITY_3D == dataArity ? TRUE : FALSE); });
      EnableWindow(hwndPaneHosts[1],DATA_ARITY_3D == dataArity ? TRUE : FALSE);
      DoOnWindowDescendants(hwndPaneHosts[1],pEnable);

      RECT rcDialog;
      RECT rcSample;

      GetWindowRect(hwnd,&rcDialog);

      if ( ! p -> overrideOwnerType ) {

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

         long currentIndex = (long)SendMessage(hwndArityTabs,TCM_GETCURSEL,0L,0L);

         if ( -1 == currentIndex ) {
            SendMessage(hwndArityTabs,TCM_SETCURSEL,0L,0L);
            currentIndex = 0;
         }

         long maxScrollPaneHeight = 0;

         for ( long k = 0; 1; k++ ) {

            if ( ! hwndScrollPanes[k] )
               break;

            RECT rcScrollPane;

            GetWindowRect(hwndScrollPanes[k],&rcScrollPane);
            
            maxScrollPaneHeight = max(maxScrollPaneHeight,rcScrollPane.bottom - rcScrollPane.top);

         }

         RECT rcTabs;

         GetWindowRect(hwndArityTabs,&rcTabs);

         long oldBottom = rcTabs.bottom;

         rcTabs.bottom = rcTabs.top + min(rcDialog.bottom - rcTabs.top - 24,maxScrollPaneHeight + 32);

         if ( ! ( oldBottom == rcTabs.bottom ) ) 
            SetWindowPos(hwndArityTabs,HWND_TOP,0,0,rcTabs.right - rcTabs.left,rcTabs.bottom - rcTabs.top,SWP_NOMOVE | SWP_SHOWWINDOW);

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

            needsToScroll[k] = si.nMax > (int)si.nPage;

            SendMessage(hwndScrollBars[k],SBM_SETSCROLLINFO,(WPARAM)TRUE,(LPARAM)&si);
            SendMessage(hwndScrollBars[k],SBM_ENABLE_ARROWS,(WPARAM)ESB_DISABLE_UP,0L);

            if ( ! ( k == currentIndex ) ) 
               ShowWindow(hwndPaneHosts[k],SW_HIDE);

            ShowWindow(hwndScrollBars[k],SW_HIDE);

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

      }

      SET_TYPES_2D

      SET_TYPES_3D

      SendMessage(hwndSampleGraphic,WMG_POSITION_SAMPLE_GRAPHIC,0L,(LPARAM)&rcSample);

      InvalidateRect(hwndSampleGraphic,NULL,TRUE);

      }
      break;

   case WM_NOTIFY: {
      NMHDR *pHeader = (NMHDR *)lParam;
      switch ( wParam ) {
      case IDDI_PLOT_TYPE_ARITY_TABS: {
         long currentIndex = (long)SendMessage(pHeader -> hwndFrom,TCM_GETCURSEL,0L,0L);
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

         Plot::typeHandler(hwnd,WM_SHOWWINDOW,1L,0L);

         InvalidateRect(hwndSampleGraphic,NULL,TRUE);

         break;
      }

      if ( IDDI_PLOT_TYPE_3D_MIN <= controlID && controlID <= max3DTypeControlId ) {

         if ( BST_CHECKED == SendMessage((HWND)lParam,BM_GETCHECK,0L,0L) )
            p -> plotType3D = p -> plotType3D | controlIdToType[controlID];
         else
            p -> plotType3D = p -> plotType3D & ( ~ controlIdToType[controlID] );

         Plot::typeHandler(hwnd,WM_SHOWWINDOW,1L,0L);

         InvalidateRect(hwndSampleGraphic,NULL,TRUE);

         break;
      }

      if ( (IDDI_PLOT_TYPE_2D_MIN + 1000) <= controlID && controlID <= (max2DTypeControlId + 1000) ) {

         ((IGSystemPlotType *)GetWindowLongPtr((HWND)lParam,GWLP_USERDATA)) -> 
            ShowProperties(Plot::plotType2DInstanceNumber[(gc2DPlotTypes)controlIdToType[controlID - 1000]],
                                 hwnd,p -> plotType2DInstanceGUIDS[controlIdToType[controlID - 1000]],NULL,NULL,0);

         Plot::typeHandler(hwnd,WM_SHOWWINDOW,1L,0L);

         InvalidateRect(hwndSampleGraphic,NULL,TRUE);

         break;
      }

      if ( (IDDI_PLOT_TYPE_3D_MIN + 1000) <= controlID && controlID <= (max3DTypeControlId + 1000) ) {

         ((IGSystemPlotType *)GetWindowLongPtr((HWND)lParam,GWLP_USERDATA)) -> 
            ShowProperties(Plot::plotType3DInstanceNumber[(gc3DPlotTypes)controlIdToType[controlID - 1000]],
                              hwnd,p -> plotType3DInstanceGUIDS[controlIdToType[controlID - 1000]],NULL,NULL,0);

         Plot::typeHandler(hwnd,WM_SHOWWINDOW,1L,0L);

         InvalidateRect(hwndSampleGraphic,NULL,TRUE);

         break;
      }

      switch ( controlID ) {

      case IDDI_PLOT_TYPE_OVERRIDE_OWNER: {
         p -> propertyOverrideOwnerType -> getWindowChecked((HWND)lParam);
         Plot::typeHandler(hwnd,WM_SHOWWINDOW,1L,0L);
         }
         break;
      }

      InvalidateRect(hwndSampleGraphic,NULL,TRUE);

      return LRESULT(0);
      }

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