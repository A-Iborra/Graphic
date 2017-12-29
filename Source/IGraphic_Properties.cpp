/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <stdio.h>

#include "utils.h"
#include "Graphic_resource.h"

#include "Graphic.h"

#include "GraphicControl_i.h"

   STDMETHODIMP G::SetProperties() {
   SendMessage(hwndGraphic,WM_COMMAND,MAKEWPARAM(IDMI_GRAPHIC_PROPERTIES,0),0L);
   return S_OK;
   }
 
 
   STDMETHODIMP G::put_Size(long cx,long cy) {
   if ( hwndFrame )
      SetWindowPos(hwndFrame,HWND_TOP,0,0,cx,cy,SWP_NOMOVE);
   else {
      containerSize.cx = cx;
      containerSize.cy = cy;
   }
   return S_OK;
   }
 
 
   STDMETHODIMP G::put_Position(long x,long y) {
   SetWindowPos(hwndGraphic,HWND_TOP,x,y,0,0,SWP_NOSIZE);
   return S_OK;
   }
 
 
   STDMETHODIMP G::put_PlotView(enum PlotViews setPlotView) {
   propertyPlotView -> put_longValue(setPlotView);
   render(0);
   return S_OK;
   }
 
 
   STDMETHODIMP G::get_PlotView(enum PlotViews* getPlotView) {
   *getPlotView = plotView;
   return S_OK;
   }
 
 
   STDMETHODIMP G::put_PlotType(PlotTypes setPlotType) {
   propertyPlotType -> put_longValue(setPlotType);
   //render(0);
   return S_OK;
   }
 
 
   STDMETHODIMP G::get_PlotType(PlotTypes* getPlotType) {
   *getPlotType = plotType;
   return S_OK;
   }
 
 
   STDMETHODIMP G::put_ViewTheta(float newTheta) {
   propertyViewTheta -> put_doubleValue((double)newTheta);
   //render(0);
   return S_OK;
   }
   STDMETHODIMP G::get_ViewTheta(float* pTheta) {
   if ( ! pTheta ) return E_POINTER;
   double d;
   propertyViewTheta -> get_doubleValue(&d);
   *pTheta = (float)d;
   return S_OK;
   }


   STDMETHODIMP G::put_ViewPhi(float newPhi) {
   propertyViewPhi -> put_doubleValue((double)newPhi);
   //render(0);
   return S_OK;
   }
   STDMETHODIMP G::get_ViewPhi(float* pPhi) {
   if ( ! pPhi ) return E_POINTER;
   double d;
   propertyViewPhi -> get_doubleValue(&d);
   *pPhi = (float)d;
   return S_OK;
   }


   STDMETHODIMP G::put_ViewSpin(float newSpin) {
   propertyViewSpin -> put_doubleValue((double)newSpin);
   //render(0);
   return S_OK;
   }
   STDMETHODIMP G::get_ViewSpin(float* pSpin) {
   if ( ! pSpin ) return E_POINTER;
   double d;
   propertyViewSpin -> get_doubleValue(&d);
   *pSpin = (float)d;
   return S_OK;
   }


   STDMETHODIMP G::put_DenyUserPropertySettings(VARIANT_BOOL b) {
   denyUserPropertySettings = b;
   return S_OK;
   }
   STDMETHODIMP G::get_DenyUserPropertySettings(VARIANT_BOOL *pb) {
   if ( ! pb ) return E_POINTER;
   *pb = denyUserPropertySettings;
   return S_OK;
   }

   
   STDMETHODIMP G::put_LightOn(long lightNumber,VARIANT_BOOL bEnabled) {
   if ( lightNumber < 1 || lightNumber > supportedLightCount ) return E_INVALIDARG;
   ppPropertyLightOn[lightNumber - 1] -> put_boolValue(bEnabled);
   return S_OK;
   }
   STDMETHODIMP G::get_LightOn(long lightNumber,VARIANT_BOOL *pbEnabled) {
   if ( ! pbEnabled ) return E_POINTER;
   if ( lightNumber < 1 || lightNumber > supportedLightCount ) return E_INVALIDARG;
   ppPropertyLightOn[lightNumber - 1] -> get_boolValue(pbEnabled);
   return S_OK;
   }


   STDMETHODIMP G::put_LightPosition(long lightNumber,VARIANT *pv) {
   if ( ! pv ) return E_POINTER;
   if ( lightNumber < 1 || lightNumber > supportedLightCount ) return E_INVALIDARG;
   char szTemp[256];
   HRESULT hr = commaStringFromVariant(pv,szTemp,256);
   if ( ! SUCCEEDED(hr) ) return hr;
   ppPropertyLightPos[lightNumber - 1] -> put_szValue(szTemp);
   return S_OK;
   }
   STDMETHODIMP G::get_LightPosition(long lightNumber,VARIANT *pv) {
   if ( ! pv ) return E_POINTER;
   if ( lightNumber < 1 || lightNumber > supportedLightCount ) return E_UNEXPECTED;
   long n;
   ppPropertyLightPos[lightNumber - 1] -> get_size(&n);
   char *pszTemp = new char[n];
   ppPropertyLightPos[lightNumber - 1] -> get_szValue(pszTemp);
   HRESULT hr = variantFromCommaString(pIEvaluator,pszTemp,pv);
   delete [] pszTemp;
   return S_OK;
   }


   STDMETHODIMP G::put_BackgroundColor(long colorIndex) {
   //render(0);
   return S_OK;
   }
   STDMETHODIMP G::get_BackgroundColor(long* pColorIndex) {
   if ( ! pColorIndex ) return E_POINTER;
   return S_OK;
   }


   STDMETHODIMP G::put_AutoPlotViewDetection(VARIANT_BOOL doAllow) {
   propertyAutoPlotViewDetection -> put_boolValue(doAllow);
   return S_OK;
   }
   STDMETHODIMP G::get_AutoPlotViewDetection(VARIANT_BOOL *pDoAllow) {
   if ( ! pDoAllow ) return E_POINTER;
   propertyAutoPlotViewDetection -> get_boolValue(pDoAllow);
   return S_OK;
   }


   STDMETHODIMP G::put_LeftMargin(long newMargin) {
   margins.left = newMargin;
   //render(0);
   return S_OK;
   }
   STDMETHODIMP G::get_LeftMargin(long* pMargin) {
   if ( ! pMargin ) return E_POINTER;
   *pMargin = margins.left;
   return S_OK;
   }

   STDMETHODIMP G::put_TopMargin(long newMargin) {
   margins.top = newMargin;
   //render(0);
   return S_OK;
   }
   STDMETHODIMP G::get_TopMargin(long* pMargin) {
   if ( ! pMargin ) return E_POINTER;
   *pMargin = margins.top;
   return S_OK;
   }

   STDMETHODIMP G::put_RightMargin(long newMargin) {
   margins.right = newMargin;
   //render(0);
   return S_OK;
   }
   STDMETHODIMP G::get_RightMargin(long* pMargin) {
   if ( ! pMargin ) return E_POINTER;
   *pMargin = margins.right;
   return S_OK;
   }

   STDMETHODIMP G::put_BottomMargin(long newMargin) {
   margins.bottom = newMargin;
   //render(0);
   return S_OK;
   }
   STDMETHODIMP G::get_BottomMargin(long* pMargin) {
   if ( ! pMargin ) return E_POINTER;
   *pMargin = margins.bottom;
   return S_OK;
   }


   STDMETHODIMP G::put_Margins(long lMargin,long tMargin,long rMargin,long bMargin) {
   margins.left = lMargin;
   margins.right = rMargin;
   margins.top = tMargin;
   margins.bottom = bMargin;
   //render(0);
   return S_OK;
   }


   STDMETHODIMP G::put_PickBoxSize(long newSize) {
   pickBoxSize.cx = pickBoxSize.cy = newSize;
   return S_OK;
   }
   STDMETHODIMP G::get_PickBoxSize(long* pSize) {
   if ( ! pSize ) return E_POINTER;
   *pSize = pickBoxSize.cx;
   return S_OK;
   }


   STDMETHODIMP G::put_UseStatusBar(VARIANT_BOOL usb) {
   short oldStatus;
   propertyShowStatusBar -> get_boolValue(&oldStatus);
   propertyShowStatusBar -> put_boolValue(usb);
   if ( abs(usb) != abs(oldStatus) ) 
      setDataSourcesVisibility(NULL,NULL);
   return S_OK;
   }
   STDMETHODIMP G::get_UseStatusBar(VARIANT_BOOL *pusb) {
   if ( ! pusb ) return E_POINTER;
   propertyShowStatusBar -> get_boolValue(pusb);
   return S_OK;
   }


   STDMETHODIMP G::put_UseGraphicsCursor(VARIANT_BOOL usg) {
   propertyUseGraphicsCursor -> put_boolValue(usg);
   return S_OK;
   }
   STDMETHODIMP G::get_UseGraphicsCursor(VARIANT_BOOL *pusg) {
   if ( ! pusg ) return E_POINTER;
   propertyUseGraphicsCursor -> get_boolValue(pusg);
   return S_OK;
   }


