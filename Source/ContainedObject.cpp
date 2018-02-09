// Copyright 2017 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"

#include "utilities.h"

#include "ContainedObject.h"


   ContainedObject::ContainedObject(G* pp,HWND hwndT,IUnknown* pIUnk,REFIID riidEvents) : 
      pParent(pp), 
      refCount(0),
      hwndTab(hwndT),
      pIUnknownObject(pIUnk),
      pIOleObject(NULL),
      riidEventsInterface(riidEvents),
      pIConnectionPoint(0),
      dwConnectionCookie(0) {

   hwndSite = CreateWindowEx(WS_EX_CLIENTEDGE,"STATIC","",WS_CHILD | WS_VISIBLE,0,0,0,0,hwndTab,NULL,NULL,(void*)this);

   if ( pIUnknownObject ) 
      pIUnknownObject -> AddRef();

   pIUnknownObject -> QueryInterface(IID_IOleObject,reinterpret_cast<void **>(&pIOleObject));

   pIOleInPlaceSite = new _IOleInPlaceSite(this);
   pIOleClientSite = new _IOleClientSite(this);

   return;
   };


   ContainedObject::~ContainedObject() {

   unConnectEvents();

   ShowWindow(hwndSite,SW_HIDE);
   DestroyWindow(hwndSite);
   hwndSite = NULL;

   if ( pIUnknownObject ) 
      pIUnknownObject -> Release();

   if ( pIOleObject )
      pIOleObject -> Release();

   delete pIOleInPlaceSite;
   delete pIOleClientSite;

   };


   int ContainedObject::connectEvents() {

   IConnectionPointContainer* pIConnectionPointContainer;

   if ( ! SUCCEEDED(pIUnknownObject -> QueryInterface(IID_IConnectionPointContainer,reinterpret_cast<void**>(&pIConnectionPointContainer))) ) 
      return 0;

   if ( ! SUCCEEDED(pIConnectionPointContainer -> FindConnectionPoint(riidEventsInterface,&pIConnectionPoint)) ) {
      pIConnectionPointContainer -> Release();
      return 0;
   }

   IUnknown* pIUnknownThis;
   QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknownThis));
   pIConnectionPoint -> Advise(pIUnknownThis,&dwConnectionCookie);

   pIUnknownThis -> Release();

   pIConnectionPointContainer -> Release();

   return 1;
   }


   int ContainedObject::unConnectEvents() {

   pIConnectionPoint -> Unadvise(dwConnectionCookie);
   pIConnectionPoint -> Release();
   pIConnectionPoint = NULL;

   return 1;
   }
   

   void ContainedObject::GetExtentPixels(SIZEL *pSizel) {

   if ( ! pSizel )
      return;

   pSizel -> cx = 0;
   pSizel -> cy = 0;

   if ( ! pIOleObject )
      return;

   pIOleObject -> GetExtent(DVASPECT_CONTENT,pSizel);

   hiMetricToPixels(pSizel,pSizel);

   return;
   }


   void ContainedObject::ReSize() {

   RECT rect;

   GetWindowRect(hwndTab,&rect);

   SetWindowPos(hwndSite,HWND_TOP,2,24,rect.right - rect.left - 4,rect.bottom - rect.top - 26,0L);

   return;
   }


   void ContainedObject::Show() {

   if ( ! pIOleObject )
      return;

   if ( ! IsWindowVisible(hwndTab) )
      return;

   if ( ! IsWindowVisible(GetParent(hwndTab)) )
      return;

   ShowWindow(hwndSite,SW_SHOW);

   UpdateWindow(hwndSite);

   return;
   }


   void ContainedObject::Hide() {

   if ( ! pIOleObject )
      return;

   ShowWindow(hwndSite,SW_HIDE);

   return;
   }

   long __stdcall ContainedObject::QueryInterface(REFIID riid,void **ppv) {

   if ( IID_IUnknown == riid ) 
      *ppv = static_cast<IUnknown*>(this);
   else

   if ( IID_IOleClientSite == riid ) 
      *ppv = reinterpret_cast<void*>(pIOleClientSite);
   else

   if ( IID_IOleInPlaceSite == riid ) 
      *ppv = reinterpret_cast<void*>(pIOleInPlaceSite);
   else

   if ( IID_IDispatch == riid ) 
      return pIOleClientSite -> QueryInterface(riid,ppv);
   else

      return E_NOINTERFACE;

   reinterpret_cast<IUnknown*>(*ppv) -> AddRef();
   return S_OK;
   }

   unsigned long __stdcall ContainedObject::AddRef() {
   return 1;
   }
   unsigned long __stdcall ContainedObject::Release() {
   return 1;
   }


