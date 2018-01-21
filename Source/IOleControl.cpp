// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"

#include "GraphicControl_i.h"


  STDMETHODIMP G::GetControlInfo(CONTROLINFO *pci) {
  return E_NOTIMPL;
  }

  STDMETHODIMP G::OnMnemonic(MSG *) {
  return E_NOTIMPL;
  }

  STDMETHODIMP G::OnAmbientPropertyChange(long) {
  return S_OK;
  }

  STDMETHODIMP G::FreezeEvents(int) {
  return S_OK;
  }