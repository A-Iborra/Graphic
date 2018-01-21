// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Plot.h"


   long __stdcall BasePlot::QueryInterface(REFIID riid,void **ppv) {
   *ppv = NULL; 
   if ( riid == IID_IUnknown )
      *ppv = static_cast<IUnknown *>(this); 
   else
      if ( riid == IID_IBasePlot ) 
         *ppv = static_cast<IBasePlot *>(this);
      else
         return E_NOINTERFACE;
   static_cast<IUnknown*>(*ppv) -> AddRef(); 
   return S_OK; 
   }
 
 
   unsigned long __stdcall BasePlot::AddRef() {
   return ++refCount;
   }
   unsigned long __stdcall BasePlot::Release() {
   if ( --refCount == 0 ) {
      delete this;
      return 0;
   }
   return refCount;
   }
 
 
   long __stdcall Plot::QueryInterface(REFIID riid,void **ppv) {
   *ppv = NULL; 
    
   if ( riid == IID_IBasePlot ) 
      return BasePlot::QueryInterface(riid,ppv);
 
   if ( riid == IID_IUnknown ) 
      *ppv = this;
   else

   if ( riid == IID_IDispatch) 
      *ppv = static_cast<IDispatch *>(this);
   else

   if ( riid == IID_IPlot ) 
      *ppv = static_cast<IPlot *>(this);
   else
 
   if ( riid == IID_IPlotServices ) 
      *ppv = static_cast<IPlotServices *>(this);
   else

   if ( riid == IID_IGProperties ) 
      return pIUnknownProperties -> QueryInterface(riid,ppv);
   else
 
   if ( riid == IID_IPersistStorage ) 
      return pIUnknownProperties -> QueryInterface(riid,ppv);
   else
 
   if ( riid == IID_IPersistStream ) 
      return pIUnknownProperties -> QueryInterface(riid,ppv);
   else
 
   if ( riid == IID_IGPropertiesClient ) 
      *ppv = static_cast<IGPropertiesClient *>(this);
   else
 
   if ( riid == IID_IGPropertyPageClient ) 
      *ppv = static_cast<IGPropertyPageClient *>(this);
   else
 
   if ( riid == IID_ISpecifyPropertyPages ) 
      return pIUnknownProperties -> QueryInterface(riid,ppv);
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
 
   static_cast<IUnknown*>(*ppv) -> AddRef(); 
   return S_OK; 
   }
   unsigned long __stdcall Plot::AddRef() {
   return ++refCount;
   }
   unsigned long __stdcall Plot::Release() {
   if ( --refCount == 0 ) {
      delete this;
      return 0;
   }
   return refCount;
   }
