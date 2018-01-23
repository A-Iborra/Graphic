
#include "ObjectInOffice.h"



   long __stdcall ObjectInOffice::QueryInterface(REFIID riid,void **ppv) {

   *ppv = NULL; 
 
   if ( riid == IID_IUnknown )
      *ppv = this; 
   else
 
   if ( riid == IID_IObjectInOffice ) 
      *ppv = static_cast<IObjectInOffice *>(this);
   else

   if ( riid == IID_IOleObject )
      *ppv = static_cast<IOleObject *>(this);
   else

   if ( riid == IID_IPersistStorage )
      *ppv = static_cast<IPersistStorage *>(this);
   else

   if ( riid == IID_IRunnableObject )
      *ppv = static_cast<IRunnableObject *>(this);
   else

   if ( riid == IID_IDataObject )
      *ppv = static_cast<IDataObject *>(this);
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
      return E_NOINTERFACE;

   AddRef();
   return S_OK;
   }
   
   unsigned long __stdcall ObjectInOffice::AddRef() {
   return ++refCount;
   }
 
   unsigned long __stdcall ObjectInOffice::Release() {
   refCount--;
   if ( refCount == 0 ) {
      delete this;
      pObjectInOffice = NULL;
      return 0;
   }
   return refCount;
   }