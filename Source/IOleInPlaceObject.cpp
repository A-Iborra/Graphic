// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"
#include "utils.h"


   STDMETHODIMP G::GetWindow(HWND *pHwnd) {
   *pHwnd = hwndFrame;
   return S_OK;
   }
 
   STDMETHODIMP G::ContextSensitiveHelp(BOOL) {
   return E_NOTIMPL;
   }
 
   STDMETHODIMP G::InPlaceActivate() {
   if ( pIOleInPlaceSite -> CanInPlaceActivate() != S_OK ) 
      return S_FALSE;
   pIOleInPlaceSite -> OnInPlaceActivate();
   return S_OK;
   }
 
   STDMETHODIMP G::InPlaceDeactivate() {
   return S_OK;
   }
 
   STDMETHODIMP G::UIDeactivate() {
   return S_OK;
   }
 
   STDMETHODIMP G::SetObjectRects(LPCRECT pRectPos,LPCRECT pRectClip) {
   containerSize.cx = pRectPos -> right - pRectPos -> left;
   containerSize.cy = pRectPos -> bottom - pRectPos -> top;
   SetWindowPos(hwndFrame,HWND_TOP,pRectPos -> left + 4,pRectPos -> top + 4,containerSize.cx - 8,containerSize.cy - 8,SWP_NOMOVE || SWP_SHOWWINDOW);
   return S_OK;
   }
 
   STDMETHODIMP G::ReactivateAndUndo() {
   return S_OK;
   }