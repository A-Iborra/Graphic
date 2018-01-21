// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"


  STDMETHODIMP G::TranslateAccelerator(LPMSG) { 
  return S_FALSE;   
  }

  STDMETHODIMP G::OnFrameWindowActivate(BOOL) {
  return S_OK;   
  }

  STDMETHODIMP G::OnDocWindowActivate(BOOL) { 
  return S_OK;   
  }

  STDMETHODIMP G::ResizeBorder(LPCRECT ,IOleInPlaceUIWindow *,BOOL) {   
  return S_OK;   
  }

  STDMETHODIMP G::EnableModeless(BOOL) {   
  return S_OK;   
  }

