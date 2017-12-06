/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include "utils.h"

#include "Graphic.h"


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