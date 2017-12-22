/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include "DataSet.h"


  STDMETHODIMP DataSet::GetWindow(HWND *pHwnd) {
Beep(2000,100);
#if 0
  *pHwnd = hwndSpecDialog;
#endif
  return S_OK;
  }


  STDMETHODIMP DataSet::InPlaceActivate() {
  return pIOleInPlaceSite -> OnInPlaceActivate();
  }


  STDMETHODIMP DataSet::InPlaceDeactivate() {
  return S_OK;
  }


  STDMETHODIMP DataSet::ContextSensitiveHelp(BOOL) {
  return E_NOTIMPL;
  }


  STDMETHODIMP DataSet::UIDeactivate() {
  return S_OK;
  }


  STDMETHODIMP DataSet::SetObjectRects(LPCRECT pRectPos,LPCRECT pRectClip) {
  SetWindowPos(hwndSpecDialog,HWND_TOP,pRectPos -> left,pRectPos -> top,pRectPos -> right - pRectPos -> left,pRectPos -> bottom - pRectPos -> top,0L);
  return S_OK;
  }


  STDMETHODIMP DataSet::ReactivateAndUndo() {
  return S_OK;
  }