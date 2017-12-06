/*

                       Copyright (c) 1996,1997,1998,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include "Graphic.h"


  STDMETHODIMP G::TranslateAccelerator(LPMSG) { 
  return S_OK;   
  }

  STDMETHODIMP G::OnFrameWindowActivate(BOOL) {
  return S_OK;   
  }

  STDMETHODIMP G::OnDocWindowActivate(BOOL) { 
  return S_OK;   
  }

  STDMETHODIMP G::ResizeBorder(LPCRECT ,IOleInPlaceUIWindow *,BOOL) {   
  return S_OK;   
  }

  STDMETHODIMP G::EnableModeless(BOOL) {   
  return S_OK;   
  }

