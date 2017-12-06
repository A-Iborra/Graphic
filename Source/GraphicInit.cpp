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


   int G::oleSetWindowRect(RECT* prcPos) {
   SetWindowPos(hwndFrame,HWND_TOP,
                      prcPos -> left + 4,prcPos -> top + 4,
                         prcPos-> right - prcPos -> left - 8,prcPos -> bottom - prcPos -> top - 8,0L);
   return TRUE;
   }
 
 
   int G::initWindows() {

   RECT rect;
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
 
   hwndFrame = CreateWindowEx(0L,"G-graphic-Frame","Graphic",WS_CHILD | WS_BORDER,0,0,0,0,hwndOwner,NULL,hModule,(void *)this);
  
   gClass.lpfnWndProc = graphicHandler;
   gClass.lpszClassName = "G-graphic";
   
   RegisterClass(&gClass);
 
   hwndGraphic = CreateWindowEx(0L,"G-graphic","Graph",WS_CHILD | WS_VISIBLE,0,0,0,0,hwndFrame,NULL,hModule,(void *)this);

   gClass.lpfnWndProc = sampleGraphicHandler;
   gClass.lpszClassName = "G-plotSettingsGraphic";
 
   RegisterClass(&gClass);

   hwndStatusBar = CreateWindowEx(0L,STATUSCLASSNAME,"",WS_CHILD | WS_VISIBLE,0,0,0,0,hwndFrame,NULL,hModule,(void *)this);
   if ( defaultStatusBarHandler ) 
      SetWindowLong(hwndStatusBar,GWL_WNDPROC,(long)statusBarHandler);
   else
      defaultStatusBarHandler = (WNDPROC)SetWindowLong(hwndStatusBar,GWL_WNDPROC,(long)statusBarHandler);

   DLGTEMPLATE *dt = (DLGTEMPLATE *)LoadResource(hModule,FindResource(hModule,MAKEINTRESOURCE(IDNOTEBOOK_GRAPHIC_DATASOURCES),RT_DIALOG));
   hwndDataSourcesDialog = CreateDialogIndirectParam(hModule,dt,hwndFrame,(DLGPROC)dataSourcesHandler,(LPARAM)this);
   SetWindowLong(hwndDataSourcesDialog,GWL_STYLE,(GetWindowLong(hwndDataSourcesDialog,GWL_STYLE) & ~ WS_CAPTION) & ~ WS_VISIBLE);

   hwndDataSourcesTab = GetDlgItem(hwndDataSourcesDialog,IDDI_DATASOURCES_TAB);

   hwndDataSourcesFunctions = GetDlgItem(hwndDataSourcesDialog,IDDI_DATASOURCES_FUNCTIONS_TAB);

   TC_ITEM tie;
   memset(&tie,0,sizeof(TC_ITEM));
   tie.mask = TCIF_TEXT;
   tie.pszText = "Functions";
   tie.cchTextMax = 18;
   TabCtrl_InsertItem(hwndDataSourcesTab, 1, &tie);

   put_PlotView(plotView);
   put_PlotType(plotType);

   IGSFunctioNater* pIFunction = (IGSFunctioNater*)NULL;

   while ( pIFunction = cachedFunctionList.GetFirst() ) {
      connectFunction(pIFunction);
      pIFunction -> put_ShowVariables(TRUE);
      pIFunction -> put_ShowExpression(TRUE);
      pIFunction -> put_ShowControls(TRUE);
      pIFunction -> put_AllowControlVisibilitySettings(FALSE);
      pIFunction -> put_ShowResults(TRUE);
      GetWindowRect(hwndFrame,&rect);
      SendMessage(hwndFrame,WM_SIZE,(WPARAM)SIZE_RESTORED,MAKELPARAM(rect.right - rect.left,rect.bottom - rect.top));
      pIFunction -> Release();
      cachedFunctionList.Remove(pIFunction);
   }

   if ( containedFunctionList.Count() > 0 ) {
      SendMessage(hwndDataSourcesFunctions,TCM_SETCURSEL,0L,0L);
      ContainedFunction* pContainedFunction = containedFunctionList.GetFirst();
      ShowWindow(pContainedFunction -> HWNDSite(),SW_SHOW);
   }

   pIOpenGLImplementation -> SetBaseWindow(hwndGraphic);

#if 0
   xaxis -> put_ParentWindow(hwndGraphic);
   yaxis -> put_ParentWindow(hwndGraphic);
   zaxis -> put_ParentWindow(hwndGraphic);
#endif

   long plotView = 0L;
   propertyPlotView -> get_longValue(&plotView);
   if ( zaxis && gcPlotView2D == plotView ) {
      IDataSet *pIDataSet;
      zaxis -> get_DataSet(&pIDataSet);
      pIDataSetMaster -> RemoveIncludedDomain(pIDataSet);
   }

   if ( pIViewSet )
      pIViewSet -> put_ParentWindow(hwndGraphic);

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

   xaxis -> Initialize(hwndGraphic,'X',xaxis,yaxis,zaxis,propertyPlotView,propertyPlotType,propertyFloor,propertyCeiling,propertyRenderOpenGLAxisText,pIDataSetMaster,pIOpenGLImplementation,pIEvaluator);
   yaxis -> Initialize(hwndGraphic,'Y',xaxis,yaxis,zaxis,propertyPlotView,propertyPlotType,propertyFloor,propertyCeiling,propertyRenderOpenGLAxisText,pIDataSetMaster,pIOpenGLImplementation,pIEvaluator);
   zaxis -> Initialize(hwndGraphic,'Z',xaxis,yaxis,zaxis,propertyPlotView,propertyPlotType,propertyFloor,propertyCeiling,propertyRenderOpenGLAxisText,pIDataSetMaster,pIOpenGLImplementation,pIEvaluator);

   pIViewSet -> Initialize(hwndGraphic,pIOpenGLImplementation,pIEvaluator,
                              propertyPlotView,
                              propertyViewTheta,propertyViewPhi,propertyViewSpin,propertyFloor,propertyCeiling,pIDataSetMaster,xaxis,yaxis,zaxis);

   return TRUE;
   }
