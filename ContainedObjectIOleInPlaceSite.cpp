/*

                       Copyright (c) 2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include "ContainedObject.h"
#include "Graphic.h"

#include "GraphicControl_i.h"


   long __stdcall ContainedObject::_IOleInPlaceSite::QueryInterface(REFIID riid,void **ppv) {

   if ( IID_IUnknown == riid ) 
      *ppv = static_cast<IUnknown*>(this);
   else

   if ( IID_IOleInPlaceSite == riid ) 
      *ppv = static_cast<::IOleInPlaceSite*>(this);
   else

      return pParent -> ContainedObject::QueryInterface(riid,ppv);

   reinterpret_cast<IUnknown*>(ppv) -> AddRef();

   return S_OK;
   }
   unsigned long __stdcall ContainedObject::_IOleInPlaceSite::AddRef() {
   return pParent -> AddRef();
   }
   unsigned long __stdcall ContainedObject::_IOleInPlaceSite::Release() {
   return pParent -> Release();
   }


   HRESULT ContainedObject::_IOleInPlaceSite::GetWindow(HWND *gwh) {
   *gwh = pParent -> hwndSite;
   return S_OK;
   }


   HRESULT ContainedObject::_IOleInPlaceSite::ContextSensitiveHelp(BOOL fEnterMode) {
   return S_OK;
   }


   HRESULT ContainedObject::_IOleInPlaceSite::CanInPlaceActivate() {
   return S_OK;
   }


   HRESULT ContainedObject::_IOleInPlaceSite::OnInPlaceActivate() {
   return S_OK;
   }


   HRESULT ContainedObject::_IOleInPlaceSite::OnUIActivate() {
   return S_OK;
   }


   HRESULT ContainedObject::_IOleInPlaceSite::GetWindowContext(IOleInPlaceFrame**,IOleInPlaceUIWindow**,RECT*,RECT*,OLEINPLACEFRAMEINFO*) {
   return S_OK;
   }


   HRESULT ContainedObject::_IOleInPlaceSite::Scroll(SIZE) {
   return S_OK;
   }


   HRESULT ContainedObject::_IOleInPlaceSite::OnUIDeactivate(BOOL) {
   return S_OK;
   }


   HRESULT ContainedObject::_IOleInPlaceSite::OnInPlaceDeactivate() {
   return S_OK;
   }


   HRESULT ContainedObject::_IOleInPlaceSite::DiscardUndoState() {
   return S_OK;
   }


   HRESULT ContainedObject::_IOleInPlaceSite::DeactivateAndUndo() {
   return S_OK;
   }


   HRESULT ContainedObject::_IOleInPlaceSite::OnPosRectChange(const RECT*) {
/*
   RECT rect;
   GetWindowRect(GetParent(GetParent(pParent -> HWNDSite())),&rect);
   SendMessage(GetParent(GetParent(pParent -> HWNDSite())),WM_SIZE,(WPARAM)SIZE_RESTORED,MAKELPARAM(rect.right - rect.left,rect.bottom - rect.top));
*/
   return S_OK;
   }
