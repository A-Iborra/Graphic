// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "axis.h"
#include "utils.h"

   HRESULT Axis::put_Type(char t) {
   type = t;
   return S_OK;
   }

   HRESULT Axis::get_Type(char *getType) {
   if ( ! getType ) return E_POINTER;
   *getType = type;
   return S_OK;
   }
 
   HRESULT Axis::get_SegmentID(long *pID) {
   return pIPlot -> get_SegmentID(pID);
   }

   HRESULT Axis::get_ActionTable(IGraphicSegmentAction **piga) {
   return pIPlot -> get_ActionTable(piga);
   }
 
 
   HRESULT Axis::get_SegmentCount(long *pCount) {
   long k;
   pIPlot -> get_SegmentCount(&k);
   *pCount = k;
   IText *pIText = NULL;
   while ( pIText = textList.GetNext(pIText) ) {
      pIText -> get_SegmentCount(&k);
      *pCount += k;
   }
   return S_OK;
   }

   HRESULT Axis::GetSegments(long *pSegments) {
   long k;
   pIPlot -> get_SegmentCount(&k);
   pIPlot -> GetSegments(pSegments);
   pSegments += k;
   IText *pIText = NULL;
   while ( pIText = textList.GetNext(pIText) ) {
      pIText -> get_SegmentCount(&k);
      pIText -> GetSegments(pSegments);
      pSegments += k;
   }
   return S_OK;
   }


   HRESULT Axis::put_StartPoint(SAFEARRAY *pStartPoint) {
   VARTYPE vType;
   void *pData;

   long cntItems = countSafeArrayItems(pStartPoint);
   if ( cntItems < 2 ) return E_INVALIDARG;

   SafeArrayGetVartype(pStartPoint,&vType);
   SafeArrayAccessData(pStartPoint,&pData);

   switch ( vType ) {
   case VT_I2: {
      int *pInt = (int *)pData;
      propertyOriginXValue -> put_longValue((long)*pInt);
      pInt++;
      propertyOriginYValue -> put_longValue((long)*pInt);
      if ( cntItems == 3 ) {
         pInt++;
         propertyOriginZValue -> put_longValue((long)*pInt);
      } else
         propertyOriginZValue -> put_longValue(0);
      }
      break;

   case VT_I4: {
      long *pLong = (long *)pData;
      propertyOriginXValue -> put_longValue(*pLong);
      pLong++;
      propertyOriginYValue -> put_longValue(*pLong);
      if ( cntItems == 3 ) {
         pLong++;
         propertyOriginZValue -> put_longValue(*pLong);
      } else
         propertyOriginZValue -> put_longValue(0);
      }
      break;

   case VT_R4: {
      float *pFloat = (float *)pData;
      propertyOriginXValue -> put_doubleValue((double)*pFloat);
      pFloat++;
      propertyOriginYValue -> put_doubleValue((double)*pFloat);
      if ( cntItems == 3 ) {
         pFloat++;
         propertyOriginZValue -> put_doubleValue((double)*pFloat);
      } else
         propertyOriginZValue -> put_doubleValue(0.0);
      }
      break;

   case VT_R8: {
      double *pDouble = (double *)pData;
      propertyOriginXValue -> put_doubleValue(*pDouble);
      pDouble++;
      propertyOriginYValue -> put_doubleValue(*pDouble);
      if ( cntItems == 3 ) {
         pDouble++;
         propertyOriginZValue -> put_doubleValue(*pDouble);
      } else
         propertyOriginZValue -> put_doubleValue(0.0);
      }
      break;

   case VT_BSTR: {
      BSTR *pBstr = (BSTR *)pData;
      propertyOriginXValue -> put_stringValue(*pBstr);
      pBstr++;
      propertyOriginYValue -> put_stringValue(*pBstr);
      if ( cntItems == 3 ) {
         pBstr++;
         propertyOriginZValue -> put_stringValue(*pBstr);
      } else
         propertyOriginZValue -> put_doubleValue(0.0);
      }
      break;

   case VT_VARIANT: {
      VARIANT *pVariant = (VARIANT *)pData;
      propertyOriginXValue -> put_variantValue(*pVariant);
      pVariant++;
      propertyOriginYValue -> put_variantValue(*pVariant);
      if ( cntItems == 3 ) {
         pVariant++;
         propertyOriginZValue -> put_variantValue(*pVariant);
      } else
         propertyOriginZValue -> put_doubleValue(0.0);
      }
      break;


   default:
      SafeArrayUnaccessData(pStartPoint);
      return E_INVALIDARG;
   }
      
   SafeArrayUnaccessData(pStartPoint);

   propertyOriginAtMinpoint -> put_boolValue(FALSE);

   return S_OK;
   }


   HRESULT Axis::get_StartPoint(SAFEARRAY **ppStartPoint) {

   if ( ! ppStartPoint ) return E_POINTER;

   SAFEARRAYBOUND rgsaBound;

   rgsaBound.lLbound = 0;
   rgsaBound.cElements = 3;

   if ( *ppStartPoint ) 
      SafeArrayRedim(*ppStartPoint,&rgsaBound);
   else
      *ppStartPoint = SafeArrayCreate(VT_R8,1,&rgsaBound);

   double *pData;
   SafeArrayAccessData(*ppStartPoint,reinterpret_cast<void**>(&pData));

   propertyOriginXValue -> get_doubleValue(pData++);
   propertyOriginYValue -> get_doubleValue(pData++);
   propertyOriginZValue -> get_doubleValue(pData);

   SafeArrayUnaccessData(*ppStartPoint);

   return S_OK;
   }


   HRESULT Axis::put_StartPointX(double startPointX) {
   return propertyOriginXValue -> put_doubleValue(startPointX);
   }

   HRESULT Axis::get_StartPointX(double *pStartPointX) {
   return propertyOriginXValue -> get_doubleValue(pStartPointX);
   }

   HRESULT Axis::put_StartPointY(double startPointY) {
   return propertyOriginYValue -> put_doubleValue(startPointY);
   }

   HRESULT Axis::get_StartPointY(double *pStartPointY) {
   return propertyOriginYValue -> get_doubleValue(pStartPointY);
   }

   HRESULT Axis::put_StartPointZ(double startPointZ) {
   return propertyOriginZValue -> put_doubleValue(startPointZ);
   }

   HRESULT Axis::get_StartPointZ(double *pStartPointZ) {
   return propertyOriginZValue -> get_doubleValue(pStartPointZ);
   }



   HRESULT Axis::put_EndPoint(SAFEARRAY *pEndPoint) {
   VARTYPE vType;
   void *pData;

   long cntItems = countSafeArrayItems(pEndPoint);
   if ( cntItems < 3 ) return E_INVALIDARG;

   SafeArrayGetVartype(pEndPoint,&vType);
   SafeArrayAccessData(pEndPoint,&pData);

   switch ( vType ) {
   case VT_I2: {
      int *pInt = (int *)pData;
      propertyEndpointXValue -> put_longValue((long)*pInt);
      pInt++;
      propertyEndpointYValue -> put_longValue((long)*pInt);
      pInt++;
      propertyEndpointZValue -> put_longValue((long)*pInt);
      }
      break;

   case VT_I4: {
      long *pLong = (long *)pData;
      propertyEndpointXValue -> put_longValue(*pLong);
      pLong++;
      propertyEndpointYValue -> put_longValue(*pLong);
      pLong++;
      propertyEndpointZValue -> put_longValue(*pLong);
      }
      break;

   case VT_R4: {
      float *pFloat = (float *)pData;
      propertyEndpointXValue -> put_doubleValue((double)*pFloat);
      pFloat++;
      propertyEndpointYValue -> put_doubleValue((double)*pFloat);
      pFloat++;
      propertyEndpointZValue -> put_doubleValue((double)*pFloat);
      }
      break;

   case VT_R8: {
      double *pDouble = (double *)pData;
      propertyEndpointXValue -> put_doubleValue(*pDouble);
      pDouble++;
      propertyEndpointYValue -> put_doubleValue(*pDouble);
      pDouble++;
      propertyEndpointZValue -> put_doubleValue(*pDouble);
      }
      break;

   case VT_BSTR: {
      BSTR *pBstr = (BSTR *)pData;
      propertyEndpointXValue -> put_stringValue(*pBstr);
      pBstr++;
      propertyEndpointYValue -> put_stringValue(*pBstr);
      pBstr++;
      propertyEndpointZValue -> put_stringValue(*pBstr);
      }
      break;

   case VT_VARIANT: {
      VARIANT *pVariant = (VARIANT *)pData;
      propertyEndpointXValue -> put_variantValue(*pVariant);
      pVariant++;
      propertyEndpointYValue -> put_variantValue(*pVariant);
      pVariant++;
      propertyEndpointZValue -> put_variantValue(*pVariant);
      }
      break;


   default:
      SafeArrayUnaccessData(pEndPoint);
      return E_INVALIDARG;
   }
      
   SafeArrayUnaccessData(pEndPoint);

   propertyEndpointAtMaxpoint -> put_boolValue(FALSE);

   return S_OK;
   }


   HRESULT Axis::get_EndPoint(SAFEARRAY **ppEndPoint) {

   if ( ! ppEndPoint ) return E_POINTER;

   SAFEARRAYBOUND rgsaBound;

   rgsaBound.lLbound = 0;
   rgsaBound.cElements = 3;

   *ppEndPoint = SafeArrayCreate(VT_BSTR,1,&rgsaBound);

   BSTR *pData;
   SafeArrayAccessData(*ppEndPoint,reinterpret_cast<void**>(&pData));

   propertyEndpointXValue -> get_stringValue(pData++);
   propertyEndpointYValue -> get_stringValue(pData++);
   propertyEndpointZValue -> get_stringValue(pData);

   SafeArrayUnaccessData(*ppEndPoint);

   return S_OK;
   }


   HRESULT Axis::put_EndPointX(double EndPointX) {
   propertyEndpointXValue -> put_doubleValue(EndPointX);
   return S_OK;
   }

   HRESULT Axis::get_EndPointX(double *pEndPointX) {
   if ( ! pEndPointX ) return E_POINTER;
   propertyEndpointXValue -> get_doubleValue(pEndPointX);
   return S_OK;
   }

   HRESULT Axis::put_EndPointY(double EndPointY) {
   propertyEndpointYValue -> put_doubleValue(EndPointY);
   return S_OK;
   }

   HRESULT Axis::get_EndPointY(double *pEndPointY) {
   if ( ! pEndPointY ) return E_POINTER;
   propertyEndpointYValue -> get_doubleValue(pEndPointY);
   return S_OK;
   }

   HRESULT Axis::put_EndPointZ(double EndPointZ) {
   propertyEndpointZValue -> put_doubleValue(EndPointZ);
   return S_OK;
   }

   HRESULT Axis::get_EndPointZ(double *pEndPointZ) {
   if ( ! pEndPointZ ) return E_POINTER;
   propertyEndpointZValue -> get_doubleValue(pEndPointZ);
   return S_OK;
   }


   HRESULT Axis::put_DefinesDomain(VARIANT_BOOL b) {
   propertyDeterminesDomain -> put_boolValue(b);
   return S_OK;
   }


   HRESULT Axis::get_DefinesDomain(VARIANT_BOOL *pb) {
   if ( ! pb ) return E_POINTER;
   propertyDeterminesDomain -> get_boolValue(pb);
   return S_OK;
   }


   HRESULT Axis::put_LineWeight(long newWeight) {
   propertyLineWeight -> put_longValue(newWeight);
   return S_OK;
   }

   HRESULT Axis::get_LineWeight(long *pWeight) {
   if ( ! pWeight ) return E_POINTER;
   propertyLineWeight -> get_longValue(pWeight);
   return S_OK;
   }


   HRESULT Axis::put_LineColor(SAFEARRAY *pLineColor) {
   if ( ! pLineColor ) return E_INVALIDARG;
   VARTYPE vType;
   void *pData;

   long cntItems = countSafeArrayItems(pLineColor);
   if ( cntItems < 3 ) return E_INVALIDARG;

   SafeArrayGetVartype(pLineColor,&vType);
   SafeArrayAccessData(pLineColor,&pData);

   float fvColor[] = {CLR_BLACK};

   switch ( vType ) {
   case VT_I2: {
      int *pInt = (int *)pData;
      fvColor[0] = (float)(*pInt);
      pInt++;
      fvColor[1] = (float)(*pInt);
      pInt++;
      fvColor[2] = (float)(*pInt);
      }
      break;

   case VT_I4: {
      long *pLong = (long *)pData;
      fvColor[0] = (float)(*pLong);
      pLong++;
      fvColor[1] = (float)(*pLong);
      pLong++;
      fvColor[2] = (float)(*pLong);
      }
      break;

   case VT_R4: {
      float *pFloat = (float *)pData;
      fvColor[0] = *pFloat;
      pFloat++;
      fvColor[1] = *pFloat;
      pFloat++;
      fvColor[2] = *pFloat;
      }
      break;

   case VT_R8: {
      double *pDouble = (double *)pData;
      fvColor[0] = (float)(*pDouble);
      pDouble++;
      fvColor[1] = (float)(*pDouble);
      pDouble++;
      fvColor[2] = (float)(*pDouble);
      }
      break;

   case VT_BSTR: {
      BSTR *pBstr = (BSTR *)pData;
      fvColor[0] = (float)evalBSTR(pIEvaluator,*pBstr);
      pBstr++;
      fvColor[1] = (float)evalBSTR(pIEvaluator,*pBstr);
      pBstr++;
      fvColor[2] = (float)evalBSTR(pIEvaluator,*pBstr);
      }
      break;

   case VT_VARIANT: {
      VARIANT *pVariant = (VARIANT *)pData;
      char *pszTemp;
      convertVariantToSzString(pVariant,&pszTemp);
      fvColor[0] = (float)atof(pszTemp);
      delete [] pszTemp;
      pVariant++;
      convertVariantToSzString(pVariant,&pszTemp);
      fvColor[1] = (float)atof(pszTemp);
      delete [] pszTemp;
      pVariant++;
      convertVariantToSzString(pVariant,&pszTemp);
      fvColor[2] = (float)atof(pszTemp);
      delete [] pszTemp;
      }
      break;


   default:
      SafeArrayUnaccessData(pLineColor);
      return E_INVALIDARG;
   }
      
   SafeArrayUnaccessData(pLineColor);

   propertyLineColor -> put_binaryValue(sizeof(fvColor),(BYTE*)&fvColor);

   if ( axisTickColorTrackLineColor ) 
      propertyLineColor -> copyTo(propertyTickLabelColor);

   if ( axisLabelColorTrackLineColor ) 
      propertyLineColor -> copyTo(propertyLabelColor);
   
   return S_OK;
   }


   HRESULT Axis::get_LineColor(SAFEARRAY **ppLineColor) {

   if ( ! ppLineColor ) return E_POINTER;

   SAFEARRAYBOUND rgsaBound;

   rgsaBound.lLbound = 0;
   rgsaBound.cElements = 3;

   if ( *ppLineColor ) 
      SafeArrayRedim(*ppLineColor,&rgsaBound);
   else
      *ppLineColor = SafeArrayCreate(VT_R4,1,&rgsaBound);

   float *pData;

   SafeArrayAccessData(*ppLineColor,reinterpret_cast<void**>(&pData));

   float fvColor[4];
   propertyLineColor -> get_binaryValue(sizeof(fvColor),(BYTE**)&fvColor[0]);

   *pData = fvColor[0];
   pData++;
   *pData = fvColor[1];
   pData++;
   *pData = fvColor[2];

   SafeArrayUnaccessData(*ppLineColor);

   return S_OK;
   }


   HRESULT Axis::put_TickCount(long setTickCount) {
   propertyTickCount -> put_longValue(setTickCount);
   return S_OK;
   }


   HRESULT Axis::get_TickCount(long *pTickCount) {
   if ( ! pTickCount ) return E_POINTER;
   propertyTickCount -> get_longValue(pTickCount);
   return S_OK;
   }


   HRESULT Axis::put_TickSize(double newSize) {
   propertyTickLength -> put_doubleValue(newSize);
   return S_OK;
   }


   HRESULT Axis::get_TickSize(double *pSize) {
   if ( ! pSize ) return E_POINTER;
   *pSize = tickLength;
   return S_OK;
   }


   HRESULT Axis::put_TickPercentAbove(double pctAbove) {
   propertyTickStraddleAbove -> put_doubleValue(pctAbove);
   return S_OK;
   }


   HRESULT Axis::get_TickPercentAbove(double *pPctAbove) {
   if ( ! pPctAbove ) return E_POINTER;
   propertyTickStraddleAbove -> get_doubleValue(pPctAbove);
   return S_OK;
   }


   HRESULT Axis::put_GridLinesPerTick(long newLines) {
   propertyGridLinesPerTick -> put_longValue(newLines);
   return S_OK;
   }


   HRESULT Axis::get_GridLinesPerTick(long *pGridLines) {
   if ( ! pGridLines ) return E_POINTER;
   propertyGridLinesPerTick -> get_longValue(pGridLines);
   return S_OK;
   }


   HRESULT Axis::Initialize(char axisType,IAxis *pX,IAxis *pY,IAxis *pZ,IGProperty* pPlotView,
                              IGProperty* pPropertyXFloor,IGProperty* pPropertyXCeiling,
                              IGProperty* pPropertyYFloor,IGProperty* pPropertyYCeiling,
                              IGProperty* pPropertyZFloor,IGProperty* pPropertyZCeiling,
                              IGProperty* pPropOpenGLText,
                              IDataSet* pds,void * pvNewOpenGLImplementation,IEvaluator *iev,void (__stdcall *pChangedCallback)(void *,ULONG_PTR),void *pChangedArg,ULONG_PTR changedCookie) {

   type = axisType;

   pXAxis = pX;
   pYAxis = pY;
   pZAxis = pZ;

   pWhenChangedCallback = pChangedCallback;

   pWhenChangedCallbackArg = pChangedArg;

   whenChangedCallbackCookie = changedCookie;

   pParentPropertyPlotView = pPlotView;

   pParentPropertyXFloor = pPropertyXFloor;
   pParentPropertyXCeiling = pPropertyXCeiling;

   pParentPropertyYFloor = pPropertyYFloor;
   pParentPropertyYCeiling = pPropertyYCeiling;

   pParentPropertyZFloor = pPropertyZFloor;
   pParentPropertyZCeiling = pPropertyZCeiling;

   pParentPropertyOpenGLText = pPropOpenGLText;

   pIOpenGLImplementation = (IOpenGLImplementation *)pvNewOpenGLImplementation;

   pIEvaluator = iev;

   pIDataSetDomain = pds;

   if ( pIDataSetDomain ) {
      pIDataSetDomain -> put_XFloor(pPropertyXFloor);
      pIDataSetDomain -> put_XCeiling(pPropertyXCeiling);
      pIDataSetDomain -> put_YFloor(pPropertyYFloor);
      pIDataSetDomain -> put_YCeiling(pPropertyYCeiling);
      pIDataSetDomain -> put_ZFloor(pPropertyZFloor);
      pIDataSetDomain -> put_ZCeiling(pPropertyZCeiling);
   }

   if ( pIPlot ) 
      pIPlot -> Release();
 
   CoCreateInstance(CLSID_Plot,NULL,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IPlot,reinterpret_cast<void **>(&pIPlot));
 
   pIPlot -> Initialize(pIDataSetDomain,pIOpenGLImplementation,pIEvaluator,propertyLineColor,propertyLineWeight,pParentPropertyPlotView,propertyPlotType,
                           NULL,NULL,
                           pPropertyXFloor,pPropertyXCeiling,
                           pPropertyYFloor,pPropertyYCeiling,
                           pPropertyZFloor,pPropertyZCeiling,
                           pWhenChangedCallback,pWhenChangedCallbackArg,changedCookie);
 
   pIPlot -> put_ActionTable(static_cast<IGraphicSegmentAction*>(this));

   pIPlot -> put_OverrideOwnerPlotView(FALSE);

   pIPlot -> put_OverrideOwnerPlotType(FALSE);

   pLabel -> Initialize(pIOpenGLImplementation,pIEvaluator,pIDataSetDomain,
                           pParentPropertyXFloor,pParentPropertyXCeiling,
                           pParentPropertyYFloor,pParentPropertyYCeiling,
                           pParentPropertyZFloor,pParentPropertyZCeiling,
                           pParentPropertyOpenGLText,NULL,NULL,pWhenChangedCallback,pWhenChangedCallbackArg,changedCookie);

   initWindows();

   pRepresentativeText -> Initialize(pIOpenGLImplementation,pIEvaluator,pIDataSetDomain,
                                       pParentPropertyXFloor,pParentPropertyXCeiling,
                                       pParentPropertyYFloor,pParentPropertyYCeiling,
                                       pParentPropertyZFloor,pParentPropertyZCeiling,
                                       pParentPropertyOpenGLText,NULL,NULL,pWhenChangedCallback,pWhenChangedCallbackArg,changedCookie);

   switch ( axisType ) {
   case 'X':
      pRepresentativeText -> put_CoordinatePlane(CoordinatePlane_XY);
      break;

   case 'Y':
      pRepresentativeText -> put_CoordinatePlane(CoordinatePlane_XY);
      break;

   case 'Z':
      pRepresentativeText -> put_CoordinatePlane(CoordinatePlane_XY);
      break;

   default:
      pRepresentativeText -> put_CoordinatePlane(CoordinatePlane_XY);
      break;
   }

   return S_OK;
   }


   HRESULT Axis::get_DataSet(IDataSet **ppds) {
   if ( ! ppds ) return E_POINTER;
   if ( ! pIPlot ) return E_UNEXPECTED;
   return pIPlot -> get_IDataSet(ppds);
   }


   HRESULT Axis::PrepData() {
   preparePlot();
   PrepText();
   IText *pIText = NULL;
   IDataSet *pIDataSetAxis = NULL;
   get_DataSet(&pIDataSetAxis);
   while ( pIText = textList.GetNext(pIText) ) {
      DataPoint minPoint,maxPoint;
      IDataSet *pIDataSetText = NULL;
      pIText -> get_DataSet(&pIDataSetText);
      pIDataSetText -> GetDomain(&minPoint,&maxPoint);
      pIDataSetAxis -> ResetLimits(&minPoint);
      pIDataSetAxis -> ResetLimits(&maxPoint);
   }
   if ( pLabel && drawAxisLabel ) {
      DataPoint minPoint,maxPoint;
      IDataSet *pIDataSetText = NULL;
      pLabel -> get_DataSet(&pIDataSetText);
      pIDataSetText -> GetDomain(&minPoint,&maxPoint);
      pIDataSetAxis -> ResetLimits(&minPoint);
      pIDataSetAxis -> ResetLimits(&maxPoint);
   }
   return S_OK;
   }


   HRESULT Axis::Draw() {
   if ( ! pIPlot ) 
      return E_UNEXPECTED;
   pIPlot -> Draw();
   return S_OK;
   }


   HRESULT Axis::DrawLabels() {
   IText *pIText = NULL;
   boolean isOpenGL;
   while ( pIText = textList.GetNext(pIText) ) 
      pIText -> Draw();
   if ( ! pLabel ) 
      return S_OK;
   pLabel -> get_TextRenderOpenGL(&isOpenGL);
   if ( isOpenGL )
      return S_OK;
   pLabel -> PrepData();
   pLabel -> Draw();
   return S_OK;
   }

   HRESULT Axis::Redraw() {
   if ( ! pIPlot ) return E_UNEXPECTED;
   pIPlot -> Redraw();
   IText *pIText = NULL;
   while ( pIText = textList.GetNext(pIText) )
      pIText -> Redraw();
   pLabel -> Redraw();
   return S_OK;
   }
   
 
   HRESULT Axis::Erase() {
   if ( ! pIPlot ) return E_UNEXPECTED;
   pIPlot -> Erase();
   IText *pIText = NULL;
   while ( pIText = textList.GetNext(pIText) )
      pIText -> Erase();
   pLabel -> Erase();
   return S_OK;
   }


   HRESULT Axis::AdviseGSystemStatusBar(IGSystemStatusBar* p) {
   if ( ! p ) {
      if ( ! pIGSystemStatusBar ) return E_UNEXPECTED;
      pIGSystemStatusBar = NULL;
   }
   pIGSystemStatusBar = p;
   return S_OK;
   }


   HRESULT Axis::ShowProperties(HWND hwndParent) {
   IUnknown *pUnknown;
   QueryInterface(IID_IUnknown,(void **)&pUnknown);
   pIGProperties -> ShowProperties(hwndParent,pUnknown);
   pUnknown -> Release();
   return S_OK;
   }