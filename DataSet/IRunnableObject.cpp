// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "DataSet.h"


  STDMETHODIMP DataSet::GetRunningClass(CLSID *pcid) {
  *pcid = CLSID_DataSet;
  return E_UNEXPECTED;
  }

  STDMETHODIMP DataSet::Run(LPBC) { 
  return S_OK; 
  }

  int __stdcall DataSet::IsRunning() { 
  return 1; 
  }

  STDMETHODIMP DataSet::LockRunning(BOOL,BOOL) { 
  return S_OK; 
  }

  STDMETHODIMP DataSet::SetContainedObject(BOOL) { 
  return S_OK; 
  }

