/*

                       Copyright (c) 2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include "utils.h"

#include "ContainedObject.h"
#include "Graphic.h"


   ContainedObject::ContainedObject(G* pp,HWND hwndT,IUnknown* pIUnk,REFIID riidEvents) : 
      pParent(pp), 
      refCount(0),
      hwndTab(hwndT),
      pIUnknownObject(pIUnk),
      riidEventsInterface(riidEvents),
      pIConnectionPoint(0),
      dwConnectionCookie(0) {

   hwndSite = CreateWindowEx(0L,"STATIC","",WS_CHILD,0,0,0,0,hwndTab,NULL,NULL,(void*)this);

   if ( pIUnknownObject ) 
      pIUnknownObject -> AddRef();

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
   

   void ContainedObject::ReSize() {

   RECT rect;

   GetWindowRect(hwndTab,&rect);

   SetWindowPos(hwndSite,HWND_TOP,2,24,rect.right - rect.left - 4,rect.bottom - rect.top - 26,0L);

   IOleObject *pIOleObject_Object = NULL;

   pIUnknownObject -> QueryInterface(IID_IOleObject,reinterpret_cast<void **>(&pIOleObject_Object));

   if ( ! pIOleObject_Object )
      return;

   RECT rcParent;

   GetWindowRect(hwndSite,&rect);
   GetWindowRect(GetParent(hwndTab),&rcParent);

   long cx = rect.right - rect.left;
   long cy = rect.bottom - rect.left;

   rect.left = 0;
   rect.top = 0;
   rect.right = cx;
   rect.bottom = cy;

   pIOleObject_Object -> DoVerb(OLEIVERB_SHOW,NULL,pIOleClientSite,0L,hwndTab,&rect);

   pIOleObject_Object -> Release();

   ShowWindow(hwndTab,SW_SHOW);

   ShowWindow(hwndSite,SW_SHOW);

   return;
   }


   void ContainedObject::Show() {

   IOleObject *pIOleObject_Object = NULL;

   pIUnknownObject -> QueryInterface(IID_IOleObject,reinterpret_cast<void **>(&pIOleObject_Object));

   if ( ! pIOleObject_Object )
      return;

   RECT rcParent;
   RECT rect;

   GetWindowRect(hwndSite,&rect);
   GetWindowRect(GetParent(hwndTab),&rcParent);

   long cx = rect.right - rect.left;
   long cy = rect.bottom - rect.top;

   rect.left = 0;
   rect.top = 0;
   rect.right = cx;
   rect.bottom = cy;

   pIOleObject_Object -> DoVerb(OLEIVERB_SHOW,NULL,pIOleClientSite,0L,hwndTab,&rect);

   pIOleObject_Object -> Release();

   ShowWindow(hwndTab,SW_SHOW);

   ShowWindow(hwndSite,SW_SHOW);

   return;
   }


   void ContainedObject::Hide() {

   IOleObject *pIOleObject_Object = NULL;

   pIUnknownObject -> QueryInterface(IID_IOleObject,reinterpret_cast<void **>(&pIOleObject_Object));

   if ( ! pIOleObject_Object )
      return;

   pIOleObject_Object -> DoVerb(OLEIVERB_HIDE,NULL,pIOleClientSite,0L,NULL,NULL);

   pIOleObject_Object -> Release();

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


