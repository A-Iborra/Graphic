// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"


   G::_IGSGraphicEvents::_IGSGraphicEvents(G *pp) :
      pParent(pp),
      refCount(1) {
   }

   G::_IGSGraphicEvents::~_IGSGraphicEvents() {
   }

  long __stdcall G::_IGSGraphicEvents::QueryInterface(REFIID riid,void **ppv) {
  *ppv = NULL; 

  if ( riid == IID_IUnknown )
     *ppv = static_cast<void *>(this);
  else

   if ( riid == IID_IGSGraphicEvents )
      *ppv = static_cast<IGSGraphicEvents *>(this);
   else
      return pParent -> QueryInterface(riid,ppv);

   AddRef();
   return S_OK;
   }

   unsigned long __stdcall G::_IGSGraphicEvents::AddRef() {
   return ++refCount;
   }

   unsigned long __stdcall G::_IGSGraphicEvents::Release() {
   refCount--;
   if ( refCount == 0 ) {
      delete this;
      return 0;
   }
   return refCount;
   }


   HRESULT G::_IGSGraphicEvents::Clicked() {
   return S_OK;
   }


   HRESULT G::_IGSGraphicEvents::ReDraw() {
   HWND hwndKeep = pParent -> pIOpenGLImplementation -> TargetHWND();
   pParent -> pIOpenGLImplementation -> SetTargetWindow(pParent -> Canvas());
   pParent -> render(0);
   pParent -> pIOpenGLImplementation -> SetTargetWindow(hwndKeep);
   return S_OK;
   }
