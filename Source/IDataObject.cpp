// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"

#include "GraphicControl_i.h"

   STDMETHODIMP G::GetData(FORMATETC *,STGMEDIUM *) {
   return S_OK;
   }

   STDMETHODIMP G::GetDataHere(FORMATETC *,STGMEDIUM *) {
   return S_OK;
   }

   STDMETHODIMP G::QueryGetData(FORMATETC *) {
   return S_OK;
   }

   STDMETHODIMP G::GetCanonicalFormatEtc(FORMATETC *,FORMATETC *) {
   return S_OK;
   }

   STDMETHODIMP G::SetData(FORMATETC *theData,STGMEDIUM *,BOOL) {
   return S_OK;
   }

   STDMETHODIMP G::EnumFormatEtc(DWORD,IEnumFORMATETC **) {
   return S_OK;
   }

   STDMETHODIMP G::DAdvise(FORMATETC *pFormatEtc,DWORD advf,IAdviseSink *pIAS,DWORD *pdwConnection) {
   return E_NOTIMPL;
  //if ( ! pDataAdviseHolder ) 
  //   CreateDataAdviseHolder(&pDataAdviseHolder);
  //if ( pDataAdviseHolder )
  //   pDataAdviseHolder -> Advise(static_cast<IDataObject *>(this),pFormatEtc,advf,pIAS,pdwConnection);
  //return S_OK;
   }

   STDMETHODIMP G::DUnadvise(DWORD dwConnection) {
   if ( pDataAdviseHolder )
      pDataAdviseHolder -> Unadvise(dwConnection);
   return S_OK;
   }

   STDMETHODIMP G::EnumDAdvise(IEnumSTATDATA **) {
   return S_OK;
   }