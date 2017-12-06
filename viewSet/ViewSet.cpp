/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <gl\gl.h>

#include "GraphicControl_i.h"

#include "Graphic_resource.h"
#include "Utils.h"

#include "ViewSet.h"
#include "list.cpp"

   static bool holdTextUpdate = false;

   ViewSet::ViewSet(IUnknown *pUnkOuter) :
      refCount(0),
      pIUnknownOuter(pUnkOuter),
      pIDataSet(0),
      pIOpenGLImplementation(0),
      parentPropertyPlotView(0),
      parentPropertyPhi(0),
      parentPropertyTheta(0),
      parentPropertySpin(0),
      parentPropertyFloor(0),
      parentPropertyCeiling(0),
      isVisible(FALSE),
      pIAxis_X(NULL),
      pIAxis_Y(NULL),
      pIAxis_Z(NULL),

      hwndOwner(NULL),
      hwndParent(NULL),
      hwndGraphicContainer(NULL),
      hwndViewSet(NULL) {
 
   IUnknown *pIUnknownThis;
   QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknownThis));
 
   CoCreateInstance(CLSID_InnoVisioNateProperties,
                    pIUnknownThis,
                    CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,
                    IID_IUnknown,
                    reinterpret_cast<void **>(&pIUnknownProperties));
 
   pIUnknownProperties -> QueryInterface(IID_IGProperties,reinterpret_cast<void**>(&iProperties));
 
   IGPropertiesClient *pIPropertiesClient;
   QueryInterface(IID_IGPropertiesClient,reinterpret_cast<void **>(&pIPropertiesClient));
   iProperties -> Advise(pIPropertiesClient);
   pIPropertiesClient -> Release();

   IGPropertyPageClient *pIPropertyPageClient;
   QueryInterface(IID_IGPropertyPageClient,reinterpret_cast<void**>(&pIPropertyPageClient));
   iProperties -> AdvisePropertyPageClient(pIPropertyPageClient,true);
   pIPropertyPageClient -> Release();
 
   iProperties -> Add(L"x",NULL);
   iProperties -> Add(L"y",NULL);
   iProperties -> Add(L"cx",NULL);
   iProperties -> Add(L"cy",NULL);
 
   ptlFrame.x = 0;
   ptlFrame.y = 0;
   sizelFrame.cx = VIEWSET_MINCX;
   sizelFrame.cy = VIEWSET_MINCY;
 
   iProperties -> DirectAccess(L"x",TYPE_LONG,&ptlFrame.x,sizeof(long));
   iProperties -> DirectAccess(L"y",TYPE_LONG,&ptlFrame.y,sizeof(long));
   iProperties -> DirectAccess(L"cx",TYPE_LONG,&sizelFrame.cx,sizeof(long));
   iProperties -> DirectAccess(L"cy",TYPE_LONG,&sizelFrame.cy,sizeof(long));
 
   CoCreateInstance(CLSID_Text,
                    NULL,
                    CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,
                    IID_IText,
                    reinterpret_cast<void **>(&pIText_X));
   CoCreateInstance(CLSID_Text,
                    NULL,
                    CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,
                    IID_IText,
                    reinterpret_cast<void **>(&pIText_Y));
   CoCreateInstance(CLSID_Text,
                    NULL,
                    CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,
                    IID_IText,
                    reinterpret_cast<void **>(&pIText_Z));

   pIText_X -> put_Text(L"X");
   pIText_X -> put_Size(10.0);
   pIText_X -> put_SizeUnits(TEXT_SIZE_POINTS);

   pIText_Y -> put_Text(L"Y");
   pIText_Y -> put_Size(10.0);
   pIText_Y -> put_SizeUnits(TEXT_SIZE_POINTS);

   pIText_Z -> put_Text(L"Z");
   pIText_Z -> put_Size(10.0);
   pIText_Z -> put_SizeUnits(TEXT_SIZE_POINTS);
   
   refCount = 0;

   return;
   }
 
 
   ViewSet::~ViewSet() {
   if ( hwndViewSet )
      DestroyWindow(hwndViewSet);
   iProperties -> Release();
   if ( pIOpenGLImplementation ) pIOpenGLImplementation -> Release();
   if ( pIDataSet ) pIDataSet -> Release();
   if ( pIAxis_X ) pIAxis_X -> Release();
   if ( pIAxis_Y ) pIAxis_Y -> Release();
   if ( pIAxis_Z ) pIAxis_Z -> Release();
   pIText_X -> Release();
   pIText_Y -> Release();
   pIText_Z -> Release();
   return;
   }
 
 
   int ViewSet::initWindows() {
   double d;
 
   DLGTEMPLATE *dt = (DLGTEMPLATE *)LoadResource(hModule,FindResource(hModule,MAKEINTRESOURCE(IDDIALOG_VIEWSET),RT_DIALOG));

   hwndViewSet = CreateDialogIndirect(hModule,dt,hwndParent,(DLGPROC)viewsetHandler);
 
   SetWindowLong(hwndViewSet,GWL_USERDATA,(LONG)this);
 
   WNDCLASS gClass;
   gClass.style = CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW;
   gClass.lpfnWndProc = viewsetGraphicHandler;
   gClass.cbClsExtra = 32;
   gClass.cbWndExtra = 32;
   gClass.hInstance = hModule;
   gClass.hIcon = NULL;
   gClass.hCursor = NULL;
   gClass.hbrBackground = 0;
   gClass.lpszMenuName = NULL;
   gClass.lpszClassName = "G-viewSetGraphic";
 
   RegisterClass(&gClass);
 
   hwndGraphic = CreateWindowEx(
                   WS_EX_WINDOWEDGE,
                   "G-viewSetGraphic",
                   "view Set",
                   WS_VISIBLE | WS_CHILD | WS_THICKFRAME,
                   0,0,0,0,
                   hwndViewSet,
                   NULL,
                   hModule,
                   (void *)this);
 
   HWND hwndTemp;

   SendMessage(hwndTemp = GetDlgItem(hwndViewSet,IDDI_VIEWSET_PHISET),TBM_SETRANGE,(WPARAM)TRUE,MAKELPARAM(0,180));
   SendMessage(hwndTemp,TBM_SETPAGESIZE,0,5);
   for ( long k = 5; k < 180; k += 5 ) 
      SendMessage(hwndTemp,TBM_SETTIC,0,k);
   parentPropertyPhi -> get_doubleValue(&d);
   SendMessage(hwndTemp,TBM_SETPOS,(WPARAM)TRUE,90 - (long)(d));
 
   SendMessage(hwndTemp = GetDlgItem(hwndViewSet,IDDI_VIEWSET_THETASET),TBM_SETRANGE,(WPARAM)TRUE,MAKELPARAM(0,360));
   SendMessage(hwndTemp,TBM_SETPAGESIZE,0,10);
   for ( long k = 10; k < 360; k += 10 ) 
      SendMessage(hwndTemp,TBM_SETTIC,0,k);
   parentPropertyTheta -> get_doubleValue(&d);
   SendMessage(hwndTemp,TBM_SETPOS,(WPARAM)TRUE,static_cast<long>(d));

   SendMessage(hwndTemp = GetDlgItem(hwndViewSet,IDDI_VIEWSET_SPINSET),TBM_SETRANGE,(WPARAM)TRUE,MAKELPARAM(0,360));
   SendMessage(hwndTemp,TBM_SETPAGESIZE,0,10);
   for ( long k = 10; k < 360; k += 10 ) 
      SendMessage(hwndTemp,TBM_SETTIC,0,k);
   parentPropertySpin -> get_doubleValue(&d);
   SendMessage(hwndTemp,TBM_SETPOS,(WPARAM)TRUE,static_cast<long>(d));

   parentPropertyPhi -> setWindowItemText(hwndViewSet,IDDI_VIEWSET_PHITEXT);
   parentPropertyTheta -> setWindowItemText(hwndViewSet,IDDI_VIEWSET_THETATEXT);
   parentPropertySpin -> setWindowItemText(hwndViewSet,IDDI_VIEWSET_SPINTEXT);
 
   long pt;
   parentPropertyPlotView -> get_longValue(&pt);
   if ( pt == gcPlotView2D ) {
      EnableWindow(GetDlgItem(hwndViewSet,IDDI_VIEWSET_THETASET),FALSE);
      EnableWindow(GetDlgItem(hwndViewSet,IDDI_VIEWSET_PHISET),FALSE);
      EnableWindow(GetDlgItem(hwndViewSet,IDDI_VIEWSET_SPINSET),FALSE);
      EnableWindow(GetDlgItem(hwndViewSet,IDDI_VIEWSET_THETATEXT),FALSE);
      EnableWindow(GetDlgItem(hwndViewSet,IDDI_VIEWSET_PHITEXT),FALSE);
      EnableWindow(GetDlgItem(hwndViewSet,IDDI_VIEWSET_SPINTEXT),FALSE);
      SendMessage(GetDlgItem(hwndViewSet,IDDI_VIEWSET_VIEW2D),BM_SETCHECK,BST_CHECKED,0L);
   } else {
      EnableWindow(GetDlgItem(hwndViewSet,IDDI_VIEWSET_THETASET),TRUE);
      EnableWindow(GetDlgItem(hwndViewSet,IDDI_VIEWSET_PHISET),TRUE);
      EnableWindow(GetDlgItem(hwndViewSet,IDDI_VIEWSET_SPINSET),TRUE);
      EnableWindow(GetDlgItem(hwndViewSet,IDDI_VIEWSET_THETATEXT),TRUE);
      EnableWindow(GetDlgItem(hwndViewSet,IDDI_VIEWSET_PHITEXT),TRUE);
      EnableWindow(GetDlgItem(hwndViewSet,IDDI_VIEWSET_SPINTEXT),TRUE);
      SendMessage(GetDlgItem(hwndViewSet,IDDI_VIEWSET_VIEW2D),BM_SETCHECK,BST_UNCHECKED,0L);
   }

   size();

   return TRUE;
   }
 
 
   int ViewSet::render() {

   if ( ! IsWindowVisible(hwndGraphic) ) return 0;

   pIOpenGLImplementation -> SetTargetWindow(hwndGraphic);

   pIOpenGLImplementation -> Erase();

   pIOpenGLImplementation -> SetUp(pIDataSet);

   short drawText[3];

   pIAxis_X -> get_DrawText(&drawText[0]);
   pIAxis_Y -> get_DrawText(&drawText[1]);
   pIAxis_Z -> get_DrawText(&drawText[2]);

   pIAxis_X -> put_DrawText(false);
   pIAxis_Y -> put_DrawText(false);
   pIAxis_Z -> put_DrawText(false);

   pIAxis_X -> PrepData();
   pIAxis_Y -> PrepData();
   pIAxis_Z -> PrepData();

   pIAxis_X -> Draw();
   pIAxis_Y -> Draw();
   pIAxis_Z -> Draw();

   pIAxis_X -> put_DrawText(drawText[0]);
   pIAxis_Y -> put_DrawText(drawText[1]);
   pIAxis_Z -> put_DrawText(drawText[2]);

   pIText_X -> Draw();
   pIText_Y -> Draw();
   pIText_Z -> Draw();

   pIText_X -> put_PartOfWorldDomain(FALSE);
   pIText_Y -> put_PartOfWorldDomain(FALSE);
   pIText_Z -> put_PartOfWorldDomain(FALSE);

   pIOpenGLImplementation -> ResetTargetWindow();

   return 0;
   }
 
 
   LRESULT CALLBACK ViewSet::viewsetHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
   ViewSet *p = (ViewSet *)GetWindowLong(hwnd,GWL_USERDATA);
 
   switch ( msg ) {

   case WM_INITDIALOG:
      SetWindowLong(hwnd,GWL_USERDATA,0);
      return LRESULT(FALSE);
 
   case WM_SIZE:
      p -> size();
      return LRESULT(0);
 
   case WM_VSCROLL:
   case WM_HSCROLL: {
      if ( ! p ) break;
      RECT rectDialog,rectItem,rectBar,rectEntryField;
      HWND hwndBar = (HWND)lParam;
      long pos = SendMessage(hwndBar,TBM_GETPOS,0,0);
 	   char szTemp[32];
      GetWindowRect(hwndBar,&rectBar);
      GetWindowRect(p -> hwndViewSet,&rectDialog);
      if ( WM_VSCROLL == msg ) {

         long controlID = GetWindowLong(hwndBar,GWL_ID);

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

      case IDDI_VIEWSET_VIEW2D: {
         long isChecked = SendMessage(GetDlgItem(hwnd,IDDI_VIEWSET_VIEW2D),BM_GETCHECK,0L,0L);
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
   ViewSet *p = (ViewSet *)GetWindowLong(hwnd,GWL_USERDATA);

   switch ( msg ) {

   case WM_CREATE: {
      CREATESTRUCT *cp = (CREATESTRUCT *)lParam;
      p = (ViewSet *)cp -> lpCreateParams;
      SetWindowLong(hwnd,GWL_USERDATA,(long)p);
      }
      return LRESULT(FALSE);
 
   case WM_PAINT: {
      PAINTSTRUCT ps;
      BeginPaint(hwnd,&ps);
      EndPaint(hwnd,&ps);
      if ( ! p ) break;
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
   GetWindowRect(hwndViewSet,&rectFrame);

   ptlFrame.x = rectFrame.left;
   ptlFrame.y = rectFrame.top;
   sizelFrame.cx = rectFrame.right - rectFrame.left;
   sizelFrame.cy = rectFrame.bottom - rectFrame.top;

   AdjustWindowRect(&rectFrame,GetWindowLong(hwndViewSet,GWL_STYLE),FALSE);

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