/* Other objects */

   STDMETHODIMP G::get_Axis(BSTR type,IDispatch **pAxis) {
   if ( ! pAxis ) return E_POINTER;
   *pAxis = NULL;
   char* szType = new char[wcslen(type) + 1];
   memset(szType,0,wcslen(type) + 1);
   WideCharToMultiByte(CP_ACP,0,type,-1,szType,wcslen(type) + 1,0,0);
   char checkType;
   IAxis *p = (IAxis *)NULL;
   while ( p = axisList.GetNext(p) ) {
      p -> get_Type(&checkType);
      if ( 0 == strncmp(szType,&checkType,1) ) {
         p -> QueryInterface(IID_IDispatch,reinterpret_cast<void**>(pAxis));
         delete [] szType;
         return S_OK;
      }
   }
   delete [] szType;
   return S_OK;
   }


   STDMETHODIMP G::get_Plot(long plotNumber,IDispatch **pPlot) {
   if ( ! pPlot ) return E_POINTER;
   *pPlot = NULL;
   IPlot *p = plotList.Get(plotNumber);
   if ( p ) {
      p -> QueryInterface(IID_IDispatch,reinterpret_cast<void**>(pPlot));
      return S_OK;
   }
   return S_OK;
   }


   STDMETHODIMP G::get_Function(BSTR expression,IDispatch **pFunction) {
   if ( ! pFunction ) return E_POINTER;
   *pFunction = NULL;
   char* szExpression = new char[wcslen(expression) + 1];
   memset(szExpression,0,wcslen(expression) + 1);
   WideCharToMultiByte(CP_ACP,0,expression,-1,szExpression,wcslen(expression) + 1,0,0);
   IGSFunctioNater *p = (IGSFunctioNater *)NULL;
   while ( p = functionList.GetNext(p) ) {
      BSTR bstrExpression;
      p -> get_Expression(&bstrExpression);
      char *szCheckExpression = new char[wcslen(bstrExpression) + 1];
      memset(szCheckExpression,0,wcslen(bstrExpression) + 1);
      WideCharToMultiByte(CP_ACP,0,bstrExpression,-1,szCheckExpression,wcslen(bstrExpression),0,0);
      SysFreeString(bstrExpression);
      if ( 0 == strcmp(szExpression,szCheckExpression) ) {
         p -> QueryInterface(IID_IDispatch,reinterpret_cast<void**>(pFunction));
         delete [] szExpression;
         delete [] szCheckExpression;
         return S_OK;
      }
     delete [] szCheckExpression;
   }
   delete [] szExpression;
   return S_OK;
   }


   STDMETHODIMP G::get_Text(BSTR text,IDispatch **pText) {
   if ( ! pText ) return E_POINTER;
   *pText = NULL;
   char* szText = new char[wcslen(text) + 1];
   memset(szText,0,wcslen(text) + 1);
   WideCharToMultiByte(CP_ACP,0,text,-1,szText,wcslen(text) + 1,0,0);
   IText *p = (IText *)NULL;
   while ( p = textList.GetNext(p) ) {
      BSTR bstrText;
      p -> get_Text(&bstrText);
      char *szCheckText = new char[wcslen(bstrText) + 1];
      memset(szCheckText,0,wcslen(bstrText) + 1);
      WideCharToMultiByte(CP_ACP,0,bstrText,-1,szCheckText,wcslen(bstrText),0,0);
      SysFreeString(bstrText);
      if ( 0 == strcmp(szText,szCheckText) ) {
         p -> QueryInterface(IID_IDispatch,reinterpret_cast<void**>(pText));
         delete [] szText;
         delete [] szCheckText;
         return S_OK;
      }
     delete [] szCheckText;
   }
   delete [] szText;
   return S_OK;
   }


   STDMETHODIMP G::put_ShowFunctions(VARIANT_BOOL bShow) {
   showFunctions = bShow;
   ShowWindow(hwndDataSourcesDialog,bShow && functionList.Count() > 0 ? SW_SHOW : SW_HIDE);
   setDataSourcesVisibility(NULL,NULL);
   return S_OK;
   }


   STDMETHODIMP G::get_ShowFunctions(VARIANT_BOOL *bShow) {
   if ( ! bShow ) return E_POINTER;
   *bShow = showFunctions;
   return S_OK;
   }

   STDMETHODIMP G::put_AllowUserSetFunctionVisibility(VARIANT_BOOL doAllow) {
   allowUserFunctionControlVisibilityAccess = VARIANT_TRUE == doAllow ? TRUE : FALSE;
   return S_OK;
   }