// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"

#include "GraphicControl_i.h"

  STDMETHODIMP G::GetData(FORMATETC *,STGMEDIUM *) {
  return S_OK;
  }

  STDMETHODIMP G::GetDataHere(FORMATETC *,STGMEDIUM *) {
  return E_NOTIMPL;
  }

  STDMETHODIMP G::QueryGetData(FORMATETC *) {
  return S_FALSE;
  }

  STDMETHODIMP G::GetCanonicalFormatEtc(FORMATETC *,FORMATETC *) {
  return E_NOTIMPL;
  }

  STDMETHODIMP G::SetData(FORMATETC *theData,STGMEDIUM *,BOOL) {
  return E_NOTIMPL;
  }

  STDMETHODIMP G::EnumFormatEtc(DWORD,IEnumFORMATETC **) {
  return E_NOTIMPL;
  }

  STDMETHODIMP G::DAdvise(FORMATETC *pFormatEtc,DWORD advf,IAdviseSink *pIAS,DWORD *pdwConnection) {
  G *p = static_cast<G *>(this);
  if ( !p -> pDataAdviseHolder ) 
     CreateDataAdviseHolder(&p -> pDataAdviseHolder);
  if ( p -> pDataAdviseHolder )
     p -> pDataAdviseHolder -> Advise(static_cast<IDataObject *>(p),pFormatEtc,advf,pIAS,pdwConnection);
  return S_OK;
  }

  STDMETHODIMP G::DUnadvise(DWORD dwConnection) {
  G *p = static_cast<G *>(this);
  if ( p -> pDataAdviseHolder )
     p -> pDataAdviseHolder -> Unadvise(dwConnection);
  return S_OK;
  }

  STDMETHODIMP G::EnumDAdvise(IEnumSTATDATA **) {
  return S_OK;
  }