/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include "DataSet.h"


  STDMETHODIMP DataSet::GetRunningClass(CLSID *pcid) {
  *pcid = CLSID_DataSet;
  return E_UNEXPECTED;
  }

  STDMETHODIMP DataSet::Run(LPBC) { 
  return S_OK; 
  }

  int __stdcall DataSet::IsRunning() { 
  return 1; 
  }

  STDMETHODIMP DataSet::LockRunning(BOOL,BOOL) { 
  return S_OK; 
  }

  STDMETHODIMP DataSet::SetContainedObject(BOOL) { 
  return S_OK; 
  }

