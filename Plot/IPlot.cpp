/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002,2008 Nathan T. Clark

*/

#include <windows.h>
#include <process.h>

#include "Utils.h"
#include "Plot.h"

#include "List.cpp"

 
   STDMETHODIMP Plot::Initialize(IDataSet* pIDataSetDomain,
                                 IOpenGLImplementation *pimp,
                                 IEvaluator *piev,
                                 IGProperty* pPropColor,
                                 IGProperty* pPropLineWeight,
                                 IGProperty* parentPlotView,
                                 IGProperty* parentPlotType,
                                 IGProperty *parentBackground,
                                 IGProperty *parentTheta,
                                 IGProperty *parentPhi,
                                 IGProperty *parentSpin,
                                 IGProperty *parentFloor,
                                 IGProperty *parentCeiling,
                                 IGProperty *pCountLights,
                                 IGProperty **ppLightingOn,
                                 IGProperty **ppAmbientLight,
                                 IGProperty **ppDiffuseLight,
                                 IGProperty **ppSpecularLight,
                                 IGProperty **ppLightPos) {
 
   if ( pPropColor ) {
      if ( propertyLineColor ) {
         pIProperties -> Remove(L"line color");
         propertyLineColor -> Release();
      }
      propertyLineColor = pPropColor;
      pIProperties -> Include(propertyLineColor);
   }

   if ( pPropLineWeight ) {
      if ( propertyLineWeight ) {
         pIProperties -> Remove(L"line weight");
         propertyLineWeight -> Release();
      }
      propertyLineWeight = pPropLineWeight;
   }

   pOwnerPropertyPlotView = parentPlotView;
   if ( ! pOwnerPropertyPlotView )
      overrideOwnerView = true;

   pOwnerPropertyPlotType = parentPlotType;

   if ( ! pOwnerPropertyPlotType )
      overrideOwnerType = true;

   pOwnerPropertyBackgroundColor = parentBackground;
   pOwnerPropertyPhi = parentPhi;
   pOwnerPropertyTheta = parentTheta;
   pOwnerPropertySpin = parentSpin;

   pOwnerPropertyCountLights = pCountLights;
   pOwnerPropertiesLightOn = ppLightingOn;
   pOwnerPropertiesAmbientLight = ppAmbientLight;
   pOwnerPropertiesDiffuseLight = ppDiffuseLight;
   pOwnerPropertiesSpecularLight = ppSpecularLight;
   pOwnerPropertiesLightPosition = ppLightPos;

   BasePlot::Initialize(pIDataSetDomain,pimp,piev,propertyLineColor,propertyLineWeight,parentFloor,parentCeiling);

   put_ActionTable(static_cast<IGraphicSegmentAction*>(this));
 
   InitNew();
 
   return S_OK;
   }


   HRESULT Plot::AddText(void *vt,unsigned char *name,int id) {
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
   pOwnerPropertyPlotType = pProp;
   return S_OK;
   }
   
   HRESULT Plot::get_PlotTypeProperty(IGProperty **pProp) {
   if ( ! pProp ) return E_POINTER;
   *pProp = pOwnerPropertyPlotType;
   return S_OK;
   }


   HRESULT Plot::put_DataSet(IDataSet *inDataSet) { return BasePlot::put_DataSet(inDataSet); }

   HRESULT Plot::get_DataSet(IDataSet **getDataSet) { return BasePlot::get_DataSet(getDataSet); }
 
 
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
   return S_OK;
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
   pIProperties -> EditProperties(NULL,L"Plot",pUnknown);
   pUnknown -> Release();
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


