/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include <olectl.h>

#include "utils.h"
#include "Graphic_resource.h"

#include "Text.h"
#include "Text_i.h"


  STDMETHODIMP Text::SetClientSite(IOleClientSite *pcs) {
  Text *p = static_cast<Text *>(this);

  if ( !pcs ) return E_INVALIDARG;

  if ( p -> pAmbientDispatch ) p -> pAmbientDispatch -> Release();
  if ( p -> pIOleInPlaceSite ) p -> pIOleInPlaceSite -> Release();
  if ( p -> pIOleClientSite ) p -> pIOleClientSite -> Release();

  p -> pIOleClientSite = pcs;
  p -> pIOleClientSite -> AddRef();

  p -> pIOleClientSite -> QueryInterface(IID_IDispatch,(void **)&p -> pAmbientDispatch);
  p -> pIOleClientSite -> QueryInterface(IID_IOleInPlaceSite,(void **)&p -> pIOleInPlaceSite);

  pIOleInPlaceSite -> GetWindow(&p -> hwndOwner);

  return S_OK;
  }


  STDMETHODIMP Text::GetClientSite(IOleClientSite **pcs) {
  Text *p = static_cast<Text *>(this);
  *pcs = p -> pIOleClientSite;
  p -> pIOleClientSite -> AddRef();
  return S_OK;
  }


  STDMETHODIMP Text::Advise(IAdviseSink *pAdvSink,DWORD *pdwConnection) {
  Text *p = static_cast<Text *>(this);
  if ( ! p -> pOleAdviseHolder )
     CreateOleAdviseHolder(&p -> pOleAdviseHolder);
  if ( p -> pOleAdviseHolder)
     p -> pOleAdviseHolder -> Advise(pAdvSink,pdwConnection);
  return S_OK;
  }


  STDMETHODIMP Text::Unadvise(DWORD dwConnection) {
  Text *p = static_cast<Text *>(this);
  if ( p -> pOleAdviseHolder)
     return p -> pOleAdviseHolder -> Unadvise(dwConnection);
  return S_OK;
  }


  STDMETHODIMP Text::EnumAdvise(IEnumSTATDATA **ppenum) {
  Text *p = static_cast<Text *>(this);
  if ( p -> pOleAdviseHolder)
     return p -> pOleAdviseHolder -> EnumAdvise(ppenum);
  return S_OK;
  }


  STDMETHODIMP Text::SetExtent(DWORD dwDrawAspect,SIZEL *pSizel) {

  if ( dwDrawAspect != DVASPECT_CONTENT ) return S_OK;

  Text *p = static_cast<Text *>(this);

  SIZEL tempSizel;
  RECT rect = {0,0,0,0};
  
  hiMetricToPixel(pSizel,&tempSizel);
  rect.right = tempSizel.cx;
  rect.bottom = tempSizel.cy;

  SetWindowPos(p -> hwndFrame,HWND_TOP,rect.left - 1,rect.top - 1,rect.right - rect.left + 2,rect.bottom - rect.top + 2,0L);

  memcpy(&p -> containerSize,&tempSizel,sizeof(SIZEL));

  return S_OK;
  }


  STDMETHODIMP Text::GetExtent(DWORD dwDrawAspect,SIZEL *pSizel) {
  Text *p = static_cast<Text *>(this);
  memcpy(pSizel,&p -> containerSize,sizeof(SIZEL));
  pixelsToHiMetric(pSizel,pSizel);
  return S_OK;
  }


  STDMETHODIMP Text::DoVerb(LONG iVerb, LPMSG ,IOleClientSite *, LONG,HWND hwndParent,LPCRECT lprcPosRect) {
  Text *p = static_cast<Text *>(this);
  switch ( iVerb ) {

  case OLEIVERB_PROPERTIES:
     SendMessage(p -> hwndObjectWindow,WM_COMMAND,MAKEWPARAM(IDMI_TEXT_PROPERTIES,0),0L);
     break;

  case OLEIVERB_UIACTIVATE:
     errorSound();
     break;

  case OLEIVERB_INPLACEACTIVATE:
  case OLEIVERB_SHOW: 
     SetWindowPos(p -> hwndFrame,HWND_TOP,
                     lprcPosRect -> left,lprcPosRect -> top,
                     lprcPosRect-> right - lprcPosRect -> left,lprcPosRect -> bottom - lprcPosRect -> top,SWP_SHOWWINDOW);
     break;

  default:
     break;
  }
  return S_OK;
  }


  STDMETHODIMP Text::SetHostNames(LPCOLESTR szContainerApp,LPCOLESTR olestrContainerObject) {
  Text *p = static_cast<Text *>(this);
  char szTemp[64];
  memset(szTemp,0,sizeof(szTemp));
  WideCharToMultiByte(CP_ACP,0,olestrContainerObject,wcslen(olestrContainerObject),szTemp,64,0,0);
//  p -> pIProperties -> put_Name(szTemp);
  return S_OK;
  }


  STDMETHODIMP Text::Close(DWORD dwOptions) {
  return S_OK;
  }


  STDMETHODIMP Text::SetMoniker(DWORD dwMonikerNo,IMoniker *pm) {
  return E_NOTIMPL;
  }


  STDMETHODIMP Text::GetMoniker(DWORD dwAssign,DWORD dwMonikerNo,IMoniker **ppm) {
  if ( !pIOleClientSite ) 
     return E_NOTIMPL;
  return pIOleClientSite -> GetMoniker(OLEGETMONIKER_FORCEASSIGN,OLEWHICHMK_OBJFULL,ppm);
  }


  STDMETHODIMP Text::InitFromData(IDataObject *pdo,BOOL fCreation,DWORD dwReserved) {
  return E_NOTIMPL;
  }


  STDMETHODIMP Text::GetClipboardData(DWORD,IDataObject **ppdo) {
  return E_NOTIMPL;
  }


  STDMETHODIMP Text::EnumVerbs(IEnumOLEVERB **ppEnumOleVerb) {
  return OleRegEnumVerbs(CLSID_Text,ppEnumOleVerb);
  }


  STDMETHODIMP Text::Update() {
  return S_OK;
  }


  STDMETHODIMP Text::IsUpToDate() {
  return S_OK;
  }


  STDMETHODIMP Text::GetUserClassID(CLSID *pclsid) {
  *pclsid = CLSID_Text;
  return S_OK;
  }


  STDMETHODIMP Text::GetUserType(DWORD dwTypeOfType,LPOLESTR *pszTypeName) {
  return OleRegGetUserType(CLSID_Text,dwTypeOfType,pszTypeName);
  }


  STDMETHODIMP Text::GetMiscStatus(DWORD dwAspect,DWORD *dwStatus) {
  return OleRegGetMiscStatus(CLSID_Text,dwAspect,dwStatus);
  }


  STDMETHODIMP Text::SetColorScheme(LOGPALETTE *) {
  return S_OK;
  }