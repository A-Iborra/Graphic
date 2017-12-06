/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include <tchar.h>
#include <olectl.h>

#include "Function.h"


  STDMETHODIMP Function::GetData(FORMATETC *,STGMEDIUM *) {
  return S_OK;
  }

  STDMETHODIMP Function::GetDataHere(FORMATETC *,STGMEDIUM *) {
  return S_OK;
  }

  STDMETHODIMP Function::QueryGetData(FORMATETC *) {
  return S_OK;
  }

  STDMETHODIMP Function::GetCanonicalFormatEtc(FORMATETC *,FORMATETC *) {
  return S_OK;
  }

  STDMETHODIMP Function::SetData(FORMATETC *,STGMEDIUM *,BOOL) {
  return S_OK;
  }

  STDMETHODIMP Function::EnumFormatEtc(DWORD,IEnumFORMATETC **) {
  return S_OK;
  }

  STDMETHODIMP Function::DAdvise(FORMATETC *pFormatEtc,DWORD advf,IAdviseSink *pIAS,DWORD *pdwConnection) {
  Function *p = static_cast<Function *>(this);
  if ( !p -> pDataAdviseHolder ) 
     CreateDataAdviseHolder(&p -> pDataAdviseHolder);
  if ( p -> pDataAdviseHolder )
     p -> pDataAdviseHolder -> Advise(static_cast<IDataObject *>(p),pFormatEtc,advf,pIAS,pdwConnection);
  return S_OK;
  }

  STDMETHODIMP Function::DUnadvise(DWORD dwConnection) {
  Function *p = static_cast<Function *>(this);
  if ( p -> pDataAdviseHolder )
     p -> pDataAdviseHolder -> Unadvise(dwConnection);
  return S_OK;
  }

  STDMETHODIMP Function::EnumDAdvise(IEnumSTATDATA **) {
  return S_OK;
  }