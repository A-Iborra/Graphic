// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "OpenGLImplementor.h"


  STDMETHODIMP OpenGLImplementor::TranslateAccelerator(LPMSG) { 
  return S_OK;   
  }

  STDMETHODIMP OpenGLImplementor::OnFrameWindowActivate(BOOL) {
  return S_OK;   
  }

  STDMETHODIMP OpenGLImplementor::OnDocWindowActivate(BOOL) { 
  return S_OK;   
  }

  STDMETHODIMP OpenGLImplementor::ResizeBorder(LPCRECT ,IOleInPlaceUIWindow *,BOOL) {   
  return S_OK;   
  }

  STDMETHODIMP OpenGLImplementor::EnableModeless(BOOL) {   
  return S_OK;   
  }

