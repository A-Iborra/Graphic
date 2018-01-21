// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Text.h"


  STDMETHODIMP Text::GetControlInfo(CONTROLINFO *pci) {
  return E_NOTIMPL;
  }

  STDMETHODIMP Text::OnMnemonic(MSG *) {
  return E_NOTIMPL;
  }

  STDMETHODIMP Text::OnAmbientPropertyChange(long) {
  return S_OK;
  }

  STDMETHODIMP Text::FreezeEvents(int) {
  return S_OK;
  }