/*

                       Copyright (c) 2000,2001 Nathan T. Clark

*/

#include <windows.h>

#include "ContainedObject.h"

   ContainedObject<class Parent>::ContainedObject(Parent* pp,HWND hwndT,IUnknown* pIUnk) : 
      pParent(pp), 
      refCount(0),
      hwndTab(hwndT),
      pIUnknownObject(pIUnk),
      pIConnectionPoint(0),
      dwConnectionCookie(0) {
   hwndSite = CreateWindowEx(0L,"STATIC","",WS_CHILD,16,16,0,0,hwndTab,NULL,NULL,(void*)this);
   if ( pIUnknownObject ) 
      pIUnknownObject -> AddRef();
   return;
   };


   ContainedObject<class Parent>::~ContainedObject() {
   ShowWindow(hwndSite,SW_HIDE);
   DestroyWindow(hwndSite);
   if ( pIUnknownObject ) 
      pIUnknownObject -> Release();
   if ( pIConnectionPoint )
      pIConnectionPoint -> Release();
   };


   void ContainedObject<class Parent>::ReSize() {
   RECT rect;
   GetWindowRect(hwndTab,&rect);
   SetWindowPos(hwndSite,HWND_TOP,8,32,rect.right - rect.left - 16,rect.bottom - rect.top - 40,0L);
   return;
   }


   HRESULT ContainedObject<class Parent>::SaveObject() {
   return S_OK;
   }

   
   HRESULT ContainedObject<class Parent>::GetMoniker(DWORD,DWORD,IMoniker**) {
   return S_OK;
   }


   HRESULT ContainedObject<class Parent>::GetContainer(IOleContainer**) {
   return S_OK;
   }


   HRESULT ContainedObject<class Parent>::ShowObject() {
   return S_OK;
   }


   HRESULT ContainedObject<class Parent>::OnShowWindow(BOOL) {
   return S_OK;
   }


   HRESULT ContainedObject<class Parent>::RequestNewObjectLayout() {
   return S_OK;
   }


   HRESULT ContainedObject<class Parent>::GetWindow(HWND *gwh) {
   *gwh = hwndSite;
   return S_OK;
   }


   HRESULT ContainedObject<class Parent>::ContextSensitiveHelp(BOOL fEnterMode) {
   return S_OK;
   }


   HRESULT ContainedObject<class Parent>::CanInPlaceActivate() {
   return S_OK;
   }


   HRESULT ContainedObject<class Parent>::OnInPlaceActivate() {
   return S_OK;
   }


   HRESULT ContainedObject<class Parent>::OnUIActivate() {
   return S_OK;
   }


   HRESULT ContainedObject<class Parent>::GetWindowContext(IOleInPlaceFrame**,IOleInPlaceUIWindow**,RECT*,RECT*,OLEINPLACEFRAMEINFO*) {
   return S_OK;
   }


   HRESULT ContainedObject<class Parent>::Scroll(SIZE) {
   return S_OK;
   }


   HRESULT ContainedObject<class Parent>::OnUIDeactivate(BOOL) {
   return S_OK;
   }


   HRESULT ContainedObject<class Parent>::OnInPlaceDeactivate() {
   return S_OK;
   }


   HRESULT ContainedObject<class Parent>::DiscardUndoState() {
   return S_OK;
   }


   HRESULT ContainedObject<class Parent>::DeactivateAndUndo() {
   return S_OK;
   }


   HRESULT ContainedObject<class Parent>::OnPosRectChange(const RECT*) {
   return S_OK;
   }
