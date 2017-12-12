/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>

#include "GraphicControl_i.h"

#include "Graphic_resource.h"
#include "Utils.h"

#include "ViewSet.h"
#include "list.cpp"

   ViewSet::ViewSet(IUnknown *pUnkOuter) :
      refCount(0),
      pIUnknownOuter(pUnkOuter),
      pIDataSet(NULL),
      pIOpenGLImplementation(NULL),
      parentPropertyPlotView(NULL),
      parentPropertyPhi(NULL),
      parentPropertyTheta(NULL),
      parentPropertySpin(NULL),
      parentPropertyFloor(NULL),
      parentPropertyCeiling(NULL),
      isVisible(FALSE),
      pIAxis_X(NULL),
      pIAxis_Y(NULL),
      pIAxis_Z(NULL),

      pPostDialogClientCallback(NULL),
      pPostDialogClientCallbackArg(NULL),

      hwndOwner(NULL),
      hwndParent(NULL),
      hwndGraphicContainer(NULL),
      hwndViewSet(NULL) {
 
   IUnknown *pIUnknownThis;

   QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknownThis));
 
   CoCreateInstance(CLSID_InnoVisioNateProperties,pIUnknownThis,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IUnknown,reinterpret_cast<void **>(&pIUnknownProperties));
 
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

   refCount = 0;

   return;
   }
 
 
   ViewSet::~ViewSet() {

   if ( hwndViewSet )
      DestroyWindow(hwndViewSet);

   iProperties -> Release();

   if ( pIOpenGLImplementation ) 
      pIOpenGLImplementation -> Release();

   if ( pIDataSet )
      pIDataSet -> Release();

   if ( pIAxis_X )
      pIAxis_X -> Release();
   if ( pIAxis_Y )
      pIAxis_Y -> Release();
   if ( pIAxis_Z )
      pIAxis_Z -> Release();

   return;
   }
 
 
   int ViewSet::initWindows() {

   double d;
 
   DLGTEMPLATE *dt = (DLGTEMPLATE *)LoadResource(hModule,FindResource(hModule,MAKEINTRESOURCE(IDDIALOG_VIEWSET),RT_DIALOG));

   hwndViewSet = CreateDialogIndirectParam(hModule,dt,hwndOwner,(DLGPROC)viewsetHandler,(LPARAM)this);
 
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
 
   hwndGraphic = CreateWindowEx(WS_EX_WINDOWEDGE,"G-viewSetGraphic","view Set",WS_VISIBLE | WS_CHILD | WS_THICKFRAME,0,0,0,0,hwndViewSet,NULL,hModule,(void *)this);
 
   SetWindowLongPtr(hwndGraphic,GWLP_HWNDPARENT,(ULONG_PTR)hwndViewSet);

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

   //pIText_X -> Initialize(hwndGraphic,pIOpenGLImplementation,pIEvaluator,pIDataSet,parentPropertyFloor,parentPropertyCeiling,NULL,NULL,NULL);
   //pIText_Y -> Initialize(hwndGraphic,pIOpenGLImplementation,pIEvaluator,pIDataSet,parentPropertyFloor,parentPropertyCeiling,NULL,NULL,NULL);
   //pIText_Z -> Initialize(hwndGraphic,pIOpenGLImplementation,pIEvaluator,pIDataSet,parentPropertyFloor,parentPropertyCeiling,NULL,NULL,NULL);

   //pIText_X -> put_PartOfWorldDomain(FALSE);
   //pIText_Y -> put_PartOfWorldDomain(FALSE);
   //pIText_Z -> put_PartOfWorldDomain(FALSE);

   //pIText_X -> put_CoordinatePlane(CoordinatePlane_XY);
   //pIText_Y -> put_CoordinatePlane(CoordinatePlane_YX);
   //pIText_Z -> put_CoordinatePlane(CoordinatePlane_YZ);

   return TRUE;
   }
 
 
   int ViewSet::render() {

   if ( ! IsWindowVisible(hwndGraphic) ) 
      return 0;

   pIOpenGLImplementation -> Erase();

   pIOpenGLImplementation -> SetUp(pIDataSet);

   short drawText[3];

   //pIAxis_X -> get_DrawText(&drawText[0]);
   //pIAxis_Y -> get_DrawText(&drawText[1]);
   //pIAxis_Z -> get_DrawText(&drawText[2]);

   //pIAxis_X -> put_DrawText(false);
   //pIAxis_Y -> put_DrawText(false);
   //pIAxis_Z -> put_DrawText(false);

   //pIAxis_X -> PrepData();
   //pIAxis_Y -> PrepData();
   //pIAxis_Z -> PrepData();

   pIAxis_X -> Draw();
   pIAxis_Y -> Draw();
   pIAxis_Z -> Draw();

   //pIAxis_X -> put_DrawText(drawText[0]);
   //pIAxis_Y -> put_DrawText(drawText[1]);
   //pIAxis_Z -> put_DrawText(drawText[2]);

   //pIText_X -> PrepData();
   //pIText_Y -> PrepData();
   //pIText_Z -> PrepData();

   //pIText_X -> Draw();
   //pIText_Y -> Draw();
   //pIText_Z -> Draw();

   //pIText_X -> put_PartOfWorldDomain(FALSE);
   //pIText_Y -> put_PartOfWorldDomain(FALSE);
   //pIText_Z -> put_PartOfWorldDomain(FALSE);

   pIOpenGLImplementation -> Flush();

   return 0;
   }
 
 