// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Text.h"


  STDMETHODIMP Text::TranslateAccelerator(LPMSG) { 
  return S_OK;   
  }

  STDMETHODIMP Text::OnFrameWindowActivate(BOOL) {
  return S_OK;   
  }

  STDMETHODIMP Text::OnDocWindowActivate(BOOL) { 
  return S_OK;   
  }

  STDMETHODIMP Text::ResizeBorder(LPCRECT ,IOleInPlaceUIWindow *,BOOL) {   
  return S_OK;   
  }

  STDMETHODIMP Text::EnableModeless(BOOL) {   
  return S_OK;   
  }

