/*

                       Copyright (c) 1996,1997,1998,1999,2000 Nathan T. Clark

*/

#include <windows.h>

#include "OpenGLImplementor.h"


  STDMETHODIMP OpenGLImplementor::GetControlInfo(CONTROLINFO *pci) {
  return E_NOTIMPL;
  }

  STDMETHODIMP OpenGLImplementor::OnMnemonic(MSG *) {
  return E_NOTIMPL;
  }

  STDMETHODIMP OpenGLImplementor::OnAmbientPropertyChange(long) {
  return S_OK;
  }

  STDMETHODIMP OpenGLImplementor::FreezeEvents(int) {
  return S_OK;
  }