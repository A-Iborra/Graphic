/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include <olectl.h>

#include "DataSet.h"


  HRESULT DataSet::_IOleInPlaceActiveObject::QueryInterface(REFIID riid,void **ppv) {
  return pParent -> QueryInterface(riid,ppv);
  }

  STDMETHODIMP_(ULONG) DataSet::_IOleInPlaceActiveObject::AddRef() {
  return pParent -> AddRef();
  }

  STDMETHODIMP_(ULONG) DataSet::_IOleInPlaceActiveObject::Release() {
  return pParent -> Release();
  }


  STDMETHODIMP DataSet::_IOleInPlaceActiveObject::GetWindow(HWND *pHwnd) {
  *pHwnd = pParent -> hwndSpecDialog;
  return S_OK;
  }

  STDMETHODIMP DataSet::_IOleInPlaceActiveObject::ContextSensitiveHelp(BOOL) {
  return E_NOTIMPL;
  }

  STDMETHODIMP DataSet::_IOleInPlaceActiveObject::TranslateAccelerator(MSG*) { 
  return S_OK;   
  }

  STDMETHODIMP DataSet::_IOleInPlaceActiveObject::OnFrameWindowActivate(BOOL) {
  return S_OK;   
  }

  STDMETHODIMP DataSet::_IOleInPlaceActiveObject::OnDocWindowActivate(BOOL) { 
  return S_OK;   
  }

  STDMETHODIMP DataSet::_IOleInPlaceActiveObject::ResizeBorder(LPCRECT ,IOleInPlaceUIWindow *,BOOL) {   
  return S_OK;   
  }

  STDMETHODIMP DataSet::_IOleInPlaceActiveObject::EnableModeless(BOOL) {   
  return S_OK;   
  }

