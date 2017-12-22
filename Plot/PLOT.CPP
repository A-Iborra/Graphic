/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <commctrl.h>
#include <process.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "general.h"
#include "Graphic_resource.h"
#include "gmessage.h"

#include "utils.h"

#include "plot.h"

#include "List.cpp"

  HANDLE Plot::globalPlotThread = 0;
  WNDPROC Plot::defaultPatchPainter = 0;
  WNDPROC Plot::defaultStaticWindowHandler = 0;

  int Plot::pCount = 0;

  Plot::Plot(IUnknown *pUnkOuter)  :
     refCount(0),
     currentPlotCount(0),
     pIUnknownOuter(pUnkOuter),
     pIViewObjectEx_IAdviseSink(NULL),
     pIProperties(NULL),
     pIPlotNotify(NULL),
     pIPlots(NULL),
     pIGSystemStatusBar(NULL),

     pOwnerProperty2DPlotType(NULL),
     pOwnerProperty3DPlotType(NULL),
     pOwnerPropertyPlotView(NULL),
     pOwnerPropertyBackgroundColor(NULL),

     propertyLineColor(NULL),
     propertyLineWeight(NULL),
     adviseSink_dwAspect(0),
     adviseSink_advf(0),

     autoViewDetection(false),
     haveAnyData(false),
     okToPlot(true),
     selected(false),
     defaultSolidSizeInPercentDomain(10.0),

     overrideOwnerView(false),
     overrideOwnerType(false),

     bstrName(NULL),

     plotThread(0),
     pID(-1),
     baselineDataSet(NULL),

     hwndParentWindow(NULL),
     hwndDimensionSettings(NULL),
     hwndObjectWindow(NULL),
     hwndTypeSettings(NULL),
     hwndColorSettings(NULL),
     hMainMenu(NULL),
     hMenu(NULL),

     BasePlot() { 

   refCount = 100;

   memset(&rectPropertiesPosition,0,sizeof(RECT));

   IUnknown *pIUnknownThis;

   QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknownThis));
 
   CoCreateInstance(CLSID_InnoVisioNateProperties,pIUnknownThis,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IUnknown,reinterpret_cast<void **>(&pIUnknownProperties));
 
   pIUnknownProperties -> QueryInterface(IID_IGProperties,reinterpret_cast<void**>(&pIProperties));
 
   pIUnknownThis -> Release();
 
   IGPropertyPageClient* pIPropertyPageClient;
   QueryInterface(IID_IGPropertyPageClient,reinterpret_cast<void**>(&pIPropertyPageClient));
   pIProperties -> AdvisePropertyPageClient(pIPropertyPageClient,true);
   pIPropertyPageClient -> Release();

   pIProperties -> put_DebuggingEnabled(TRUE);

   IGPropertiesClient *pIPropertiesClient;
   QueryInterface(IID_IGPropertiesClient,reinterpret_cast<void **>(&pIPropertiesClient));
   pIProperties -> Advise(pIPropertiesClient);
   pIPropertiesClient -> Release();

   messageTable.size = 1;
   messageTable.start = new MessageTableEntry[messageTable.size];
   messageTable.start -> messageNumber = (unsigned int)-1;
   messageTable.start -> messageInterface = NULL;
   messageTable.start -> hwndTarget = 0L;

   pIProperties -> Add(L"name",&propertyName);

   pIProperties -> Add(L"line color",&propertyLineColor);
   pIProperties -> Add(L"line weight",&propertyLineWeight);
   propertyLineWeight -> put_type(TYPE_LONG);

   pIProperties -> Add(L"top surface color",&propertyTopSurfaceColor);
 
   pIProperties -> Add(L"bottom surface color",&propertyBottomSurfaceColor);
 
   pIProperties -> Add(L"line end",&propertyLineEnd);
   pIProperties -> Add(L"line join",&propertyLineJoin);

   pIProperties -> Add(L"plot view",&propertyPlotView);
   pIProperties -> Add(L"plot type",&propertyPlotType);
 
   pIProperties -> Add(L"custom colors",&propertyCustomColors);
 
   pIProperties -> Add(L"properties position",&propertyPropertiesPosition);
 
   pIProperties -> Add(L"override owner view",&propertyOverrideOwnerView);

   pIProperties -> Add(L"override owner type",&propertyOverrideOwnerType);

   pIProperties -> Add(L"plot number",&propertyPlotNumber);

   pIProperties -> DirectAccess(L"name",TYPE_STRING,&bstrName,sizeof(bstrName));
   pIProperties -> DirectAccess(L"override owner view",TYPE_BOOL,&overrideOwnerView,sizeof(overrideOwnerView));
   pIProperties -> DirectAccess(L"override owner type",TYPE_BOOL,&overrideOwnerType,sizeof(overrideOwnerType));

   pIProperties -> DirectAccess(L"plot view",TYPE_LONG,&plotView,sizeof(enum PlotView));

   pIProperties -> DirectAccess(L"plot type",TYPE_LONG,&plotType,sizeof(enum PlotType));
   pIProperties -> DirectAccess(L"properties position", TYPE_BINARY,&rectPropertiesPosition,sizeof(RECT));
   pIProperties -> DirectAccess(L"plot number",TYPE_LONG,&pID,sizeof(pID));

   pIProperties -> Add(L"data extents",&propertyDataExtents);
   propertyDataExtents -> put_type(TYPE_BINARY);
   propertyDataExtents -> put_size(128);

   pIProperties -> Add(L"Default solid size in percent domain",&propertyDefaultSolidSizeInPercentDomain);
   pIProperties -> DirectAccess(L"Default solid size in percent domain",TYPE_DOUBLE,&defaultSolidSizeInPercentDomain,sizeof(defaultSolidSizeInPercentDomain));

   refCount = 0;

   WNDCLASS aClass;
 
   aClass.style = 0;
   aClass.lpfnWndProc = handler;
   aClass.cbClsExtra = 32;
   aClass.cbWndExtra = 32;
   aClass.hInstance = hModule;
   aClass.hIcon = NULL;
   aClass.hCursor = NULL;
   aClass.hbrBackground = 0;
   aClass.lpszMenuName = NULL;
   aClass.lpszClassName = "p-handler";
 
   RegisterClass(&aClass);
 
   hMainMenu = LoadMenu((HINSTANCE)hModule,MAKEINTRESOURCE(IDMENU_PLOT));

   hMenu = GetSubMenu(hMainMenu,0);

   return;
   }


   Plot::~Plot() {

   if ( baselineDataSet ) 
      baselineDataSet -> Release();

   if ( pIProperties )
      pIProperties -> Release();

   DestroyWindow(hwndObjectWindow);

   if ( hwndDimensionSettings)
      DestroyWindow(hwndDimensionSettings);

   if ( hwndColorSettings)
      DestroyWindow(hwndColorSettings);

   if ( bstrName ) 
      SysFreeString(bstrName);

   DestroyMenu(hMainMenu);

   return;
   }  
 
   HANDLE Plot::PlotThread() { return plotThread; }


