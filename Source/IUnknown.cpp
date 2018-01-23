// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"

#include "GraphicControl_i.h"

   long __stdcall G::QueryInterface(REFIID riid,void **ppv) {

   *ppv = NULL; 
 
   if ( riid == IID_IUnknown )
      *ppv = this; 
   else
 
      if ( riid == IID_IGSGraphic ) 
         *ppv = static_cast<IGSGraphic *>(this);
      else
 
      if ( riid == IID_IDataObject ) 
         *ppv = static_cast<IDataObject *>(this);
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

      if ( riid == IID_IGPropertiesClient )
         *ppv = static_cast<IGPropertiesClient *>(pIPropertiesClient);
      else
 
      if ( riid == IID_IGPropertyPageClient )
         *ppv = static_cast<IGPropertyPageClient *>(pIPropertyPageClient);
      else
 
      if ( riid == IID_ISpecifyPropertyPages )
         *ppv = static_cast<ISpecifyPropertyPages *>(this);
      else
 
      if ( riid == IID_IGProperties )
         return pIGProperties -> QueryInterface(IID_IGProperties,ppv);
      else

      if ( riid == IID_IPersistStorage ) 
         return pIGProperties -> QueryInterface(IID_IPersistStorage,ppv);
      else
 
      if ( riid == IID_IPersistStreamInit )
         return pIGProperties -> QueryInterface(IID_IPersistStreamInit,ppv);
      else
 
      if ( riid == IID_IPersistStream )
         return pIGProperties -> QueryInterface(IID_IPersistStream,ppv);
      else
 
      if ( riid == IID_IProvideClassInfo ) 
         *ppv = static_cast<IProvideClassInfo2 *>(this);
      else
 
      if ( riid == IID_IProvideClassInfo2 ) 
         *ppv = static_cast<IProvideClassInfo2 *>(this);
      else

      if ( riid == IID_IRunnableObject ) 
         *ppv = static_cast<IRunnableObject *>(this);
      else
 
     if ( riid == IID_IViewObject ) 
         *ppv = static_cast<IViewObject *>(this);
      else
 
      if ( riid == IID_IViewObject2 ) 
         *ppv = static_cast<IViewObject2 *>(this);
      else
 
      if ( riid == IID_IViewObjectEx ) 
         *ppv = static_cast<IViewObjectEx *>(this);
      else

      if ( riid == IID_IPlotNotify ) 
         *ppv = static_cast<IPlotNotify *>(this);
      else
 
      if ( riid == IID_ITextNotify ) 
         *ppv = static_cast<ITextNotify *>(this);
      else
 
      if ( riid == IID_IAdviseSink ) 
         *ppv = static_cast<IAdviseSink *>(this);
      else

      if ( riid == IID_IGSystemStatusBar ) 
         *ppv = static_cast<IGSystemStatusBar *>(this);
      else

      if ( riid == IID_IGSGraphicServices )
         *ppv = static_cast<IGSGraphicServices *>(this);
      else

      // Container Interfaces

/*
      if ( riid == IID_IOleClientSite )
         *ppv = &iOleClientSite;
      else

      if ( riid == IID_IOleInPlaceSite )
         *ppv = &iOleInPlaceSite;
      else
*/
         return E_NOINTERFACE;
 
   reinterpret_cast<IUnknown*>(*ppv) -> AddRef();
  
   return S_OK; 
   }
 
   unsigned long __stdcall G::AddRef() {
   return ++refCount;
   }
 
   unsigned long __stdcall G::Release() {
   refCount--;
   if ( refCount == 0 ) {
      stop();
      delete this;
      pStaticObject = NULL;
      return 0;
   }
   return refCount;
   }