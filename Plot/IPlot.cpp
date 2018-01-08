/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002,2008 Nathan T. Clark

*/

#include "Plot.h"

#include "Utils.h"
 
   STDMETHODIMP Plot::Initialize(IDataSet* pIDataSetDomain,
                                 IOpenGLImplementation *pimp,
                                 IEvaluator *piev,
                                 IGProperty* pPropColor,
                                 IGProperty* pPropLineWeight,
                                 IGProperty* parentPlotView,
                                 IGProperty* parentDefault2DPlotType,
                                 IGProperty* parentDefault3DPlotType,
                                 IGProperty *parentBackground,
                                 IGProperty *parentFloor,
                                 IGProperty *parentCeiling,
                                 void (__stdcall *pCallback)(void *),void *pCallbarkArg) {
 
   if ( pPropColor ) {
      if ( propertyLineColor ) {
         pIProperties -> Remove(L"line color");
         propertyLineColor -> Release();
      }
      propertyLineColor = pPropColor;
   }

   if ( pPropLineWeight ) {
      if ( propertyLineWeight ) {
         pIProperties -> Remove(L"line weight");
         propertyLineWeight -> Release();
      }
      propertyLineWeight = pPropLineWeight;
   }

   pWhenChangedCallback = pCallback;
   pWhenChangedCallbackArg = pCallbarkArg;

   BasePlot::Initialize(pIDataSetDomain,pimp,piev,propertyLineColor,propertyLineWeight,parentFloor,parentCeiling);

   InitNew();

   pOwnerPropertyPlotView = parentPlotView;
   if ( ! pOwnerPropertyPlotView )
      overrideOwnerView = true;

   pOwnerProperty2DPlotType = parentDefault2DPlotType;

   pOwnerProperty3DPlotType = parentDefault3DPlotType;

   if ( ! pOwnerProperty2DPlotType && ! pOwnerProperty3DPlotType )
      overrideOwnerType = true;

   pOwnerPropertyBackgroundColor = parentBackground;

   put_ActionTable(static_cast<IGraphicSegmentAction*>(this));

   return S_OK;
   }


   HRESULT Plot::AddText(void *vt,char *name,int id) {
   return BasePlot::AddText(vt,name,id);
   }
 
 
   HRESULT Plot::RemoveText(void *vt) {
   return BasePlot::RemoveText(vt);
   }
 
 
   HRESULT Plot::put_Name(BSTR newName) {
   return propertyName -> put_stringValue(newName);
   }

   HRESULT Plot::get_Name(BSTR *pName) {
   return propertyName -> get_stringValue(pName);
   }

   HRESULT Plot::get_OkToPlot(long *setOkToPlot) {
   *setOkToPlot = okToPlot;
   return S_OK;
   }
   HRESULT Plot::put_OkToPlot(long setOkToPlot) {
   okToPlot = setOkToPlot;
   return S_OK;
   }
 
 
   HRESULT Plot::get_SegmentID(long* newID) {
   return BasePlot::get_SegmentID(newID);
   }


   HRESULT Plot::put_ActionTable(IGraphicSegmentAction *newActionTable) {
   return BasePlot::put_ActionTable(newActionTable);
   }
   HRESULT Plot::get_ActionTable(IGraphicSegmentAction **getActionTable) {
   return BasePlot::get_ActionTable(getActionTable);
   }

 
   HRESULT Plot::get_AutoPlotViewDetection(long *setAutoViewDetection) {
   *setAutoViewDetection = autoViewDetection;
   return S_OK;
   }
   HRESULT Plot::put_AutoPlotViewDetection(long setAutoViewDetection) {
   autoViewDetection = setAutoViewDetection;
   return S_OK;
   }
 

   HRESULT Plot::put_OverrideOwnerPlotView(VARIANT_BOOL doOverride) {
   propertyOverrideOwnerView -> put_boolValue(doOverride);
   return S_OK;
   }

   HRESULT Plot::get_OverrideOwnerPlotView(VARIANT_BOOL *pdoOverride) {
   if ( ! pdoOverride ) return E_POINTER;
   propertyOverrideOwnerView -> get_boolValue(pdoOverride);
   return S_OK;
   }


   HRESULT Plot::put_OverrideOwnerPlotType(VARIANT_BOOL doOverride) {
   propertyOverrideOwnerType -> put_boolValue(doOverride);
   return S_OK;
   }

   HRESULT Plot::get_OverrideOwnerPlotType(VARIANT_BOOL *pdoOverride) {
   if ( ! pdoOverride ) return E_POINTER;
   propertyOverrideOwnerType -> get_boolValue(pdoOverride);
   return S_OK;
   }


   HRESULT Plot::put_PlotViewProperty(IGProperty *pProp) {
   pOwnerPropertyPlotView = pProp;
   return S_OK;
   }
   
   HRESULT Plot::get_PlotViewProperty(IGProperty **pProp) {
   if ( ! pProp ) return E_POINTER;
   *pProp = pOwnerPropertyPlotView;
   return S_OK;
   }


   HRESULT Plot::put_PlotTypeProperty(IGProperty *pProp) {
   pOwnerProperty2DPlotType = pProp;
   return S_OK;
   }
   
   HRESULT Plot::get_PlotTypeProperty(IGProperty **pProp) {
   if ( ! pProp ) return E_POINTER;
   *pProp = pOwnerProperty2DPlotType;
   return S_OK;
   }


