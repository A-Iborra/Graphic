/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <olectl.h>

#include "resource.h"

#include "DataSet.h"

   STDMETHODIMP DataSet::SetClientSite(IOleClientSite *pcs) {
 
   if ( pIOleInPlaceSite ) 
      pIOleInPlaceSite -> Release();

   pIOleInPlaceSite = NULL;

   if ( pIOleClientSite ) 
      pIOleClientSite -> Release();

   pIOleClientSite = NULL;
 
   if ( ! pcs ) {
      DestroyWindow(hwndSpecDialog);
      hwndSpecDialog = NULL;
      return S_OK;
   }

   pIOleClientSite = pcs;
   pIOleClientSite -> AddRef();
   pIOleClientSite -> QueryInterface(IID_IOleInPlaceSite,(void **)&pIOleInPlaceSite);

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

   pSizel -> cx = containerSize.cx;
   pSizel -> cy = containerSize.cy;

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
      ShowWindow(hwndSpecDialog,SW_SHOW);
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
