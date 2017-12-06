/*

                       Copyright (c) 1996,1997,1998,2000 Nathan T. Clark

*/

#include <windows.h>

#include "Graphic.h"

#include "GraphicControl_i.h"


  STDMETHODIMP G::GetRunningClass(CLSID *pcid) {
  *pcid = CLSID_GSystemGraphic;
  return E_UNEXPECTED;
  }

  STDMETHODIMP G::Run(LPBC) { 
  return S_OK; 
  }

  int __stdcall G::IsRunning() { 
  return 1; 
  }

  STDMETHODIMP G::LockRunning(BOOL,BOOL) { 
  return S_OK; 
  }

  STDMETHODIMP G::SetContainedObject(BOOL) { 
  return S_OK; 
  }