/*
   enum PlotTypes {	
      gcPlotTypeNone =      0x00000000,
      gcPlotTypeNatural =   0x00010003,
      gcPlotTypeSurface =   0x00020002,
      gcPlotTypeWireFrame = 0x00040002,
      gcPlotTypeStacks =    0x00080003,
      gcPlotTypeBlocks =    0x00100003,
      gcPlotTypeBalls =     0x00200003,
      gcPlotTypePie =       0x00400001,
      gcPlotTypeContour =   0x00800003,
      gcPlotTypeQuads =     0x01000003,
      gcPlotTypeTriangles = 0x01200003
      };
*/

   HRESULT Plot::get_PlotTypeHasSurfaces(enum gc3DPlotTypes theType,VARIANT_BOOL *pResult) {

   if ( ! pResult )
      return E_POINTER;

   *pResult = VARIANT_FALSE;
#if 1
*pResult = VARIANT_TRUE;
Beep(2000,100);
#else
   long pType = (long)plotType & gcPlotViewMask;

   if ( (pType & gcPlotTypeSurface) || (pType & gcPlotTypeStacks) ||
         (pType & gcPlotTypeBlocks) || (pType & gcPlotTypeBalls) )
      *pResult = VARIANT_TRUE;
#endif
   return S_OK;
   }


   HRESULT Plot::put_IDataSet(IDataSet *inDataSet) { 
   if ( propertyDataSet ) {
      propertyDataSet -> Release();
      pIProperties -> Remove(L"dataset");
      propertyDataSet = NULL;
   }
   return BasePlot::put_IDataSet(inDataSet); 
   }

   HRESULT Plot::get_IDataSet(IDataSet **getDataSet) { return BasePlot::get_IDataSet(getDataSet); }
 
   enum DataArity __stdcall Plot::DataArity() { return BasePlot::DataArity(); }
 
   HRESULT Plot::put_PlotNotify(IPlotNotify *setIPlotNotify) {
   if ( pIPlotNotify )
      pIPlotNotify -> Release();
   pIPlotNotify = setIPlotNotify;
   if ( pIPlotNotify )
      pIPlotNotify -> AddRef();
   return S_OK;
   }
   HRESULT Plot::get_PlotNotify(IPlotNotify **getIPlotNotify) {
   *getIPlotNotify = pIPlotNotify;
   return S_OK;
   }


   HRESULT Plot::put_Color(SAFEARRAY *pColor) {
   DataPoint dpColor;
   GetDataPointSafeArray(pColor,&dpColor);
   float fvColor[] = {(float)dpColor.x,(float)dpColor.y,(float)dpColor.z,0.0};
   if ( propertyLineColor )
      propertyLineColor -> put_binaryValue(sizeof(fvColor),(BYTE*)fvColor);
   return S_OK;
   }

   HRESULT Plot::get_Color(SAFEARRAY **ppColor) {
   float fvColor[4];
   propertyLineColor -> get_binaryValue(sizeof(fvColor),(BYTE**)&fvColor);
   DataPoint dpColor = {(double)fvColor[0],(double)fvColor[1],(double)fvColor[2]};
   return PutDataPointSafeArray(ppColor,&dpColor);
   }



   HRESULT Plot::put_ColorProperty(IGProperty* pColor) {
   if ( ! pColor ) 
      return BasePlot::put_ColorProperty(pColor);
   propertyLineColor = pColor;
   return BasePlot::put_ColorProperty(pColor);
   }

   HRESULT Plot::get_ColorProperty(IGProperty** ppColor) {
   return BasePlot::get_ColorProperty(ppColor);
   }


   HRESULT Plot::put_LineWeight(IGProperty* pLineWeight) {
   propertyLineWeight = pLineWeight;
   return BasePlot::put_LineWeight(pLineWeight);
   }

   HRESULT Plot::get_LineWeight(IGProperty** ppLineWeight) {
   return BasePlot::get_LineWeight(ppLineWeight);
   }


   HRESULT Plot::put_ParentWindow(HWND hwndParent) {
   hwndParentWindow = hwndParent;
   return S_OK;
   }


   HRESULT Plot::get_SegmentCount(long *pCount) {
   return BasePlot::get_SegmentCount(pCount);
   }


   HRESULT Plot::GetSegments(long *pSegmentArray) {
   return BasePlot::GetSegments(pSegmentArray);
   }


   HRESULT Plot::StubPlot() {
   return BasePlot::StubPlot();
   }
 
 
   HRESULT Plot::Draw() {
   pIPlots = reinterpret_cast<IPlot**>(CoTaskMemAlloc(sizeof(IPlot*)));
   QueryInterface(IID_IPlot,reinterpret_cast<void **>(pIPlots));
   currentPlotCount = 1;
   plotter((void*)this);
   return S_OK;
   }
 
 
   HRESULT Plot::Redraw() { 
   return BasePlot::Redraw();
   }
 
 
   HRESULT Plot::Erase() {
   return BasePlot::Erase();
   }


   HRESULT Plot::Plotter(long cntPlots,IPlot*** thePlotList) {
   currentPlotCount = cntPlots;
   pIPlots = *thePlotList;
   plotter((void*)this);
   return S_OK;
   }
 
 
   HRESULT Plot::PlotList(long cntPlots,IPlot*** thePlotList,long *theHandle) {
   currentPlotCount = cntPlots;
   pIPlots = *thePlotList;
   unsigned int threadAddr;
  *theHandle = (long)_beginthreadex(NULL,0,plotter,(void*)this,0L,&threadAddr);
   return S_OK;
   }
 

   STDMETHODIMP Plot::EditProperties() {
   IUnknown *pUnknown;
   QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pUnknown));
   pIProperties -> ShowProperties(NULL,pUnknown);
   pUnknown -> Release();
   if ( pWhenChangedCallback )
      pWhenChangedCallback(pWhenChangedCallbackArg);
   return S_OK;
   }


   STDMETHODIMP Plot::AdviseGSystemStatusBar(IGSystemStatusBar* p) {
   if ( ! p ) {
      if ( ! pIGSystemStatusBar ) return E_UNEXPECTED;
      pIGSystemStatusBar = NULL;
   }
   pIGSystemStatusBar = p;
   return S_OK;
   }


   STDMETHODIMP Plot::AdviseGSGraphicServices(void *pvIGSGraphicServices) {
   if ( ! pvIGSGraphicServices ) {
      if ( ! pIGSGraphicServices ) return E_UNEXPECTED;
      pIGSGraphicServices = NULL;
   }
   pIGSGraphicServices = (IGSGraphicServices *)pvIGSGraphicServices;
   return S_OK;
   }
