/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include <olectl.h>

#include "Function.h"


  HRESULT Function::_IOleInPlaceActiveObject::QueryInterface(REFIID riid,void **ppv) {
  return pParent -> QueryInterface(riid,ppv);
  }

  STDMETHODIMP_(ULONG) Function::_IOleInPlaceActiveObject::AddRef() {
  return pParent -> AddRef();
  }

  STDMETHODIMP_(ULONG) Function::_IOleInPlaceActiveObject::Release() {
  return pParent -> Release();
  }


  STDMETHODIMP Function::_IOleInPlaceActiveObject::GetWindow(HWND *pHwnd) {
  *pHwnd = pParent -> hwndSpecDialog;
  return S_OK;
  }

  STDMETHODIMP Function::_IOleInPlaceActiveObject::ContextSensitiveHelp(BOOL) {
  return E_NOTIMPL;
  }

  STDMETHODIMP Function::_IOleInPlaceActiveObject::TranslateAccelerator(MSG*) { 
  return S_OK;   
  }

  STDMETHODIMP Function::_IOleInPlaceActiveObject::OnFrameWindowActivate(BOOL) {
  return S_OK;   
  }

  STDMETHODIMP Function::_IOleInPlaceActiveObject::OnDocWindowActivate(BOOL) { 
  return S_OK;   
  }

  STDMETHODIMP Function::_IOleInPlaceActiveObject::ResizeBorder(LPCRECT ,IOleInPlaceUIWindow *,BOOL) {   
  return S_OK;   
  }

  STDMETHODIMP Function::_IOleInPlaceActiveObject::EnableModeless(BOOL) {   
  return S_OK;   
  }

