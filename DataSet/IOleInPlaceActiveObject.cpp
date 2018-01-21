// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

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

