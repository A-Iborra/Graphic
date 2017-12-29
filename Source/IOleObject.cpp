/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002,2008 Nathan T. Clark

*/

#include <windows.h>
#include <olectl.h>

#include "utils.h"

#include "Graphic.h"


   STDMETHODIMP G::SetClientSite(IOleClientSite *pcs) {
 
   if ( pIOleInPlaceSite ) 
      pIOleInPlaceSite -> Release();
   pIOleInPlaceSite = NULL;

   if ( pIOleClientSite ) 
      pIOleClientSite -> Release();
   pIOleClientSite = NULL;
 
   if ( ! pcs ) {
      DestroyWindow(hwndFrame);
      hwndFrame = 0L;
      return S_OK;
   }
 
   pIOleClientSite = pcs;
   pIOleClientSite -> AddRef();

   pIOleClientSite -> QueryInterface(IID_IOleInPlaceSite,(void **)&pIOleInPlaceSite);

   IDispatch* pIDispatch;

   HRESULT rc = pIOleClientSite -> QueryInterface(IID_IDispatch,reinterpret_cast<void **>(&pIDispatch));

   if ( S_OK == rc ) {
      DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
      VARIANT var = {VT_EMPTY};
      var.vt = VT_BOOL;
      pIDispatch -> Invoke(DISPID_AMBIENT_USERMODE,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_PROPERTYGET,&dispparamsNoArgs,&var,NULL,NULL);
      pIDispatch -> Release();
      isRunning = (var.bVal == 0 ? false : true);
   } else
      isRunning = true;

   initWindows();

   ShowWindow(hwndFrame,SW_SHOW);
   
   return S_OK;
   }
 
 
   STDMETHODIMP G::GetClientSite(IOleClientSite **pcs) {
   *pcs = pIOleClientSite;
   if ( pIOleClientSite )
      pIOleClientSite -> AddRef();
   return S_OK;
   }
 
 
   STDMETHODIMP G::Advise(IAdviseSink *pAdvSink,DWORD *pdwConnection) {
   if ( ! pOleAdviseHolder )
      CreateOleAdviseHolder(&pOleAdviseHolder);
   if ( pOleAdviseHolder)
      pOleAdviseHolder -> Advise(pAdvSink,pdwConnection);
   return S_OK;
   }
 
 
   STDMETHODIMP G::Unadvise(DWORD dwConnection) {
   if ( pOleAdviseHolder)
      return pOleAdviseHolder -> Unadvise(dwConnection);
   return S_OK;
   }
 
 
   STDMETHODIMP G::EnumAdvise(IEnumSTATDATA **ppenum) {
   if ( pOleAdviseHolder)
      return pOleAdviseHolder -> EnumAdvise(ppenum);
   return S_OK;
   }
 
 
   STDMETHODIMP G::SetExtent(DWORD dwDrawAspect,SIZEL *pSizel) {
   if ( dwDrawAspect != DVASPECT_CONTENT ) return S_OK;
   SIZEL tempSizel;
   RECT rect = {0,0,0,0};
   hiMetricToPixel(pSizel,&tempSizel);
   rect.right = tempSizel.cx;
   rect.bottom = tempSizel.cy;
   //SetWindowPos(hwndFrame,HWND_TOP,0L + 4,0L + 4,rect.right - rect.left - 8,rect.bottom - rect.top - 8,SWP_NOMOVE);
   SetWindowPos(hwndFrame,HWND_TOP,0L,0L,rect.right - rect.left,rect.bottom - rect.top,SWP_NOMOVE);
   memcpy(&containerSize,&tempSizel,sizeof(SIZEL));
   return S_OK;
   }
 
 
   STDMETHODIMP G::GetExtent(DWORD dwDrawAspect,SIZEL *pSizel) {
   memcpy(pSizel,&containerSize,sizeof(SIZEL));
   pixelsToHiMetric(pSizel,pSizel);
   return S_OK;
   }
 
 
   STDMETHODIMP G::DoVerb(LONG iVerb, LPMSG ,IOleClientSite *, LONG,HWND hwndParent,LPCRECT lprcPosRect) {

   switch ( iVerb ) {
 
   case OLEIVERB_PROPERTIES:
      pIGProperties  -> ShowProperties(hwndOwner,pIUnknownThis);
      break;
 
   case OLEIVERB_UIACTIVATE:
      return E_NOTIMPL;
 
   case OLEIVERB_INPLACEACTIVATE:
      SetObjectRects(lprcPosRect,NULL);
      pIOleInPlaceSite -> OnInPlaceActivate();
      break;

   case OLEIVERB_SHOW: 
      if ( ! hwndFrame ) {
         hwndOwner = hwndParent;
         initWindows();
         SetWindowPos(hwndFrame,HWND_TOP,
                        lprcPosRect -> left + 4,lprcPosRect -> top + 4,
                            lprcPosRect-> right - lprcPosRect -> left - 8,lprcPosRect -> bottom - lprcPosRect -> top - 8,0L);
      }
      ShowWindow(hwndFrame,SW_SHOW);
      break;
 
   default:
      break;
   }
   return S_OK;
   }
 
 
   STDMETHODIMP G::SetHostNames(LPCOLESTR szContainerApp,LPCOLESTR olestrContainerObject) {
   char szTemp[64];
   memset(szTemp,0,sizeof(szTemp));
   WideCharToMultiByte(CP_ACP,0,olestrContainerObject,wcslen(olestrContainerObject),szTemp,64,0,0);
   strncpy(szName,szTemp,sizeof(szName));
//   name(szTemp);
   return S_OK;
   }
 
 
   STDMETHODIMP G::Close(DWORD dwOptions) {
   return S_OK;
   }
 
 
   STDMETHODIMP G::SetMoniker(DWORD dwMonikerNo,IMoniker *pm) {
   return E_NOTIMPL;
   }
 
 
   STDMETHODIMP G::GetMoniker(DWORD dwAssign,DWORD dwMonikerNo,IMoniker **ppm) {
   if ( ! pIOleClientSite ) 
      return E_NOTIMPL;
   return pIOleClientSite -> GetMoniker(OLEGETMONIKER_FORCEASSIGN,OLEWHICHMK_OBJFULL,ppm);
   }
 
 
   STDMETHODIMP G::InitFromData(IDataObject *pdo,BOOL fCreation,DWORD dwReserved) {
   return E_NOTIMPL;
   }
 
 
   STDMETHODIMP G::GetClipboardData(DWORD,IDataObject **ppdo) {
   return E_NOTIMPL;
   }
 
 
   STDMETHODIMP G::EnumVerbs(IEnumOLEVERB **ppEnumOleVerb) {
   return OleRegEnumVerbs(CLSID_GSystemGraphic,ppEnumOleVerb);
   }
 
 
   STDMETHODIMP G::Update() {
   return S_OK;
   }
 
 
   STDMETHODIMP G::IsUpToDate() {
   return S_OK;
   }
 
 
   STDMETHODIMP G::GetUserClassID(CLSID *pclsid) {
   *pclsid = CLSID_GSystemGraphic;
   return S_OK;
   }
 
 
   STDMETHODIMP G::GetUserType(DWORD dwTypeOfType,LPOLESTR *pszTypeName) {
   return OleRegGetUserType(CLSID_GSystemGraphic,dwTypeOfType,pszTypeName);
   }
 
 
   STDMETHODIMP G::GetMiscStatus(DWORD dwAspect,DWORD *dwStatus) {
   if ( dwAspect == DVASPECT_CONTENT )
      *dwStatus = oleMisc;
   else
      *dwStatus = 0;
//   return OleRegGetMiscStatus(CLSID_GSystemGraphic,dwAspect,dwStatus);
   return S_OK;
   }
 
 
   STDMETHODIMP G::SetColorScheme(LOGPALETTE *) {
   return S_OK;
   }