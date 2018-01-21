// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

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
                                 IGProperty *parentXFloor,IGProperty *parentXCeiling,
                                 IGProperty *parentYFloor,IGProperty *parentYCeiling,
                                 IGProperty *parentZFloor,IGProperty *parentZCeiling,
                                 void (__stdcall *pCallback)(void *,ULONG_PTR),void *pCallbarkArg,ULONG_PTR callbackCookie) {
 
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
   whenChangedCallbackCookie = callbackCookie;

   BasePlot::Initialize(pIDataSetDomain,pimp,piev,propertyLineColor,propertyLineWeight,parentXFloor,parentXCeiling,parentYFloor,parentYCeiling,parentZFloor,parentZCeiling);

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


   HRESULT Plot::get_PlotTypeUsesMaterialShading(long the2DTypes,long the3DTypes,VARIANT_BOOL *pResult) {

   if ( ! pResult )
      return E_POINTER;

   IGSystemPlotType *pProvider = NULL;

   for ( std::pair<gc3DPlotTypes,IGSystemPlotType *> pair : plotType3DProviderInstances ) {

      if ( NULL == pair.second )
         continue;

      if ( ! ( pair.first & the3DTypes ) )
         continue;

      long item = plotType3DInstanceNumber[pair.first];

      pair.second -> get_UsesMaterialShading(item,pResult);

      if ( VARIANT_TRUE == *pResult )
         return S_OK;

   }

   for ( std::pair<gc2DPlotTypes,IGSystemPlotType *> pair : plotType2DProviderInstances ) {

      if ( NULL == pair.second )
         continue;

      if ( ! ( pair.first & the2DTypes ) )
         continue;

      long item = plotType2DInstanceNumber[pair.first];

      pair.second -> get_UsesMaterialShading(item,pResult);

      if ( VARIANT_TRUE == *pResult )
         return S_OK;

   }

   *pResult = VARIANT_FALSE;

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



   HRESULT Plot::put_LineColorProperty(IGProperty* pColor) {
   if ( ! pColor ) 
      return BasePlot::put_ColorProperty(pColor);
   propertyLineColor = pColor;
   return BasePlot::put_ColorProperty(pColor);
   }

   HRESULT Plot::get_LineColorProperty(IGProperty** ppProperty) {
   return BasePlot::get_ColorProperty(ppProperty);
   }


   HRESULT Plot::put_LineWeightProperty(IGProperty* pProperty) {
   propertyLineWeight = pProperty;
   return BasePlot::put_LineWeight(pProperty);
   }

   HRESULT Plot::get_LineWeightProperty(IGProperty** ppProperty) {
   if ( ! ppProperty )
      return E_POINTER;
   return BasePlot::get_LineWeight(ppProperty);
   }


   HRESULT Plot::put_TopSurfaceColorProperty(IGProperty* pProperty) {
   propertyTopSurfaceColor = pProperty;
   return S_OK;
   }

   HRESULT Plot::get_TopSurfaceColorProperty(IGProperty** ppProperty) {
   if ( ! ppProperty )
      return E_POINTER;
   *ppProperty = propertyTopSurfaceColor;
   return S_OK;
   }

   HRESULT Plot::put_BottomSurfaceColorProperty(IGProperty* pProperty) {
   propertyBottomSurfaceColor = pProperty;
   return S_OK;
   }

   HRESULT Plot::get_BottomSurfaceColorProperty(IGProperty** ppProperty) {
   if ( ! ppProperty )
      return E_POINTER;
   *ppProperty = propertyBottomSurfaceColor;
   return S_OK;
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
      pWhenChangedCallback(pWhenChangedCallbackArg,whenChangedCallbackCookie);
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
