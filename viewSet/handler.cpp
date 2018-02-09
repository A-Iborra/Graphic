// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ViewSet.h"

#include <commctrl.h>
#include <stdio.h>

#include "Graphic_resource.h"

   static bool holdTextUpdate = false;

   LRESULT CALLBACK ViewSet::viewsetHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   ViewSet *p = (ViewSet *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch ( msg ) {

   case WM_INITDIALOG: {
      p = (ViewSet *)lParam;
      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);
      }
      return LRESULT(FALSE);
 
   case WM_SIZE:
      p -> size();
      return LRESULT(0);

   case WM_SHOWWINDOW:

      if ( ! wParam )
         break;

      if ( lParam )
         break;

      p -> pIOpenGLImplementation -> SetTargetWindow(p -> hwndGraphic);

      break;

   case WM_VSCROLL:
   case WM_HSCROLL: {
      if ( ! p ) break;

      RECT rectDialog,rectItem,rectBar,rectEntryField;
      HWND hwndBar = (HWND)lParam;
      long pos = (long)SendMessage(hwndBar,TBM_GETPOS,0,0);
 	   char szTemp[32];
      GetWindowRect(hwndBar,&rectBar);
      GetWindowRect(p -> hwndViewSet,&rectDialog);

      if ( WM_VSCROLL == msg ) {

         long controlID = (long)GetWindowLongPtr(hwndBar,GWLP_ID);

         switch ( controlID ) {
         case IDDI_VIEWSET_PHISET: {

            long y;

            p -> parentPropertyPhi -> put_doubleValue(90.0 - (double)pos);

 		      if ( ! holdTextUpdate ) {
               sprintf(szTemp,"%4.1f",90.0 - (double)pos);
               SetWindowText(GetDlgItem(hwnd,IDDI_VIEWSET_PHITEXT),szTemp);
            }

            y = rectBar.top - rectDialog.top;
            y += (long)((double)(rectBar.bottom - rectBar.top - 24) * (double)pos / 180.0);

            SetWindowPos(GetDlgItem(hwnd,IDDI_VIEWSET_PHITEXT),HWND_TOP,rectBar.left - rectDialog.left - 40,y,0,0,SWP_NOSIZE);

            GetWindowRect(GetDlgItem(hwnd,IDDI_VIEWSET_PHITEXT),&rectEntryField);

            GetWindowRect(GetDlgItem(hwnd,IDDI_VIEWSET_PHITEXT_MIN),&rectItem);
            if ( abs(rectEntryField.bottom - rectItem.bottom) < 20 )
               ShowWindow(GetDlgItem(hwnd,IDDI_VIEWSET_PHITEXT_MIN),SW_HIDE);
            else
               ShowWindow(GetDlgItem(hwnd,IDDI_VIEWSET_PHITEXT_MIN),SW_SHOW);

            GetWindowRect(GetDlgItem(hwnd,IDDI_VIEWSET_PHITEXT_MAX),&rectItem);
            if ( abs(rectEntryField.bottom - rectItem.bottom) < 20 )
               ShowWindow(GetDlgItem(hwnd,IDDI_VIEWSET_PHITEXT_MAX),SW_HIDE);
            else
               ShowWindow(GetDlgItem(hwnd,IDDI_VIEWSET_PHITEXT_MAX),SW_SHOW);

            }
            break;

         case IDDI_VIEWSET_SPINSET: {

            long y;

            p -> parentPropertySpin -> put_doubleValue(360.0 - (double)pos);

 		      if ( ! holdTextUpdate ) {
               sprintf(szTemp,"%4.1f",360.0 - (double)pos);
               SetWindowText(GetDlgItem(hwnd,IDDI_VIEWSET_SPINTEXT),szTemp);
            }

            y = rectBar.top - rectDialog.top;
            y += (long)((double)(rectBar.bottom - rectBar.top - 24) * (double)pos / 360.0);

            SetWindowPos(GetDlgItem(hwnd,IDDI_VIEWSET_SPINTEXT),HWND_TOP,rectBar.left - rectDialog.left + 40,y,0,0,SWP_NOSIZE);

            GetWindowRect(GetDlgItem(hwnd,IDDI_VIEWSET_SPINTEXT),&rectEntryField);

            GetWindowRect(GetDlgItem(hwnd,IDDI_VIEWSET_SPINTEXT_MIN),&rectItem);
            if ( abs(rectEntryField.bottom - rectItem.bottom) < 20 )
               ShowWindow(GetDlgItem(hwnd,IDDI_VIEWSET_SPINTEXT_MIN),SW_HIDE);
            else
               ShowWindow(GetDlgItem(hwnd,IDDI_VIEWSET_SPINTEXT_MIN),SW_SHOW);

            GetWindowRect(GetDlgItem(hwnd,IDDI_VIEWSET_SPINTEXT_MAX),&rectItem);
            if ( abs(rectEntryField.bottom - rectItem.bottom) < 20 )
               ShowWindow(GetDlgItem(hwnd,IDDI_VIEWSET_SPINTEXT_MAX),SW_HIDE);
            else
               ShowWindow(GetDlgItem(hwnd,IDDI_VIEWSET_SPINTEXT_MAX),SW_SHOW);

            }
            break;
         }

      } else {

         long x;

         p -> parentPropertyTheta -> put_doubleValue(static_cast<double>(pos));

 		   if ( ! holdTextUpdate ) {
            sprintf(szTemp,"%5.1f",(double)pos);
          	SetWindowText(GetDlgItem(hwnd,IDDI_VIEWSET_THETATEXT),szTemp);
         }

         x = rectBar.left - rectDialog.left;
         x += (long)((double)(rectBar.right - rectBar.left - 16) * (double)pos / 360.0);

         SetWindowPos(GetDlgItem(hwnd,IDDI_VIEWSET_THETATEXT),HWND_TOP,x - 12,rectBar.bottom - rectDialog.top,28,20,SWP_NOSIZE);

         GetWindowRect(GetDlgItem(hwnd,IDDI_VIEWSET_THETATEXT),&rectEntryField);

         GetWindowRect(GetDlgItem(hwnd,IDDI_VIEWSET_THETATEXT_MIN),&rectItem);
         if ( abs(rectEntryField.left - rectItem.left) < 20 || rectEntryField.left < rectItem.right )
            ShowWindow(GetDlgItem(hwnd,IDDI_VIEWSET_THETATEXT_MIN),SW_HIDE);
         else
            ShowWindow(GetDlgItem(hwnd,IDDI_VIEWSET_THETATEXT_MIN),SW_SHOW);

         GetWindowRect(GetDlgItem(hwnd,IDDI_VIEWSET_THETATEXT_MAX),&rectItem);
         if ( abs(rectEntryField.right - rectItem.right) < 20 || rectEntryField.right > rectItem.left )
            ShowWindow(GetDlgItem(hwnd,IDDI_VIEWSET_THETATEXT_MAX),SW_HIDE);
         else
            ShowWindow(GetDlgItem(hwnd,IDDI_VIEWSET_THETATEXT_MAX),SW_SHOW);
      }

      p -> render();

      }

      break;
 
 
   case WM_COMMAND:

      switch (LOWORD(wParam)) {

      case IDOK:
         if ( p -> pPostDialogClientCallback )
            p -> pPostDialogClientCallback(p -> pPostDialogClientCallbackArg);
         p -> pPostDialogClientCallbackArg = NULL;
         p -> pPostDialogClientCallback = NULL;
         EndDialog(hwnd,0);
         break;

      case IDDI_VIEWSET_VIEW2D: {
         long isChecked = (long)SendMessage(GetDlgItem(hwnd,IDDI_VIEWSET_VIEW2D),BM_GETCHECK,0L,0L);
         if ( isChecked == BST_CHECKED ) {
            EnableWindow(GetDlgItem(hwnd,IDDI_VIEWSET_THETASET),FALSE);
            EnableWindow(GetDlgItem(hwnd,IDDI_VIEWSET_PHISET),FALSE);
            EnableWindow(GetDlgItem(hwnd,IDDI_VIEWSET_THETATEXT),FALSE);
            EnableWindow(GetDlgItem(hwnd,IDDI_VIEWSET_PHITEXT),FALSE);
            p -> parentPropertyPlotView -> put_longValue((long)gcPlotView2D);
         } else {
            EnableWindow(GetDlgItem(hwnd,IDDI_VIEWSET_THETASET),TRUE);
            EnableWindow(GetDlgItem(hwnd,IDDI_VIEWSET_PHISET),TRUE);
            EnableWindow(GetDlgItem(hwnd,IDDI_VIEWSET_THETATEXT),TRUE);
            EnableWindow(GetDlgItem(hwnd,IDDI_VIEWSET_PHITEXT),TRUE);
            p -> parentPropertyPlotView -> put_longValue((long)gcPlotView3D);
         }
         p -> render();
         }
         return LRESULT(0);

      case IDDI_VIEWSET_THETATEXT:
      case IDDI_VIEWSET_PHITEXT:
      case IDDI_VIEWSET_SPINTEXT:
         if ( ! p ) break;
         switch ( HIWORD(wParam) ) {
         case EN_UPDATE: {
            double d;
            char szTemp[32];
            GetWindowText((HWND)lParam,szTemp,32);
            d = atof(szTemp);
            holdTextUpdate = true;
            if ( IDDI_VIEWSET_PHITEXT == LOWORD(wParam) ) {
               LPARAM lp = 90 - (long)d;
               p -> parentPropertyPhi -> put_doubleValue(d);
               SendMessage(GetDlgItem(p -> hwndViewSet,IDDI_VIEWSET_PHISET),TBM_SETPOS,(WPARAM)TRUE,lp);
               SendMessage(p -> hwndViewSet,WM_VSCROLL,0L,(LPARAM)GetDlgItem(p -> hwndViewSet,IDDI_VIEWSET_PHISET));
            } else {
               if ( IDDI_VIEWSET_THETATEXT == LOWORD(wParam) ) {
                  LPARAM lp = (long)d;
                  p -> parentPropertyTheta -> put_doubleValue(d);
                  SendMessage(GetDlgItem(p -> hwndViewSet,IDDI_VIEWSET_THETASET),TBM_SETPOS,(WPARAM)TRUE,lp);
                  SendMessage(p -> hwndViewSet,WM_HSCROLL,0L,(LPARAM)GetDlgItem(p -> hwndViewSet,IDDI_VIEWSET_THETASET));
               } else {
                  LPARAM lp = 360 - (long)d;
                  p -> parentPropertySpin -> put_doubleValue(d);
                  SendMessage(GetDlgItem(p -> hwndViewSet,IDDI_VIEWSET_SPINSET),TBM_SETPOS,(WPARAM)TRUE,lp);
                  SendMessage(p -> hwndViewSet,WM_VSCROLL,0L,(LPARAM)GetDlgItem(p -> hwndViewSet,IDDI_VIEWSET_SPINSET));
               }
            }
            holdTextUpdate = false;
            }
            break;
         case EN_CHANGE:
            break;
         default:
            break;
         }

      default:
         break;
 
      }
      break;
 
   }
 
   return LRESULT(0);
   }
 
 
   LRESULT CALLBACK ViewSet::viewsetGraphicHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   ViewSet *p = (ViewSet *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_CREATE: {
      CREATESTRUCT *cp = (CREATESTRUCT *)lParam;
      p = (ViewSet *)cp -> lpCreateParams;
      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);
      }
      return LRESULT(FALSE);
 
   case WM_GETDLGCODE:
      break;

   case WM_PAINT: {
      PAINTSTRUCT ps;
      BeginPaint(hwnd,&ps);
      EndPaint(hwnd,&ps);
      if ( p ) 
         p -> render();
      }
      return LRESULT(FALSE);
 
 
   default:
      break;
   }

   return DefWindowProc(hwnd,msg,wParam,lParam);
   }


   int ViewSet::size() {

   if ( ! hwndViewSet ) return 0;

   RECT rectText,rectFrame,rectClient,rectSlider;
   int sliderWidth,cxButton,cyButton,buttonSpace;
 
   GetClientRect(hwndViewSet,&rectClient);

   rectClient.bottom -= 36;

   GetWindowRect(hwndViewSet,&rectFrame);

   ptlFrame.x = rectFrame.left;
   ptlFrame.y = rectFrame.top;
   sizelFrame.cx = rectFrame.right - rectFrame.left;
   sizelFrame.cy = rectFrame.bottom - rectFrame.top;

   AdjustWindowRectEx(&rectFrame,(DWORD)GetWindowLongPtr(hwndViewSet,GWL_STYLE),FALSE,(DWORD)GetWindowLongPtr(hwndViewSet,GWL_EXSTYLE));

   buttonSpace = 8;

   long cxPhiSet;
   long xPhiSet,yPhiSet;
   long xSpinSet;

   cyButton = 24;

   cxButton = 0;
   cxPhiSet = 40;
   yPhiSet = 2*cyButton;

   xPhiSet = rectClient.right - cxPhiSet;
   xSpinSet = 5;

   GetWindowRect(GetDlgItem(hwndViewSet,IDDI_VIEWSET_THETASET),&rectSlider);

   rectSlider.left = rectFrame.left + 16;

   GetWindowRect(GetDlgItem(hwndViewSet,IDDI_VIEWSET_THETATEXT_MAX),&rectText);

   SetWindowPos(GetDlgItem(hwndViewSet,IDDI_VIEWSET_THETASET),HWND_TOP,
                  16,32 - 2*(rectText.bottom - rectText.top),
                  sliderWidth = rectClient.right - cxButton - 20 - 32,rectSlider.bottom - rectSlider.top,0);

   SetWindowPos(GetDlgItem(hwndViewSet,IDDI_VIEWSET_THETATEXT_MIN),HWND_TOP,22,26,0,0,SWP_NOSIZE);

   SetWindowPos(GetDlgItem(hwndViewSet,IDDI_VIEWSET_THETATEXT_MAX),HWND_TOP,16 + rectClient.right - 20 - 32 - (rectText.right - rectText.left),26,0,0,SWP_NOSIZE);

   SetWindowPos(GetDlgItem(hwndViewSet,IDDI_VIEWSET_PHISET),HWND_TOP,xPhiSet,yPhiSet,0,0,SWP_NOSIZE); 
   SetWindowPos(GetDlgItem(hwndViewSet,IDDI_VIEWSET_SPINSET),HWND_TOP,5,yPhiSet,0,0,SWP_NOSIZE); 

   long y;
   long cx = rectText.left - rectFrame.left - 32;
   long cy = rectClient.bottom - yPhiSet;
   long bottomSpace = 64;

   SetWindowPos(GetDlgItem(hwndViewSet,IDDI_VIEWSET_VIEW2D),HWND_TOP,100,yPhiSet + cy - 8 - cyButton,0,0,SWP_NOSIZE);

   SetWindowPos(GetDlgItem(hwndViewSet,IDDI_VIEWSET_PHISET),HWND_TOP,0,0,cxPhiSet,rectClient.bottom - yPhiSet - 8,SWP_NOMOVE);

   SetWindowPos(GetDlgItem(hwndViewSet,IDDI_VIEWSET_SPINSET),HWND_TOP,0,0,cxPhiSet,rectClient.bottom - yPhiSet - 8,SWP_NOMOVE);

   SetWindowPos(GetDlgItem(hwndViewSet,IDDI_VIEWSET_PHITEXT_MAX),HWND_TOP,xPhiSet - 16,yPhiSet + 8,24,16,0L);
   SetWindowPos(GetDlgItem(hwndViewSet,IDDI_VIEWSET_PHITEXT_MIN),HWND_TOP,xPhiSet - 24,yPhiSet + cy - 32,24,16,0L);
   SetWindowPos(GetDlgItem(hwndViewSet,IDDI_VIEWSET_PHITEXT),HWND_TOP,xPhiSet - 16,0,40,20,0L);

   SetWindowPos(GetDlgItem(hwndViewSet,IDDI_VIEWSET_SPINTEXT_MAX),HWND_TOP,xSpinSet + cxPhiSet,yPhiSet + 8,24,16,0L);
   SetWindowPos(GetDlgItem(hwndViewSet,IDDI_VIEWSET_SPINTEXT_MIN),HWND_TOP,xSpinSet + cxPhiSet,yPhiSet + cy - 32,24,16,0L);
   SetWindowPos(GetDlgItem(hwndViewSet,IDDI_VIEWSET_SPINTEXT),HWND_TOP,xSpinSet + cxPhiSet - 8,0,40,20,0L);

   GetWindowRect(GetDlgItem(hwndViewSet,IDDI_VIEWSET_THETATEXT_MIN),&rectText);

   y = rectText.bottom + 16 - rectFrame.top;

   SetWindowPos(hwndGraphic,HWND_TOP,48 + cxPhiSet,y,xPhiSet - 2 * cxPhiSet - 48,yPhiSet + cy - 8 - cyButton - y - 8,0);

   RECT rcButton;
   GetWindowRect(GetDlgItem(hwndViewSet,IDOK),&rcButton);

   SetWindowPos(GetDlgItem(hwndViewSet,IDOK),HWND_TOP,rectClient.right - rectClient.left - (rcButton.right - rcButton.left) - 32,rectClient.bottom - rectClient.top + 36 - (rcButton.bottom - rcButton.top) - 20,0,0,SWP_NOSIZE);

   double pos;

   parentPropertyPhi -> get_doubleValue(&pos);
   SendMessage(GetDlgItem(hwndViewSet,IDDI_VIEWSET_PHISET),TBM_SETPOS,(WPARAM)TRUE,90 - (long)pos);
   SendMessage(hwndViewSet,WM_VSCROLL,0L,(LPARAM)GetDlgItem(hwndViewSet,IDDI_VIEWSET_PHISET));

   parentPropertyTheta -> get_doubleValue(&pos);
   SendMessage(GetDlgItem(hwndViewSet,IDDI_VIEWSET_THETASET),TBM_SETPOS,(WPARAM)TRUE,(long)pos);
   SendMessage(hwndViewSet,WM_HSCROLL,0L,(LPARAM)GetDlgItem(hwndViewSet,IDDI_VIEWSET_THETASET));

   parentPropertySpin -> get_doubleValue(&pos);
   SendMessage(GetDlgItem(hwndViewSet,IDDI_VIEWSET_SPINSET),TBM_SETPOS,(WPARAM)TRUE,(long)pos);
   SendMessage(hwndViewSet,WM_VSCROLL,0L,(LPARAM)GetDlgItem(hwndViewSet,IDDI_VIEWSET_SPINSET));

   return 0;
   }
