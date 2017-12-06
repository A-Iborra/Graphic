/*

                       Copyright (c) 1996,1997,1998,2000 Nathan T. Clark

*/

#include <windows.h>

#include "Text.h"
#include "Text_i.h"


  STDMETHODIMP Text::TranslateAccelerator(LPMSG) { 
  return S_OK;   
  }

  STDMETHODIMP Text::OnFrameWindowActivate(BOOL) {
  return S_OK;   
  }

  STDMETHODIMP Text::OnDocWindowActivate(BOOL) { 
  return S_OK;   
  }

  STDMETHODIMP Text::ResizeBorder(LPCRECT ,IOleInPlaceUIWindow *,BOOL) {   
  return S_OK;   
  }

  STDMETHODIMP Text::EnableModeless(BOOL) {   
  return S_OK;   
  }

