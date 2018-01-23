// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Function.h"

  STDMETHODIMP Function::GetControlInfo(CONTROLINFO *pci) {
  return E_NOTIMPL;
  }

  STDMETHODIMP Function::OnMnemonic(MSG *) {
  return E_NOTIMPL;
  }

  STDMETHODIMP Function::OnAmbientPropertyChange(long) {
  return S_OK;
  }

  STDMETHODIMP Function::FreezeEvents(int bFreeze) {
  if ( bFreeze ) 
     freezeEvents++;
  else
     freezeEvents--;
  return S_OK;
  }