// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"


  STDMETHODIMP G::GetClassInfo(ITypeInfo **pti) {
  *pti = pITypeInfo_CLSID_G;
  pITypeInfo_CLSID_G -> AddRef();
  return S_OK;
  }


  STDMETHODIMP G::GetGUID(DWORD dwKind,GUID *pGuid) {
  if ( dwKind != GUIDKIND_DEFAULT_SOURCE_DISP_IID || !pGuid ) return E_INVALIDARG;
  *pGuid = CLSID_GSystemGraphic;
  return S_OK;
  }
