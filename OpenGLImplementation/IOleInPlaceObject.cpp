// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "OpenGLImplementor.h"


   STDMETHODIMP OpenGLImplementor::GetWindow(HWND *pHwnd) {
   OpenGLImplementor *p = static_cast<OpenGLImplementor *>(this);
   *pHwnd = p -> hwndControl;
   return S_OK;
   }

   STDMETHODIMP OpenGLImplementor::ContextSensitiveHelp(BOOL) {
   return E_NOTIMPL;
   }
 
   STDMETHODIMP OpenGLImplementor::InPlaceActivate() {
   OpenGLImplementor *p = static_cast<OpenGLImplementor *>(this);
   if ( p -> pIOleInPlaceSite -> CanInPlaceActivate() != S_OK ) 
      return S_FALSE;
   p -> pIOleInPlaceSite -> OnInPlaceActivate();
   return S_OK;
   }
 
   STDMETHODIMP OpenGLImplementor::InPlaceDeactivate() {
   return S_OK;
   }
 
   STDMETHODIMP OpenGLImplementor::UIDeactivate() {
   return S_OK;
   }
 
   STDMETHODIMP OpenGLImplementor::SetObjectRects(LPCRECT pRectPos,LPCRECT pRectClip) {
   return S_OK;
   }
 
   STDMETHODIMP OpenGLImplementor::ReactivateAndUndo() {
   return S_OK;
   }