// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Plot.h"
#include <windowsx.h>

   LRESULT CALLBACK Plot::sampleGraphicHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   Plot *p = (Plot *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_CREATE: {

      p = (Plot *)((CREATESTRUCT *)lParam) -> lpCreateParams;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      HWND hwndCurrentOpenGL = p -> pIOpenGLImplementation -> TargetHWND();

      RECT rc;
      GetWindowRect(hwndCurrentOpenGL,&rc);

      p -> sizeMainGraphic.cx = rc.right - rc.left;
      p -> sizeMainGraphic.cy = rc.bottom - rc.top;

      }
      return (LRESULT)0L;
 
   case WMG_POSITION_SAMPLE_GRAPHIC: {

      double aspectRatio = (double)p -> sizeMainGraphic.cx / (double)p -> sizeMainGraphic.cy;

      RECT rcThis;

      memcpy(&rcThis,(BYTE *)lParam,sizeof(RECT));

      long cx = rcThis.right - rcThis.left;
      long cy = (long)((double)cx / aspectRatio);

      if ( rcThis.bottom - rcThis.top < cy ) {
         long cySubtract = cy - (rcThis.bottom - rcThis.top);
         cy = rcThis.bottom - rcThis.top;
         cx -= (long)((double)cySubtract * aspectRatio);
      }

      long x = (rcThis.right + rcThis.left - cx) / 2;
      long y = (rcThis.bottom + rcThis.top - cy) / 2;

      SetWindowPos(hwnd,HWND_TOP,x,y,cx,cy,SWP_SHOWWINDOW);

      if ( p -> hwndSampleGraphicSurface )
         DestroyWindow(p -> hwndSampleGraphicSurface);

      p -> hwndSampleGraphicSurface = CreateWindowEx(0L,"plot-sample-graphic-surface","",WS_CHILD | WS_VISIBLE,0,0,cx,cy,hwnd,NULL,hModule,(void *)p);

      return (LRESULT)0L;
      }

   case WM_ERASEBKGND:
   case WM_PAINT: {
      PAINTSTRUCT ps;
      BeginPaint(hwnd,&ps);
      EndPaint(hwnd,&ps);
      return (LRESULT)FALSE;
      }

   default:
      break;
   }
 
   return DefWindowProc(hwnd,msg,wParam,lParam);
   }


   static IText *pIText_MoveCandidate = NULL;
   static RECT rcCandidateHome{0,0,0,0};
   static HBITMAP hbmSave = NULL;
   static POINTL ptlAnchor;

   LRESULT CALLBACK Plot::sampleGraphicSurfaceHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   Plot *p = (Plot *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_CREATE: {
      p = (Plot *)((CREATESTRUCT *)lParam) -> lpCreateParams;
      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);
      p -> pIOpenGLImplementation -> SetTargetWindow(hwnd);
      }
      break;

   case WM_DESTROY:
      if ( hbmSave )
         DeleteObject(hbmSave);
      break;

   case WM_MOUSEMOVE: {

      if ( ( wParam & MK_LBUTTON ) && pIText_MoveCandidate ) {

         POINTL ptlNew{GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam)};

         long x = rcCandidateHome.left;
         long y = rcCandidateHome.top;
         long cx = rcCandidateHome.right - rcCandidateHome.left;
         long cy = rcCandidateHome.bottom - rcCandidateHome.top;

         HDC hdc = GetDC(hwnd);

         HDC hdcSource = CreateCompatibleDC(hdc);

         SelectObject(hdcSource,hbmSave);

         BitBlt(hdc,x - 2,y - 2,cx + 4,cy + 4,hdcSource,x - 2,y - 2,SRCCOPY);

         rcCandidateHome.left += ptlNew.x - ptlAnchor.x;
         rcCandidateHome.top += ptlNew.y - ptlAnchor.y;

         rcCandidateHome.right = rcCandidateHome.left + cx + 4;
         rcCandidateHome.bottom = rcCandidateHome.top + cy + 4;

         pIText_MoveCandidate -> put_GDIBoundingBox(&rcCandidateHome);

         pIText_MoveCandidate -> Draw();

         ptlAnchor = ptlNew;

         DeleteDC(hdcSource);

         ReleaseDC(hwnd,hdc);

         break;

      }

      POINTL ptlMouse{GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam)};

      IText *pIText = NULL;

      while ( pIText = p -> textList.GetNext(pIText) ) {
   
         RECT rcGDI;
         pIText -> get_GDIBoundingBox(&rcGDI);

         if ( ptlMouse.x < rcGDI.left || ptlMouse.x > rcGDI.right || ptlMouse.y < rcGDI.top || ptlMouse.y > rcGDI.bottom ) {
            p -> pIGSystemStatusBar -> put_StatusText(0,"");
            pIText_MoveCandidate = NULL;
            continue;
         }

         pIText_MoveCandidate = pIText;

         p -> pIGSystemStatusBar -> put_StatusText(0,"Click-drag to move this text.");
      }

      }
      break;

   case WM_LBUTTONDOWN: {

      if ( ! pIText_MoveCandidate )
         break;

      ptlAnchor.x = GET_X_LPARAM(lParam);
      ptlAnchor.y = GET_Y_LPARAM(lParam);

      pIText_MoveCandidate -> get_GDIBoundingBox(&rcCandidateHome);

      p -> pIGSystemStatusBar -> put_StatusText(0,"Drag to move this text");

      sampleGraphicThread((void *)p);

      HDC hdc = GetDC(hwnd);

      HDC hdcTarget = CreateCompatibleDC(hdc);

      RECT rcThis;

      GetWindowRect(hwnd,&rcThis);

      long cx = rcThis.right - rcThis.left;
      long cy = rcThis.bottom - rcThis.top;

      hbmSave = CreateCompatibleBitmap(hdc,cx,cy);

      HGDIOBJ oldBitmap = SelectObject(hdcTarget,hbmSave);

      BitBlt(hdcTarget,0,0,cx,cy,hdc,0,0,SRCCOPY);

      SelectObject(hdcTarget,oldBitmap);

      DeleteDC(hdcTarget);
      
      }
      break;

   case WM_LBUTTONUP:

      p -> pIGSystemStatusBar -> put_StatusText(0,"");

      if ( ! pIText_MoveCandidate )
         break;

      pIText_MoveCandidate -> put_GDIBoundingBox(&rcCandidateHome);

      pIText_MoveCandidate -> put_WindowPosition((POINTL *)&rcCandidateHome);

      double x,y,z;

      pIText_MoveCandidate -> get_PositionX(&x);
      pIText_MoveCandidate -> get_PositionY(&y);
      pIText_MoveCandidate -> get_PositionZ(&z);

      char szTemp[128];

      sprintf_s(szTemp,128,"Text moved to: %g %g %g",x,y,z);

      p -> pIGSystemStatusBar -> put_StatusText(0,szTemp);

      pIText_MoveCandidate = NULL;

      break;

   case WM_ERASEBKGND:
   case WM_PAINT: {
      PAINTSTRUCT ps;
      BeginPaint(hwnd,&ps);
      sampleGraphicThread((void *)p);
      EndPaint(hwnd,&ps);
      return (LRESULT)FALSE;
      }

   }

   return DefWindowProc(hwnd,msg,wParam,lParam);
   }


   DWORD WINAPI Plot::sampleGraphicThread(void *pvArg) {

   Plot *p = (Plot *)pvArg;

   if ( p -> overrideOwnerView )
      p -> pIOpenGLImplementation -> SetUp(p -> pIDataSetDomain,p -> propertyPlotView);
   else
      p -> pIOpenGLImplementation -> SetUp(p -> pIDataSetDomain,p -> pOwnerPropertyPlotView);

   p -> pIGSGraphicServices -> ActivateLighting();

   p -> pIOpenGLImplementation -> Erase(p -> pOwnerPropertyBackgroundColor);

   long keepOkayToPlot;

   p -> get_OkToPlot(&keepOkayToPlot);

   p -> put_OkToPlot(1L);

   p -> Draw();

   IText *pIText = NULL;
   while ( pIText = p -> textList.GetNext(pIText) ) {
      boolean isOpenGLRendered;
      pIText -> get_TextRender(&isOpenGLRendered);
      if ( ! isOpenGLRendered )
         continue;
      if ( pIText == pIText_MoveCandidate )
         continue;
      pIText -> Draw();
   }

   p -> pIOpenGLImplementation -> Finalize();

   while ( pIText = p -> textList.GetNext(pIText) ) {
      boolean isOpenGLRendered;
      pIText -> get_TextRender(&isOpenGLRendered);
      if ( isOpenGLRendered )
         continue;
      if ( pIText == pIText_MoveCandidate )
         continue;
      pIText -> Draw();
   }

   p -> put_OkToPlot(keepOkayToPlot);

   return 0L;
   }