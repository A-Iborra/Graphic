/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <olectl.h>

#include "resource.h"

#include "DataSet.h"

   LONG APIENTRY CalcDimensions(HWND hwndParent,SIZEL *pResult);

   STDMETHODIMP DataSet::SetClientSite(IOleClientSite *pcs) {
 
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

      isDesignMode = ! varUserMode.bVal;

   }

   initWindows();

   return S_OK;
   }
 
 
   STDMETHODIMP DataSet::GetClientSite(IOleClientSite **pcs) {
   *pcs = pIOleClientSite;
   pIOleClientSite -> AddRef();
   return S_OK;
   }
 
 
   STDMETHODIMP DataSet::Advise(IAdviseSink *pAdvSink,DWORD *pdwConnection) {
   HRESULT hr = S_OK;
   if ( ! pOleAdviseHolder )
      hr = CreateOleAdviseHolder(&pOleAdviseHolder);
   if ( FAILED(hr) ) 
      return hr;
   return pOleAdviseHolder -> Advise(pAdvSink,pdwConnection);
   }
 
 
   STDMETHODIMP DataSet::Unadvise(DWORD dwConnection) {
   HRESULT hr = E_FAIL;
   if ( pOleAdviseHolder)
      hr = pOleAdviseHolder -> Unadvise(dwConnection);
   return hr;
   }
 
 
   STDMETHODIMP DataSet::EnumAdvise(IEnumSTATDATA **ppenum) {
   HRESULT hr = E_FAIL;
   if ( pOleAdviseHolder)
      hr = pOleAdviseHolder -> EnumAdvise(ppenum);
   return hr;
   }
 
 
   STDMETHODIMP DataSet::SetExtent(DWORD dwDrawAspect,SIZEL *pSizel) {
   if ( ! ( dwDrawAspect & DVASPECT_CONTENT ) )
      return E_NOTIMPL;
   SIZEL containerSize;
   hiMetricToPixel(pSizel,&containerSize);
   if ( hwndSpecDialog )
      SetWindowPos(hwndSpecDialog,HWND_TOP,0,0,containerSize.cx,containerSize.cy,SWP_NOMOVE);
   return S_OK;
   }
 
 
   STDMETHODIMP DataSet::GetExtent(DWORD dwDrawAspect,SIZEL *pSizel) {

   if ( ! ( dwDrawAspect & DVASPECT_CONTENT) )
      return E_NOTIMPL;

   if ( ! hwndSpecDialog )
      initWindows();

   RECT rc;

   GetWindowRect(hwndSpecDialog,&rc);

   pSizel -> cx = rc.right - rc.left;
   pSizel -> cy = rc.bottom - rc.top;

   pixelsToHiMetric(pSizel,pSizel);

   return S_OK;
   }
 
 
   STDMETHODIMP DataSet::DoVerb(LONG iVerb,LPMSG,IOleClientSite *ipc,LONG,HWND hwndParent,LPCRECT prc) {

   switch ( iVerb ) {
   case OLEIVERB_PROPERTIES: {
      IUnknown* pIUnknown;
      QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknown));
      pIGProperties -> ShowProperties(hwndParent,pIUnknown);
      pIUnknown -> Release();
      }
      break;
   case OLEIVERB_SHOW:
      if ( ! hwndSpecDialog ) 
         initWindows();
      SetObjectRects(prc,NULL);
      SetWindowPos(hwndSpecDialog,HWND_TOP,prc -> left,prc -> top,prc -> right - prc -> left,prc -> bottom - prc -> top,SWP_SHOWWINDOW);
      break;
   case OLEIVERB_HIDE:
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
 
 
   STDMETHODIMP DataSet::SetHostNames(LPCOLESTR szContainerApp,LPCOLESTR olestrContainerObject) {
   return S_OK;
   }
 
 
   STDMETHODIMP DataSet::Close(DWORD dwOptions) {
   return S_OK;
   }
 
 
   STDMETHODIMP DataSet::SetMoniker(DWORD dwMonikerNo,IMoniker *pm) {
   return E_NOTIMPL;
   }
 
 
   STDMETHODIMP DataSet::GetMoniker(DWORD dwAssign,DWORD dwMonikerNo,IMoniker **ppm) {
   return E_NOTIMPL;
   }
 
 
   STDMETHODIMP DataSet::InitFromData(IDataObject *pdo,BOOL fCreation,DWORD dwReserved) {
   return E_NOTIMPL;
   }
 
 
   STDMETHODIMP DataSet::GetClipboardData(DWORD,IDataObject **ppdo) {
   return E_NOTIMPL;
   }
 
 
   STDMETHODIMP DataSet::EnumVerbs(IEnumOLEVERB **ppEnumOleVerb) {
#ifdef FUNCTION_SAMPLE
   HRESULT hr = OleRegEnumVerbs(CLSID_GSystemFunctioNaterSample,ppEnumOleVerb);
#else
   HRESULT hr = OleRegEnumVerbs(CLSID_DataSet,ppEnumOleVerb);
#endif
   return hr;
   }
 
 
   STDMETHODIMP DataSet::Update() {
   return S_OK;
   }
 
 
   STDMETHODIMP DataSet::IsUpToDate() {
   return S_OK;
   }
 
 
   STDMETHODIMP DataSet::GetUserClassID(CLSID *pclsid) {
   *pclsid = CLSID_DataSet;
   return S_OK;
   }
 
 
   STDMETHODIMP DataSet::GetUserType(DWORD dwTypeOfType,LPOLESTR *pszTypeName) {
   return OleRegGetUserType(CLSID_DataSet,dwTypeOfType,pszTypeName);
   }
 
 
   STDMETHODIMP DataSet::GetMiscStatus(DWORD dwAspect,DWORD *dwStatus) {
   if ( dwAspect == DVASPECT_CONTENT )
      *dwStatus = oleMiscStatus;
   else
      *dwStatus = 0;
   return S_OK;
   }
 
 
   STDMETHODIMP DataSet::SetColorScheme(LOGPALETTE *) {
   return E_NOTIMPL;
   }


   BOOL CALLBACK testDimensions(HWND hwndTest,LPARAM lParam);

   static long maxHeight;
   static long maxWidth;

   LONG APIENTRY CalcDimensions(HWND hwndParent,SIZEL *pResult) {
   maxHeight = 0L;
   maxWidth = 0L;
   EnumChildWindows(hwndParent,testDimensions,(LPARAM)hwndParent);
   pResult -> cx = maxWidth;
   pResult -> cy = maxHeight;
   return 0;
   }

   BOOL CALLBACK testDimensions(HWND hwndTest,LPARAM lParam) {

   if ( ! IsWindowVisible(hwndTest) )
      return TRUE;

   RECT rcChild;
   RECT rcParent;
   GetWindowRect((HWND)lParam,&rcParent);
   GetWindowRect(hwndTest,&rcChild);

   maxHeight = max(maxHeight,rcChild.bottom - rcParent.top);
   maxWidth = max(maxWidth,rcChild.right - rcParent.left);

   return TRUE;
   }