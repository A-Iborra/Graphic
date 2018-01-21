// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "DataSet.h"


  STDMETHODIMP DataSet::GetWindow(HWND *pHwnd) {
Beep(2000,100);
#if 0
  *pHwnd = hwndSpecDialog;
#endif
  return S_OK;
  }


  STDMETHODIMP DataSet::InPlaceActivate() {
  return pIOleInPlaceSite -> OnInPlaceActivate();
  }


  STDMETHODIMP DataSet::InPlaceDeactivate() {
  return S_OK;
  }


  STDMETHODIMP DataSet::ContextSensitiveHelp(BOOL) {
  return E_NOTIMPL;
  }


  STDMETHODIMP DataSet::UIDeactivate() {
  return S_OK;
  }


  STDMETHODIMP DataSet::SetObjectRects(LPCRECT pRectPos,LPCRECT pRectClip) {
  SetWindowPos(hwndSpecDialog,HWND_TOP,pRectPos -> left,pRectPos -> top,pRectPos -> right - pRectPos -> left,pRectPos -> bottom - pRectPos -> top,0L);
  return S_OK;
  }


  STDMETHODIMP DataSet::ReactivateAndUndo() {
  return S_OK;
  }