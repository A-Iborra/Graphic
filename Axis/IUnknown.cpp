// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Axis.h"
#include "utils.h"

  long __stdcall Axis::QueryInterface(REFIID riid,void **ppv) {
  *ppv = NULL; 

  if ( riid == IID_IUnknown )
     *ppv = this; 
  else

  if ( riid == IID_IAxis ) 
     *ppv = static_cast<IAxis *>(this);
  else

  if ( riid == IID_IDispatch) 
     *ppv = static_cast<IDispatch *>(this);
  else

  if ( riid == IID_IGProperties ) 
     return pIUnknownProperties -> QueryInterface(riid,ppv);
  else

  if ( riid == IID_IGPropertiesClient ) 
     *ppv = static_cast<IGPropertiesClient *>(this);
  else

  if ( riid == IID_IGPropertyPageClient ) 
     *ppv = static_cast<IGPropertyPageClient *>(this);
  else

  if ( riid == IID_IPersistStorage ) 
     return pIUnknownProperties -> QueryInterface(riid,ppv);
  else

  if ( riid == IID_IPersistStreamInit )
     return pIUnknownProperties -> QueryInterface(riid,ppv);
  else

  if ( riid == IID_IPersistStream )
     return pIUnknownProperties -> QueryInterface(riid,ppv);
  else

  if ( riid == IID_ISpecifyPropertyPages ) 
     return pIUnknownProperties -> QueryInterface(riid,ppv);
  else

     return E_NOINTERFACE;

  AddRef(); 
  return S_OK; 
  }
  unsigned long __stdcall Axis::AddRef() {
  refCount++;
  return refCount;
  }
  unsigned long __stdcall Axis::Release() {
  refCount--;
  if ( refCount == 0 ) {
     delete this;
     return 0;
  }
  return refCount;
  }
