/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include "Function.h"


  STDMETHODIMP Function::GetWindow(HWND *pHwnd) {
  *pHwnd = hwndSpecDialog;
  return S_OK;
  }


  STDMETHODIMP Function::InPlaceActivate() {
  return pIOleInPlaceSite -> OnInPlaceActivate();
  }


  STDMETHODIMP Function::InPlaceDeactivate() {
  return S_OK;
  }


  STDMETHODIMP Function::ContextSensitiveHelp(BOOL) {
  return E_NOTIMPL;
  }


  STDMETHODIMP Function::UIDeactivate() {
  return S_OK;
  }


  STDMETHODIMP Function::SetObjectRects(LPCRECT pRectPos,LPCRECT pRectClip) {
  rectDialog.left = pRectPos -> left;
  rectDialog.top = pRectPos -> top;
#if 0
  containerSize.cx = pRectPos -> right - pRectPos -> left;
  containerSize.cy = pRectPos -> bottom - pRectPos -> top;
#endif
  rectDialog.right = rectDialog.left + containerSize.cx;
  rectDialog.bottom = rectDialog.bottom + containerSize.cy;
  SetWindowPos(hwndSpecDialog,HWND_TOP,rectDialog.left,rectDialog.top,containerSize.cx,containerSize.cy,0L);
  return S_OK;
  }


  STDMETHODIMP Function::ReactivateAndUndo() {
  return S_OK;
  }