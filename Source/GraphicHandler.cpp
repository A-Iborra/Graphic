/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>

#include "Graphic_resource.h"
#include "utils.h"
#include "Graphic.h"

#include "List.cpp"

   LRESULT CALLBACK G::graphicHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   G *p = (G *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   bool isOpenGLActive = true;

   if ( WM_MOUSEFIRST <= msg && msg <= WM_MOUSELAST ) {
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
 
         HDC hdc = GetDC(p -> hwndGraphic);

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

         ReleaseDC(p -> hwndGraphic,hdc);

      } else {
 
         POINTL ptl = {LOWORD(lParam),HIWORD(lParam)};

         DataPoint dp[2];

         p -> pIDataSetMaster -> GetDomain(&dp[0],&dp[1]);

         if ( dp[0].x == DBL_MAX ) 
            return (LRESULT)TRUE;

         p -> hitTableHits = 0;

         p -> drawGraphicCursor(&ptl,p -> doPickBox(&ptl));
 
      }
      }

      return LRESULT(TRUE);
 

   case WM_RBUTTONDOWN: {
      if ( ! isOpenGLActive )
         break;
      POINTL ptl = {LOWORD(lParam),HIWORD(lParam)};
      p -> eraseGraphicCursor();
      p -> rightMouseClickPosition = ptl;
      if ( ! p -> pick(&ptl,processMenus,FALSE) ) {
         p -> ptlMouseBeforeMenu.x = ptl.x;
         p -> ptlMouseBeforeMenu.y = ptl.y;
         POINT pt{ptl.x,ptl.y};
         ClientToScreen(hwnd,&pt);
         ptl.x = pt.x;
         ptl.y = pt.y;
         TrackPopupMenu(p -> hwndMenuPlot(),TPM_LEFTALIGN | TPM_RIGHTBUTTON,ptl.x,ptl.y,0,hwnd,NULL);
      }
      }
      return LRESULT(TRUE);
 
 
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
         p -> render(0);
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

            if ( strcmp("evaluate",szFunctionName) == 0 ) {

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

                  char* pszExpression = new char[n = wcslen(bstrExpression) + 1];

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
         }
         break;

      }
      }
      break;
 

   case WM_COMMAND: {

      WORD idCommand = LOWORD(wParam);

      switch ( idCommand ) {

      case IDMI_GRAPHIC_SAVE_AS: {
         BSTR savedFile = SysAllocStringLen(NULL,1024);
         p -> pIGProperties -> SaveAs(&savedFile);
         SysFreeString(savedFile);
         }
         return LRESULT(TRUE);

      case IDMI_GRAPHIC_OPEN: {
         BSTR openedFile = SysAllocStringLen(NULL,1024);
         p -> pIGProperties -> Open(&openedFile);
         SysFreeString(openedFile);
         }
         return LRESULT(TRUE);

      case IDMI_GRAPHIC_PRINT: 
         if ( S_OK == p -> pIOpenGLImplementation -> PrintSetup() ) {
           p -> render(0);
           p -> pIOpenGLImplementation -> PrintFinish();
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
         p -> pIViewSet -> Properties(G::someObjectChanged,(void *)p);
         return LRESULT(TRUE);

      case IDDI_GRAPHIC_SUB_STYLE_SURFACE:
         p -> put_PlotType(gcPlotTypeSurface);
         return SendMessage(hwnd,WM_COMMAND,(WPARAM)IDMI_GRAPHIC_VIEW_3D,lParam);
 
      case IDDI_GRAPHIC_SUB_STYLE_WIREFRAME:
         p -> put_PlotType(gcPlotTypeWireFrame);
         return SendMessage(hwnd,WM_COMMAND,(WPARAM)IDMI_GRAPHIC_VIEW_3D,lParam);
 
      case IDDI_GRAPHIC_SUB_STYLE_NATURAL:
         p -> put_PlotType(gcPlotTypeNatural);
         return SendMessage(hwnd,WM_COMMAND,(WPARAM)IDMI_GRAPHIC_VIEW_3D,lParam);
 
      case IDDI_GRAPHIC_SUB_STYLE_STACKS:
         p -> put_PlotType(gcPlotTypeStacks);
         return SendMessage(hwnd,WM_COMMAND,(WPARAM)IDMI_GRAPHIC_VIEW_3D,lParam);

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
    
      case IDMI_GRAPHIC_PROPERTIES: {
         IUnknown *pIUnknownThis;
         p -> QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknownThis));
         p -> pIGProperties -> ShowProperties(p -> hwndGraphic,pIUnknownThis);
         pIUnknownThis -> Release();
         p -> setDataSourcesVisibility(NULL,NULL);
         p -> pIOpenGLImplementation -> SetTargetWindow(p -> hwndGraphic);
         p -> render(0);
         }
         return LRESULT(TRUE);
 
      case IDMI_GRAPHIC_SET_PERSPECTIVE: {
         IUnknown* pIUnknownObject;
         p -> pIViewSet -> QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknownObject));
         p -> pIGProperties -> ShowProperties(hwnd,pIUnknownObject);
         pIUnknownObject -> Release();
         p -> pIOpenGLImplementation -> SetTargetWindow(p -> hwndGraphic);
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