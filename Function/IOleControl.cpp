/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include "Function.h"


  STDMETHODIMP Function::GetControlInfo(CONTROLINFO *pci) {
  return E_NOTIMPL;
  }

  STDMETHODIMP Function::OnMnemonic(MSG *) {
  return E_NOTIMPL;
  }

  STDMETHODIMP Function::OnAmbientPropertyChange(long) {
  return S_OK;
  }

  STDMETHODIMP Function::FreezeEvents(int bFreeze) {
  if ( bFreeze ) 
     freezeEvents++;
  else
     freezeEvents--;
  return S_OK;
  }