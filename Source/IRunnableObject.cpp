// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"

#include "GraphicControl_i.h"


  STDMETHODIMP G::GetRunningClass(CLSID *pcid) {
  *pcid = CLSID_GSystemGraphic;
  return E_UNEXPECTED;
  }

  STDMETHODIMP G::Run(LPBC) { 
  return S_OK; 
  }

  int __stdcall G::IsRunning() { 
  return 1; 
  }

  STDMETHODIMP G::LockRunning(BOOL,BOOL) { 
  return S_OK; 
  }

  STDMETHODIMP G::SetContainedObject(BOOL) { 
  return S_OK; 
  }

