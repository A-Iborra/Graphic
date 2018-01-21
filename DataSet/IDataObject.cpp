// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "DataSet.h"


  STDMETHODIMP DataSet::GetData(FORMATETC *,STGMEDIUM *) {
  return S_OK;
  }

  STDMETHODIMP DataSet::GetDataHere(FORMATETC *,STGMEDIUM *) {
  return S_OK;
  }

  STDMETHODIMP DataSet::QueryGetData(FORMATETC *) {
  return S_OK;
  }

  STDMETHODIMP DataSet::GetCanonicalFormatEtc(FORMATETC *,FORMATETC *) {
  return S_OK;
  }

  STDMETHODIMP DataSet::SetData(FORMATETC *,STGMEDIUM *,BOOL) {
  return S_OK;
  }

  STDMETHODIMP DataSet::EnumFormatEtc(DWORD,IEnumFORMATETC **) {
  return S_OK;
  }

  STDMETHODIMP DataSet::DAdvise(FORMATETC *pFormatEtc,DWORD advf,IAdviseSink *pIAS,DWORD *pdwConnection) {
  DataSet *p = static_cast<DataSet *>(this);
  if ( !p -> pDataAdviseHolder ) 
     CreateDataAdviseHolder(&p -> pDataAdviseHolder);
  if ( p -> pDataAdviseHolder )
     p -> pDataAdviseHolder -> Advise(static_cast<IDataObject *>(p),pFormatEtc,advf,pIAS,pdwConnection);
  return S_OK;
  }

  STDMETHODIMP DataSet::DUnadvise(DWORD dwConnection) {
  DataSet *p = static_cast<DataSet *>(this);
  if ( p -> pDataAdviseHolder )
     p -> pDataAdviseHolder -> Unadvise(dwConnection);
  return S_OK;
  }

  STDMETHODIMP DataSet::EnumDAdvise(IEnumSTATDATA **) {
  return S_OK;
  }