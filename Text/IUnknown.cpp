/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002,2003,2004,2005,2006,2007,2008 Nathan T. Clark

*/

#include <windows.h>

#include "Text.h"


   long __stdcall Text::QueryInterface(REFIID riid,void **ppv) {
   *ppv = NULL;
  
   if ( riid == IID_IUnknown )
      *ppv = reinterpret_cast<IUnknown *>(this); 
   else
 
   if ( riid == IID_IDispatch) 
      *ppv = static_cast<IDispatch *>(this);
   else

   if ( riid == IID_IText ) 
      *ppv = static_cast<IText *>(this);
   else
 
   if ( riid == IID_IDataSet )
     *ppv = static_cast<IDataSet *>(pIDataSet);
   else

   if ( riid == IID_IGPropertiesClient ) 
      *ppv = reinterpret_cast<void *>(&iPropertiesClient);
   else
 
   if ( riid == IID_IGPropertyPageClient ) 
      *ppv = static_cast<IGPropertyPageClient *>(this);
   else
 
   if ( riid == IID_ISpecifyPropertyPages )
      return pIProperties -> QueryInterface(IID_ISpecifyPropertyPages,ppv);
   else
  
   if ( riid == IID_IPersistStorage ) 
      return pIProperties -> QueryInterface(IID_IPersistStorage,ppv);
   else
   
   if ( riid == IID_IPersistStreamInit )
      return pIProperties -> QueryInterface(IID_IPersistStreamInit,ppv);
   else
   
   if ( riid == IID_IPersistStream )
      return pIProperties -> QueryInterface(IID_IPersistStream,ppv);
   else
 
   if ( riid == IID_IGProperties )
      return pIProperties -> QueryInterface(riid,ppv);
   else
 
   if ( pIUnknownOuter )
      return pIUnknownOuter -> QueryInterface(riid,ppv);
   else
 
      return E_NOINTERFACE;
   
   reinterpret_cast<IUnknown*>(*ppv) -> AddRef(); 
   return S_OK; 
   }
 
   unsigned long __stdcall Text::AddRef() { 
   return ++refCount;
   }
   unsigned long __stdcall Text::Release() {
   if ( --refCount == 0 ) {
      delete this;
      return 0;
   }
   return refCount;
   }

