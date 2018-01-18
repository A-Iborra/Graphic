/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002,2008 Nathan T. Clark

*/

#include <windows.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <process.h>
#include <math.h>

#include <commctrl.h>

#include <gl\gl.h>

#include "Graphic_resource.h"
#include "utils.h"

#include "ContainedFunction.h"
#include "Graphic.h"

#include "axis_i.c"

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
   gClass.style = CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW;
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

   hwndGraphic = CreateWindowEx(0L,"G-graphic","Graph",WS_CHILD | WS_VISIBLE,0,0,0,0,hwndFrame,NULL,hModule,(void *)this);

   hwndStatusBar = CreateWindowEx(0L,STATUSCLASSNAME,"",WS_CHILD | WS_VISIBLE,0,0,100,100,hwndFrame,NULL,hModule,(void *)this);

   if ( defaultStatusBarHandler ) 
      SetWindowLongPtr(hwndStatusBar,GWLP_WNDPROC,(ULONG_PTR)statusBarHandler);
   else
      defaultStatusBarHandler = (WNDPROC)SetWindowLong(hwndStatusBar,GWL_WNDPROC,(long)statusBarHandler);

   DLGTEMPLATE *dt = (DLGTEMPLATE *)LoadResource(hModule,FindResource(hModule,MAKEINTRESOURCE(IDNOTEBOOK_GRAPHIC_DATASOURCES),RT_DIALOG));

   hwndDataSourcesDialog = CreateDialogIndirectParam(hModule,dt,hwndFrame,(DLGPROC)dataSourcesHandler,(LPARAM)this);

   SetWindowLongPtr(hwndDataSourcesDialog,GWL_STYLE,(ULONG_PTR)((GetWindowLongPtr(hwndDataSourcesDialog,GWL_STYLE) & ~ WS_CAPTION) & ~ WS_VISIBLE));

   hwndDataSourcesTab = GetDlgItem(hwndDataSourcesDialog,IDDI_DATASOURCES_TAB);

   hwndDataSourcesFunctions = GetDlgItem(hwndDataSourcesDialog,IDDI_DATASOURCES_FUNCTIONS_TAB);

   hwndDataSourcesDataSets = GetDlgItem(hwndDataSourcesDialog,IDDI_DATASOURCES_DATASETS_TAB);

   put_PlotView(plotView);
   put_PlotType(default2DPlotType);

   if ( containedFunctionList.Count() > 0 ) {
      SendMessage(hwndDataSourcesFunctions,TCM_SETCURSEL,0L,0L);
      ContainedFunction* pContainedFunction = containedFunctionList.GetFirst();
      ShowWindow(pContainedFunction -> HWNDSite(),SW_SHOW);
   }

   pIOpenGLImplementation -> SetBaseWindow(hwndGraphic);

   long plotView = 0L;
   propertyPlotView -> get_longValue(&plotView);
   if ( zaxis && gcPlotView2D == plotView ) {
      IDataSet *pIDataSet;
      zaxis -> get_DataSet(&pIDataSet);
      pIDataSetMaster -> RemoveIncludedDomain(pIDataSet);
   }

   pIOpenGLImplementation -> SetViewProperties(
      propertyPlotView,
      propertyViewTheta,
      propertyViewPhi,
      propertyViewSpin,
      propertyPlotLeftMargin,
      propertyPlotTopMargin,
      propertyPlotRightMargin,
      propertyPlotBottomMargin,
      propertyPlotMarginUnits,
      propertyPlotMarginsStretchAll);

   xaxis -> Initialize(hwndGraphic,'X',xaxis,yaxis,zaxis,propertyPlotView,
                              propertyXFloor,propertyXCeiling,
                              propertyYFloor,propertyYCeiling,
                              propertyZFloor,propertyZCeiling,
                              propertyRenderOpenGLAxisText,pIDataSetMaster,pIOpenGLImplementation,pIEvaluator,someObjectChanged,(void *)this,(ULONG_PTR)xaxis);
   yaxis -> Initialize(hwndGraphic,'Y',xaxis,yaxis,zaxis,propertyPlotView,
                              propertyXFloor,propertyXCeiling,
                              propertyYFloor,propertyYCeiling,
                              propertyZFloor,propertyZCeiling,
                              propertyRenderOpenGLAxisText,pIDataSetMaster,pIOpenGLImplementation,pIEvaluator,someObjectChanged,(void *)this,(ULONG_PTR)yaxis);
   zaxis -> Initialize(hwndGraphic,'Z',xaxis,yaxis,zaxis,propertyPlotView,
                              propertyXFloor,propertyXCeiling,
                              propertyYFloor,propertyYCeiling,
                              propertyZFloor,propertyZCeiling,
                              propertyRenderOpenGLAxisText,pIDataSetMaster,pIOpenGLImplementation,pIEvaluator,someObjectChanged,(void *)this,(ULONG_PTR)zaxis);

   pIViewSet -> Initialize(hwndGraphic,pIOpenGLImplementation,pIEvaluator,
                              propertyPlotView,
                              propertyViewTheta,propertyViewPhi,propertyViewSpin,
                              propertyZFloor,propertyZCeiling,pIDataSetMaster,xaxis,yaxis,zaxis);

   return TRUE;
   }
