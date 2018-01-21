// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "OpenGLImplementor.h"
 
   long __stdcall OpenGLImplementor::QueryInterface(REFIID riid,void **ppv) {
   *ppv = NULL; 
 
   if ( riid == IID_IUnknown )
      *ppv = this; 
   else
 
      if ( riid == IID_IOpenGLImplementation ) 
         *ppv = static_cast<IOpenGLImplementation *>(this);
      else
 
      if ( riid == IID_IDispatch ) 
         *ppv = static_cast<IDispatch *>(this);
      else
 
      if ( riid == IID_IOleControl )
         *ppv = static_cast<IOleControl *>(this);
      else
 
      if ( riid == IID_IOleInPlaceActiveObject ) 
         *ppv = static_cast<IOleInPlaceActiveObject *>(this);
      else
 
      if ( riid == IID_IOleInPlaceObject ) 
         *ppv = static_cast<IOleInPlaceObject *>(this);
      else
 
      if ( riid == IID_IOleObject )
         *ppv = static_cast<IOleObject *>(this);
      else
 
      if ( riid == IID_IOleWindow ) 
         *ppv = static_cast<IOleInPlaceObject *>(this);
      else

      if ( riid == IID_ISpecifyPropertyPages )
         return pIUnknownProperties -> QueryInterface(IID_ISpecifyPropertyPages,ppv);
      else

      if ( riid == IID_IGProperties )
         return pIUnknownProperties -> QueryInterface(IID_IGProperties,ppv);
      else

      if ( riid == IID_IPersistStorage )
         return pIUnknownProperties -> QueryInterface(IID_IPersistStorage,ppv);
      else
 
      if ( riid == IID_IPersistStreamInit )
         return pIUnknownProperties -> QueryInterface(IID_IPersistStreamInit,ppv);
      else
 
      if ( riid == IID_IPersistStream )
         return pIUnknownProperties -> QueryInterface(IID_IPersistStream,ppv);
      else

      if ( riid == IID_IProvideClassInfo ) 
         *ppv = static_cast<IProvideClassInfo2 *>(this);
      else
 
      if ( riid == IID_IProvideClassInfo2 ) 
         *ppv = static_cast<IProvideClassInfo2 *>(this);
      else

      if ( riid == IID_IViewObject ) 
         *ppv = static_cast<IViewObjectEx *>(this);
      else
 
      if ( riid == IID_IViewObject2 ) 
         *ppv = static_cast<IViewObjectEx *>(this);
      else
 
      if ( riid == IID_IViewObjectEx ) 
         *ppv = static_cast<IViewObjectEx *>(this);
      else

         return E_NOINTERFACE;
 
   AddRef(); 

   return S_OK; 
   }
 
   unsigned long __stdcall OpenGLImplementor::AddRef() {
   refCount++;
   return refCount;
   }
 
   unsigned long __stdcall OpenGLImplementor::Release() {
   refCount--;
   if ( ! refCount ) {
      delete this;
      return 0;
   }
   return refCount;
   }