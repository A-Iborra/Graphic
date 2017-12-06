/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include "Function.h"


  STDMETHODIMP Function::GetRunningClass(CLSID *pcid) {
#ifdef FUNCTION_SAMPLE
  *pcid = CLSID_GSystemFunctioNaterSample;
#else
  *pcid = CLSID_GSystemFunctioNater;
#endif
  return E_UNEXPECTED;
  }

  STDMETHODIMP Function::Run(LPBC) { 
  return S_OK; 
  }

  int __stdcall Function::IsRunning() { 
  return 1; 
  }

  STDMETHODIMP Function::LockRunning(BOOL,BOOL) { 
  return S_OK; 
  }

  STDMETHODIMP Function::SetContainedObject(BOOL) { 
  return S_OK; 
  }

