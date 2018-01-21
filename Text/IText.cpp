// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Text.h"

#include "Graphic_resource.h"
#include "utils.h"

#include "list.cpp"


   HRESULT Text::put_Text(BSTR setText) {
   propertyContent -> put_stringValue(setText);
   return S_OK;
   }
 
   HRESULT Text::get_Text(BSTR *pGetText) {
   if ( ! propertyContent -> pointer() ) {
      *pGetText = SysAllocString(L"");
      return S_FALSE;
   }
   long n = strlen((char *)propertyContent -> pointer());
   if ( 0 == n ) {
      *pGetText = SysAllocString(L"");
      return S_FALSE;
   }
   *pGetText = SysAllocStringLen(NULL,n);
   MultiByteToWideChar(CP_ACP,0,(char *)propertyContent -> pointer(),-1,*pGetText,n);
   return S_OK;
   }
 
 
   HRESULT Text::put_TextRender(boolean doRender) {
   doOpenGLRendering = doRender;
   if ( ! doOpenGLRendering && pIDataSetWorld )
      pIDataSetWorld -> RemoveIncludedDomain(pIDataSet);
   return S_OK;
   }

   HRESULT Text::get_TextRender(boolean *pDoRender) {
   if ( ! pDoRender )
      return E_POINTER;
   *pDoRender = doOpenGLRendering;
   return S_OK;
   }


   HRESULT Text::Initialize(HWND ho,IOpenGLImplementation *pimp,IEvaluator *piev,IDataSet* pidsw,
                              IGProperty* pPropXFloor,IGProperty* pPropXCeiling,
                              IGProperty* pPropYFloor,IGProperty* pPropYCeiling,
                              IGProperty* pPropZFloor,IGProperty* pPropZCeiling,
                              IGProperty* pPropRenderUsingOpenGL,char *intext,DataPoint *inPosition,
                                    void (__stdcall *pWhenChanged)(void *,ULONG_PTR),void *pWhenChangedArg,ULONG_PTR changedCallbackCookie) {
 
   hwndOwner = ho;

   pIOpenGLImplementation = pimp;
 
   pIEvaluator = piev;

   pIDataSetWorld = pidsw;

   if ( pIDataSetWorld ) 
      pIDataSetWorld -> AddRef();

   pWhenChangedCallback = pWhenChanged;
   pWhenChangedCallbackArg = pWhenChangedArg;
   whenChangedCallbackCookie = changedCallbackCookie;

   if ( pPropRenderUsingOpenGL ) {
      short v;
      pPropRenderUsingOpenGL -> get_boolValue(&v);
      doOpenGLRendering = v ? TRUE : FALSE;
   }

   pIBasePlot -> Initialize(doOpenGLRendering ? pIDataSetWorld : NULL,pIOpenGLImplementation,pIEvaluator,
                              propertyTextColor,propertyLineWeight,
                              pPropXFloor,pPropXCeiling,pPropYFloor,pPropYCeiling,pPropZFloor,pPropZCeiling);

   pIBasePlotBoundingBox -> Initialize(doOpenGLRendering ? pIDataSetWorld : NULL,pIOpenGLImplementation,pIEvaluator,
                                          propertyTextColor,propertyLineWeight,
                                          pPropXFloor,pPropXCeiling,pPropYFloor,pPropYCeiling,pPropZFloor,pPropZCeiling);

   pIBasePlot -> get_IDataSet(&pIDataSet);

   pIBasePlotBoundingBox -> get_IDataSet(&pIDataSetBoundingBox);

   if ( pIDataSetWorld ) {
      pIDataSetWorld -> RemoveIncludedDomain(pIDataSet);
      pIDataSetWorld -> RemoveIncludedDomain(pIDataSetBoundingBox);
   }

   pIDataSet -> AddRef();
   pIDataSetBoundingBox -> AddRef();
 
   pIBasePlot -> put_ActionTable(static_cast<IGraphicSegmentAction *>(this));

   pIBasePlotBoundingBox -> put_ActionTable(static_cast<IGraphicSegmentAction *>(this));
 
   if ( intext ) {
      BSTR bstrText = SysAllocStringLen(NULL,strlen(intext));
      MultiByteToWideChar(CP_ACP,0,intext,-1,bstrText,strlen(intext));
      put_Text(bstrText);
      SysFreeString(bstrText);
   }
 
   if ( inPosition ) {
      propertyPositionX -> put_doubleValue(inPosition -> x);
      propertyPositionY -> put_doubleValue(inPosition -> y);
      propertyPositionZ -> put_doubleValue(inPosition -> z);
   }
 
   put_PartOfWorldDomain(partOfWorldDomain);

   return S_OK;
   }
 
 
   HRESULT Text::put_Font(BSTR bstrFontName) {
   hFont = 0;
   if ( pIOpenGLImplementation ) {
      HDC hdc;
      pIOpenGLImplementation -> get_HDC(&hdc);
      SelectObject(hdc,hOriginalFont);
   }
   WideCharToMultiByte(CP_ACP,0,bstrFontName,-1,szFace,MAX_PATH,NULL,NULL);
   return S_OK;
   }
 
 
   HRESULT Text::get_Font(BSTR *pFontName) {
   *pFontName = SysAllocStringLen(NULL,MAX_PATH);
   MultiByteToWideChar(CP_ACP,0,szFace,-1L,*pFontName,MAX_PATH);
   return S_OK;
   }
 
 
   HRESULT Text::put_SizeProperty(IGProperty* propSize,IGProperty* propSizeUnits) {

   if ( ! propSize ) {
#if 0
      if ( propertySize ) {
         pIProperties -> Remove(L"text size");
         propertySize -> Release();
      }
      propertySize = NULL;
#endif
   }

   if ( ! propSizeUnits ) {
      if ( propertySizeUnits ) {
         pIProperties -> Remove(L"text size units");
         propertySizeUnits -> Release();
      }
      propertySizeUnits = NULL;
   }

   if ( ! propSize && ! propSizeUnits ) return S_OK;

#if 0
   propertySize = propSize;
   if ( propertySize ) {
      propertySize -> AddRef();
   }
#endif
   propertySizeUnits = propSizeUnits;
   if ( propertySizeUnits ) {
      propertySizeUnits -> AddRef();
   }

   return S_OK;
   }
 

   HRESULT Text::put_Size(double size) {
   fontSize = size;
   return S_OK;
   }
   HRESULT Text::get_Size(double *pSize) {
   if ( ! pSize )
      return E_POINTER;
   *pSize = fontSize;
   return S_OK;
   }
    

   HRESULT Text::put_SizeUnits(enum TextSizeUnits sizeUnits) {
   return propertySizeUnits -> put_longValue((long)sizeUnits);
   }
   HRESULT Text::get_SizeUnits(enum TextSizeUnits *pSizeUnits) {
   return propertySizeUnits -> get_longValue((long *)pSizeUnits);
   }


   HRESULT Text::put_Format(long newFormat) {
   TextFormat tf;
   propertyFormat -> get_longValue(reinterpret_cast<long *>(&tf));
   return propertyFormat -> put_longValue(newFormat | tf);
   }
 
 
   HRESULT Text::get_Format(long* getFormat) {
   TextFormat tf;
   propertyFormat -> get_longValue(reinterpret_cast<long *>(&tf));
   *getFormat = static_cast<long>(tf);
   return S_OK;
   }
 
 
   HRESULT Text::put_Position(SAFEARRAY *pPosition) {
   VARTYPE vType;
   void *pData;

   long cntItems = countSafeArrayItems(pPosition);

   if ( cntItems < 2 ) 
      return E_INVALIDARG;

   SafeArrayGetVartype(pPosition,&vType);
   SafeArrayAccessData(pPosition,&pData);

   switch ( vType ) {
   case VT_I2: {
      int *pInt = (int *)pData;
      propertyPositionX -> put_longValue((long)*pInt);
      pInt++;
      propertyPositionY -> put_longValue((long)*pInt);
      if ( cntItems == 3 ) {
         pInt++;
         propertyPositionZ -> put_longValue((long)*pInt);
      } else
         propertyPositionZ -> put_longValue(0);
      }
      break;

   case VT_I4: {
      long *pLong = (long *)pData;
      propertyPositionX -> put_longValue(*pLong);
      pLong++;
      propertyPositionY -> put_longValue(*pLong);
      if ( cntItems == 3 ) {
         pLong++;
         propertyPositionZ -> put_longValue(*pLong);
      } else
         propertyPositionZ -> put_longValue(0);
      }
      break;

   case VT_R4: {
      float *pFloat = (float *)pData;
      propertyPositionX -> put_doubleValue((double)*pFloat);
      pFloat++;
      propertyPositionY -> put_doubleValue((double)*pFloat);
      if ( cntItems == 3 ) {
         pFloat++;
         propertyPositionZ -> put_doubleValue((double)*pFloat);
      } else
         propertyPositionZ -> put_doubleValue(0.0);
      }
      break;

   case VT_R8: {
      double *pDouble = (double *)pData;
      propertyPositionX -> put_doubleValue(*pDouble);
      pDouble++;
      propertyPositionY -> put_doubleValue(*pDouble);
      if ( cntItems == 3 ) {
         pDouble++;
         propertyPositionZ -> put_doubleValue(*pDouble);
      } else
         propertyPositionZ -> put_doubleValue(0.0);
      }
      break;

   case VT_BSTR: {
      BSTR *pBstr = (BSTR *)pData;
      propertyPositionX -> put_stringValue(*pBstr);
      pBstr++;
      propertyPositionY -> put_stringValue(*pBstr);
      if ( cntItems == 3 ) {
         pBstr++;
         propertyPositionZ -> put_stringValue(*pBstr);
      } else
         propertyPositionZ -> put_doubleValue(0.0);
      }
      break;

   case VT_VARIANT: {
      VARIANT *pVariant = (VARIANT *)pData;
      propertyPositionX -> put_variantValue(*pVariant);
      pVariant++;
      propertyPositionY -> put_variantValue(*pVariant);
      if ( cntItems == 3 ) {
         pVariant++;
         propertyPositionZ -> put_variantValue(*pVariant);
      } else
         propertyPositionZ -> put_doubleValue(0.0);
      }
      break;


   default:
      SafeArrayUnaccessData(pPosition);
      return E_INVALIDARG;
   }
      
   SafeArrayUnaccessData(pPosition);

   return S_OK;
   }


 
   HRESULT Text::get_Position(SAFEARRAY **ppPosition) {

   if ( ! ppPosition ) return E_POINTER;

   SAFEARRAYBOUND rgsaBound;

   rgsaBound.lLbound = 0;
   rgsaBound.cElements = 3;

   if ( ! *ppPosition ) {

      *ppPosition = SafeArrayCreate(VT_BSTR,1,&rgsaBound);

      BSTR *pData;
      SafeArrayAccessData(*ppPosition,reinterpret_cast<void**>(&pData));
   
      propertyPositionX -> get_stringValue(pData);
      pData++;
      propertyPositionY -> get_stringValue(pData);
      pData++;
      propertyPositionZ -> get_stringValue(pData);
   
      SafeArrayUnaccessData(*ppPosition);
      return S_OK;

   }

   VARTYPE vType;
   void *pData;

   long cntItems = countSafeArrayItems(*ppPosition);

   if ( cntItems < 2 ) 
      return E_INVALIDARG;

   SafeArrayGetVartype(*ppPosition,&vType);
   SafeArrayAccessData(*ppPosition,&pData);

   switch ( vType ) {
   case VT_I2: {
      int *pInt = (int *)pData;
      long i;
      propertyPositionX -> get_longValue(&i);
      *pInt = (int)i;
      pInt++;
      propertyPositionY -> get_longValue(&i);
      *pInt = (int)i;
      if ( cntItems == 3 ) {
         pInt++;
         propertyPositionZ -> get_longValue(&i);
         *pInt = (int)i;
      } else
         *pInt = 0;
      }
      break;

   case VT_I4: {
      long *pLong = (long *)pData;
      propertyPositionX -> get_longValue(pLong);
      pLong++;
      propertyPositionY -> get_longValue(pLong);
      if ( cntItems == 3 ) {
         pLong++;
         propertyPositionZ -> get_longValue(pLong);
      } else
         *pLong = 0;
      }
      break;

   case VT_R4: {
      float *pFloat = (float *)pData;
      double d;
      propertyPositionX -> get_doubleValue(&d);
      *pFloat = (float)d;
      pFloat++;
      propertyPositionY -> get_doubleValue(&d);
      *pFloat = (float)d;
      if ( cntItems == 3 ) {
         pFloat++;
         propertyPositionZ -> get_doubleValue(&d);
         *pFloat = (float)d;
      } else
         *pFloat = (float)0.0;
      }
      break;

   case VT_R8: {
      double *pDouble = (double *)pData;
      propertyPositionX -> get_doubleValue(pDouble);
      pDouble++;
      propertyPositionY -> get_doubleValue(pDouble);
      if ( cntItems == 3 ) {
         pDouble++;
         propertyPositionZ -> get_doubleValue(pDouble);
      } else
         *pDouble = 0.0;
      }
      break;

   case VT_BSTR: {
      BSTR *pBstr = (BSTR *)pData;
      propertyPositionX -> get_stringValue(pBstr);
      pBstr++;
      propertyPositionY -> get_stringValue(pBstr);
      if ( cntItems == 3 ) {
         pBstr++;
         propertyPositionZ -> get_stringValue(pBstr);
      }
      }
      break;

   case VT_VARIANT: {
      VARIANT *pVariant = (VARIANT *)pData;
      propertyPositionX -> get_variantValue(pVariant);
      pVariant++;
      propertyPositionY -> get_variantValue(pVariant);
      if ( cntItems == 3 ) {
         pVariant++;
         propertyPositionZ -> get_variantValue(pVariant);
      }
      }
      break;


   default:
      SafeArrayUnaccessData(*ppPosition);
      return E_INVALIDARG;
   }
      
   SafeArrayUnaccessData(*ppPosition);

   return S_OK;
   }
 
 
   HRESULT Text::put_PositionString(BSTR bstrPosition) {
   propertyPositionString -> put_stringValue(bstrPosition);
   return S_OK;
   }


   HRESULT Text::get_PositionString(BSTR *pBstrPosition) {
   propertyPositionString -> get_stringValue(pBstrPosition);
   return S_OK;
   }


   HRESULT Text::put_PositionX(double x) {
   return propertyPositionX -> put_doubleValue(x);
   }
   HRESULT Text::get_PositionX(double *px) {
   if ( ! px ) return E_POINTER;
   BSTR bstrValue;
   propertyPositionX -> get_stringValue(&bstrValue);
   *px = evalBSTR(pIEvaluator,bstrValue);
   SysFreeString(bstrValue);
   return S_OK;
   }

   HRESULT Text::put_PositionY(double y) {
   return propertyPositionY -> put_doubleValue(y);
   }
   HRESULT Text::get_PositionY(double *py) {
   if ( ! py ) return E_POINTER;
   BSTR bstrValue;
   propertyPositionY -> get_stringValue(&bstrValue);
   *py = evalBSTR(pIEvaluator,bstrValue);
   SysFreeString(bstrValue);
   return S_OK;
   }

   HRESULT Text::put_PositionZ(double z) {
   return propertyPositionZ -> put_doubleValue(z);
   }
   HRESULT Text::get_PositionZ(double *pz) {
   if ( ! pz ) return E_POINTER;
   BSTR bstrValue;
   propertyPositionZ -> get_stringValue(&bstrValue);
   *pz = evalBSTR(pIEvaluator,bstrValue);
   SysFreeString(bstrValue);
   return S_OK;
   }


   HRESULT Text::put_WindowPosition(POINTL *dp) {
   DataPoint dpSource = {static_cast<double>(dp -> x),static_cast<double>(dp -> y),1.0};
   DataPoint dpTarget;
   pIOpenGLImplementation -> WindowToData(&dpSource,&dpTarget);
   propertyPositionX -> put_doubleValue(dpTarget.x);
   propertyPositionY -> put_doubleValue(dpTarget.y);
   propertyPositionZ -> put_doubleValue(dpTarget.z);
   return S_OK;
   }
 
 
   HRESULT Text::get_WindowPosition(POINTL *gp) {
   DataPoint dpSource;

   BSTR bstrValue;
   propertyPositionX -> get_stringValue(&bstrValue);
   dpSource.x = evalBSTR(pIEvaluator,bstrValue);
   SysFreeString(bstrValue);

   propertyPositionY -> get_stringValue(&bstrValue);
   dpSource.y = evalBSTR(pIEvaluator,bstrValue);
   SysFreeString(bstrValue);

   propertyPositionZ -> get_stringValue(&bstrValue);
   dpSource.z = evalBSTR(pIEvaluator,bstrValue);
   SysFreeString(bstrValue);

   DataPoint dpTarget;
   pIOpenGLImplementation -> DataToWindow(&dpSource,UNIT_PIXEL,&dpTarget);
   gp -> x = static_cast<long>(dpTarget.x);
   gp -> y = static_cast<long>(dpTarget.y);
   return S_OK;
   }
 
 
   HRESULT Text::put_DirectionForward(SAFEARRAY *pDirection) {
   if ( ! pDirection ) return E_POINTER;
   GetDataPointSafeArray(pDirection,&directionForward);
   return S_OK;
   }
 
 
   HRESULT Text::get_DirectionForward(SAFEARRAY **pArray) {
   return PutDataPointSafeArray(pArray,&directionForward);
   }
 
 
   HRESULT Text::put_DirectionForwardX(double x) {
   directionForward.x = x;
   return S_OK;
   }
   HRESULT Text::get_DirectionForwardX(double *px) {
   if ( ! px ) return E_POINTER;
   *px = directionForward.x;
   return S_OK;
   }


   HRESULT Text::put_DirectionForwardY(double y) {
   directionForward.y = y;
   return S_OK;
   }
   HRESULT Text::get_DirectionForwardY(double *py) {
   if ( ! py ) return E_POINTER;
   *py = directionForward.y;
   return S_OK;
   }


   HRESULT Text::put_DirectionForwardZ(double z) {
   directionForward.z = z;
   return S_OK;
   }
   HRESULT Text::get_DirectionForwardZ(double *pz) {
   if ( ! pz ) return E_POINTER;
   *pz = directionForward.z;
   return S_OK;
   }


   HRESULT Text::put_DirectionUp(SAFEARRAY *pDirection) {
   if ( ! pDirection ) return E_POINTER;
   GetDataPointSafeArray(pDirection,&directionUp);
   return S_OK;
   }
 
 
   HRESULT Text::get_DirectionUp(SAFEARRAY **pArray) {
   return PutDataPointSafeArray(pArray,&directionUp);
   }
 
 
   HRESULT Text::put_DirectionUpX(double x) {
   directionUp.x = x;
   return S_OK;
   }
   HRESULT Text::get_DirectionUpX(double *px) {
   if ( ! px ) return E_POINTER;
   *px = directionUp.x;
   return S_OK;
   }


   HRESULT Text::put_DirectionUpY(double y) {
   directionUp.y = y;
   return S_OK;
   }
   HRESULT Text::get_DirectionUpY(double *py) {
   if ( ! py ) return E_POINTER;
   *py = directionUp.y;
   return S_OK;
   }


   HRESULT Text::put_DirectionUpZ(double z) {
   directionUp.z = z;
   return S_OK;
   }
   HRESULT Text::get_DirectionUpZ(double *pz) {
   if ( ! pz ) return E_POINTER;
   *pz = directionUp.z;
   return S_OK;
   }


   HRESULT Text::put_Color(SAFEARRAY *pColor) {
   DataPoint dpColor;
   GetDataPointSafeArray(pColor,&dpColor);
   float fvColor[] = {(float)dpColor.x,(float)dpColor.y,(float)dpColor.z,0.0};
   propertyTextColor -> put_binaryValue(sizeof(fvColor),(BYTE*)fvColor);
   return S_OK;
   }

   HRESULT Text::get_Color(SAFEARRAY **ppColor) {
   float fvColor[4];
   propertyTextColor -> get_binaryValue(sizeof(fvColor),(BYTE**)&fvColor);
   DataPoint dpColor = {(double)fvColor[0],(double)fvColor[1],(double)fvColor[2]};
   return PutDataPointSafeArray(ppColor,&dpColor);
   }


   HRESULT Text::put_LineWeight(double lw) {
   return propertyLineWeight -> put_doubleValue(lw);
   }

   HRESULT Text::get_LineWeight(double *plw) {
   return propertyLineWeight -> get_doubleValue(plw);
   }


   HRESULT Text::get_SegmentID(long* pID) {
   return pIBasePlot -> get_SegmentID(pID);
   }
 
 
   HRESULT Text::get_ActionTable(IGraphicSegmentAction **pigsa) {
   return pIBasePlot -> get_ActionTable(pigsa);
   }
 
   HRESULT Text::put_CoordinatePlane(CoordinatePlane putCP) {
   HRESULT hr = propertyCoordinatePlane -> put_longValue(putCP);
   return hr;
   }
 
   HRESULT Text::get_CoordinatePlane(CoordinatePlane *getCP) {
   return propertyCoordinatePlane -> get_longValue((long*)getCP);
   }
 
 
   HRESULT Text::put_FlipHorizontal(VARIANT_BOOL b) {
   return propertyFlipHorizontal -> put_boolValue(b);
   }

   HRESULT Text::get_FlipHorizontal(VARIANT_BOOL *pb) {
   return propertyFlipHorizontal -> get_boolValue(pb);
   }


   HRESULT Text::put_FlipVertical(VARIANT_BOOL b) {
   return propertyFlipVertical -> put_boolValue(b);
   }

   HRESULT Text::get_FlipVertical(VARIANT_BOOL *pb) {
   return propertyFlipVertical -> get_boolValue(pb);
   }


   HRESULT Text::get_minX(double *pv) {
   if ( ! pv ) return E_POINTER;
   return pIDataSet -> get_minX(pv);
   }


   HRESULT Text::get_minY(double *pv) {
   if ( ! pv ) return E_POINTER;
   return pIDataSet -> get_minY(pv);
   }


   HRESULT Text::get_minZ(double *pv) {
   if ( ! pv ) return E_POINTER;
   return pIDataSet -> get_minZ(pv);
   }


   HRESULT Text::get_maxX(double *pv) {
   if ( ! pv ) return E_POINTER;
   return pIDataSet -> get_maxX(pv);
   }


   HRESULT Text::get_maxY(double *pv) {
   if ( ! pv ) return E_POINTER;
   return pIDataSet -> get_maxY(pv);
   }


   HRESULT Text::get_maxZ(double *pv) {
   if ( ! pv ) return E_POINTER;
   return pIDataSet -> get_maxZ(pv);
   }


   HRESULT Text::put_PartOfWorldDomain(VARIANT_BOOL b) {
   partOfWorldDomain = b;
   if (  b ) {
      if ( pIDataSetWorld && doOpenGLRendering )
         pIDataSetWorld -> IncludeDomain(pIDataSet);
   } else {
      if ( pIDataSetWorld )
         pIDataSetWorld -> RemoveIncludedDomain(pIDataSet);
   }
   return S_OK;
   }

 
   HRESULT Text::get_PartOfWorldDomain(VARIANT_BOOL *pb) {
   if ( ! pb ) return E_POINTER;
   if ( ! doOpenGLRendering )
      *pb = false;
   else
      *pb = partOfWorldDomain;
   return S_OK;
   }

 
   HRESULT Text::put_ShowContentPropertyPage(VARIANT_BOOL b) {
   showContentPropertyPage = b;
   return S_OK;
   }
   HRESULT Text::get_ShowContentPropertyPage(VARIANT_BOOL *pb) {
   if ( ! pb ) return E_POINTER;
   *pb = showContentPropertyPage;
   return S_OK;
   }

   HRESULT Text::put_EnablePositionSettings(VARIANT_BOOL b) {
   enablePositionSettings = b;
   return S_OK;
   }
   HRESULT Text::get_EnablePositionSettings(VARIANT_BOOL *pb) {
   if ( ! pb ) return E_POINTER;
   *pb = enablePositionSettings;
   return S_OK;
   }


   HRESULT Text::put_Description(BSTR bstrDescription) {
   propertyDescription -> put_stringValue(bstrDescription);
   return S_OK;
   }
   HRESULT Text::get_Description(BSTR *pBstrDescription) {
   return propertyDescription -> get_stringValue(pBstrDescription);
   }

   HRESULT Text::put_PartOfMainGraphic(VARIANT_BOOL bstrPartOfMainGraphic) {
   propertyPartOfMainGraphic -> put_boolValue(bstrPartOfMainGraphic);
   return S_OK;
   }
   HRESULT Text::get_PartOfMainGraphic(VARIANT_BOOL *pBstrPartOfMainGraphic) {
   return propertyPartOfMainGraphic -> get_boolValue(pBstrPartOfMainGraphic);
   }


   HRESULT Text::get_SegmentCount(long *pCount) {
   *pCount = 1;
   return S_OK;
   }


   HRESULT Text::GetSegments(long *pSegments) {
   return pIBasePlot -> GetSegments(pSegments);
   }


   HRESULT Text::put_TextNotify(ITextNotify *setITextNotify) {
   if ( pITextNotify )
      pITextNotify -> Release();
   pITextNotify = setITextNotify;
   if ( pITextNotify )
      pITextNotify -> AddRef();
   return S_OK;
   }


   HRESULT Text::TextColorProperty(IGProperty* pSource) {
   propertyTextColor = pSource;
   pIBasePlot -> put_ColorProperty(pSource);
   pIBasePlotBoundingBox -> put_ColorProperty(pSource);
   return S_OK;
   }
 
 
   HRESULT Text::AdviseGSystemStatusBar(IGSystemStatusBar* p) {
   if ( ! p ) {
      if ( ! pIGSystemStatusBar ) return E_UNEXPECTED;
      pIGSystemStatusBar = NULL;
   }
   pIGSystemStatusBar = p;
   return S_OK;
   }


   STDMETHODIMP Text::EditProperties() {
   IUnknown *pUnknown;
   QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pUnknown));
   pIProperties -> ShowProperties(NULL,pUnknown);
   pUnknown -> Release();
   if ( pWhenChangedCallback )
      pWhenChangedCallback(pWhenChangedCallbackArg,whenChangedCallbackCookie);
   return S_OK;
   }


   STDMETHODIMP Text::CopyFrom(IText *pSource) {
   IGProperties *pPropertiesSource;
   pSource -> QueryInterface(IID_IGProperties,reinterpret_cast<void**>(&pPropertiesSource));
   pPropertiesSource -> CopyTo(pIProperties);
   pPropertiesSource -> Release();
   return S_OK;
   }

   STDMETHODIMP Text::put_GDIBoundingBox(RECT *pRect) {
   return E_NOTIMPL;
   }

   STDMETHODIMP Text::get_GDIBoundingBox(RECT *pRect) {
   if ( ! pRect )
      return E_POINTER;

   DataPoint minPoint,maxPoint;

   if ( S_OK != pIDataSet -> GetDomainGDI(&minPoint,&maxPoint) ) {
      pIDataSet -> GenerateGDICoordinates(pIOpenGLImplementation);
      if ( S_OK != pIDataSet -> GetDomainGDI(&minPoint,&maxPoint) ) 
         return E_UNEXPECTED;
   }

   pRect -> left = (long)minPoint.x;
   pRect -> top = (long)minPoint.y;
   pRect -> right = (long)maxPoint.x;
   pRect -> bottom = (long)maxPoint.y;

   return S_OK;
   }

   STDMETHODIMP Text::LineUp() {
   DataPoint minPoint,maxPoint;
   pIDataSet -> GetDomain(&minPoint,&maxPoint);
   DataPoint dpTranslate = {0.0,0.0,0.0};
   switch ( coordinatePlane ) {
   case CoordinatePlane_XY:
      dpTranslate.y -= maxPoint.y - minPoint.y;
      break;

   case CoordinatePlane_YX: 
      dpTranslate.x -= maxPoint.x - minPoint.x;
      break;

   default:
      return E_UNEXPECTED;   
   }
   pIDataSet -> Translate(&dpTranslate);
   pIDataSet -> GenerateGDICoordinates(pIOpenGLImplementation);
   return S_OK;
   }

   STDMETHODIMP Text::get_DataSet(IDataSet **pResult) {
   if ( ! pResult )
      return E_POINTER;
   *pResult = pIDataSet;
   return S_OK;
   }