// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "plot.h"

#include "general.h"
#include "Graphic_resource.h"
#include "gmessage.h"

#include "utils.h"

#include "List.cpp"

   long Plot::instanceCount = 0L;

   std::list<CLSID *> Plot::plotTypeProviderGUIDs;

   std::map<gc2DPlotTypes,IGSystemPlotType *> Plot::plotType2DProviderInstances;
   std::map<gc2DPlotTypes,char *> Plot::plotType2DProviderNames;
   std::map<gc2DPlotTypes,long> Plot::plotType2DInstanceNumber;

   std::map<gc3DPlotTypes,IGSystemPlotType *> Plot::plotType3DProviderInstances;
   std::map<gc3DPlotTypes,char *> Plot::plotType3DProviderNames;
   std::map<gc3DPlotTypes,long> Plot::plotType3DInstanceNumber;

   HANDLE Plot::globalPlotThread = NULL;
   WNDPROC Plot::defaultPatchPainter = NULL;
   HWND Plot::hwndSampleGraphic = NULL;
   HWND Plot::hwndSampleGraphicSurface = NULL;

   Plot::Plot(IUnknown *pUnkOuter)  :
     refCount(0),
     currentPlotCount(0),
     pIUnknownOuter(pUnkOuter),
     pIViewObjectEx_IAdviseSink(NULL),
     pIProperties(NULL),
     pIPlotNotify(NULL),
     pIPlots(NULL),
     pIGSystemStatusBar(NULL),
     pIGSGraphicServices(NULL),

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
     hwndTextSettings(NULL),
     hMainMenu(NULL),
     hMenu(NULL),

     BasePlot() { 

   refCount = 100;

   instanceCount++;

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

   pIProperties -> Add(L"name",&propertyName);

   pIProperties -> Add(L"line color",&propertyLineColor);
   propertyLineColor -> put_type(TYPE_BINARY);

   pIProperties -> Add(L"line weight",&propertyLineWeight);
   propertyLineWeight -> put_type(TYPE_LONG);

   pIProperties -> Add(L"top surface color",&propertyTopSurfaceColor);
   propertyTopSurfaceColor -> put_type(TYPE_BINARY);
 
   pIProperties -> Add(L"bottom surface color",&propertyBottomSurfaceColor);
   propertyBottomSurfaceColor -> put_type(TYPE_BINARY);
 
   pIProperties -> Add(L"line end",&propertyLineEnd);
   pIProperties -> Add(L"line join",&propertyLineJoin);

   pIProperties -> Add(L"plot view",&propertyPlotView);
   pIProperties -> Add(L"plot 2D type",&property2DPlotType);
   pIProperties -> Add(L"plot 3D type",&property3DPlotType);

   property2DPlotType -> directAccess(TYPE_LONG,&plotType2D,sizeof(plotType2D));
   property3DPlotType -> directAccess(TYPE_LONG,&plotType3D,sizeof(plotType3D));
 
   pIProperties -> Add(L"custom colors",&propertyCustomColors);
 
   pIProperties -> Add(L"properties position",&propertyPropertiesPosition);
 
   pIProperties -> Add(L"override owner view",&propertyOverrideOwnerView);

   pIProperties -> Add(L"override owner type",&propertyOverrideOwnerType);

   pIProperties -> Add(L"plot number",&propertyPlotNumber);

   pIProperties -> DirectAccess(L"name",TYPE_STRING,&bstrName,sizeof(bstrName));
   pIProperties -> DirectAccess(L"override owner view",TYPE_BOOL,&overrideOwnerView,sizeof(overrideOwnerView));
   pIProperties -> DirectAccess(L"override owner type",TYPE_BOOL,&overrideOwnerType,sizeof(overrideOwnerType));

   pIProperties -> DirectAccess(L"plot view",TYPE_LONG,&plotView,sizeof(enum PlotView));

   pIProperties -> DirectAccess(L"properties position", TYPE_BINARY,&rectPropertiesPosition,sizeof(RECT));
   pIProperties -> DirectAccess(L"plot number",TYPE_LONG,&pID,sizeof(pID));

   pIProperties -> Add(L"data extents",&propertyDataExtents);
   propertyDataExtents -> put_type(TYPE_BINARY);
   propertyDataExtents -> put_size(128);

   pIProperties -> Add(L"dataset",&propertyDataSet);

   pIProperties -> Add(L"2D plot type instance identifiers",&propertyPlotTypes2DInstanceIdentifiers);

   pIProperties -> Add(L"3D plot type instance identifiers",&propertyPlotTypes3DInstanceIdentifiers);

   pIProperties -> Add(L"storage for plot types instances",&propertyPlotTypesStorage);

   pIProperties -> Add(L"text objects",&propertyTexts);

   propertyPlotTypesStorage -> put_type(TYPE_BINARY);

   refCount = 0;

   hMainMenu = LoadMenu((HINSTANCE)hModule,MAKEINTRESOURCE(IDMENU_PLOT));

   hMenu = GetSubMenu(hMainMenu,0);

   if ( 1 == instanceCount ) {

      pIProperties -> Add(L"plot type global defaults",&propertyPlotTypesGlobal);

      InitNew();

      WNDCLASS aClass;
 
      aClass.style = 0;
      aClass.lpfnWndProc = Plot::handler;
      aClass.cbClsExtra = 32;
      aClass.cbWndExtra = 32;
      aClass.hInstance = hModule;
      aClass.hIcon = NULL;
      aClass.hCursor = NULL;
      aClass.hbrBackground = 0;
      aClass.lpszMenuName = NULL;
      aClass.lpszClassName = "p-handler";
 
      RegisterClass(&aClass);
 
      aClass.lpfnWndProc = Plot::sampleGraphicHandler;
      aClass.cbClsExtra = 32;
      aClass.cbWndExtra = 32;
      aClass.hInstance = hModule;
      aClass.hIcon = NULL;
      aClass.hCursor = NULL;
      aClass.hbrBackground = 0;
      aClass.lpszMenuName = NULL;
      aClass.lpszClassName = "plot-sample-graphic";
 
      RegisterClass(&aClass);

      aClass.lpfnWndProc = Plot::sampleGraphicSurfaceHandler;
      aClass.lpszClassName = "plot-sample-graphic-surface";
 
      RegisterClass(&aClass);

      findKnownPlotTypeProviders();

   }

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

   --instanceCount;

   if ( 0 == instanceCount ) {
      for ( std::pair<gc2DPlotTypes,IGSystemPlotType *> p : plotType2DProviderInstances )
         if ( p.second )
            p.second -> Release();
      for ( std::pair<gc3DPlotTypes,IGSystemPlotType *> p : plotType3DProviderInstances )
         if ( p.second )
            p.second -> Release();
      for ( CLSID *p : plotTypeProviderGUIDs )
         delete p;
      for ( std::pair<gc2DPlotTypes,char *> p : plotType2DProviderNames )
         delete [] p.second;
      for ( std::pair<gc3DPlotTypes,char *> p : plotType3DProviderNames )
         delete [] p.second;

   }

   return;
   }  
 

   HANDLE Plot::PlotThread() { return plotThread; }


   long Plot::findKnownPlotTypeProviders() {

   ICatInformation *pICatInformation = NULL;

   CoCreateInstance(CLSID_StdComponentCategoriesMgr,NULL,CLSCTX_ALL,IID_ICatInformation,reinterpret_cast<void **>(&pICatInformation));

   IEnumCLSID *pIEnumCLSID = NULL;

   CATID catIds[] = { IID_IGSystemPlotType };

   pICatInformation -> EnumClassesOfCategories(1,catIds,0,NULL,&pIEnumCLSID);

   CATEGORYINFO catInfo;

   memset(&catInfo,0,sizeof(CATEGORYINFO));

   unsigned long countFound;
   CLSID clsid;

   long totalCount = 0;

   long external2DPlotType = (long)gcPlotType2DExternal1;
   long external3DPlotType = (long)gcPlotType3DExternal1;

   while ( S_OK == pIEnumCLSID -> Next(1,&clsid,&countFound) ) {

      if ( ! countFound )
         break;

      totalCount++;

      IGSystemPlotType *pIGSystemPlotType = NULL;

      HRESULT rc = CoCreateInstance(clsid,NULL,CLSCTX_INPROC_SERVER,IID_IGSystemPlotType,reinterpret_cast<void **>(&pIGSystemPlotType));

      if ( ! ( S_OK == rc ) )
         continue;

      long countProvided = 0L;

      pIGSystemPlotType -> get_Count(&countProvided);

      for ( long itemIndex = 0; itemIndex < countProvided; itemIndex++ ) {

         VARIANT_BOOL is3DOnly = VARIANT_FALSE;

         pIGSystemPlotType -> get_Is3DOnly(itemIndex + 1,&is3DOnly);

         CLSID *pKeep = new CLSID(clsid);

         plotTypeProviderGUIDs.push_back(pKeep);

         BSTR bstrName;

         pIGSystemPlotType -> get_Name(itemIndex + 1,&bstrName);

         long n = (DWORD)wcslen(bstrName) + 1;

         char *pszName = new char[n];

         WideCharToMultiByte(CP_ACP,0,bstrName,-1,pszName,n,0,0);

         SysFreeString(bstrName);

         if ( VARIANT_TRUE == is3DOnly ) {
 
            plotType3DProviderNames[(gc3DPlotTypes)external3DPlotType] = pszName;

            plotType3DProviderInstances[(gc3DPlotTypes)external3DPlotType] = pIGSystemPlotType;

            plotType3DInstanceNumber[(gc3DPlotTypes)external3DPlotType] = itemIndex + 1;

            external3DPlotType = external3DPlotType << 1;
 
         } else {

            plotType2DProviderNames[(gc2DPlotTypes)external2DPlotType] = pszName;

            plotType2DProviderInstances[(gc2DPlotTypes)external2DPlotType] = pIGSystemPlotType;

            plotType2DInstanceNumber[(gc2DPlotTypes)external2DPlotType] = itemIndex + 1;

            external2DPlotType = external2DPlotType << 1;

         }

      }

   }

   return totalCount;
   }


   IText *Plot::newText() {

   IText *pIText;

   CoCreateInstance(CLSID_Text,NULL,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IText,reinterpret_cast<void **>(&pIText));

   pIText -> put_PartOfWorldDomain(FALSE);

   pIText -> AdviseGSGraphicServices(pIGSGraphicServices);

   pIText -> Initialize(pIOpenGLImplementation,pIEvaluator,
                           pIDataSetDomain,
                           pPropertyXFloor,pPropertyXCeiling,
                           pPropertyYFloor,pPropertyYCeiling,
                           pPropertyZFloor,pPropertyZCeiling,
                           NULL,NULL,pWhenChangedCallback,pWhenChangedCallbackArg,(ULONG_PTR)whenChangedCallbackCookie);

   DataPoint dpMin,dpMax;

   pIDataSetDomain -> GetDomain(&dpMin,&dpMax);

   dpMin.x = (dpMin.x + dpMax.x) / 2.0;
   dpMin.y = (dpMin.y + dpMax.y) / 2.0;
   dpMin.z = (dpMin.z + dpMax.z) / 2.0;

   pIText -> put_PositionX(dpMin.x);
   pIText -> put_PositionY(dpMin.y);
   pIText -> put_PositionZ(dpMin.z);

   pIText -> AdviseGSystemStatusBar(pIGSystemStatusBar);

   textList.Add(pIText);

   return pIText;
   }


   void Plot::deleteText(IText *pIText) {

   pIText -> Release();

   textList.Remove(pIText);

   return;
   }