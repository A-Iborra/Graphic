// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "DataSet.h"


  STDMETHODIMP DataSet::GetClassInfo(ITypeInfo **pti) {
  DataSet *p = static_cast<DataSet *>(this);
  ITypeLib *ptl = 0;
  HRESULT hr = LoadRegTypeLib(LIBID_DataSet,1,0,0,&ptl);
  ptl -> GetTypeInfoOfGuid(CLSID_DataSet,pti);
  ptl -> Release();
  return S_OK;
  }


  STDMETHODIMP DataSet::GetGUID(DWORD dwKind,GUID *pGuid) {
  if ( dwKind != GUIDKIND_DEFAULT_SOURCE_DISP_IID || !pGuid ) return E_INVALIDARG;
  *pGuid = CLSID_DataSet;
  return S_OK;
  }
