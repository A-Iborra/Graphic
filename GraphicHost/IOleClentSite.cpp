
#include <windows.h>

#include "GraphicHost.h"

   GraphicHost::_IOleClientSite::_IOleClientSite(GraphicHost *pp)
    : pParent(pp)
   {
   };


   long __stdcall GraphicHost::_IOleClientSite::QueryInterface(REFIID riid,void **ppv) {

   if ( IID_IUnknown == riid ) 
      *ppv = static_cast<IUnknown*>(this);
   else

   if ( IID_IOleClientSite == riid ) 
      *ppv = static_cast<::IOleClientSite*>(this);
   else

      return pParent -> QueryInterface(riid,ppv);

   reinterpret_cast<IUnknown*>(*ppv) -> AddRef();

   return S_OK;
   }
   unsigned long __stdcall GraphicHost::_IOleClientSite::AddRef() {
   return pParent -> AddRef();
   }
   unsigned long __stdcall GraphicHost::_IOleClientSite::Release() {
   return pParent -> Release();
   }


   HRESULT GraphicHost::_IOleClientSite::SaveObject() {
   return S_OK;
   }

   
   HRESULT GraphicHost::_IOleClientSite::GetMoniker(DWORD,DWORD,IMoniker**) {
   return S_OK;
   }


   HRESULT GraphicHost::_IOleClientSite::GetContainer(IOleContainer**) {
   return S_OK;
   }


   HRESULT GraphicHost::_IOleClientSite::ShowObject() {
   return S_OK;
   }


   HRESULT GraphicHost::_IOleClientSite::OnShowWindow(BOOL) {
   return S_OK;
   }


   HRESULT GraphicHost::_IOleClientSite::RequestNewObjectLayout() {
   return S_OK;
   }