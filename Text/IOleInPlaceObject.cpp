/*

                       Copyright (c) 1996,1997,1998,1999,2000 Nathan T. Clark

*/

#include <windows.h>

#include <tchar.h>
#include <olectl.h>

#include "utils.h"

#include "Text.h"

#include "Text_i.h"


  STDMETHODIMP Text::GetWindow(HWND *pHwnd) {
  Text *p = static_cast<Text *>(this);
  *pHwnd = p -> hwndFrame;
  return S_OK;
  }


  STDMETHODIMP Text::InPlaceActivate() {
  Text *p = static_cast<Text *>(this);

  if ( ! p -> pIOleClientSite ) return S_OK;

  if ( p -> pIOleInPlaceSite -> CanInPlaceActivate() != S_OK ) 
     return S_FALSE;

  p -> pIOleInPlaceSite -> OnInPlaceActivate();

  return S_OK;
  }

  STDMETHODIMP Text::InPlaceDeactivate() {
  return S_OK;
  }

  STDMETHODIMP Text::ContextSensitiveHelp(BOOL) {
  return E_NOTIMPL;
  }

  STDMETHODIMP Text::UIDeactivate() {
  return S_OK;
  }

  STDMETHODIMP Text::SetObjectRects(LPCRECT pRectPos,LPCRECT pRectClip) {
  return S_OK;
  }

  STDMETHODIMP Text::ReactivateAndUndo() {
  return S_OK;
  }