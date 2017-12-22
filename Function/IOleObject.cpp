/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <olectl.h>

#include "resource.h"

#include "Function.h"


   STDMETHODIMP Function::SetClientSite(IOleClientSite *pcs) {
 
   if ( pIOleInPlaceSite ) 
      pIOleInPlaceSite -> Release();

   pIOleInPlaceSite = NULL;

   if ( pIOleClientSite ) 
      pIOleClientSite -> Release();

   pIOleClientSite = NULL;
 
   if ( ! pcs ) {
      DestroyWindow(hwndSpecDialog);
      hwndSpecDialog = 0L;
      return S_OK;
   }
 
   pIOleClientSite = pcs;
   pIOleClientSite -> AddRef();
   pIOleClientSite -> QueryInterface(IID_IOleInPlaceSite,(void **)&pIOleInPlaceSite);

   IDispatch *pIDispatch;

   if ( pIOleClientSite && SUCCEEDED(pIOleClientSite -> QueryInterface(IID_IDispatch,reinterpret_cast<void**>(&pIDispatch))) ) {

	   DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
      VARIANT varBackColor;
      VARIANT varFont;
      VARIANT varForeColor;
      VARIANT varUserMode;

      memset(&varBackColor,0,sizeof(VARIANT));
      memset(&varFont,0,sizeof(VARIANT));
      memset(&varForeColor,0,sizeof(VARIANT));

      varUserMode.vt = VT_BOOL;

      VariantClear(&varBackColor);
      VariantClear(&varFont);
      VariantClear(&varForeColor);
      VariantClear(&varUserMode);

      pIDispatch -> Invoke(DISPID_AMBIENT_BACKCOLOR,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_PROPERTYGET,&dispparamsNoArgs,&varBackColor,NULL,NULL);
      pIDispatch -> Invoke(DISPID_AMBIENT_FORECOLOR,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_PROPERTYGET,&dispparamsNoArgs,&varForeColor,NULL,NULL);
      pIDispatch -> Invoke(DISPID_AMBIENT_FONT,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_PROPERTYGET,&dispparamsNoArgs,&varFont,NULL,NULL);
      pIDispatch -> Invoke(DISPID_AMBIENT_USERMODE,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_PROPERTYGET,&dispparamsNoArgs,&varUserMode,NULL,NULL);

      pIDispatch -> Release();

      OleTranslateColor(varBackColor.lVal,NULL,&backgroundColor);
      OleTranslateColor(varForeColor.lVal,NULL,&foregroundColor);

      userMode = varUserMode.bVal;
   }

   initWindows();

   return S_OK;
   }
 
 
   STDMETHODIMP Function::GetClientSite(IOleClientSite **pcs) {
   *pcs = pIOleClientSite;
   pIOleClientSite -> AddRef();
   return S_OK;
   }
 
 
   STDMETHODIMP Function::Advise(IAdviseSink *pAdvSink,DWORD *pdwConnection) {
   HRESULT hr = S_OK;
   if ( ! pOleAdviseHolder )
      hr = CreateOleAdviseHolder(&pOleAdviseHolder);
   if ( FAILED(hr) ) 
      return hr;
   return pOleAdviseHolder -> Advise(pAdvSink,pdwConnection);
   }
 
 
   STDMETHODIMP Function::Unadvise(DWORD dwConnection) {
   HRESULT hr = E_FAIL;
   if ( pOleAdviseHolder)
      hr = pOleAdviseHolder -> Unadvise(dwConnection);
   return hr;
   }
 
 
   STDMETHODIMP Function::EnumAdvise(IEnumSTATDATA **ppenum) {
   HRESULT hr = E_FAIL;
   if ( pOleAdviseHolder)
      hr = pOleAdviseHolder -> EnumAdvise(ppenum);
   return hr;
   }
 
 
   STDMETHODIMP Function::SetExtent(DWORD dwDrawAspect,SIZEL *pSizel) {
   if ( ! ( dwDrawAspect & DVASPECT_CONTENT ) )
      return E_NOTIMPL;
   SIZEL sizel{pSizel -> cx,pSizel -> cy};
   hiMetricToPixel(&sizel,&sizel);
   resultingWidth = sizel.cx;
   resultingHeight = sizel.cy;
   SetWindowPos(hwndSpecDialog,HWND_TOP,0,0,resultingWidth,resultingHeight,SWP_NOMOVE);
   return S_OK;
   }
 
 
   STDMETHODIMP Function::GetExtent(DWORD dwDrawAspect,SIZEL *pSizel) {

   if ( ! ( dwDrawAspect & DVASPECT_CONTENT) )
      return E_NOTIMPL;

   if ( ! hwndSpecDialog )
      initWindows();

   resize();

   pSizel -> cx = resultingWidth;
   pSizel -> cy = resultingHeight;

   pixelsToHiMetric(pSizel,pSizel);

   return S_OK;
   }
 
 
   STDMETHODIMP Function::DoVerb(LONG iVerb,LPMSG,IOleClientSite *ipc,LONG,HWND hwndParent,LPCRECT prc) {
   switch ( iVerb ) {
   case OLEIVERB_PROPERTIES: {
      IUnknown* pIUnknown;
      QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknown));
      iProperties -> ShowProperties(hwndParent,pIUnknown);
      pIUnknown -> Release();
      }
      break;
   case OLEIVERB_SHOW:
      if ( ! hwndSpecDialog ) 
         initWindows();
      if ( ! anyVisible() )
         return S_OK;
      SetObjectRects(prc,NULL);
      resize();
      ShowWindow(hwndSpecDialog,SW_SHOW);
      break;
   case OLEIVERB_HIDE:
      ShowWindow(GetParent(hwndSpecDialog),SW_HIDE);
      ShowWindow(hwndSpecDialog,SW_HIDE);
      break;
   case OLEIVERB_UIACTIVATE:
      return E_NOTIMPL;
   case OLEIVERB_INPLACEACTIVATE:
      if ( ! hwndSpecDialog )
         initWindows();
      SetWindowPos(hwndSpecDialog,HWND_TOP,prc -> left,prc -> top,prc -> right - prc -> left,prc -> bottom - prc -> top,SWP_SHOWWINDOW);
      SetObjectRects(prc,NULL);
      pIOleInPlaceSite -> OnInPlaceActivate();
      break;
   default:
      break;
   }
   return S_OK;
   }
 
 
   STDMETHODIMP Function::SetHostNames(LPCOLESTR szContainerApp,LPCOLESTR olestrContainerObject) {
   return S_OK;
   }
 
 
   STDMETHODIMP Function::Close(DWORD dwOptions) {
   return S_OK;
   }
 
 
   STDMETHODIMP Function::SetMoniker(DWORD dwMonikerNo,IMoniker *pm) {
   return E_NOTIMPL;
   }
 
 
   STDMETHODIMP Function::GetMoniker(DWORD dwAssign,DWORD dwMonikerNo,IMoniker **ppm) {
   return E_NOTIMPL;
   }
 
 
   STDMETHODIMP Function::InitFromData(IDataObject *pdo,BOOL fCreation,DWORD dwReserved) {
   return E_NOTIMPL;
   }
 
 
   STDMETHODIMP Function::GetClipboardData(DWORD,IDataObject **ppdo) {
   return E_NOTIMPL;
   }
 
 
   STDMETHODIMP Function::EnumVerbs(IEnumOLEVERB **ppEnumOleVerb) {
#ifdef FUNCTION_SAMPLE
   HRESULT hr = OleRegEnumVerbs(CLSID_GSystemFunctioNaterSample,ppEnumOleVerb);
#else
   HRESULT hr = OleRegEnumVerbs(CLSID_GSystemFunctioNater,ppEnumOleVerb);
#endif
   return hr;
   }
 
 
   STDMETHODIMP Function::Update() {
   return S_OK;
   }
 
 
   STDMETHODIMP Function::IsUpToDate() {
   return S_OK;
   }
 
 
   STDMETHODIMP Function::GetUserClassID(CLSID *pclsid) {
#ifdef FUNCTION_SAMPLE
   *pclsid = CLSID_GSystemFunctioNaterSample;
#else
   *pclsid = CLSID_GSystemFunctioNater;
#endif
   return S_OK;
   }
 
 
   STDMETHODIMP Function::GetUserType(DWORD dwTypeOfType,LPOLESTR *pszTypeName) {
#ifdef FUNCTION_SAMPLE
   return OleRegGetUserType(CLSID_GSystemFunctioNaterSample,dwTypeOfType,pszTypeName);
#else
   return OleRegGetUserType(CLSID_GSystemFunctioNater,dwTypeOfType,pszTypeName);
#endif
   }
 
 
   STDMETHODIMP Function::GetMiscStatus(DWORD dwAspect,DWORD *dwStatus) {
   if ( dwAspect == DVASPECT_CONTENT )
      *dwStatus = oleMiscStatus;
   else
      *dwStatus = 0;
   return S_OK;
   }
 
 
   STDMETHODIMP Function::SetColorScheme(LOGPALETTE *) {
   return E_NOTIMPL;
   }