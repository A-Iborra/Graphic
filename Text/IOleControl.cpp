/*

                       Copyright (c) 1996,1997,1998,1999,2000 Nathan T. Clark

*/

#include <windows.h>

#include "Text.h"
#include "Text_i.h"


  STDMETHODIMP Text::GetControlInfo(CONTROLINFO *pci) {
  return E_NOTIMPL;
  }

  STDMETHODIMP Text::OnMnemonic(MSG *) {
  return E_NOTIMPL;
  }

  STDMETHODIMP Text::OnAmbientPropertyChange(long) {
  return S_OK;
  }

  STDMETHODIMP Text::FreezeEvents(int) {
  return S_OK;
  }