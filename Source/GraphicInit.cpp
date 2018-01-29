// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"

#include "Graphic_resource.h"
#include "utils.h"

#include "ContainedFunction.h"

#include "List.cpp"


   int G::initWindows() {

   WNDCLASS gClass;
 
   HRESULT hr = pIOleInPlaceSite -> GetWindow(&hwndOwner);

   if ( ! hwndOwner )
      return 0;

   pIOpenGLImplementation -> Start();

   GetWindowText(hwndOwner,windowTitle,32);
 
   hMenuPlot = LoadMenu((HINSTANCE)hModule,MAKEINTRESOURCE(IDMENU_GRAPHIC));
   hMenuPlot = GetSubMenu(hMenuPlot,0);
 
   memset(&gClass,0,sizeof(WNDCLASS));
   gClass.style = CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW | CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
   gClass.lpfnWndProc = graphicFrameHandler;
   gClass.cbClsExtra = windowStorageBytes;
   gClass.cbWndExtra = windowStorageBytes;
   gClass.hInstance = hModule;
   gClass.hIcon = NULL;
   gClass.hCursor = NULL;
   gClass.hbrBackground = 0;
   gClass.lpszMenuName = NULL;
   gClass.lpszClassName = "G-graphic-Frame";
  
   RegisterClass(&gClass);
  
   gClass.lpfnWndProc = graphicHandler;
   gClass.lpszClassName = "G-graphic";
   
   RegisterClass(&gClass);
 
   gClass.lpfnWndProc = sampleGraphicHandler;
   gClass.lpszClassName = "G-plotSettingsGraphic";
 
   RegisterClass(&gClass);

   gClass.lpfnWndProc = sampleGraphicSurfaceHandler;
   gClass.lpszClassName = "G-plotSettingsGraphic-Surface";
 
   RegisterClass(&gClass);

   hwndFrame = CreateWindowEx(0*WS_EX_CONTROLPARENT,"G-graphic-Frame","Graphic",WS_CHILD,0,0,0,0,hwndOwner,NULL,hModule,(void *)this);

   RECT rcInitial{0,0,0,0};

   hwndCanvas = newCanvas(&rcInitial);

   hwndStatusBar = CreateWindowEx(0L,STATUSCLASSNAME,"",WS_CHILD | WS_VISIBLE,0,0,0,0,hwndFrame,NULL,hModule,(void *)this);

   if ( defaultStatusBarHandler ) 
      SetWindowLongPtr(hwndStatusBar,GWLP_WNDPROC,(ULONG_PTR)statusBarHandler);
   else
      defaultStatusBarHandler = (WNDPROC)SetWindowLongPtr(hwndStatusBar,GWLP_WNDPROC,(ULONG_PTR)statusBarHandler);

   DLGTEMPLATE *dt = (DLGTEMPLATE *)LoadResource(hModule,FindResource(hModule,MAKEINTRESOURCE(IDNOTEBOOK_GRAPHIC_DATASOURCES),RT_DIALOG));

   hwndDataSourcesDialog = CreateDialogIndirectParam(hModule,dt,hwndFrame,(DLGPROC)dataSourcesHandler,(LPARAM)this);

   SetWindowLongPtr(hwndDataSourcesDialog,GWL_STYLE,(ULONG_PTR)((GetWindowLongPtr(hwndDataSourcesDialog,GWL_STYLE) & ~ WS_CAPTION) & ~ WS_VISIBLE));

   hwndDataSourcesTab = GetDlgItem(hwndDataSourcesDialog,IDDI_DATASOURCES_TAB);

   hwndDataSourcesFunctions = GetDlgItem(hwndDataSourcesDialog,IDDI_DATASOURCES_FUNCTIONS_TAB);

   hwndDataSourcesDataSets = GetDlgItem(hwndDataSourcesDialog,IDDI_DATASOURCES_DATASETS_TAB);


      IGSFunctioNater *pIFunction = NULL;
      while ( pIFunction = functionList.GetNext(pIFunction) )
         connectFunction(pIFunction);

      ContainedFunction *pcf = (ContainedFunction *)NULL;
      while ( pcf = containedFunctionList.GetNext(pcf) )
         ShowWindow(pcf -> HWNDSite(),SW_HIDE);

      //SendMessage(pParent -> hwndDataSourcesFunctions,TCM_SETCURSEL,(WPARAM)0,0L);

      IDataSet *pIDataSet = NULL;
      while ( pIDataSet = dataSetList.GetNext(pIDataSet) )
         connectDataSet(pIDataSet);

      ContainedDataSet *pCds = (ContainedDataSet *)NULL;
      while ( pCds = containedDataSetList.GetNext(pCds) )
         ShowWindow(pCds -> HWNDSite(),SW_HIDE);

      SendMessage(hwndDataSourcesDataSets,TCM_SETCURSEL,(WPARAM)0,0L);

      if ( 0 < containedFunctionList.Count() ) {
         SendMessage(hwndDataSourcesFunctions,TCM_SETCURSEL,0L,0L);
         ContainedFunction* pContainedFunction = containedFunctionList.GetFirst();
         ShowWindow(pContainedFunction -> HWNDSite(),SW_SHOW);
      }

      setDataSourcesVisibility(NULL,NULL);



   put_PlotView(plotView);
   put_PlotType(default2DPlotType);

   pIOpenGLImplementation -> SetBaseWindow(Canvas());

   long plotView = 0L;
   propertyPlotView -> get_longValue(&plotView);
   if ( zaxis && gcPlotView2D == plotView ) {
      IDataSet *pIDataSet;
      zaxis -> get_DataSet(&pIDataSet);
      pIDataSetMaster -> RemoveIncludedDomain(pIDataSet);
   }

   pIOpenGLImplementation -> SetViewProperties(propertyPlotView,propertyViewTheta,propertyViewPhi,propertyViewSpin,
                                                   propertyPlotLeftMargin,propertyPlotTopMargin,propertyPlotRightMargin,propertyPlotBottomMargin,
                                                   propertyPlotMarginUnits,propertyPlotMarginsStretchAll);

   xaxis -> Initialize('X',xaxis,yaxis,zaxis,propertyPlotView,
                              propertyXFloor,propertyXCeiling,
                              propertyYFloor,propertyYCeiling,
                              propertyZFloor,propertyZCeiling,
                              propertyRenderOpenGLAxisText,pIDataSetMaster,pIOpenGLImplementation,pIEvaluator,someObjectChanged,(void *)this,(ULONG_PTR)xaxis);

   yaxis -> Initialize('Y',xaxis,yaxis,zaxis,propertyPlotView,
                              propertyXFloor,propertyXCeiling,
                              propertyYFloor,propertyYCeiling,
                              propertyZFloor,propertyZCeiling,
                              propertyRenderOpenGLAxisText,pIDataSetMaster,pIOpenGLImplementation,pIEvaluator,someObjectChanged,(void *)this,(ULONG_PTR)yaxis);

   zaxis -> Initialize('Z',xaxis,yaxis,zaxis,propertyPlotView,
                              propertyXFloor,propertyXCeiling,
                              propertyYFloor,propertyYCeiling,
                              propertyZFloor,propertyZCeiling,
                              propertyRenderOpenGLAxisText,pIDataSetMaster,pIOpenGLImplementation,pIEvaluator,someObjectChanged,(void *)this,(ULONG_PTR)zaxis);

   pIViewSet -> Initialize(pIOpenGLImplementation,pIEvaluator,
                              propertyPlotView,
                              propertyViewTheta,propertyViewPhi,propertyViewSpin,
                              propertyZFloor,propertyZCeiling,pIDataSetMaster,xaxis,yaxis,zaxis);

   return TRUE;
   }


   HWND G::newCanvas(RECT *pRect) {
   if ( hwndCanvas )
      DestroyWindow(hwndCanvas);
   if ( ! hwndFrame )
      return NULL;
   hwndCanvas = CreateWindowEx(WS_EX_CONTROLPARENT,"G-graphic","Graph",WS_CHILD | WS_VISIBLE,pRect -> left,pRect -> top,pRect -> right - pRect -> left,pRect -> bottom - pRect -> top,hwndFrame,NULL,hModule,(void *)this);
   return hwndCanvas;
   }

