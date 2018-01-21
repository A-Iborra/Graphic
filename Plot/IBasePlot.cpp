// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "plot.h"

#include "general.h"
#include "Graphic_resource.h"

#include "utils.h"

#include "list.cpp"


   DataPoint movePoint = {-DBL_MAX,-DBL_MAX,-DBL_MAX};


   HRESULT BasePlot::Initialize(IDataSet* pDS,IOpenGLImplementation *pimp,IEvaluator *piev,
                                 IGProperty* propLineColor,
                                 IGProperty* propLineWeight,
                                 IGProperty* propXFloor,IGProperty* propXCeiling,
                                 IGProperty* propYFloor,IGProperty* propYCeiling,
                                 IGProperty* propZFloor,IGProperty* propZCeiling) {
 
   if ( pIDataSetDomain )
      pIDataSetDomain -> Release();

   pIDataSetDomain = pDS;
   if ( pIDataSetDomain )
      pIDataSetDomain -> AddRef();

   pIOpenGLImplementation = pimp;

   pIEvaluator = piev;

   pPropertyXFloor = propXFloor;
   pPropertyXCeiling = propXCeiling;

   pPropertyYFloor = propYFloor;
   pPropertyYCeiling = propYCeiling;

   pPropertyZFloor = propZFloor;
   pPropertyZCeiling = propZCeiling;

   if ( ! pIDataSet ) {
      CoCreateInstance(CLSID_DataSet,NULL,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IDataSet,reinterpret_cast<void **>(&pIDataSet));
      externalDataSet = FALSE;
   }

   pIDataSet -> put_XFloor(pPropertyXFloor);
   pIDataSet -> put_XCeiling(pPropertyXCeiling);

   pIDataSet -> put_YFloor(pPropertyYFloor);
   pIDataSet -> put_YCeiling(pPropertyYCeiling);

   pIDataSet -> put_ZFloor(pPropertyZFloor);
   pIDataSet -> put_ZCeiling(pPropertyZCeiling);
 
#if 0
   if ( pIDataSetDomain )
      pIDataSetDomain -> IncludeDomain(pIDataSet);
#endif

   pIGraphicSegment -> Initialize(pIOpenGLImplementation,propLineColor,propLineWeight);
 
   put_PlotView(gcPlotView2D);

   put_PlotType2D(gcPlotTypeNone);
 
   return S_OK;
   }
 
 
   HRESULT BasePlot::put_IDataSet(IDataSet *newDataSet) {
   if ( ! externalDataSet ) 
      if ( pIDataSet )
         pIDataSet -> Release();
   pIDataSet = newDataSet;
   externalDataSet = TRUE;
   if ( pIDataSet ) 
      pIDataSet -> AddRef();
   return S_OK;
   }
   HRESULT BasePlot::get_IDataSet(IDataSet **getDataSet) {
   *getDataSet = pIDataSet;
   return S_OK;
   }
 
   enum DataArity __stdcall BasePlot::DataArity() {
   if ( pIDataSet )
      return pIDataSet -> DataArity();
   return DATA_ARITY_UNKNOWN;
   }

   HRESULT BasePlot::get_SegmentID(long* newID) {
   return pIGraphicSegment -> get_SegmentID(newID);
   }


   HRESULT BasePlot::put_ActionTable(IGraphicSegmentAction *newActionTable) {
   return pIGraphicSegment -> put_ActionTable(newActionTable);
   }
 
   HRESULT BasePlot::get_ActionTable(IGraphicSegmentAction **getActionTable) {
   return pIGraphicSegment -> get_ActionTable(getActionTable);
   }


   HRESULT BasePlot::put_PlotView(PlotViews inView) {
   plotView = inView;
   return S_OK;
   }
   HRESULT BasePlot::get_PlotView(PlotViews *getPlotView) {
   *getPlotView = plotView;
   return S_OK;
   }
   
 
   HRESULT BasePlot::put_PlotType2D(gc2DPlotTypes inType) {
   plotType2D = (long)inType;
   return S_OK;
   }
   HRESULT BasePlot::get_PlotType2D(gc2DPlotTypes *getPlotType) {
   *getPlotType = (gc2DPlotTypes)plotType2D;
   return S_OK;
   }

   HRESULT BasePlot::put_PlotType3D(gc3DPlotTypes inType) {
   plotType3D = (long)inType;
   return S_OK;
   }
   HRESULT BasePlot::get_PlotType3D(gc3DPlotTypes *getPlotType) {
   *getPlotType = (gc3DPlotTypes)plotType3D;
   return S_OK;
   }


   HRESULT BasePlot::put_ColorProperty(IGProperty* pColor) {
   return pIGraphicSegment -> put_ColorProperty(pColor);
   }

   HRESULT BasePlot::get_ColorProperty(IGProperty** ppColor) {
   return pIGraphicSegment -> get_ColorProperty(ppColor);
   }


   HRESULT BasePlot::put_LineWeight(IGProperty* pLineWeight) {
   return pIGraphicSegment -> put_LineWeight(pLineWeight);
   }

   HRESULT BasePlot::get_LineWeight(IGProperty** ppLineWeight) {
   return pIGraphicSegment -> get_LineWeight(ppLineWeight);
   }


   HRESULT BasePlot::get_SegmentCount(long *pCount) {
   *pCount = 1;
   return S_OK;
   }


   HRESULT BasePlot::GetSegments(long *pSegmentArray) {
   get_SegmentID(pSegmentArray);
   return S_OK;
   }


   HRESULT BasePlot::Create(long *pSegmentID) {
   return pIGraphicSegment -> Create(pSegmentID);
   }

   HRESULT BasePlot::Destroy() {
   return pIGraphicSegment -> Destroy();
   }


   HRESULT BasePlot::Erase() {

   VARIANT_BOOL isInitalized;
   pIOpenGLImplementation -> IsInitialized(&isInitalized);
   if ( ! isInitalized ) return S_OK;

   IGProperty* propBackground;
   HRESULT hr = pIOpenGLImplementation -> get_BackgroundColor(&propBackground);
   if ( ! SUCCEEDED(hr) ) return hr;

   IGProperty* currentColor,*currentLineWeight;
   get_ColorProperty(&currentColor);
   put_ColorProperty(propBackground);
   get_LineWeight(&currentLineWeight);
   put_LineWeight(pPropertyLastDrawLineWeight);
   Draw();
   put_ColorProperty(currentColor);
   put_LineWeight(currentLineWeight);
   propBackground -> Release();

   return hr;
   }

 
   HRESULT BasePlot::AddText(void *vt,char *name,int id) {
   IText *t = reinterpret_cast<IText *>(vt);
   t -> AddRef();
   textList.Add(t,(char *)name,id);
   return S_OK;
   }
 
 
   HRESULT BasePlot::RemoveText(void *vt) {
   IText *t = reinterpret_cast<IText *>(vt);
   if ( t == (IText *)NULL ) {
      IText *p = reinterpret_cast<IText *>(NULL);
      while ( p = textList.GetFirst() ) {
         p -> Release();
         textList.Remove(p);
      }
      return S_OK;
   }
   textList.Remove(t);
   return S_OK;
   }
 

   HRESULT BasePlot::StubPlot() {

   if ( ! pIDataSet ) return E_FAIL;
 
   DataPoint dp,minPoint,maxPoint;

   if ( ! SUCCEEDED(pIDataSetDomain -> get_minX(&dp.x)) ) return E_FAIL;
   if ( ! SUCCEEDED(pIDataSetDomain -> get_minY(&dp.y)) ) return E_FAIL;
   if ( ! SUCCEEDED(pIDataSetDomain -> get_minZ(&dp.z)) ) return E_FAIL;
   if ( ! SUCCEEDED(pIDataSetDomain -> get_maxX(&dp.x)) ) return E_FAIL;
   if ( ! SUCCEEDED(pIDataSetDomain -> get_maxY(&dp.y)) ) return E_FAIL;
   if ( ! SUCCEEDED(pIDataSetDomain -> get_maxZ(&dp.z)) ) return E_FAIL;
 
   VARIANT_BOOL isInitalized;
   pIOpenGLImplementation -> IsInitialized(&isInitalized);
   if ( ! isInitalized ) return S_OK;

   pIDataSetDomain -> GetDomain(&minPoint,&maxPoint);

   pIDataSet -> pushDataPoint(&movePoint);

   dp.x = minPoint.x;
   dp.y = maxPoint.y;
   dp.z = minPoint.z;
   pIDataSet -> pushDataPoint(&dp);
 
   dp.x = minPoint.x;
   dp.y = maxPoint.y;
   dp.z = maxPoint.z;
   pIDataSet -> pushDataPoint(&dp);
 
   dp.x = minPoint.x;
   dp.y = maxPoint.y;
   dp.z = minPoint.z;
   pIDataSet -> pushDataPoint(&dp);
 
   dp.x = maxPoint.x;
   dp.y = maxPoint.y;
   dp.z = minPoint.z;
   pIDataSet -> pushDataPoint(&dp);
 
   dp.x = maxPoint.x;
   dp.y = maxPoint.y;
   dp.z = maxPoint.z;
   pIDataSet -> pushDataPoint(&dp);
 
   dp.x = maxPoint.x;
   dp.y = maxPoint.y;
   dp.z = minPoint.z;
   pIDataSet -> pushDataPoint(&dp);
 
   dp.x = maxPoint.x;
   dp.y = minPoint.y;
   dp.z = minPoint.z;
   pIDataSet -> pushDataPoint(&dp);
 
   dp.x = maxPoint.x;
   dp.y = minPoint.y;
   dp.z = maxPoint.z;
   pIDataSet -> pushDataPoint(&dp);
 
   dp.x = maxPoint.x;
   dp.y = minPoint.y;
   dp.z = minPoint.z;
   pIDataSet -> pushDataPoint(&dp);
 
   pIDataSet -> pushDataPoint(&movePoint);
 
   dp.x = minPoint.x;
   dp.y = minPoint.y;
   dp.z = maxPoint.z;
   pIDataSet -> pushDataPoint(&dp);

   dp.x = minPoint.x;
   dp.y = maxPoint.y;
   dp.z = maxPoint.z;
   pIDataSet -> pushDataPoint(&dp);

   dp.x = maxPoint.x;
   dp.y = maxPoint.y;
   dp.z = maxPoint.z;
   pIDataSet -> pushDataPoint(&dp);

   dp.x = maxPoint.x;
   dp.y = minPoint.y;
   dp.z = maxPoint.z;
   pIDataSet -> pushDataPoint(&dp);

   dp.x = minPoint.x;
   dp.y = minPoint.y;
   dp.z = maxPoint.z;
   pIDataSet -> pushDataPoint(&dp);
 
   pIDataSet -> pushDataPoint(&movePoint);

   pIDataSet -> pushDataPoint(&movePoint);
 
   BasePlot::PlotData(pIDataSet);

   pIDataSet -> ReSet();

   return S_OK;
   }


   HRESULT BasePlot::Draw() {

   VARIANT_BOOL isInitalized;

   pIOpenGLImplementation -> IsInitialized(&isInitalized);

   if ( ! isInitalized ) 
      return S_OK;

   IGProperty* propertyLineWeight;

   get_LineWeight(&propertyLineWeight);

   if ( propertyLineWeight )
      propertyLineWeight -> copyTo(pPropertyLastDrawLineWeight);

   pIGraphicSegment -> Open();

   DataList *pItem = NULL;

   pIDataSet -> peek(pItem,&pItem);

   if ( pItem ) {

      if ( ! ( -DBL_MAX == pItem -> data.x ) ) {
         pIOpenGLImplementation -> NewLine(&pItem -> data);
         pIDataSet -> peek(pItem,&pItem);
      }

      while ( pItem ) {
         if ( -DBL_MAX == pItem -> data.x ) {
            while ( pItem && -DBL_MAX == pItem -> data.x ) 
               pIDataSet -> peek(pItem,&pItem);
            if ( ! pItem ) 
               break;
            pIOpenGLImplementation -> NewLine(&pItem -> data);
         }
         pIOpenGLImplementation -> Vertex(&pItem -> data);
         pIDataSet -> peek(pItem,&pItem);
      }

   }

   pIGraphicSegment -> Close(TRUE);

   IText *t = NULL;

   while ( t = textList.GetNext(t) ) 
      t -> Draw();

   return S_OK;
   }
 
 
   HRESULT BasePlot::Redraw() {
   pIGraphicSegment -> Redraw();
   return S_OK;
   }


   HRESULT BasePlot::DrawPolygon() {

   VARIANT_BOOL isInitalized;
   pIOpenGLImplementation -> IsInitialized(&isInitalized);
   if ( ! isInitalized ) return S_OK;

   DataList *item = reinterpret_cast<DataList*>(NULL);
   pIDataSet -> peek(item,&item);

   pIOpenGLImplementation -> ResetDepth();
 
   pIGraphicSegment -> Open();

   if ( item ) {
 
      if ( -DBL_MAX != item -> data.x ) {
         pIOpenGLImplementation -> NewLine(&item -> data);
         pIDataSet -> peek(item,&item);
      }

      while ( item ) {
         if ( -DBL_MAX == item -> data.x ) {
            while ( item && -DBL_MAX == item -> data.x ) pIDataSet -> peek(item,&item);
            if ( ! item ) break;
            pIOpenGLImplementation -> NewPolygon(&item -> data);
         }
         pIOpenGLImplementation -> Vertex(&item -> data);
         pIDataSet -> peek(item,&item);
      }

   }

   pIGraphicSegment -> Close(TRUE);
 
   IText *t = NULL;
   while ( t = textList.GetNext(t) ) t -> Draw();
 
   //pIOpenGLImplementation -> Flush();
 
   return S_OK;
   }
 
 
   
   HRESULT BasePlot::DrawGDI() {

   VARIANT_BOOL isInitalized;
   pIOpenGLImplementation -> IsInitialized(&isInitalized);
   if ( ! isInitalized ) return S_OK;

   HDC hdc;
   pIOpenGLImplementation -> get_HDC(&hdc);
 
   DataList *item = reinterpret_cast<DataList*>(NULL);
   pIDataSet -> peekGDI(item,&item);
   while ( item ) {
      if ( -DBL_MAX == item -> data.x ) {
         while ( item && -DBL_MAX == item -> data.x ) pIDataSet -> peekGDI(item,&item);
         if ( ! item ) break;
         MoveToEx(hdc,(long)item -> data.x,(long)item -> data.y,NULL);
         continue;
      }
      LineTo(hdc,(long)item -> data.x,(long)item -> data.y);
      pIDataSet -> peekGDI(item,&item);
   }
 
   return S_OK;
   }
 
 
   HRESULT BasePlot::RemoveFromWorldDomain() {
   pIDataSetDomain -> RemoveIncludedDomain(pIDataSet);
   return S_OK;
   }

   HRESULT BasePlot::GetDomainGDI(DataPoint *min,DataPoint *max) {
   return pIDataSetDomain -> GetDomainGDI(min,max);
   }
 

