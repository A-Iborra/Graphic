/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <commctrl.h>
#include <ocidl.h>

#include "Graphic_resource.h"

#include "Properties_i.h"
#include "ViewSet_i.c"
#include "ViewSet.h"

       
  long __stdcall ViewSet::QueryInterface(REFIID riid,void **ppv) {
  *ppv = NULL; 

  if ( riid == IID_IUnknown || riid == IID_IViewSet )
     *ppv = static_cast<void*>(this);
  else

      if ( riid == IID_ISpecifyPropertyPages )
         return pIUnknownProperties -> QueryInterface(IID_ISpecifyPropertyPages,ppv);
      else
 
      if ( riid == IID_IGProperties )
         return pIUnknownProperties -> QueryInterface(IID_IGProperties,ppv);
      else

      if ( riid == IID_IPersistStorage ) 
         return pIUnknownProperties -> QueryInterface(IID_IPersistStorage,ppv);
      else
 
      if ( riid == IID_IPersistStreamInit )
         return pIUnknownProperties -> QueryInterface(IID_IPersistStreamInit,ppv);
      else
 
      if ( riid == IID_IPersistStream )
         return pIUnknownProperties -> QueryInterface(IID_IPersistStream,ppv);
      else
 
      if ( riid == IID_IGPropertiesClient ) 
         *ppv = static_cast<IGPropertiesClient *>(this);
      else

      if ( riid == IID_IGPropertyPageClient ) 
         *ppv = static_cast<IGPropertyPageClient *>(this);
      else

         return E_NOINTERFACE;

  AddRef(); 
  return S_OK; 
  }

  unsigned long __stdcall ViewSet::AddRef() {
  return ++refCount;
  }

  unsigned long __stdcall ViewSet::Release() {
  refCount--;
  if ( refCount == 0 ) {
     delete this;
     return 0;
  }
  return refCount;
  }