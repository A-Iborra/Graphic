// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"

#include "ContainedObject.h"

#include "GraphicControl_i.h"


   ContainedObject::_IOleClientSite::_IOleClientSite(ContainedObject *pp)
    : pParent(pp),iDispatch(this) 
   {
   };


   long __stdcall ContainedObject::_IOleClientSite::QueryInterface(REFIID riid,void **ppv) {

   if ( IID_IUnknown == riid ) 
      *ppv = static_cast<IUnknown*>(this);
   else

   if ( IID_IDispatch == riid ) 
      *ppv = static_cast<IDispatch*>(&iDispatch);
   else

   if ( IID_IOleClientSite == riid ) 
      *ppv = static_cast<::IOleClientSite*>(this);
   else

      return pParent -> ContainedObject::QueryInterface(riid,ppv);

   reinterpret_cast<IUnknown*>(*ppv) -> AddRef();

   return S_OK;
   }
   unsigned long __stdcall ContainedObject::_IOleClientSite::AddRef() {
   return pParent -> AddRef();
   }
   unsigned long __stdcall ContainedObject::_IOleClientSite::Release() {
   return pParent -> Release();
   }


   HRESULT ContainedObject::_IOleClientSite::SaveObject() {
   return S_OK;
   }

   
   HRESULT ContainedObject::_IOleClientSite::GetMoniker(DWORD,DWORD,IMoniker**) {
   return S_OK;
   }


   HRESULT ContainedObject::_IOleClientSite::GetContainer(IOleContainer**) {
   return S_OK;
   }


   HRESULT ContainedObject::_IOleClientSite::ShowObject() {
   return S_OK;
   }


   HRESULT ContainedObject::_IOleClientSite::OnShowWindow(BOOL) {
   return S_OK;
   }


   HRESULT ContainedObject::_IOleClientSite::RequestNewObjectLayout() {
   return S_OK;
   }