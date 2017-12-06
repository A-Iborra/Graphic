/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include <tchar.h>
#include <olectl.h>

#include "Function.h"


  STDMETHODIMP Function::GetClassInfo(ITypeInfo **pti) {
  Function *p = static_cast<Function *>(this);
  ITypeLib *ptl = 0;
#ifdef FUNCTION_SAMPLE
  HRESULT hr = LoadRegTypeLib(LIBID_FunctioNaterSample,1,0,0,&ptl);
  ptl -> GetTypeInfoOfGuid(CLSID_GSystemFunctioNaterSample,pti);
#else
  HRESULT hr = LoadRegTypeLib(LIBID_FunctioNater,1,0,0,&ptl);
  ptl -> GetTypeInfoOfGuid(CLSID_GSystemFunctioNater,pti);
#endif
  ptl -> Release();
  return S_OK;
  }


  STDMETHODIMP Function::GetGUID(DWORD dwKind,GUID *pGuid) {
  if ( dwKind != GUIDKIND_DEFAULT_SOURCE_DISP_IID || !pGuid ) return E_INVALIDARG;
#ifdef FUNCTION_SAMPLE
  *pGuid = CLSID_GSystemFunctioNaterSample;
#else
  *pGuid = CLSID_GSystemFunctioNater;
#endif
  return S_OK;
  }
