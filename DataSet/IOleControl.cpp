// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "DataSet.h"

  STDMETHODIMP DataSet::GetControlInfo(CONTROLINFO *pci) {
  return E_NOTIMPL;
  }

  STDMETHODIMP DataSet::OnMnemonic(MSG *) {
  return E_NOTIMPL;
  }

  STDMETHODIMP DataSet::OnAmbientPropertyChange(long) {
  return S_OK;
  }

  STDMETHODIMP DataSet::FreezeEvents(int bFreeze) {
  if ( bFreeze ) 
     freezeEvents++;
  else
     freezeEvents--;
  return S_OK;
  }