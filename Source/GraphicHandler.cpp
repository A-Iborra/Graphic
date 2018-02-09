// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"

#include "Graphic_resource.h"
#include "utils.h"
#include "imageUtilities.h"

#include "List.cpp"

   LRESULT CALLBACK G::graphicHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   G *p = (G *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   bool isOpenGLActive = true;

   if ( WM_MOUSEFIRST <= msg && msg <= WM_MOUSELAST ) {
      if ( p && p -> isInitializing )
         return DefWindowProc(hwnd,msg,wParam,lParam);
      HWND hwndCurrent = p -> pIOpenGLImplementation -> TargetHWND();
      if ( ! ( hwnd == hwndCurrent ) )
         isOpenGLActive = false;
      else 
         if ( ! p -> pIOpenGLImplementation -> IsRendered() )
            isOpenGLActive = false;
   }
 
   switch (msg) {
 
   case WM_CREATE: {
      CREATESTRUCT *cp = (CREATESTRUCT *)lParam;
      p = (G *)cp -> lpCreateParams;
      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);
      }
      return LRESULT(FALSE);

   case WM_MOUSELEAVE:
      if ( ! isOpenGLActive )
         break;
      p -> eraseGraphicCursor();
      if ( ! ( -1L == p -> ptlZoomFloat.x ) ) {
         HDC hdc;
         p -> pIOpenGLImplementation -> get_HDC(&hdc);
         p -> restoreBox(&p -> ptlZoomAnchor,&p -> ptlZoomFloat,hdc,p -> boxBitmaps);
      }
      break;


   case WM_MOUSEMOVE: {

      if ( ! isOpenGLActive )
         break;

      TRACKMOUSEEVENT tme = {0};
      tme.cbSize = sizeof(TRACKMOUSEEVENT);
      tme.dwFlags = TME_QUERY;
      TrackMouseEvent(&tme);

      if ( ! tme.dwFlags ) {
         tme.dwFlags = TME_LEAVE;
         tme.hwndTrack = hwnd;
         tme.dwHoverTime = HOVER_DEFAULT;
         TrackMouseEvent(&tme);
      }

      p -> eraseGraphicCursor();

      if ( wParam & MK_LBUTTON ) {

         POINT ptl = {LOWORD(lParam),HIWORD(lParam)};

         if ( p -> pSelectedGraphicSegmentAction ) {
            p -> pSelectedGraphicSegmentAction -> MouseMove(&ptl);
            break;
         }

         if ( abs(p -> ptlZoomAnchor.x - ptl.x) < 2 * p -> pickBoxSize.cx || abs(p -> ptlZoomAnchor.y - ptl.y) < 2 * p -> pickBoxSize.cy )
            break;
 
         HDC hdc = GetDC(p -> Canvas());

         if ( ! ( -1L == p -> ptlZoomFloat.x ) ) 
            p -> restoreBox(&p -> ptlZoomAnchor,&p -> ptlZoomFloat,hdc,p -> boxBitmaps);

         p -> ptlZoomFloat.x = ptl.x;
         p -> ptlZoomFloat.y = ptl.y;

         p -> saveBox(&p -> ptlZoomAnchor,&p -> ptlZoomFloat,hdc,p -> boxBitmaps);

         MoveToEx(hdc,p -> ptlZoomAnchor.x,p -> ptlZoomAnchor.y,(POINT *)NULL); 

         LineTo(hdc,p -> ptlZoomFloat.x,p -> ptlZoomAnchor.y);
         LineTo(hdc,p -> ptlZoomFloat.x,p -> ptlZoomFloat.y);
         LineTo(hdc,p -> ptlZoomAnchor.x,p -> ptlZoomFloat.y);
         LineTo(hdc,p -> ptlZoomAnchor.x,p -> ptlZoomAnchor.y);
       
         if ( p -> ptlZoomAnchor.x != 0 && p -> ptlZoomAnchor.y != 0 ) 
            p -> trackedMouse = TRUE;

         ReleaseDC(p -> Canvas(),hdc);

         return (LRESULT)TRUE;

      }
 
      POINTL ptl = {LOWORD(lParam),HIWORD(lParam)};

      DataPoint dp[2];

      p -> pIDataSetMaster -> GetDomain(&dp[0],&dp[1]);

      if ( dp[0].x == DBL_MAX ) 
         return (LRESULT)TRUE;

      p -> hitTableHits = 0;

      p -> drawGraphicCursor(&ptl,p -> doPickBox(&ptl));
 
      }

      return LRESULT(TRUE);
 

   case WM_RBUTTONDOWN: {

      POINTL ptl = {LOWORD(lParam),HIWORD(lParam)};
      p -> rightMouseClickPosition = ptl;
      POINT ptMenu{ptl.x,ptl.y};
      ClientToScreen(hwnd,&ptMenu);
      if ( isOpenGLActive ) {
         p -> eraseGraphicCursor();
         if ( ! p -> pick(&ptl,processMenus,FALSE) )
            TrackPopupMenu(p -> hwndMenuPlot(),TPM_LEFTALIGN | TPM_RIGHTBUTTON,ptMenu.x,ptMenu.y,0,hwnd,NULL);
      } else
         TrackPopupMenu(p -> hwndMenuPlot(),TPM_LEFTALIGN | TPM_RIGHTBUTTON,ptMenu.x,ptMenu.y,0,hwnd,NULL);
      }
      return LRESULT(TRUE);
 
   case WM_LBUTTONDBLCLK: {

      if ( ! isOpenGLActive ) 
         break;

      POINTL ptl = {LOWORD(lParam),HIWORD(lParam)};

      p -> hitTableHits = 0;

      p -> eraseGraphicCursor();

      p -> pick(&ptl,processDefaultAction,FALSE);

      }
      break;

   case WM_LBUTTONDOWN: {

      if ( ! isOpenGLActive )
         break;

      POINTL ptl = {LOWORD(lParam),HIWORD(lParam)};
 
      p -> eraseGraphicCursor();

      p -> pick(&ptl,processSelections,TRUE);

      p -> ptlZoomFloat.x = -1L;
      p -> ptlZoomFloat.y = -1L;

      p -> ptlZoomAnchor.x = ptl.x;
      p -> ptlZoomAnchor.y = ptl.y;
 
      }
      return LRESULT(TRUE);
 
 
   case WM_LBUTTONUP:
 
      if ( ! isOpenGLActive )
         break;

      p -> eraseGraphicCursor();

      if ( p -> pSelectedGraphicSegmentAction ) {
         p -> pSelectedGraphicSegmentAction -> MouseRelease();
         p -> pSelectedGraphicSegmentAction = NULL;
         break;
      }

      if ( ! ( -1L == p -> ptlZoomFloat.x ) ) {
         HDC hdc;
         p -> pIOpenGLImplementation -> get_HDC(&hdc);
         p -> restoreBox(&p -> ptlZoomAnchor,&p -> ptlZoomFloat,hdc,p -> boxBitmaps);
      }

      p -> ptlZoomFloat.x = -1L;
      p -> ptlZoomFloat.y = -1L;

      p -> ptlZoomAnchor.x = -1L;
      p -> ptlZoomAnchor.y = -1L;

      return LRESULT(TRUE);

   case WM_PAINT: {
      if ( ! p ) break;
      PAINTSTRUCT ps;
      BeginPaint(hwnd,&ps);
      HWND hwndCurrent = p -> pIOpenGLImplementation -> TargetHWND();
      if ( hwndCurrent == hwnd )
         p -> render(-1);
      p -> ptlPickBox.x = -1L;
      p -> ptlPickBox.y = -1L;
      EndPaint(hwnd,&ps);
      }
      return LRESULT(FALSE);
 
   case WM_INITMENUPOPUP: {

      HMENU hmenuTemp = (HMENU)wParam;
      MENUITEMINFO mi;
      memset(&mi,0,sizeof(mi));
      mi.cbSize = sizeof(mi);
      mi.fMask = MIIM_ID;
      GetMenuItemInfo(hmenuTemp,0,TRUE,&mi);

      switch ( mi.wID ) {

      case IDMI_GRAPHIC_MENU_TITLE: {
         long n = GetMenuItemCount(hmenuTemp);
         char szFunctionName[256];
         for ( long k = 0; k < n; k++ ) {

            memset(szFunctionName,0,256);
            memset(&mi,0,sizeof(mi));
            mi.cbSize = sizeof(mi);
            mi.fMask = MIIM_TYPE | MIIM_SUBMENU;
            mi.dwTypeData = szFunctionName;
            mi.cch = 256;
            GetMenuItemInfo(hmenuTemp,k,MF_BYPOSITION,&mi);

            if ( strcmp("Evaluate",szFunctionName) == 0 ) {

               if ( 0 == p -> functionList.Count() ) {
                  mi.fMask = MIIM_STATE;
                  mi.fState = MFS_GRAYED;
                  SetMenuItemInfo(hmenuTemp,k,MF_BYPOSITION,&mi);
                  break;
               }

               long menuPosition = k;

               DestroyMenu(mi.hSubMenu);

               HMENU hmenuSubMenu = CreateMenu();

               BSTR bstrExpression;
               long k = 0;
               IGSFunctioNater* f = (IGSFunctioNater*)NULL;
               while ( f = p -> functionList.GetNext(f) ) {

                  f -> get_Expression(&bstrExpression);

                  char* pszExpression = new char[n = (DWORD)wcslen(bstrExpression) + 1];

                  memset(pszExpression,0,n);
                  WideCharToMultiByte(CP_ACP,0,bstrExpression,-1,pszExpression,n,0,0);

                  SysFreeString(bstrExpression);

                  memset(&mi,0,sizeof(mi));
                  mi.cbSize = sizeof(mi);

                  mi.fMask = MF_STRING | MIIM_TYPE | MIIM_ID;
                  mi.fType = MFT_STRING;
                  mi.wID = IDMI_GRAPHIC_EVALUATE_FUNCTION + k;
                  mi.dwTypeData = pszExpression;

                  InsertMenuItem(hmenuSubMenu,k + 1,MF_BYPOSITION,&mi);

                  delete [] pszExpression;

                  k++;

               }

               memset(&mi,0,sizeof(mi));
               mi.cbSize = sizeof(mi);
               mi.fMask = MIIM_STATE | MIIM_SUBMENU;
               mi.hSubMenu = hmenuSubMenu;
               mi.fState = p -> functionList.Count() > 0 ? MFS_ENABLED : MFS_DISABLED;

               SetMenuItemInfo(hmenuTemp,menuPosition,MF_BYPOSITION,&mi);

               break;
            }
         }
         }
         break;


      case IDMI_GRAPHIC_VIEW_2D: {
         mi.fMask = MIIM_STATE;
         mi.fState = MFS_CHECKED;
         SetMenuItemInfo(hmenuTemp,p -> plotView == gcPlotView2D ? 0 : 1,TRUE,&mi);
         mi.fState = MFS_UNCHECKED;
         SetMenuItemInfo(hmenuTemp,p -> plotView == gcPlotView2D ? 1 : 0,TRUE,&mi);
         //mi.fState = MFS_DISABLED;
         //if ( p -> plotView == gcPlotView3D )
         //   mi.fState = MFS_ENABLED;
         //SetMenuItemInfo(hmenuTemp,2,TRUE,&mi);
         }
         break;

      }
      }
      break;
 

   case WM_COMMAND: {

      WORD idCommand = LOWORD(wParam);

      switch ( idCommand ) {

      case IDMI_GRAPHIC_SAVE_AS: {

         char szFilter[MAX_PATH],szFile[MAX_PATH];

         char szExtensions[][16] = {"graphic","bmp","png","jpg-jpeg","gif","tiff",NULL};

         memset(szFilter,0,sizeof(szFilter));
         memset(szFile,0,sizeof(szFile));

         OPENFILENAME openFileName = {0};
         memset(&openFileName,0,sizeof(OPENFILENAME));

         long k = sprintf(szFilter,"Graphic (*.graphic)");
         k = k + (DWORD)sprintf(szFilter + k + 1,"*.graphic") + 1;

         k = k + (DWORD)sprintf(szFilter + k + 1,"Bitmap (*.bmp)") + 1;
         k = k + sprintf(szFilter + k + 1,"*.bmp") + 1;

         k = k + (DWORD)sprintf(szFilter + k + 1,"PNG (*.png)") + 1;
         k = k + sprintf(szFilter + k + 1,"*.png") + 1;

         k = k + (DWORD)sprintf(szFilter + k + 1,"JPG (*.jpg,*.jpeg)") + 1;
         k = k + sprintf(szFilter + k + 1,"*.jpg,*.jpeg") + 1;

         k = k + (DWORD)sprintf(szFilter + k + 1,"GIF (*.gif)") + 1;
         k = k + sprintf(szFilter + k + 1,"*.gif") + 1;

         k = k + (DWORD)sprintf(szFilter + k + 1,"TIFF (*.tiff)") + 1;
         k = k + sprintf(szFilter + k + 1,"*.tiff") + 1;

         k = k + (DWORD)sprintf(szFilter + k + 1,"All Files (*.*)") + 1;
         k = k + sprintf(szFilter + k + 1,"*.*") + 1;

         openFileName.lStructSize = sizeof(OPENFILENAME);
         openFileName.hwndOwner = p -> Canvas();
         openFileName.Flags = OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
         openFileName.lpstrFilter = szFilter;
         openFileName.lpstrFile = szFile;
         openFileName.lpstrDefExt = "graphic";
         openFileName.nFilterIndex = 1;
         openFileName.nMaxFile = MAX_PATH;
         openFileName.lpstrTitle = "Specify a destination file";

         if ( ! GetSaveFileName(&openFileName) ) 
            break;

         if ( 7 == openFileName.nFilterIndex ) {

            char *pDot = strrchr(openFileName.lpstrFile,'.');
   
            if ( ! pDot ) 
               openFileName.nFilterIndex = 1;
            else {
               char szLower[32];
               strcpy(szLower,pDot + 1);
               _strlwr(szLower);
               for ( long k = 0; 1; k++ ) {
                  if ( ! szExtensions[k] )
                     break;
                  if ( strstr(szExtensions[k],szLower) ) {
                     openFileName.nFilterIndex = k + 1;
                     break;
                  }
               }
            }
      
            if ( 7 == openFileName.nFilterIndex )
               openFileName.nFilterIndex = 1;
         }

         switch ( openFileName.nFilterIndex ) {
         default:
         case 1: {
            BSTR savedFile = SysAllocStringLen(NULL,1024);
            MultiByteToWideChar(CP_ACP,0,openFileName.lpstrFile,-1,savedFile,1024);
            p -> pIGProperties -> SaveTo(savedFile);
            SysFreeString(savedFile);
            }
            break;

         case 2:
         case 3:
         case 4:
         case 5:
         case 6: {

            HDC hdcSource = GetDC(p -> Canvas());

            RECT rcClient;
            GetClientRect(p -> Canvas(),&rcClient);
            long cx = rcClient.right - rcClient.left;
            long cy = rcClient.bottom - rcClient.top;

            HDC hdcTarget = CreateCompatibleDC(hdcSource);

            HBITMAP hBitmap = CreateCompatibleBitmap(hdcSource,cx,cy);

            HGDIOBJ oldBitmap = SelectObject(hdcTarget,hBitmap);

            BitBlt(hdcTarget,0,0,cx,cy,hdcSource,0,0,SRCCOPY);

            SelectObject(hdcTarget,oldBitmap);

            DeleteDC(hdcTarget);

            char *pDot = strrchr(openFileName.lpstrFile,'.');

            if ( pDot )
               *pDot = '\0';

            if ( 2 == openFileName.nFilterIndex ) {

               strcat(openFileName.lpstrFile,".bmp");

               SaveBitmapFile(hdcSource,hBitmap,openFileName.lpstrFile);

               ReleaseDC(p -> Canvas(),hdcSource);

               break;

            } 

            char szExtension[][8] = {"","","",".png",".jpg",".gif",".tiff"};

            WCHAR szwEncoder[][32] = {L"",L"",L"",L"image/jpeg",L"image/png",L"image/gif",L"image/tiff"};

            strcat(openFileName.lpstrFile,szExtension[openFileName.nFilterIndex]);

            BITMAP bitMap;
            GetObject(hBitmap,sizeof(BITMAP),&bitMap);

            long colorTableSize = bitMap.bmHeight * ((bitMap.bmWidth * bitMap.bmPlanes * bitMap.bmBitsPixel + 31) & ~31) / 8 ;

            long entireSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTableSize;

            BYTE *pBuffer = new BYTE[entireSize];

            memset(pBuffer,0,entireSize * sizeof(BYTE));

            BITMAPFILEHEADER *pBitmapFileHeader = (BITMAPFILEHEADER *)pBuffer;

            BITMAPINFO *pBitmapInfo = (BITMAPINFO *)(pBuffer + sizeof(BITMAPFILEHEADER));

            BITMAPINFOHEADER *pBitmapInfoHeader = &(pBitmapInfo -> bmiHeader);
   
            pBitmapInfoHeader -> biSize = sizeof(BITMAPINFOHEADER); 
            pBitmapInfoHeader -> biWidth = bitMap.bmWidth;
            pBitmapInfoHeader -> biHeight = bitMap.bmHeight;
            pBitmapInfoHeader -> biPlanes = bitMap.bmPlanes; 
            pBitmapInfoHeader -> biBitCount = bitMap.bmBitsPixel;
            pBitmapInfoHeader -> biCompression = BI_RGB;
            if ( 1 == bitMap.bmBitsPixel ) {
               pBitmapInfoHeader -> biClrUsed = 2;
               pBitmapInfoHeader -> biClrImportant = 2;
            } else {
               pBitmapInfoHeader -> biClrUsed = 0;
               pBitmapInfoHeader -> biClrImportant = 0;
            }

            pBitmapInfoHeader -> biSizeImage = bitMap.bmHeight * ((bitMap.bmWidth * bitMap.bmPlanes * bitMap.bmBitsPixel + 31) & ~31) / 8 ;

            BYTE *pBits = pBuffer + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

            long rc = GetDIBits(hdcSource,hBitmap,0,bitMap.bmHeight,pBits,(BITMAPINFO *)pBitmapInfo,DIB_RGB_COLORS);

            SaveWithEncoder((BYTE *)pBitmapInfoHeader,openFileName.lpstrFile,szwEncoder[openFileName.nFilterIndex]);

            delete [] pBuffer;

            ReleaseDC(p -> Canvas(),hdcSource);
            
            }
            break;

         }
         }
         return LRESULT(TRUE);

      case IDMI_GRAPHIC_OPEN: {
         BSTR openedFile = SysAllocStringLen(NULL,1024);
         p -> pIGProperties -> Open(&openedFile);
         SysFreeString(openedFile);
         p -> render(0);
         }
         return LRESULT(TRUE);

      case IDMI_GRAPHIC_NEW: {

         p -> isInitializing = TRUE;

         p -> propertyFunctions -> clearStorageObjects();
         p -> propertyFunctions -> writeStorageObjects();
         p -> propertyDataSets -> clearStorageObjects();
         p -> propertyDataSets -> writeStorageObjects();
         p -> propertyTexts -> clearStorageObjects();
         p -> propertyTexts -> writeStorageObjects();
         p -> propertyAxiis -> clearStorageObjects();
         p -> propertyAxiis -> writeStorageObjects();

         p -> pIGProperties -> New();
         p -> pIPropertiesClient -> Loaded();
         p -> isInitializing = TRUE;
         p -> render(0);
         p -> isInitializing = FALSE;
         }
         break;

      case IDMI_GRAPHIC_PRINT: {

         PRINTDLG printDlg = {0};

         printDlg.lStructSize = sizeof(PRINTDLG);
         printDlg.hwndOwner = p -> Canvas();
         printDlg.Flags = PD_ALLPAGES | PD_RETURNDC;

         if ( ! PrintDlg(&printDlg) )
            break;

         DOCINFO docInfo;
         memset(&docInfo,0,sizeof(docInfo));
         docInfo.cbSize = sizeof(docInfo);
         docInfo.lpszDocName = "InnoVisioNate Graphic";
         
         StartDoc(printDlg.hDC, &docInfo);

         StartPage(printDlg.hDC);

         HDC hdcSource = GetDC(p -> Canvas());

         RECT rcClient;
         GetClientRect(p -> Canvas(),&rcClient);

         BOOL rc = BitBlt(printDlg.hDC,0,0,rcClient.right - rcClient.left,rcClient.bottom - rcClient.top,hdcSource,0,0,SRCCOPY);

         EndPage(printDlg.hDC);

         EndDoc(printDlg.hDC); 

         ReleaseDC(p -> Canvas(),hdcSource);
         }

         return LRESULT(TRUE);
 
      case IDMI_GRAPHIC_ERASE:
         p -> erase();
         return LRESULT(TRUE);
 
      case IDMI_GRAPHIC_REDRAW:
         p -> render(0);
         return LRESULT(TRUE);
 
      case IDMI_GRAPHIC_CANCEL:
         p -> stop();
         return LRESULT(TRUE);
 
      case IDMI_GRAPHIC_VIEW_2D:
         p -> put_PlotView(gcPlotView2D);
         p -> render(0);
         return LRESULT(TRUE);
 
      case IDMI_GRAPHIC_VIEW_3D:
         p -> put_PlotView(gcPlotView3D);
         p -> render(0);
         return LRESULT(TRUE);

      case IDMI_GRAPHIC_VIEW_SET:
         p -> pIViewSet -> Properties(p -> Canvas(),G::menuHandlerSomeObjectChanged,(void *)p);
         return LRESULT(TRUE);

#if 0
      case IDMI_GRAPHIC_ZOOM_FULL: {
 
/*
         p -> pIOpenGLImplementation -> SetZoomedExtents(DBL_MAX,DBL_MAX,DBL_MAX,-DBL_MAX,-DBL_MAX,-DBL_MAX);
         
         p -> pIOpenGLImplementation -> SetUp(p -> pIDataSetMaster,
                                                 p -> plotView,p -> propertyViewTheta,p -> propertyViewPhi,p -> propertyViewSpin,
                                                 p -> propertyPlotLeftMargin,p -> propertyPlotTopMargin,p -> propertyPlotRightMargin,p -> propertyPlotBottomMargin);
*/
         p -> erase();
         p -> redraw();
         }
         return LRESULT(TRUE);
#endif

      case IDMI_GRAPHIC_PROPERTIES: {
         IUnknown *pIUnknownThis;
         p -> QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknownThis));
         p -> pIGProperties -> ShowProperties(p -> Canvas(),pIUnknownThis);
         pIUnknownThis -> Release();
         p -> setDataSourcesVisibility(NULL,NULL);
         p -> pIOpenGLImplementation -> SetTargetWindow(p -> Canvas());
         p -> render(0);
         }
         return LRESULT(TRUE);
 
      case IDMI_GRAPHIC_SET_PERSPECTIVE: {
         IUnknown* pIUnknownObject;
         p -> pIViewSet -> QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknownObject));
         p -> pIGProperties -> ShowProperties(hwnd,pIUnknownObject);
         pIUnknownObject -> Release();
         p -> pIOpenGLImplementation -> SetTargetWindow(p -> Canvas());
         }
         return LRESULT(TRUE);
 
      case IDMI_GRAPHIC_TEXT: {
         p -> AddTextInteractive();
         }
         return LRESULT(TRUE);
         
      default: {

         if ( idCommand >= IDMI_GRAPHIC_EVALUATE_FUNCTION && idCommand <= (IDMI_GRAPHIC_EVALUATE_FUNCTION + p -> functionList.Count()) ) {
            IGSFunctioNater* f = p -> functionList.GetByIndex(idCommand - IDMI_GRAPHIC_EVALUATE_FUNCTION);
            f -> Start();
         }

         }
         break;

      }
      }
      break;
 
   default:
      break;
   }
 
   return DefWindowProc(hwnd,msg,wParam,lParam);
   }
 
 
   LRESULT CALLBACK G::graphicFrameHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   G *p = (G *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch (msg) {

   case WM_CREATE: {
      CREATESTRUCT *cp = (CREATESTRUCT *)lParam;
      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)cp -> lpCreateParams);
      }
      return LRESULT(FALSE);
 
   case WM_SIZE: {
      if ( ! p )
         break;
      p -> setDataSourcesVisibility(NULL,NULL); 
      HWND hwndCurrent = p -> Canvas();
      if ( ! hwndCurrent )
         break;
      RECT rcCurrent,rcParent;
      GetWindowRect(hwndCurrent,&rcCurrent);
      GetWindowRect(hwnd,&rcParent);
      long cx = rcCurrent.right - rcCurrent.left;
      long cy = rcCurrent.bottom - rcCurrent.top;
      rcCurrent.left -= rcParent.left;
      rcCurrent.top -= rcParent.top;
      rcCurrent.right = rcCurrent.left + cx;
      rcCurrent.bottom = rcCurrent.top + cy;
      HWND hwndNew = p -> newCanvas(&rcCurrent);
      if ( hwndNew )
         p -> pIOpenGLImplementation -> SetTargetWindow(hwndNew);
      }
      return (LRESULT)0;

   }
 
   return DefWindowProc(hwnd,msg,wParam,lParam);
   }


   void G::saveBox(POINTL *pAnchor,POINTL *pFloat,HDC hdc,HBITMAP *pBitmaps) {

   long x1 = pAnchor -> x;
   long x2 = pFloat -> x;
   long cx = x2 - x1;
   if ( 0 > cx ) {
      cx = x2;
      x2 = x1;
      x1 = cx;
      cx = x2 - x1;
   }

   long y1 = pAnchor -> y;
   long y2 = pFloat -> y;
   long cy = y2 - y1;
   if ( 0 > cy ) {
      cy = y2;
      y2 = y1;
      y1 = cy;
      cy = y2 - y1;
   }

   HDC hdcTarget = CreateCompatibleDC(NULL);

   pBitmaps[0] = CreateCompatibleBitmap(hdc,cx + 4,8);
   HGDIOBJ oldBitmap = SelectObject(hdcTarget,pBitmaps[0]);
   BitBlt(hdcTarget,0,0,cx + 4,8,hdc,x1 - 2,y1 - 4,SRCCOPY);

   pBitmaps[1] = CreateCompatibleBitmap(hdc,8,cy + 4);
   oldBitmap = SelectObject(hdcTarget,pBitmaps[1]);
   BitBlt(hdcTarget,0,0,8,cy + 4,hdc,x2 - 4,y1 - 2,SRCCOPY);

   pBitmaps[2] = CreateCompatibleBitmap(hdc,cx + 4,8);
   oldBitmap = SelectObject(hdcTarget,pBitmaps[2]);
   BitBlt(hdcTarget,0,0,cx + 4,8,hdc,x1 - 2,y2 - 4,SRCCOPY);

   pBitmaps[3] = CreateCompatibleBitmap(hdc,8,cy + 4);
   oldBitmap = SelectObject(hdcTarget,pBitmaps[3]);
   BitBlt(hdcTarget,0,0,8,cy + 4,hdc,x1 - 4,y1 - 2,SRCCOPY);

   if ( 0 >= cx || 0 >= cy ) {
      pBitmaps[4] = NULL;
      return;
   }

   long pixelCount = cx * cy;

   float *pPixels = new float[4 * pixelCount];

   memset(pPixels,0,4 * pixelCount * sizeof(float));

   pIOpenGLImplementation -> GetPixels(x1,y1,x2,y2,(BYTE *)pPixels);

   pBitmaps[4] = CreateCompatibleBitmap(hdc,cx,cy);

   oldBitmap = SelectObject(hdcTarget,pBitmaps[4]);

   BitBlt(hdcTarget,0,0,cx,cy,hdc,x1,y1,SRCCOPY);

   float *pf = pPixels;
   BYTE b[3];

   for ( int k = 0; k < cy; k++ ) {
      for ( int j = 0; j < cx; j++ ) {
         b[0] = (BYTE)(255.0 * pf[0]);
         b[1] = (BYTE)(255.0 * pf[1]);
         b[2] = (BYTE)(255.0 * pf[2]);
         COLORREF cr = RGB(b[0],b[1],b[2]) & GetPixel(hdcTarget,j,k);
         SetPixel(hdcTarget,j,k,cr);
         pf += 4;
      }
   }

   delete [] pPixels;

   SelectObject(hdcTarget,oldBitmap);

   DeleteDC(hdcTarget);

   return;
   }

   void G::restoreBox(POINTL *pAnchor,POINTL *pFloat,HDC hdcTarget,HBITMAP *pBitmaps) {

   long x1 = pAnchor -> x;
   long x2 = pFloat -> x;
   long cx = x2 - x1;
   if ( 0 > cx ) {
      cx = x2;
      x2 = x1;
      x1 = cx;
      cx = x2 - x1;
   }

   long y1 = pAnchor -> y;
   long y2 = pFloat -> y;
   long cy = y2 - y1;
   if ( 0 > cy ) {
      cy = y2;
      y2 = y1;
      y1 = cy;
      cy = y2 - y1;
   }

   HDC hdcSource = CreateCompatibleDC(NULL);

   HGDIOBJ oldBitmap = SelectObject(hdcSource,pBitmaps[0]);
   BitBlt(hdcTarget,x1 - 2,y1 - 4,cx + 4,8,hdcSource,0,0,SRCCOPY);
   oldBitmap = SelectObject(hdcSource,pBitmaps[1]);
   DeleteObject(oldBitmap);
   pBitmaps[0] = NULL;

   BitBlt(hdcTarget,x2 - 4,y1 - 2,8,cy + 4,hdcSource,0,0,SRCCOPY);
   oldBitmap = SelectObject(hdcSource,pBitmaps[2]);
   DeleteObject(oldBitmap);
   pBitmaps[1] = NULL;

   BitBlt(hdcTarget,x1 - 2,y2 - 4,cx + 4,8,hdcSource,0,0,SRCCOPY);
   oldBitmap = SelectObject(hdcSource,pBitmaps[3]);
   DeleteObject(oldBitmap);
   pBitmaps[2] = NULL;

   BitBlt(hdcTarget,x1 - 4,y1 - 2,8,cy + 4,hdcSource,0,0,SRCCOPY);
   oldBitmap = SelectObject(hdcSource,pBitmaps[4]);
   DeleteObject(oldBitmap);
   pBitmaps[3] = NULL;

   if ( ! ( NULL == pBitmaps[4] ) ) {
      BitBlt(hdcTarget,x1,y1,cx,cy,hdcSource,0,0,SRCCOPY);
      DeleteObject(pBitmaps[4]);
   }

   pBitmaps[4] = NULL;

   DeleteDC(hdcSource);

   return;
   }