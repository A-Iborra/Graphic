/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

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
