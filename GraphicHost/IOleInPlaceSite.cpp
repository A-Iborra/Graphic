/*

                       Copyright (c) 2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include "GraphicHost.h"
#include "GraphicControl_i.h"


   long __stdcall GraphicHost::_IOleInPlaceSite::QueryInterface(REFIID riid,void **ppv) {

   if ( IID_IUnknown == riid ) 
      *ppv = static_cast<IUnknown*>(this);
   else

   if ( IID_IOleInPlaceSite == riid ) 
      *ppv = static_cast<::IOleInPlaceSite*>(this);
   else

      return pParent -> GraphicHost::QueryInterface(riid,ppv);

   reinterpret_cast<IUnknown*>(ppv) -> AddRef();

   return S_OK;
   }
   unsigned long __stdcall GraphicHost::_IOleInPlaceSite::AddRef() {
   return pParent -> AddRef();
   }
   unsigned long __stdcall GraphicHost::_IOleInPlaceSite::Release() {
   return pParent -> Release();
   }


   HRESULT GraphicHost::_IOleInPlaceSite::GetWindow(HWND *gwh) {
   *gwh = pParent -> hwndSite;
   return S_OK;
   }


   HRESULT GraphicHost::_IOleInPlaceSite::ContextSensitiveHelp(BOOL fEnterMode) {
   return S_OK;
   }


   HRESULT GraphicHost::_IOleInPlaceSite::CanInPlaceActivate() {
   return S_OK;
   }


   HRESULT GraphicHost::_IOleInPlaceSite::OnInPlaceActivate() {
   return S_OK;
   }


   HRESULT GraphicHost::_IOleInPlaceSite::OnUIActivate() {
   return S_OK;
   }


   HRESULT GraphicHost::_IOleInPlaceSite::GetWindowContext(IOleInPlaceFrame**,IOleInPlaceUIWindow**,RECT*,RECT*,OLEINPLACEFRAMEINFO*) {
   return S_OK;
   }


   HRESULT GraphicHost::_IOleInPlaceSite::Scroll(SIZE) {
   return S_OK;
   }


   HRESULT GraphicHost::_IOleInPlaceSite::OnUIDeactivate(BOOL) {
   return S_OK;
   }


   HRESULT GraphicHost::_IOleInPlaceSite::OnInPlaceDeactivate() {
   return S_OK;
   }


   HRESULT GraphicHost::_IOleInPlaceSite::DiscardUndoState() {
   return S_OK;
   }


   HRESULT GraphicHost::_IOleInPlaceSite::DeactivateAndUndo() {
   return S_OK;
   }


   HRESULT GraphicHost::_IOleInPlaceSite::OnPosRectChange(const RECT*) {
   return S_OK;
   }
