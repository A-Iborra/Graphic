// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"

#include "Graphic_resource.h"

#include "utils.h"
#include "ContainedFunction.h"

#include "List.cpp"

   WNDPROC G::defaultStatusBarHandler = NULL;
   WNDPROC G::defaultPatchPainter = NULL;
   HWND G::hwndSampleGraphic = NULL;
   HWND G::hwndSampleGraphicSurface = NULL;

   G::G(IUnknown *pUnkOuter) :

      plotList(),
      functionList(),

      dataSetList(),
      
      renderThreadList(),
      graphicCursorList(),

      refCount(0),
 
      pIUnknownOuter(pUnkOuter),
 
      pIGProperties(NULL),
      pIOpenGLImplementation(NULL),
      pIEvaluator(NULL),

      pIOleClientSite(NULL),
      pIOleInPlaceSite(NULL),
      pAdviseSink(NULL),
      pOleAdviseHolder(NULL),

      pSelectedGraphicSegmentAction(NULL),
      pIGSystemStatusBar(NULL),

      pDataAdviseHolder(NULL),
 
      pIDataSetMaster(NULL),

      pPlot_IClassFactory(NULL),
      pFunction_IClassFactory(NULL),
      pDataSet_IClassFactory(NULL),

      pIPlotServicesObject(NULL),

      pIViewSet(NULL),

      hwndFrame(NULL),
      hwndCanvas(NULL),
      hwndOwner(NULL),
      hwndStatusBar(NULL),
      hMenuPlot(NULL),
      hwndAppearanceSettings(NULL),
      hwndStyleSettings(NULL),
      hwndTextSettings(NULL),
      hwndLightingSettings(NULL),
      hwndBackgroundSettings(NULL),
      hwndPlotSettings(NULL),
      hwndFunctionSettings(NULL),
      hwndAxisSettings(NULL),
      hwndDataSourcesDialog(NULL),
      hwndDataSourcesTab(NULL),
      hwndDataSourcesFunctions(NULL),
      hwndDataSourcesDataSets(NULL),

      hitTable(NULL),
      hitTableHits(0),
      rendering(FALSE),
      eraseMode(FALSE),
      windowStorageBytes(128),
      windowStyle(0),
      windowFrameFlags(0),

      autoClear(TRUE),
      autoPlotViewDetection(TRUE),
      showStatusBar(TRUE),
      useGraphicsCursor(TRUE),
      denyUserPropertySettings(FALSE),
      allowUserFunctionControlVisibilityAccess(FALSE),
      isRunning(FALSE),
      isInitializing(TRUE),

      showMargins(FALSE),
      showFunctions(FALSE),
      stretchToMargins(FALSE),

      xaxis(0),
      yaxis(0),
      zaxis(0),

      plotView(gcPlotView2D),

      defaultPlotView(gcPlotView2D),
      default2DPlotType(gcPlotType2DExternal1),
      default3DPlotType(gcPlotType3DNone),

      dataSetCount(0),
      textCount(0),
      functionCount(0),
      currentPlotSourceID(0),

      supportedLightCount(0),
      lightPositions(NULL),

      adviseSink_dwAspect(0),
      adviseSink_advf(0),

      pIGSGraphicEvents(NULL),
      pIConnectionPointViewSet(NULL),
      dwViewSetEventSourceCookie(0),

      xPixelsPerUnit(0),
      yPixelsPerUnit(0),

      zLevel(1.0),
      floorX(0.0),
      ceilingX(0.0),
      floorY(0.0),
      ceilingY(0.0),
      floorZ(0.0),
      ceilingZ(1.0)
  {

   statusTextWidth[0] = -1;
   statusTextWidth[1] = -1;

   pIPropertiesClient = new _IGPropertiesClient(this);
   pIPropertyPageClient = new _IGPropertyPageClient(this);

   memset(pIPropertyPage,0,sizeof(pIPropertyPage));

   refCount = 100;
 
   containerSize.cx = PLOT_DEFAULT_WIDTH;
   containerSize.cy = PLOT_DEFAULT_HEIGHT;
 
   memset(szName,0,sizeof(szName));

   memset(windowTitle,0,sizeof(windowTitle));

   memset(&rightMouseClickPosition,0,sizeof(rightMouseClickPosition));

   memset(&rectStatusText,0,sizeof(rectStatusText));

   memset(plotMarginUnits,0,sizeof(plotMarginUnits));

   margins.left = PLOT_MARGIN_LEFT;
   margins.top = PLOT_MARGIN_TOP;
   margins.right = PLOT_MARGIN_RIGHT;
   margins.bottom = PLOT_MARGIN_BOTTOM;

   pickBoxSize.cx = pickBoxSize.cy = PICK_WINDOW_SIZE;
 
   memset(&ptlZoomAnchor,0,sizeof(POINT));
   memset(&ptlZoomFloat,0,sizeof(POINT));
   memset(&ptlLastMouse,0,sizeof(POINT));
   ptlPickBox.x = -1L;
   ptlPickBox.y = -1L;

   trackedMouse = FALSE;

   QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknownThis));
 
   QueryInterface(IID_IGSystemStatusBar,reinterpret_cast<void**>(&pIGSystemStatusBar));

   pIGSystemStatusBar -> Release();

   long rc = CoCreateInstance(CLSID_InnoVisioNateProperties,NULL,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IGProperties,reinterpret_cast<void **>(&pIGProperties));
 
   rc = CoGetClassObject(CLSID_Plot,CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER,NULL,IID_IClassFactory,reinterpret_cast<void **>(&pPlot_IClassFactory));

   pPlot_IClassFactory -> CreateInstance(pIUnknownOuter,IID_IPlotServices,reinterpret_cast<void **>(&pIPlotServicesObject));

   rc = CoGetClassObject(CLSID_DataSet,CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER,NULL,IID_IClassFactory,reinterpret_cast<void **>(&pDataSet_IClassFactory));

   rc = CoGetClassObject(CLSID_GSystemFunctioNater,CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER,NULL,IID_IClassFactory,reinterpret_cast<void **>(&pFunction_IClassFactory));

   rc = CoCreateInstance(CLSID_DataSet,pIUnknownOuter,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IDataSet,reinterpret_cast<void **>(&pIDataSetMaster));

   rc = CoCreateInstance(CLSID_OpenGLImplementor,pIUnknownThis,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IOpenGLImplementation,reinterpret_cast<void **>(&pIOpenGLImplementation));

   rc = CoCreateInstance(CLSID_Evaluator,pIUnknownThis,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IEvaluator,reinterpret_cast<void **>(&pIEvaluator));

   pIOpenGLImplementation -> Initialize(pIEvaluator);

   pIOpenGLImplementation -> AdviseGSystemStatusBar(pIGSystemStatusBar);

   pIGProperties -> Advise(pIPropertiesClient);

   pIGProperties -> AdvisePropertyPageClient(pIPropertyPageClient,true);

   pIGProperties -> put_DebuggingEnabled(true);

   pIDataSetMaster -> ReSet();
 
   pIGProperties -> Add(L"auto clear",&propertyAutoClear);

   pIGProperties -> Add(L"plot view",&propertyPlotView);
   propertyPlotView -> directAccess(TYPE_LONG,&defaultPlotView,sizeof(long));

   pIGProperties -> Add(L"auto plotView detection",&propertyAutoPlotViewDetection);

   pIGProperties -> Add(L"default 2d plot type",&propertyDefault2DPlotType);
   pIGProperties -> Add(L"default 3d plot type",&propertyDefault3DPlotType);

   propertyDefault2DPlotType -> directAccess(TYPE_LONG,&default2DPlotType,sizeof(long));
   propertyDefault3DPlotType -> directAccess(TYPE_LONG,&default3DPlotType,sizeof(long));

   pIGProperties -> Add(L"plot width",NULL);
   pIGProperties -> Add(L"plot height",NULL);

   pIGProperties -> Add(L"plot margin units",&propertyPlotMarginUnits);
   pIGProperties -> Add(L"plot left margin",&propertyPlotLeftMargin);
   pIGProperties -> Add(L"plot right margin",&propertyPlotRightMargin);
   pIGProperties -> Add(L"plot top margin",&propertyPlotTopMargin);
   pIGProperties -> Add(L"plot bottom margin",&propertyPlotBottomMargin);
   pIGProperties -> Add(L"plot margins",&propertyPlotMargins);
   pIGProperties -> Add(L"plot margins stretch all",&propertyPlotMarginsStretchAll);
                                                                                                                                                                                    
   pIGProperties -> Add(L"text count",NULL);
   pIGProperties -> Add(L"function count",NULL);
                                                                                                                                                                                    
   pIGProperties -> Add(L"floor x",&propertyXFloor);
   pIGProperties -> Add(L"ceiling x",&propertyXCeiling);
   pIGProperties -> Add(L"floor y",&propertyYFloor);
   pIGProperties -> Add(L"ceiling y",&propertyYCeiling);
   pIGProperties -> Add(L"floor z",&propertyZFloor);
   pIGProperties -> Add(L"ceiling z",&propertyZCeiling);

   propertyXFloor -> put_type(TYPE_DOUBLE);
   propertyXCeiling -> put_type(TYPE_DOUBLE);

   propertyYFloor -> put_type(TYPE_DOUBLE);
   propertyYCeiling -> put_type(TYPE_DOUBLE);

   propertyZFloor -> put_type(TYPE_DOUBLE);
   propertyZCeiling-> put_type(TYPE_DOUBLE);

   IGProperty* pTemp;
   pIGProperties -> Add(L"pick box size",&pTemp);
   pTemp -> put_type(TYPE_BINARY);
   pTemp -> put_size(sizeof(SIZEL));

   pIGProperties -> Add(L"print properties",&propertyPrintProperties);
   propertyPrintProperties -> put_type(TYPE_BINARY);
   propertyPrintProperties -> put_size(sizeof(PRINTDLG));

   pIGProperties -> Add(L"data extents",&propertyDataExtents);
   propertyDataExtents -> put_type(TYPE_BINARY);
   propertyDataExtents -> put_size(128);

   pIGProperties -> Add(L"background color",&propertyBackgroundColor);
   propertyBackgroundColor -> put_type(TYPE_BINARY);
   propertyBackgroundColor -> put_size(4 * sizeof(float));

   pIGProperties -> Add(L"text color",&propertyTextColor);
   propertyTextColor -> put_type(TYPE_BINARY);
   propertyTextColor -> put_size(4 * sizeof(float));

   pIGProperties -> Add(L"text background color",&propertyTextBackgroundColor);
   propertyTextBackgroundColor -> put_type(TYPE_BINARY);
   propertyTextBackgroundColor -> put_size(4 * sizeof(float));

   pIGProperties -> Add(L"custom colors",&propertyCustomColors);
   propertyCustomColors -> put_type(TYPE_BINARY);
   propertyCustomColors -> put_size(128);

   pIGProperties -> Add(L"view phi",&propertyViewPhi);
   pIGProperties -> Add(L"view theta",&propertyViewTheta);
   pIGProperties -> Add(L"view spin",&propertyViewSpin);

   pIGProperties -> Add(L"deny user property settings",&propertyDenyUserPropertySettings);

   pIGProperties -> Add(L"light count",&propertyCountLights);

   pIGProperties -> Add(L"specular reference",&propertySpecularReference);
   propertySpecularReference -> put_type(TYPE_BINARY);
   propertySpecularReference -> put_size(4 * sizeof(float));

   pIGProperties -> Add(L"ambient reference",&propertyAmbientReference);
   propertyAmbientReference -> put_type(TYPE_BINARY);
   propertyAmbientReference -> put_size(4 * sizeof(float));

   pIGProperties -> Add(L"shinyness",&propertyShinyness);

   pIOpenGLImplementation -> get_SupportedLightCount(&supportedLightCount);

   ppPropertyLightOn = new IGProperty*[supportedLightCount];
   ppPropertyAmbientLight = new IGProperty*[supportedLightCount];
   ppPropertyDiffuseLight = new IGProperty*[supportedLightCount];
   ppPropertySpecularLight = new IGProperty*[supportedLightCount];
   ppPropertyLightPos = new IGProperty*[supportedLightCount];

   lightPositions = new char**[supportedLightCount];
   memset(lightPositions,0,supportedLightCount * sizeof(char**));

   WCHAR szwTemp[64];

   for ( long k = 0; k < supportedLightCount; k ++ ) {

      swprintf(szwTemp,L"light %ld enabled",k);  
      pIGProperties -> Add(szwTemp,&ppPropertyLightOn[k]);

      swprintf(szwTemp,L"ambient light %ld",k);  
      pIGProperties -> Add(szwTemp,&ppPropertyAmbientLight[k]);

      swprintf(szwTemp,L"diffuse light %ld",k);  
      pIGProperties -> Add(szwTemp,&ppPropertyDiffuseLight[k]);

      swprintf(szwTemp,L"specular light %ld",k); 
      pIGProperties -> Add(szwTemp,&ppPropertySpecularLight[k]);

      swprintf(szwTemp,L"light position %ld",k); 
      pIGProperties -> Add(szwTemp,&ppPropertyLightPos[k]);

   }

   pIGProperties -> Add(L"functions",&propertyFunctions);
   pIGProperties -> Add(L"datasets",&propertyDataSets);
   pIGProperties -> Add(L"show functions",&propertyShowFunctions);
   pIGProperties -> Add(L"allow user ability to access function control visibility",&propertyAllowUserFunctionControlVisibilityAccess);
   pIGProperties -> Add(L"texts",&propertyTexts);
   pIGProperties -> Add(L"axiis",&propertyAxiis);
   propertyAxiis -> put_type(TYPE_OBJECT_STORAGE_ARRAY);

   pIGProperties -> Add(L"show status bar",&propertyShowStatusBar);
   pIGProperties -> Add(L"status text",&propertyStatusText);
   propertyStatusText -> put_type(TYPE_SZSTRING);
   propertyStatusText -> put_szValue("");
   pIGProperties -> Add(L"show margins",&propertyShowMargins);
   
   pIGProperties -> Add(L"use graphics cursor",&propertyUseGraphicsCursor);

   pIGProperties -> DirectAccess(L"auto clear",TYPE_BOOL,&autoClear,sizeof(autoClear));
   pIGProperties -> DirectAccess(L"plot view",TYPE_LONG,&plotView,sizeof(plotView));

   pIGProperties -> DirectAccess(L"plot width",TYPE_LONG,&containerSize.cx,sizeof(containerSize.cx));
   pIGProperties -> DirectAccess(L"plot height",TYPE_LONG,&containerSize.cy,sizeof(containerSize.cy));
   pIGProperties -> DirectAccess(L"dataset count",TYPE_LONG,&dataSetCount,sizeof(dataSetCount));
   pIGProperties -> DirectAccess(L"text count",TYPE_LONG,&textCount,sizeof(textCount));
   pIGProperties -> DirectAccess(L"function count",TYPE_LONG,&functionCount,sizeof(functionCount));
   pIGProperties -> DirectAccess(L"pick box size",TYPE_BINARY,&pickBoxSize,sizeof(pickBoxSize));

   pIGProperties -> DirectAccess(L"floor x",TYPE_DOUBLE,&floorX,sizeof(floorX));
   pIGProperties -> DirectAccess(L"ceiling x",TYPE_DOUBLE,&ceilingX,sizeof(ceilingX));

   pIGProperties -> DirectAccess(L"floor y",TYPE_DOUBLE,&floorY,sizeof(floorY));
   pIGProperties -> DirectAccess(L"ceiling y",TYPE_DOUBLE,&ceilingY,sizeof(ceilingY));

   pIGProperties -> DirectAccess(L"floor z",TYPE_DOUBLE,&floorZ,sizeof(floorZ));
   pIGProperties -> DirectAccess(L"ceiling z",TYPE_DOUBLE,&ceilingZ,sizeof(ceilingZ));

   pIGProperties -> DirectAccess(L"show functions",TYPE_BOOL,&showFunctions,sizeof(showFunctions));

   propertyDenyUserPropertySettings -> directAccess(TYPE_BOOL,&denyUserPropertySettings,sizeof(denyUserPropertySettings));
   propertyAllowUserFunctionControlVisibilityAccess -> directAccess(TYPE_BOOL,&allowUserFunctionControlVisibilityAccess,sizeof(allowUserFunctionControlVisibilityAccess));

   propertyShowStatusBar -> directAccess(TYPE_BOOL,&showStatusBar,sizeof(showStatusBar));
   propertyAutoPlotViewDetection -> directAccess(TYPE_BOOL,&autoPlotViewDetection,sizeof(autoPlotViewDetection));
   propertyUseGraphicsCursor -> directAccess(TYPE_BOOL,&useGraphicsCursor,sizeof(useGraphicsCursor));
   propertyShowMargins -> directAccess(TYPE_BOOL,&showMargins,sizeof(showMargins));

   propertyPlotMarginUnits -> directAccess(TYPE_SZSTRING,plotMarginUnits,sizeof(plotMarginUnits));
   propertyPlotLeftMargin -> directAccess(TYPE_LONG,&margins.left,sizeof(margins.left));
   propertyPlotTopMargin -> directAccess(TYPE_LONG,&margins.top,sizeof(margins.top));
   propertyPlotRightMargin -> directAccess(TYPE_LONG,&margins.right,sizeof(margins.right));
   propertyPlotBottomMargin -> directAccess(TYPE_LONG,&margins.bottom,sizeof(margins.bottom));
   propertyPlotMargins -> directAccess(TYPE_BINARY,&margins,sizeof(RECT));
   propertyPlotMarginsStretchAll -> directAccess(TYPE_BOOL,&stretchToMargins,sizeof(BOOL));

   rc = CoCreateInstance(CLSID_ViewSet,pIUnknownOuter,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IViewSet,reinterpret_cast<void **>(&pIViewSet));

   IUnknown* pIViewSet_Unknown;
   pIViewSet -> QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIViewSet_Unknown));
   pIGProperties -> AddPropertyPage(pIViewSet_Unknown);
   pIViewSet_Unknown -> Release();

   pIGProperties -> Add(L"plot types persistence storage",&propertyPlotServicesObject);

   pIPropertiesClient -> InitNew();

   pIPropertyPage[0] = new _IPropertyPage(this,CLSID_GSystemGraphicPropertiesPosSize);
   pIPropertyPage[1] = new _IPropertyPage(this,CLSID_GSystemGraphicPropertiesStyle);
   pIPropertyPage[2] = new _IPropertyPage(this,CLSID_GSystemGraphicPropertiesBackground);
   pIPropertyPage[3] = new _IPropertyPage(this,CLSID_GSystemGraphicPropertiesText);
   pIPropertyPage[4] = new _IPropertyPage(this,CLSID_GSystemGraphicPropertiesLighting);
   pIPropertyPage[5] = new _IPropertyPage(this,CLSID_GSystemGraphicPropertiesAxis);
   pIPropertyPage[6] = new _IPropertyPage(this,CLSID_GSystemGraphicPropertiesPlot);
   pIPropertyPage[7] = new _IPropertyPage(this,CLSID_GSystemGraphicPropertiesDataSets);
   pIPropertyPage[8] = new _IPropertyPage(this,CLSID_GSystemGraphicPropertiesFunctions);

   pIGSGraphicEvents = new _IGSGraphicEvents(this);

   refCount = 0;

   IConnectionPointContainer *pIConnectionPointContainer = NULL;

   rc = pIViewSet -> QueryInterface(IID_IConnectionPointContainer,reinterpret_cast<void **>(&pIConnectionPointContainer));

   if ( ! ( S_OK == rc ) )
      return;

   rc = pIConnectionPointContainer -> FindConnectionPoint(IID_IGSGraphicEvents,&pIConnectionPointViewSet);

   if ( ! ( S_OK == rc ) ) {
      pIConnectionPointContainer -> Release();
      return;
   }

   refCount = 100;

   IUnknown *pIUnknownThis = NULL;

   QueryInterface(IID_IUnknown,reinterpret_cast<void **>(&pIUnknownThis));

   pIConnectionPointViewSet -> Advise(pIUnknownThis,&dwViewSetEventSourceCookie);

   pIUnknownThis -> Release();

   refCount = 0;

   return;
   }
 
 
   G::~G() {
 
   stop();

   delete [] ppPropertyLightOn;
   delete [] ppPropertyAmbientLight;
   delete [] ppPropertyDiffuseLight;
   delete [] ppPropertySpecularLight;
   delete [] ppPropertyLightPos;

   pIPlotServicesObject -> Release();

   IPlot *p;
   while ( p = plotList.GetFirst() ) { 
      p -> Release();
      plotList.Remove(p); 
   }

   IGSFunctioNater *pf;
   while ( pf = functionList.GetFirst() ) { 
      ContainedFunction* pContainedFunction = containedFunctionList.Get((ULONG_PTR)pf);
      if ( pContainedFunction ) {
         IOleObject *pIOleObject;
         pf -> QueryInterface(IID_IOleObject,reinterpret_cast<void**>(&pIOleObject));
         pIOleObject -> SetClientSite(NULL);
         pf -> Release();
         containedFunctionList.Remove(pContainedFunction);
         delete pContainedFunction;
      }
      pf -> Release();
      functionList.Remove(pf); 
   }

   IDataSet *pds;
   while ( pds = dataSetList.GetFirst() ) { 
      ContainedDataSet* pContainedDataSet = containedDataSetList.Get((ULONG_PTR)pds);
      if ( pContainedDataSet ) {
         IOleObject *pIOleObject;
         pf -> QueryInterface(IID_IOleObject,reinterpret_cast<void**>(&pIOleObject));
         pIOleObject -> SetClientSite(NULL);
         pf -> Release();
         containedDataSetList.Remove(pContainedDataSet);
         delete pContainedDataSet;
      }
      pds -> Release();
      dataSetList.Remove(pds); 
   }

   IText *t;
   while ( t = textList.GetFirst() ) {
      t -> Release();
      textList.Remove(t);
   }
   textList.Delete();
 
   if ( pIViewSet ) {
      pIConnectionPointViewSet -> Unadvise(dwViewSetEventSourceCookie);
      pIViewSet -> Release();
   }

   if ( xaxis ) xaxis -> Release();
   if ( yaxis ) yaxis -> Release();
   if ( zaxis ) zaxis -> Release();

   pIDataSetMaster -> Release();

   if ( pIOpenGLImplementation ) {
      pIOpenGLImplementation -> UnSetViewProperties();
      pIOpenGLImplementation -> Stop();      
      pIOpenGLImplementation -> Release();
   }
 
   SetParent(hwndFrame,NULL);
   DestroyWindow(hwndFrame);

   pIGProperties -> Release();

   pIPropertiesClient -> Release();

   delete pIPropertiesClient;
   delete pIPropertyPageClient;

   pIPropertiesClient = NULL;

   if ( pIOleInPlaceSite ) 
      pIOleInPlaceSite -> Release();

// The following statement causes too early of a release (there is an additional AddRef() somewhere )
//??   if ( pIOleClientSite ) 
//??      pIOleClientSite -> Release();

   pIOleInPlaceSite = NULL;
   pIOleClientSite = NULL;

   DestroyMenu(hMenuPlot);

   delete pIPropertyPage[0];
   delete pIPropertyPage[1];
   delete pIPropertyPage[2];
   delete pIPropertyPage[3];
   delete pIPropertyPage[4];
   delete pIPropertyPage[5];
   delete pIPropertyPage[6];
   delete pIPropertyPage[7];
   delete pIPropertyPage[8];

   return;
   }
 
 
   void G::changeType() {

Beep(2000,100);
#if 0
   MENUITEMINFO mi;
   memset(&mi,0,sizeof(mi));
   mi.cbSize = sizeof(mi);
   mi.fMask = MIIM_STATE;
   mi.fState = MFS_UNCHECKED;

   SetMenuItemInfo(hwndMenuPlot(),IDMI_GRAPHIC_VIEW_2D,MF_BYCOMMAND,&mi);
   SetMenuItemInfo(hwndMenuPlot(),IDDI_GRAPHIC_SUB_STYLE_SURFACE,MF_BYCOMMAND,&mi);
   SetMenuItemInfo(hwndMenuPlot(),IDDI_GRAPHIC_SUB_STYLE_WIREFRAME,MF_BYCOMMAND,&mi);
   SetMenuItemInfo(hwndMenuPlot(),IDDI_GRAPHIC_SUB_STYLE_NATURAL,MF_BYCOMMAND,&mi);
   SetMenuItemInfo(hwndMenuPlot(),IDDI_GRAPHIC_SUB_STYLE_STACKS,MF_BYCOMMAND,&mi);

   mi.fState = MFS_CHECKED;

   switch ( plotView ) {

   case gcPlotView2D:
      SetMenuItemInfo(hwndMenuPlot(),IDMI_GRAPHIC_VIEW_2D,MF_BYCOMMAND,&mi);
      break;
 
   case gcPlotView3D:

      SetMenuItemInfo(hwndMenuPlot(),IDMI_GRAPHIC_VIEW_3D,MF_BYCOMMAND,&mi);

      switch ( default2DPlotType ) {
      case gcPlotTypeSurface:
         SetMenuItemInfo(hwndMenuPlot(),IDDI_GRAPHIC_SUB_STYLE_SURFACE,MF_BYCOMMAND,&mi);
         break;
 
      case gcPlotTypeWireFrame:
         SetMenuItemInfo(hwndMenuPlot(),IDDI_GRAPHIC_SUB_STYLE_WIREFRAME,MF_BYCOMMAND,&mi);
         break;
 
      case gcPlotTypeNatural:
         SetMenuItemInfo(hwndMenuPlot(),IDDI_GRAPHIC_SUB_STYLE_NATURAL,MF_BYCOMMAND,&mi);
         break;
 
      case gcPlotTypeStacks:
         SetMenuItemInfo(hwndMenuPlot(),IDDI_GRAPHIC_SUB_STYLE_STACKS,MF_BYCOMMAND,&mi);
         break;
 
      }
 
      break;
   }
#endif
   return;
   }
 

   IDataSet * G::newDataSet(bool connectNow) {

   long dataSetID = (long)PlotIdBands::plotIdDataSets + dataSetList.Count() + 1;

   IDataSet *pIDataSet = (IDataSet *)NULL;

   pDataSet_IClassFactory -> CreateInstance(pIUnknownOuter,IID_IDataSet,reinterpret_cast<void **>(&pIDataSet));

   pIDataSet -> Initialize((void *)pIDataSetMaster,(void *)pIOpenGLImplementation,pIEvaluator,NULL,NULL,
                              propertyPlotView,
                              propertyDefault2DPlotType,
                              propertyDefault3DPlotType,
                              propertyBackgroundColor,
                              propertyXFloor,propertyXCeiling,
                              propertyYFloor,propertyYCeiling,
                              propertyZFloor,propertyZCeiling,
                              someObjectChanged,(void *)this,(ULONG_PTR)pIDataSet);

   IPlot *pIPlot = NULL;

   pIDataSet -> get_IPlot((void **)&pIPlot);

   pIPlot -> put_OkToPlot(0);

   plotList.Add(pIPlot,NULL,dataSetID);

   dataSetList.Add(pIDataSet,NULL,dataSetID);

   pIDataSet -> AdviseGSystemStatusBar(pIGSystemStatusBar);

   IGSGraphicServices *pIGSGraphicServices = NULL;

   QueryInterface(IID_IGSGraphicServices,reinterpret_cast<void **>(&pIGSGraphicServices));

   pIDataSet -> AdviseGSGraphicServices(reinterpret_cast<void *>(pIGSGraphicServices));

   pIGSGraphicServices -> Release();

   if ( connectNow ) 
      connectDataSet(pIDataSet);

   return pIDataSet;
   }
   
   void G::connectDataSet(IDataSet *pIDataSet) {

   IUnknown* pIUnknownDataSet;
   pIDataSet -> QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknownDataSet));
   ContainedDataSet *pContainedDataSet = new ContainedDataSet(this,dataSetList.ID(pIDataSet),hwndDataSourcesDataSets,pIDataSet,pIUnknownDataSet,IID_IDataSetEvents);
   pIUnknownDataSet -> Release();

   pIDataSet -> put_OnChangeCallback(someObjectChanged,(void *)this,(ULONG_PTR)pIDataSet);

   IOleObject* pIOleObject;
   IOleClientSite* pIOleClientSite;

   pIDataSet -> QueryInterface(IID_IOleObject,reinterpret_cast<void**>(&pIOleObject));

   pContainedDataSet -> QueryInterface(IID_IOleClientSite,reinterpret_cast<void**>(&pIOleClientSite));

   pIOleObject -> SetClientSite(pIOleClientSite);

   pContainedDataSet -> connectEvents();

   containedDataSetList.Add(pContainedDataSet,NULL,(ULONG_PTR)pIDataSet);

   pIOleObject -> Release();

   pIOleClientSite -> Release();

   return;
   } 


   void G::deleteDataSet(IDataSet *pIDataSet) {

   IOleObject* pIOleObject;
   pIDataSet -> QueryInterface(IID_IOleObject,reinterpret_cast<void**>(&pIOleObject));
   pIOleObject -> SetClientSite(NULL);
   pIOleObject -> Release();

   IPlot *pIPlot = NULL;

   pIDataSet -> get_IPlot((void **)&pIPlot);

   if ( pIPlot )
      plotList.Remove(pIPlot);

   ContainedDataSet *pContainedDataSet = containedDataSetList.Get((ULONG_PTR)pIDataSet);

   containedDataSetList.Remove(pContainedDataSet);

   delete pContainedDataSet;

   dataSetList.Remove(pIDataSet);

   setDataSourcesVisibility(NULL,NULL);

   pIDataSet -> Release();

   return;
   }


   IGSFunctioNater* G::newFunction(bool connectNow) {

   ULONG_PTR functionID = (ULONG_PTR)PlotIdBands::plotIdFunctions + (ULONG_PTR)(functionList.Count() + 1);

   IGSFunctioNater *pIFunction = (IGSFunctioNater*)NULL;

   pFunction_IClassFactory -> CreateInstance(pIUnknownOuter,IID_IGSFunctioNater,reinterpret_cast<void **>(&pIFunction));

   pIFunction -> Initialize(pIDataSetMaster,pIOpenGLImplementation,NULL,NULL,
                              propertyPlotView,
                              propertyDefault2DPlotType,
                              propertyDefault3DPlotType,
                              propertyBackgroundColor,
                              propertyXFloor,propertyXCeiling,
                              propertyYFloor,propertyYCeiling,
                              propertyZFloor,propertyZCeiling,
                              someObjectChanged,(void *)this,(ULONG_PTR)pIFunction);
 
   IPlot *pIPlot = NULL;

   pIFunction -> get_IPlot((void **)&pIPlot);

   pIPlot -> put_OkToPlot(0);

   plotList.Add(pIPlot,NULL,functionID);

   pIFunction -> AdviseGSystemStatusBar(pIGSystemStatusBar);

   IGSGraphicServices *pIGSGraphicServices = NULL;

   QueryInterface(IID_IGSGraphicServices,reinterpret_cast<void **>(&pIGSGraphicServices));

   pIFunction -> AdviseGSGraphicServices(reinterpret_cast<void *>(pIGSGraphicServices));

   pIGSGraphicServices -> Release();

   functionList.Add(pIFunction,NULL,functionID);

   if ( connectNow )
      connectFunction(pIFunction);

   return pIFunction;
   }


   void G::connectFunction(IGSFunctioNater *pIFunction) {

   IUnknown* pIUnknownFunction;
   pIFunction -> QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknownFunction));
   ContainedFunction *pContainedFunction = new ContainedFunction(this,functionList.ID(pIFunction),hwndDataSourcesFunctions,pIFunction,pIUnknownFunction,DIID_IGSFunctioNaterEvents);
   pIUnknownFunction -> Release();

   pIFunction -> put_AllowPropertySettings(VARIANT_TRUE);

   pIFunction -> put_AllowControlVisibilitySettings(allowUserFunctionControlVisibilityAccess);

   pIFunction -> put_OnChangeCallback(someObjectChanged,(void *)this,(ULONG_PTR)pIFunction);

   IOleObject* pIOleObject;
   IOleClientSite* pIOleClientSite;

   pIFunction -> QueryInterface(IID_IOleObject,reinterpret_cast<void**>(&pIOleObject));

   pContainedFunction -> QueryInterface(IID_IOleClientSite,reinterpret_cast<void**>(&pIOleClientSite));

   pIOleObject -> SetClientSite(pIOleClientSite);

   pContainedFunction -> connectEvents();

   containedFunctionList.Add(pContainedFunction,NULL,(ULONG_PTR)pIFunction);

   pIOleObject -> Release();

   pIOleClientSite -> Release();

   return;
   }


   void G::deleteFunction(IGSFunctioNater *pIFunction) {

   IOleObject* pIOleObject;
   pIFunction -> QueryInterface(IID_IOleObject,reinterpret_cast<void**>(&pIOleObject));
   pIOleObject -> SetClientSite(NULL);
   pIOleObject -> Release();

   ContainedFunction* pContainedFunction = containedFunctionList.Get((ULONG_PTR)pIFunction);

   containedFunctionList.Remove(pContainedFunction);

   IPlot *pIPlot = NULL;

   pIFunction -> get_IPlot((void **)&pIPlot);

   functionList.Remove(pIFunction);

   plotList.Remove(pIPlot);

   pIFunction -> Release();

   delete pContainedFunction;

   setDataSourcesVisibility(NULL,NULL);

   return;
   }


   IText* G::newText() {

   IText* pIText;

   CoCreateInstance(CLSID_Text,pIUnknownOuter,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IText,reinterpret_cast<void **>(&pIText));

// Possible Release() problem !!!

   ITextNotify *pITextNotify;
   QueryInterface(IID_ITextNotify,reinterpret_cast<void **>(&pITextNotify));
   pIText -> put_TextNotify(pITextNotify);
   pITextNotify -> Release();

   pIText -> put_PartOfWorldDomain(FALSE);

   pIText -> Initialize(pIOpenGLImplementation,pIEvaluator,pIDataSetMaster,
                           propertyXFloor,propertyXCeiling,
                           propertyYFloor,propertyYCeiling,
                           propertyZFloor,propertyZCeiling,
                           NULL,NULL,someObjectChanged,(void *)this,(ULONG_PTR)pIText);

   pIText -> AdviseGSystemStatusBar(pIGSystemStatusBar);

   IGSGraphicServices *pIGSGraphicServices = NULL;

   QueryInterface(IID_IGSGraphicServices,reinterpret_cast<void **>(&pIGSGraphicServices));

   pIText -> AdviseGSGraphicServices(reinterpret_cast<void *>(pIGSGraphicServices));

   pIGSGraphicServices -> Release();

   textList.Add(pIText);

   return pIText;
   }


   void G::deleteText(IText *pIText) {

   pIText -> Release();

   textList.Remove(pIText);

   return;
   }

 
   IPlot* G::newPlot(ULONG_PTR plotID) {

   IPlot* pIPlot;

   pPlot_IClassFactory -> CreateInstance(pIUnknownOuter,IID_IPlot,reinterpret_cast<void **>(&pIPlot));

   pIPlot -> Initialize(pIDataSetMaster,pIOpenGLImplementation,pIEvaluator,NULL,NULL,
                           propertyPlotView,
                           propertyDefault2DPlotType,
                           propertyDefault3DPlotType,
                           propertyBackgroundColor,
                           propertyXFloor,propertyXCeiling,
                           propertyYFloor,propertyYCeiling,
                           propertyZFloor,propertyZCeiling,
                           someObjectChanged,(void *)this,(ULONG_PTR)pIPlot);
 
   IPlotNotify *pIPlotNotify;
   QueryInterface(IID_IPlotNotify,reinterpret_cast<void **>(&pIPlotNotify));
   pIPlot -> put_PlotNotify(pIPlotNotify);
   pIPlotNotify -> Release();
   pIPlotNotify -> Release();

   IAdviseSink *pIAdviseSink;
   IViewObjectEx *pIViewObjectEx;

   QueryInterface(IID_IAdviseSink,reinterpret_cast<void **>(&pIAdviseSink));
   pIPlot -> QueryInterface(IID_IViewObjectEx,reinterpret_cast<void **>(&pIViewObjectEx));
   pIViewObjectEx -> SetAdvise(0,0,pIAdviseSink);

   pIViewObjectEx -> Release();

   pIAdviseSink -> Release();

   plotList.Add(pIPlot,NULL,plotID);
 
   pIPlot -> AdviseGSystemStatusBar(pIGSystemStatusBar);

   pIPlot -> put_ParentWindow(Canvas());

   pIPlot -> put_PlotViewProperty(propertyPlotView);

   pIPlot -> put_PlotTypeProperty(propertyDefault2DPlotType);

   IGSGraphicServices *pIGSGraphicServices = NULL;

   QueryInterface(IID_IGSGraphicServices,reinterpret_cast<void **>(&pIGSGraphicServices));

   pIPlot -> AdviseGSGraphicServices(reinterpret_cast<void *>(pIGSGraphicServices));

   pIGSGraphicServices -> Release();

   return pIPlot;
   }


   void G::clearData() {
   IPlot* pIPlot = (IPlot *)NULL;
   while ( pIPlot = plotList.GetNext(pIPlot) )
      pIPlot -> ResetData();
   pIDataSetMaster -> ReSet();
   }


   int G::statusPosition() {
   POINT ptMouse;
   double minZ,maxZ;
   pIDataSetMaster -> get_minZ(&minZ);
   pIDataSetMaster -> get_maxZ(&maxZ);
   GetCursorPos(&ptMouse);
   DataPoint dpNewPosition,dpMouse = {(double)ptMouse.x,(double)ptMouse.y,1.0};
   pIOpenGLImplementation -> WindowToData(&dpMouse,&dpNewPosition);
   char szText[128];
   sprintf(szText,"(%lf,%lf,%lf)",dpNewPosition.x,dpNewPosition.y,dpNewPosition.z);
   if ( pIGSystemStatusBar )
      pIGSystemStatusBar -> put_StatusText(1,szText);
   return 0;
   }


   int G::eraseBackground() {

   float fv[4];
   BYTE *pb = (BYTE *)fv;

   propertyBackgroundColor -> get_binaryValue(sizeof(fv),(BYTE **)&pb);

   BYTE vb[3];
   vb[0] = (BYTE)(255.0f*fv[0]);
   vb[1] = (BYTE)(255.0f*fv[1]);
   vb[2] = (BYTE)(255.0f*fv[2]);

   HDC hdc = pIOpenGLImplementation -> TargetDC();

   HBRUSH hb = CreateSolidBrush(RGB(vb[0],vb[1],vb[2]));

   RECT rc = {0};
   GetClientRect(pIOpenGLImplementation -> TargetHWND(),&rc);

   SelectObject(hdc,hb);

   FillRect(hdc,&rc,hb);

   DeleteObject(hb);

   return 0;
   }


   int G::getSegments(long **pSegments) {

   *pSegments = NULL;

   long cntSegments = 0;
   long k;

   IAxis *pIAxis = NULL;
   while ( pIAxis = axisList.GetNext(pIAxis) ) {
      pIAxis -> get_SegmentCount(&k);
      cntSegments += k;
   }

   IPlot *pIPlot = NULL;
   while ( pIPlot = visiblePlotList.GetNext(pIPlot) ) {
      pIPlot -> get_SegmentCount(&k);
      cntSegments += k;
   }

   //IText *pIText = NULL;
   //while ( pIText = textList.GetNext(pIText) ) {
   //   pIText -> get_SegmentCount(&k);
   //   cntSegments += k;
   //}

   if ( ! cntSegments ) 
      return 0;

   *pSegments = new long[cntSegments + 1];
   memset(*pSegments,0,(cntSegments + 1) * sizeof(long));
   long *pLong = *pSegments;

   k = 0;
   while ( pIAxis = axisList.GetNext(pIAxis) ) {
      pIAxis -> get_SegmentCount(&k);
      pIAxis -> GetSegments(pLong);
      pLong += k;
   }

   while ( pIPlot = visiblePlotList.GetNext(pIPlot) ) {
      pIPlot -> get_SegmentCount(&k);
      pIPlot -> GetSegments(pLong);
      pLong += k;
   }

   //while ( pIText = textList.GetNext(pIText) ) {
   //   pIText -> get_SegmentCount(&k);
   //   pIText -> GetSegments(pLong);
   //   pLong += k;
   //}

   return cntSegments;
   }

   int G::getSegmentsExcludingAxiis(long **pSegments) {

   *pSegments = NULL;

   long cntSegments = 0;
   long k;

   IPlot *pIPlot = NULL;
   while ( pIPlot = visiblePlotList.GetNext(pIPlot) ) {
      pIPlot -> get_SegmentCount(&k);
      cntSegments += k;
   }

   //IText *pIText = NULL;
   //while ( pIText = textList.GetNext(pIText) ) {
   //   pIText -> get_SegmentCount(&k);
   //   cntSegments += k;
   //}

   if ( ! cntSegments ) 
      return 0;

   *pSegments = new long[cntSegments + 1];
   memset(*pSegments,0,(cntSegments + 1) * sizeof(long));
   long *pLong = *pSegments;

   k = 0;

   while ( pIPlot = visiblePlotList.GetNext(pIPlot) ) {
      pIPlot -> get_SegmentCount(&k);
      pIPlot -> GetSegments(pLong);
      pLong += k;
   }

   //while ( pIText = textList.GetNext(pIText) ) {
   //   pIText -> get_SegmentCount(&k);
   //   pIText -> GetSegments(pLong);
   //   pLong += k;
   //}

   return cntSegments;
   }


   void G::someObjectChanged(void *pArg,ULONG_PTR cookie) {

   G *p = (G *)pArg;

   p -> pIOpenGLImplementation -> SetTargetWindow(p -> Canvas());

   ULONG_PTR k = p -> plotList.ID((IPlot *)cookie);

   if ( 0 < k ) {
      p -> render(k);
      return;
   }

   k = p -> dataSetList.ID((IDataSet *)cookie);

   if ( 0 < k ) {

      IDataSet *pIDataSet = (IDataSet *)cookie;

      IPlot *pIPlot = NULL;

      pIDataSet -> get_IPlot((void **)&pIPlot);

      if ( ! pIPlot )
         return;

      p -> render(p -> plotList.ID(pIPlot));

      return;
   }

   k = p -> functionList.ID((IGSFunctioNater *)cookie);

   if ( 0 < k ) {

      IGSFunctioNater *pIFunction = (IGSFunctioNater *)cookie;

      IPlot *pIPlot = NULL;

      pIFunction -> get_IPlot((void **)&pIPlot);

      if ( ! pIPlot )
         return;

      p -> render(p -> plotList.ID(pIPlot));

      return;
   }

   p -> setDataSourcesVisibility(NULL,NULL);

   p -> render(0);

   return;
   }

   void G::menuHandlerSomeObjectChanged(void *pArg) {
   G *p = (G *)pArg;
   p -> pIOpenGLImplementation -> SetTargetWindow(p -> Canvas());
   p -> render(0);
   return;
   }

   void G::styleHandlerSomeObjectChanged(void *pArg) {
   G *p = (G *)pArg;
   if ( ! ( NULL == p -> hwndSampleGraphic ) ) {
      p -> pIOpenGLImplementation -> SetTargetWindow(p -> hwndSampleGraphic);
      InvalidateRect(p -> hwndSampleGraphic,NULL,TRUE);
   }
   EnableWindow(p -> hwndStyleSettings,TRUE);
   return;
   }