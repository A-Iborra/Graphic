/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include <tchar.h>
#include <olectl.h>

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
