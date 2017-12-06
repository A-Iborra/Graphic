/*

                       Copyright (c) 1996,1997,1998,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include "OpenGLImplementor.h"


  STDMETHODIMP OpenGLImplementor::TranslateAccelerator(LPMSG) { 
  return S_OK;   
  }

  STDMETHODIMP OpenGLImplementor::OnFrameWindowActivate(BOOL) {
  return S_OK;   
  }

  STDMETHODIMP OpenGLImplementor::OnDocWindowActivate(BOOL) { 
  return S_OK;   
  }

  STDMETHODIMP OpenGLImplementor::ResizeBorder(LPCRECT ,IOleInPlaceUIWindow *,BOOL) {   
  return S_OK;   
  }

  STDMETHODIMP OpenGLImplementor::EnableModeless(BOOL) {   
  return S_OK;   
  }

