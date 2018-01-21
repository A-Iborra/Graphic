// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Text.h"


  STDMETHODIMP Text::GetWindow(HWND *pHwnd) {
  Text *p = static_cast<Text *>(this);
  *pHwnd = p -> hwndFrame;
  return S_OK;
  }


  STDMETHODIMP Text::InPlaceActivate() {
  Text *p = static_cast<Text *>(this);

  if ( ! p -> pIOleClientSite ) return S_OK;

  if ( p -> pIOleInPlaceSite -> CanInPlaceActivate() != S_OK ) 
     return S_FALSE;

  p -> pIOleInPlaceSite -> OnInPlaceActivate();

  return S_OK;
  }

  STDMETHODIMP Text::InPlaceDeactivate() {
  return S_OK;
  }

  STDMETHODIMP Text::ContextSensitiveHelp(BOOL) {
  return E_NOTIMPL;
  }

  STDMETHODIMP Text::UIDeactivate() {
  return S_OK;
  }

  STDMETHODIMP Text::SetObjectRects(LPCRECT pRectPos,LPCRECT pRectClip) {
  return S_OK;
  }

  STDMETHODIMP Text::ReactivateAndUndo() {
  return S_OK;
  }