/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <olectl.h>

#include "utils.h"

#include "OpenGLImplementor.h"


   STDMETHODIMP OpenGLImplementor::SetClientSite(IOleClientSite *pcs) {
 
   if ( ! pcs ) return E_INVALIDARG;
 
   if ( pIOleClientSite ) pIOleClientSite -> Release();

   pIOleClientSite = pcs;
   pIOleClientSite -> AddRef();

   if ( pIOleInPlaceSite ) pIOleInPlaceSite -> Release();
   pIOleInPlaceSite = NULL;
   pIOleClientSite -> QueryInterface(IID_IOleInPlaceSite,(void **)&pIOleInPlaceSite);

   return S_OK;
   }
 
 
   STDMETHODIMP OpenGLImplementor::GetClientSite(IOleClientSite **pcs) {
   *pcs = pIOleClientSite;
   pIOleClientSite -> AddRef();
   return S_OK;
   }
 
 
   STDMETHODIMP OpenGLImplementor::Advise(IAdviseSink *pAdvSink,DWORD *pdwConnection) {
   if ( ! pIOleAdviseHolder )
      CreateOleAdviseHolder(&pIOleAdviseHolder);
   if ( pIOleAdviseHolder)
      pIOleAdviseHolder -> Advise(pAdvSink,pdwConnection);
   return S_OK;
   }
 
 
   STDMETHODIMP OpenGLImplementor::Unadvise(DWORD dwConnection) {
   if ( pIOleAdviseHolder)
      return pIOleAdviseHolder -> Unadvise(dwConnection);
   return S_OK;
   }
 
 
   STDMETHODIMP OpenGLImplementor::EnumAdvise(IEnumSTATDATA **ppenum) {
   if ( pIOleAdviseHolder)
      return pIOleAdviseHolder -> EnumAdvise(ppenum);
   return S_OK;
   }
 
 
   STDMETHODIMP OpenGLImplementor::SetExtent(DWORD dwDrawAspect,SIZEL *pSizel) {
 
   if ( dwDrawAspect != DVASPECT_CONTENT ) return S_OK;

   SIZEL tempSizel;
   RECT rect = {0,0,0,0};
   
   hiMetricToPixel(pSizel,&tempSizel);
   rect.right = tempSizel.cx;
   rect.bottom = tempSizel.cy;
   SetWindowPos(hwndControl,HWND_TOP,0L,0L,rect.right - rect.left + 2,rect.bottom - rect.top + 2,SWP_NOMOVE);
 
   pIPropertySize -> put_binaryValue(sizeof(SIZEL),reinterpret_cast<unsigned char *>(&tempSizel));

   return S_OK;
   }
 
 
   STDMETHODIMP OpenGLImplementor::GetExtent(DWORD dwDrawAspect,SIZEL *pSizel) {
   pIPropertySize -> get_binaryValue(sizeof(SIZEL),reinterpret_cast<unsigned char **>(&pSizel));
   pixelsToHiMetric(pSizel,pSizel);
   return S_OK;
   }
 
 
   STDMETHODIMP OpenGLImplementor::DoVerb(LONG iVerb,LPMSG,IOleClientSite *,LONG,HWND hwndP,LPCRECT lprcPosRect) {
   switch ( iVerb ) {
 
   case OLEIVERB_PROPERTIES: {
      IUnknown* pIUnknown;
      QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknown));
      pIProperties -> ShowProperties(hwndParent,pIUnknown);
      pIUnknown -> Release();
      }
      break;
 
   case OLEIVERB_UIACTIVATE:
      break;
 
   case OLEIVERB_INPLACEACTIVATE:
   case OLEIVERB_SHOW: 
      if ( ! hwndControl ) {
         pIOleInPlaceSite -> GetWindow(&hwndParent);
         initWindows();
      }
      ShowWindow(hwndControl,SW_SHOW);
      break;
 
   default:
      break;
   }

   return S_OK;
   }
 
 
   STDMETHODIMP OpenGLImplementor::SetHostNames(LPCOLESTR szContainerApp,LPCOLESTR olestrContainerObject) {
   return S_OK;
   }
 
 
   STDMETHODIMP OpenGLImplementor::Close(DWORD dwOptions) {
   return S_OK;
   }
 
 
   STDMETHODIMP OpenGLImplementor::SetMoniker(DWORD dwMonikerNo,IMoniker *pm) {
   return E_NOTIMPL;
   }
 
 
   STDMETHODIMP OpenGLImplementor::GetMoniker(DWORD dwAssign,DWORD dwMonikerNo,IMoniker **ppm) {
   if ( ! pIOleClientSite ) 
      return E_NOTIMPL;
   return pIOleClientSite -> GetMoniker(OLEGETMONIKER_FORCEASSIGN,OLEWHICHMK_OBJFULL,ppm);
   return E_NOTIMPL;
   }
 
 
   STDMETHODIMP OpenGLImplementor::InitFromData(IDataObject *pdo,BOOL fCreation,DWORD dwReserved) {
   return E_NOTIMPL;
   }
 
 
   STDMETHODIMP OpenGLImplementor::GetClipboardData(DWORD,IDataObject **ppdo) {
   return E_NOTIMPL;
   }
 
 
   STDMETHODIMP OpenGLImplementor::EnumVerbs(IEnumOLEVERB **ppEnumOleVerb) {
   return OleRegEnumVerbs(CLSID_OpenGLImplementor,ppEnumOleVerb);
   }
 
 
   STDMETHODIMP OpenGLImplementor::Update() {
   return S_OK;
   }
 
 
   STDMETHODIMP OpenGLImplementor::IsUpToDate() {
   return S_OK;
   }
 
 
   STDMETHODIMP OpenGLImplementor::GetUserClassID(CLSID *pclsid) {
   *pclsid = CLSID_OpenGLImplementor;
   return S_OK;
   }
 
 
   STDMETHODIMP OpenGLImplementor::GetUserType(DWORD dwTypeOfType,LPOLESTR *pszTypeName) {
   return OleRegGetUserType(CLSID_OpenGLImplementor,dwTypeOfType,pszTypeName);
   }
 
 
   STDMETHODIMP OpenGLImplementor::GetMiscStatus(DWORD dwAspect,DWORD *dwStatus) {
   return OleRegGetMiscStatus(CLSID_OpenGLImplementor,dwAspect,dwStatus);
   }
 
 
   STDMETHODIMP OpenGLImplementor::SetColorScheme(LOGPALETTE *) {
   return S_OK;
   }