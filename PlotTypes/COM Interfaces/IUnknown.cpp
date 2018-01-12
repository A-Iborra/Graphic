
#include "PlotTypes.h"

   long __stdcall PlotTypes::QueryInterface(REFIID riid,void **ppv) {

   *ppv = NULL; 

   if ( riid == IID_IUnknown )
      *ppv = this; 
   else

   if ( riid == IID_IGSystemPlotType ) 
      *ppv = static_cast<IGSystemPlotType *>(this);
   else

   if ( riid == IID_IPersistStream )
      return pIGProperties -> QueryInterface(riid,ppv);
   else

   if ( riid == IID_IPersistStorage )
      return pIGProperties -> QueryInterface(riid,ppv);
   else
      return E_NOINTERFACE;

   AddRef();

   return S_OK; 
   }

   unsigned long __stdcall PlotTypes::AddRef() {
   return ++refCount;
   }
   unsigned long __stdcall PlotTypes::Release() {
   if ( --refCount == 0 ) {
      delete this;
      return 0;
   }
   return refCount;
   }
