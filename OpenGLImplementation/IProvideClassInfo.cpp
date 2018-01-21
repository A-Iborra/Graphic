// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "OpenGLImplementor.h"


  STDMETHODIMP OpenGLImplementor::GetClassInfo(ITypeInfo **pti) {
  *pti = pITypeInfo;
  pITypeInfo -> AddRef();
  return S_OK;
  }


  STDMETHODIMP OpenGLImplementor::GetGUID(DWORD dwKind,GUID *pGuid) {
  if ( dwKind != GUIDKIND_DEFAULT_SOURCE_DISP_IID || !pGuid ) return E_INVALIDARG;
  *pGuid = CLSID_OpenGLImplementor;
  return S_OK;
  }
