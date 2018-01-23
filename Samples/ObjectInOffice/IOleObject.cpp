// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ObjectInOffice.h"
#include <OleCtl.h>
#include "utils.h"


   STDMETHODIMP ObjectInOffice::SetClientSite(IOleClientSite *pcs) {
 
  if ( pIOleInPlaceSite ) 
      pIOleInPlaceSite -> Release();
   pIOleInPlaceSite = NULL;

   if ( pIOleClientSite ) 
      pIOleClientSite -> Release();
   pIOleClientSite = NULL;
 
   if ( ! pcs ) {
      DestroyWindow(hwndMain);
      hwndMain = 0L;
      return S_OK;
   }
 
   pIOleClientSite = pcs;
   pIOleClientSite -> AddRef();

   pIOleClientSite -> QueryInterface(IID_IOleInPlaceSite,(void **)&pIOleInPlaceSite);

   //IDispatch* pIDispatch;

   //HRESULT rc = pIOleClientSite -> QueryInterface(IID_IDispatch,reinterpret_cast<void **>(&pIDispatch));

   //if ( S_OK == rc ) {
   //   DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
   //   VARIANT var = {VT_EMPTY};
   //   var.vt = VT_BOOL;
   //   pIDispatch -> Invoke(DISPID_AMBIENT_USERMODE,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_PROPERTYGET,&dispparamsNoArgs,&var,NULL,NULL);
   //   pIDispatch -> Release();
   //   isRunning = (var.bVal == 0 ? false : true);
   //} else
   //   isRunning = true;

   initWindows();

   ShowWindow(hwndMain,SW_SHOW);

   return S_OK;
   }
 
 
   STDMETHODIMP ObjectInOffice::GetClientSite(IOleClientSite **pcs) {
   *pcs = pIOleClientSite;
   if ( pIOleClientSite )
      pIOleClientSite -> AddRef();
   return S_OK;
   }
 
 
   STDMETHODIMP ObjectInOffice::Advise(IAdviseSink *pAdvSink,DWORD *pdwConnection) {
   if ( ! pOleAdviseHolder )
      CreateOleAdviseHolder(&pOleAdviseHolder);
   if ( pOleAdviseHolder)
      pOleAdviseHolder -> Advise(pAdvSink,pdwConnection);
   return S_OK;
   }
 
 
   STDMETHODIMP ObjectInOffice::Unadvise(DWORD dwConnection) {
   if ( pOleAdviseHolder)
      return pOleAdviseHolder -> Unadvise(dwConnection);
   return S_OK;
   }
 
 
   STDMETHODIMP ObjectInOffice::EnumAdvise(IEnumSTATDATA **ppenum) {
   if ( pOleAdviseHolder)
      return pOleAdviseHolder -> EnumAdvise(ppenum);
   return S_OK;
   }
 
 
   STDMETHODIMP ObjectInOffice::SetExtent(DWORD dwDrawAspect,SIZEL *pSizel) {
   if ( dwDrawAspect != DVASPECT_CONTENT ) return S_OK;
   SIZEL tempSizel;
   RECT rect = {0,0,0,0};
   hiMetricToPixel(pSizel,&tempSizel);
   rect.right = tempSizel.cx;
   rect.bottom = tempSizel.cy;
   SetWindowPos(hwndMain,HWND_TOP,0L,0L,rect.right - rect.left,rect.bottom - rect.top,SWP_NOMOVE);
   return S_OK;
   }
 
 
   STDMETHODIMP ObjectInOffice::GetExtent(DWORD dwDrawAspect,SIZEL *pSizel) {
   RECT rc;
   GetWindowRect(hwndMain,&rc);
   pSizel -> cx = rc.right - rc.left;
   pSizel -> cy = rc.bottom - rc.top;
   pixelsToHiMetric(pSizel,pSizel);
   return S_OK;
   }
 
 
   STDMETHODIMP ObjectInOffice::DoVerb(LONG iVerb, LPMSG ,IOleClientSite *, LONG,HWND hwndParent,LPCRECT lprcPosRect) {

   switch ( iVerb ) {
 
   case OLEIVERB_PROPERTIES:
      //pIGProperties  -> ShowProperties(hwndOwner,pIUnknownThis);
      break;
 
   case OLEIVERB_UIACTIVATE:
      return E_NOTIMPL;
 
   case OLEIVERB_INPLACEACTIVATE:
      //SetObjectRects(lprcPosRect,NULL);
      pIOleInPlaceSite -> OnInPlaceActivate();
      break;

   case OLEIVERB_SHOW: 
      if ( ! hwndMain ) {
         hwndOwner = hwndParent;
         initWindows();
      }
      SetWindowPos(hwndMain,HWND_TOP,
                     lprcPosRect -> left + 4,lprcPosRect -> top + 4,
                           lprcPosRect-> right - lprcPosRect -> left - 8,lprcPosRect -> bottom - lprcPosRect -> top - 8,SWP_SHOWWINDOW);
      break;
 
   default:
      break;
   }

   return S_OK;
   }
 
 
   STDMETHODIMP ObjectInOffice::SetHostNames(LPCOLESTR szContainerApp,LPCOLESTR olestrContainerObject) {
   //char szTemp[64];
   //memset(szTemp,0,sizeof(szTemp));
   //WideCharToMultiByte(CP_ACP,0,olestrContainerObject,wcslen(olestrContainerObject),szTemp,64,0,0);
   //strncpy(szName,szTemp,sizeof(szName));
   return S_OK;
   }
 
 
   STDMETHODIMP ObjectInOffice::Close(DWORD dwOptions) {
   return S_OK;
   }
 
 
   STDMETHODIMP ObjectInOffice::SetMoniker(DWORD dwMonikerNo,IMoniker *pm) {
   return E_NOTIMPL;
   }
 
 
   STDMETHODIMP ObjectInOffice::GetMoniker(DWORD dwAssign,DWORD dwMonikerNo,IMoniker **ppm) {
   if ( ! pIOleClientSite ) 
      return E_NOTIMPL;
   return pIOleClientSite -> GetMoniker(OLEGETMONIKER_FORCEASSIGN,OLEWHICHMK_OBJFULL,ppm);
   }
 
 
   STDMETHODIMP ObjectInOffice::InitFromData(IDataObject *pdo,BOOL fCreation,DWORD dwReserved) {
   return E_NOTIMPL;
   }
 
 
   STDMETHODIMP ObjectInOffice::GetClipboardData(DWORD,IDataObject **ppdo) {
   return E_NOTIMPL;
   }
 
 
   STDMETHODIMP ObjectInOffice::EnumVerbs(IEnumOLEVERB **ppEnumOleVerb) {
   return OleRegEnumVerbs(CLSID_ObjectInOffice,ppEnumOleVerb);
   }
 
 
   STDMETHODIMP ObjectInOffice::Update() {
   return S_OK;
   }
 
 
   STDMETHODIMP ObjectInOffice::IsUpToDate() {
   return S_OK;
   }
 
 
   STDMETHODIMP ObjectInOffice::GetUserClassID(CLSID *pclsid) {
   *pclsid = CLSID_ObjectInOffice;
   return S_OK;
   }
 
 
   STDMETHODIMP ObjectInOffice::GetUserType(DWORD dwTypeOfType,LPOLESTR *pszTypeName) {
   return OleRegGetUserType(CLSID_ObjectInOffice,dwTypeOfType,pszTypeName);
   }
 
 
   STDMETHODIMP ObjectInOffice::GetMiscStatus(DWORD dwAspect,DWORD *pdwStatus) {
   if ( dwAspect == DVASPECT_CONTENT )
      *pdwStatus = oleMisc;
   else
      *pdwStatus = 0;
//   return OleRegGetMiscStatus(CLSID_GSystemGraphic,dwAspect,dwStatus);
   return S_OK;
   }
 
 
   STDMETHODIMP ObjectInOffice::SetColorScheme(LOGPALETTE *) {
   return S_OK;
   }