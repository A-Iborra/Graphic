// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ObjectInOffice.h"


   STDMETHODIMP ObjectInOffice::GetData(FORMATETC *,STGMEDIUM *) {
   return S_OK;
   }

   STDMETHODIMP ObjectInOffice::GetDataHere(FORMATETC *,STGMEDIUM *) {
   return S_OK;
   }

   STDMETHODIMP ObjectInOffice::QueryGetData(FORMATETC *) {
   return S_OK;
   }

   STDMETHODIMP ObjectInOffice::GetCanonicalFormatEtc(FORMATETC *,FORMATETC *) {
   return S_OK;
   }

   STDMETHODIMP ObjectInOffice::SetData(FORMATETC *,STGMEDIUM *,BOOL) {
   return S_OK;
   }

   STDMETHODIMP ObjectInOffice::EnumFormatEtc(DWORD,IEnumFORMATETC **) {
   return S_OK;
   }

   STDMETHODIMP ObjectInOffice::DAdvise(FORMATETC *pFormatEtc,DWORD advf,IAdviseSink *pIAS,DWORD *pdwConnection) {
   return E_NOTIMPL;
  //if ( ! pDataAdviseHolder ) 
  //   CreateDataAdviseHolder(&pDataAdviseHolder);
  //if ( pDataAdviseHolder )
  //   pDataAdviseHolder -> Advise(static_cast<IDataObject *>(this),pFormatEtc,advf,pIAS,pdwConnection);
  //return S_OK;
   }

   STDMETHODIMP ObjectInOffice::DUnadvise(DWORD dwConnection) {
   if ( pDataAdviseHolder )
      pDataAdviseHolder -> Unadvise(dwConnection);
   return S_OK;
   }

   STDMETHODIMP ObjectInOffice::EnumDAdvise(IEnumSTATDATA **) {
   return S_OK;
   }