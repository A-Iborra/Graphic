/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include "Graphic.h"

#include "GraphicControl_i.h"


  STDMETHODIMP G::GetControlInfo(CONTROLINFO *pci) {
  return E_NOTIMPL;
  }

  STDMETHODIMP G::OnMnemonic(MSG *) {
  return E_NOTIMPL;
  }

  STDMETHODIMP G::OnAmbientPropertyChange(long) {
  return S_OK;
  }

  STDMETHODIMP G::FreezeEvents(int) {
  return S_OK;
  }