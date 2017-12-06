/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

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
