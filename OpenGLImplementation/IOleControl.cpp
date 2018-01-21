// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "OpenGLImplementor.h"


  STDMETHODIMP OpenGLImplementor::GetControlInfo(CONTROLINFO *pci) {
  return E_NOTIMPL;
  }

  STDMETHODIMP OpenGLImplementor::OnMnemonic(MSG *) {
  return E_NOTIMPL;
  }

  STDMETHODIMP OpenGLImplementor::OnAmbientPropertyChange(long) {
  return S_OK;
  }

  STDMETHODIMP OpenGLImplementor::FreezeEvents(int) {
  return S_OK;
  }