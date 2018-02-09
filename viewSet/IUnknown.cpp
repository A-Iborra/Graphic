// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ViewSet.h"

#include "Graphic_resource.h"

#include "Properties_i.h"

       
  long __stdcall ViewSet::QueryInterface(REFIID riid,void **ppv) {
  *ppv = NULL; 

  if ( riid == IID_IUnknown || riid == IID_IViewSet )
     *ppv = static_cast<void*>(this);
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
 
      if ( riid == IID_IGPropertiesClient ) 
         *ppv = static_cast<IGPropertiesClient *>(this);
      else

      if ( riid == IID_IGPropertyPageClient ) 
         *ppv = static_cast<IGPropertyPageClient *>(this);
      else

      if ( riid == IID_IConnectionPointContainer )
         *ppv = static_cast<IConnectionPointContainer *>(this);
      else

         return E_NOINTERFACE;

  AddRef(); 
  return S_OK; 
  }

  unsigned long __stdcall ViewSet::AddRef() {
  return ++refCount;
  }

  unsigned long __stdcall ViewSet::Release() {
  refCount--;
  if ( refCount == 0 ) {
     delete this;
     return 0;
  }
  return refCount;
  }