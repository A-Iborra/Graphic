/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include "DataSet.h"


  STDMETHODIMP DataSet::GetControlInfo(CONTROLINFO *pci) {
  return E_NOTIMPL;
  }

  STDMETHODIMP DataSet::OnMnemonic(MSG *) {
  return E_NOTIMPL;
  }

  STDMETHODIMP DataSet::OnAmbientPropertyChange(long) {
  return S_OK;
  }

  STDMETHODIMP DataSet::FreezeEvents(int bFreeze) {
  if ( bFreeze ) 
     freezeEvents++;
  else
     freezeEvents--;
  return S_OK;
  }