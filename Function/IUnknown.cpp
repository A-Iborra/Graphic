/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/
#include <windows.h>

#include "Function.h"

   long __stdcall Function::QueryInterface(REFIID riid,void **ppv) {

   *ppv = NULL; 

   if ( riid == IID_IUnknown )
      *ppv = static_cast<void*>(this); 
   else
 
      if ( riid == IID_IDispatch ) 
         *ppv = static_cast<IDispatch *>(this);
      else
 
      if ( riid == IID_IGSFunctioNater ) 
         *ppv = static_cast<IGSFunctioNater *>(this);
      else
 
      if ( riid == IID_IOleObject )
         *ppv = static_cast<IOleObject *>(this);
      else
 
      if ( riid == IID_IViewObject ) 
         *ppv = static_cast<IViewObjectEx *>(this);
      else
 
      if ( riid == IID_IOleControl )
         *ppv = static_cast<IOleControl *>(this);
      else

      if ( riid == IID_IQuickActivate )
         *ppv = static_cast<IQuickActivate *>(this);
      else

      if ( riid == IID_IConnectionPointContainer ) 
         *ppv = static_cast<IConnectionPointContainer *>(this);
      else

      if ( riid == IID_IPersistStorage )
         return iProperties -> QueryInterface(riid,ppv);
      else

      if ( riid == IID_IPersistStreamInit )
         return iProperties -> QueryInterface(riid,ppv);
      else
 
      if ( riid == IID_IPersistStream ) 
         return iProperties -> QueryInterface(riid,ppv);
      else
 
      if ( riid == IID_IPersistPropertyBag ) 
         return iProperties -> QueryInterface(riid,ppv);
      else
 
      if ( riid == IID_IPersistPropertyBag2 ) 
         return iProperties -> QueryInterface(riid,ppv);
      else

      if ( riid == IID_ISpecifyPropertyPages ) 
         return pIUnknownProperties -> QueryInterface(riid,ppv);
      else

      if ( riid == IID_IGProperties ) 
         return iProperties -> QueryInterface(riid,ppv);
      else

      if ( riid == IID_IViewObject2 ) 
         *ppv = static_cast<IViewObjectEx *>(this);
      else
 
      if ( riid == IID_IViewObjectEx ) 
         *ppv = static_cast<IViewObjectEx *>(this);
      else

      if ( riid == IID_IOleInPlaceObject ) {
         askedForInPlaceObject = true;
         *ppv = static_cast<IOleInPlaceObject *>(this);
      }
      else
 
      if ( riid == IID_IOleInPlaceActiveObject ) 
         *ppv = pIOleInPlaceActiveObject;
      else
 
      if ( riid == IID_IOleWindow ) 
         *ppv = static_cast<IOleInPlaceObject *>(this);
      else
 
      if ( riid == IID_IGPropertiesClient )
         *ppv = static_cast<IGPropertiesClient *>(this);
      else

      if ( riid == IID_IGPropertyPageClient )
         *ppv = static_cast<IGPropertyPageClient *>(this);
      else

      if ( riid == IID_IProvideClassInfo ) 
         *ppv = static_cast<IProvideClassInfo2 *>(this);
      else
 
      if ( riid == IID_IProvideClassInfo2 ) 
         *ppv = static_cast<IProvideClassInfo2 *>(this);
      else
 
      if ( riid == IID_IRunnableObject ) 
         *ppv = static_cast<IRunnableObject *>(this);
      else

         return E_NOINTERFACE;
 
   static_cast<IUnknown*>(*ppv) -> AddRef(); 
   return S_OK; 
   }
 
   unsigned long __stdcall Function::AddRef() {
   return ++refCount;
   }
 
   unsigned long __stdcall Function::Release() {
   if ( --refCount == 0 ) {
      delete this;
      return 0;
   }
   return refCount;
   }